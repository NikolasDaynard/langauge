#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/BasicBlock.h>

#include "functions.cpp"
#include "parser.h"
#include "../parseHelpers.h"

llvm::Value* parser::getVariable(const std::string& name) {
    auto it = contextStack.back().variableMap.find(name);
    if (it != contextStack.back().variableMap.end()) {
        return it->second; // Return variable if found
    }

    return nullptr;
}

llvm::Value *parser::evaluateValue(std::string name, std::string value, std::size_t i) {
    if (lexedCode[i] == "call") {
        i++;
        currentFunction = functionsWrapper->getFunction(lexedCode[i]);
        currentArgs.clear();
        while (true) {
            i++;
            if (lexedCode[i] == "end" || lexedCode[i - 1] == "end") {
                break;
            }
            std::cout << "looped " + lexedCode[i] << std::endl;
            currentArgs.push_back(evaluateValue(lexedCode[i], lexedCode[i], i));
        }

        functionNests++;
    }

    if(functionNests != 0 && lexedCode[i] == "end") {
        llvm::Value *call = Builder->CreateCall(currentFunction, currentArgs);
        std::cout << call << std::endl;
        currentArgs.clear(); // remove all args
        functionNests--;
        std::cout << "returning call" << std::endl;
        return call;
    }

    // strings
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') { 
        value = value.substr(1, value.size() - 2);
        replaceAll(value, "\\n", "\n"); // reinsert newlines
        if (strings[value] != nullptr) {
            return strings[value];
        }
        
        llvm::Value *variable = Builder->CreateGlobalStringPtr(value.c_str(), "constStr");
        strings[value] = variable;
        return variable;
    }

    // numbers
    if (std::isdigit(value.c_str()[0])) { 
        llvm::Value *variable = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*Context), std::stod(value));
        return variable;
    }


    llvm::Type *varType = llvm::Type::getDoubleTy(*Context);
    llvm::Value *val = parser::getVariable(value);
    if (val != nullptr) {
        return Builder->CreateLoad(Builder->getDoubleTy(), val, "loadedNum");
    }

    if (isMathKeyword(value)) {
        std::cout << "oh dear god it's math" << std::endl;
        llvm::Value *first;
        llvm::Value *second;
        int originalIndex = i;
        if (isMathKeyword(lexedCode[i])) {
            i++;
            first = evaluateValue(lexedCode[i], lexedCode[i], i);
            i++;
            second = evaluateValue(lexedCode[i], lexedCode[i], i);
        }

        if (lexedCode[originalIndex] == "add") {
            return Builder->CreateFAdd(first, second, "AdditionTemp");
        }else if (lexedCode[originalIndex] == "sub") {
            return Builder->CreateFSub(first, second, "SubTemp");
        }else if (lexedCode[originalIndex] == "mul") {
            return Builder->CreateFMul(first, second, "MultiplicationTemp");
        }else if (lexedCode[originalIndex] == "div") {
            return Builder->CreateFDiv(first, second, "DivisionTemp");
        }else if (lexedCode[originalIndex] == "cmp") {
            // Create the floating-point comparison
            llvm::Value *FCmp = Builder->CreateFCmpOEQ(first, second, "FCMP");

            // Convert the i1 result (boolean) to i32 (32-bit integer)
            llvm::Value *IntResult = Builder->CreateZExt(FCmp, llvm::Type::getInt32Ty(*Context), "zext");

            // Convert the i32 result to a double (64-bit floating point)
            llvm::Value *DoubleResult = Builder->CreateSIToFP(IntResult, llvm::Type::getDoubleTy(*Context), "boolToDouble");

            return DoubleResult;
        }else if (lexedCode[originalIndex] == "les") { // <
            // Create the floating-point comparison
            llvm::Value *FCmp = Builder->CreateFCmpOLT(first, second, "FCMP");

            // Convert the i1 result (boolean) to i32 (32-bit integer)
            llvm::Value *IntResult = Builder->CreateZExt(FCmp, llvm::Type::getInt32Ty(*Context), "zext");

            // Convert the i32 result to a double (64-bit floating point)
            llvm::Value *DoubleResult = Builder->CreateSIToFP(IntResult, llvm::Type::getDoubleTy(*Context), "boolToDouble");

            return DoubleResult;
        }else if (lexedCode[originalIndex] == "pow") {
            return Builder->CreateCall(functionsWrapper->getFunction("pow"), {first, second}); // TODO: this is undefined
        }
        i++;

        return val;
    }

    std::cout << value + " was not evaluated" << std::endl;

    return nullptr;
}

void parser::evaluateConditional(std::string name, std::string value, std::size_t i) {
    // Check for type (while if etc.)
    std::cout << "evalling" << name << value << lexedCode[i - 2] << std::endl;
    std::string conditionType = lexedCode[i - 2];
    if (conditionType == "if") {
        if (name == "condblock") {
            std::cout << "prev back" << contextStack.back().function->getName().str() << std::endl;

            // Step 5: Create the "then", "else", and "merge" blocks
            llvm::BasicBlock *CondBB = llvm::BasicBlock::Create(*Context, "cond", contextStack.back().function);
            llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*Context, "then", contextStack.back().function);
            llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*Context, "else", contextStack.back().function);
            llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*Context, "merge", contextStack.back().function);

            std::map<std::string, llvm::Value *> map = contextStack.back().variableMap;
            contextStack.push_back({Builder->saveIP(), contextId, {{"cond", CondBB}, {"then", ThenBB}, {"else", ElseBB}, {"merge", MergeBB}}, contextStack.back().function, "ifEntry"});
            contextStack.back().variableMap = map;

            Builder->CreateBr(CondBB);

            Builder->SetInsertPoint(CondBB);
        }else if (name == "cond") {
            std::cout << "prev back" << contextStack.back().function->getName().str() << std::endl;

            // Step 4: Evaluate the condition (assuming evaluateValue already returns an llvm::Value *)
            llvm::Value *Condition = Builder->CreateFCmpOEQ(
                evaluateValue(value, value, i), 
                llvm::ConstantFP::get(llvm::Type::getDoubleTy(*Context), 1.0), 
                "cond"
            );

            // Step 6: Create a conditional branch based on the condition
            Builder->CreateCondBr(Condition, contextStack.back().basicBlocks.find("then")->second, contextStack.back().basicBlocks.find("else")->second);

            // Step 7: Populate the "then" block
            Builder->SetInsertPoint(contextStack.back().basicBlocks.find("then")->second);
        }
    }else if (conditionType == "while") {
        if (name == "condblock") {
            std::cout << "prev back" << contextStack.back().function->getName().str() << std::endl;

            // Step 5: Create the "then", "else", and "merge" blocks
            llvm::BasicBlock *CondBB = llvm::BasicBlock::Create(*Context, "cond", contextStack.back().function);
            llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(*Context, "loop", contextStack.back().function);
            llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*Context, "merge", contextStack.back().function);
            std::map<std::string, llvm::Value *> map = contextStack.back().variableMap;

            contextStack.push_back({Builder->saveIP(), contextId, {{"cond", CondBB}, {"loop", LoopBB}, {"merge", MergeBB}}, contextStack.back().function, "loopEntry"});
            contextStack.back().variableMap = map;

            Builder->CreateBr(CondBB);

            Builder->SetInsertPoint(CondBB);
        }if (name == "cond") {
            // Step 4: Evaluate the condition (assuming evaluateValue already returns an llvm::Value *)
            llvm::Value *Condition = Builder->CreateFCmpOEQ(
                evaluateValue(value, value, i), 
                llvm::ConstantFP::get(llvm::Type::getDoubleTy(*Context), 1.0), 
                "cond"
            );


            // Step 6: Create a conditional branch based on the condition
            Builder->CreateCondBr(Condition, contextStack.back().basicBlocks.find("loop")->second, contextStack.back().basicBlocks.find("merge")->second);

            // Step 7: Populate the "then" block
            Builder->SetInsertPoint(contextStack.back().basicBlocks.find("loop")->second); 
        }
    }else if (name == "endcond") {
        if (contextStack.back().mergeRet != nullptr) {
            Builder->CreateBr(contextStack.back().mergeRet);
            contextStack.back().mergeRet = nullptr; // just to be safe
        }

        llvm::BasicBlock *MergeBB = contextStack.back().basicBlocks.find("merge")->second;

        for (std::pair<std::string, llvm::BasicBlock *> BB : contextStack.back().basicBlocks) {
            std::cout << "Reading BB: " << BB.first << std::endl;
            if (!BB.second->getTerminator()) { // dont merge if we've already broken
                if (BB.first != "merge" && BB.first != "cond" && BB.first != "loop") {
                    Builder->SetInsertPoint(BB.second);
                    Builder->CreateBr(MergeBB);
                }else if (BB.first == "loop") {
                    Builder->SetInsertPoint(BB.second);
                    Builder->CreateBr(contextStack.back().basicBlocks.find("cond")->second);
                }
            }
        }
        contextStack.pop_back();
        int mergeBlocksBack = 0;

        test:
        std::cout << mergeBlocksBack << " back" << std::endl;
        if (contextStack.size() <= 1) { // this is a function
            Builder->CreateRet(Builder->getInt32(0));
            Builder->SetInsertPoint(contextStack.back().insertionPoint.getBlock());
            return; 
        }

        if (contextStack[contextStack.size() - (1 + mergeBlocksBack)].basicBlocks.find("merge")->first != "" && 
            contextStack[contextStack.size() - (1 + mergeBlocksBack)].basicBlocks.find("merge")->second->getName().str() != "") {
            
            mergeBlocksBack = mergeBlocksBack + 1;
            goto test;
        }

        // Return to block
        Builder->SetInsertPoint(MergeBB);
        if (mergeBlocksBack != 0 && contextStack[contextStack.size() - 1].basicBlocks.find("merge")->second->getName().str() != "") {
            // std::cout << "adding the thing" << std::endl;
            if (contextStack[contextStack.size() - 1].basicBlocks.find("loop")->first == "") {
                contextStack.back().mergeRet = contextStack[contextStack.size() - 1].basicBlocks.find("merge")->second;
            }else { // if it's a loop, jump to the conditional
                contextStack.back().mergeRet = contextStack[contextStack.size() - 1].basicBlocks.find("cond")->second;
            }
        }

        evaluateValue(value, value, i);
    }else if (name == "else") {
        llvm::BasicBlock *ElseBB = contextStack.back().basicBlocks.find("else")->second;
        Builder->SetInsertPoint(ElseBB);
    }
    if (name == "condblock") {
        // std::string str = lexedCode[++i];
        // name = str;
        // str = lexedCode[++i];
        // parser::createVariable(name, str, i);
    }
}

void parser::evaluateFunction(std::string name, std::string value, std::size_t i) {
    name = value;
    std::cout << "evaluating function " << name << " with return type " << value << std::endl;

    // Create the function type (example with void return type and no arguments)
    llvm::Type *returnType = llvm::Type::getInt32Ty(*Context);
    llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, true);

    // Create the function with external linkage (can be changed to internal as needed)
    llvm::Function *function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, name, Module);

    llvm::BasicBlock *MainBB = llvm::BasicBlock::Create(*Context, "ma2in", function);

    Builder->SetInsertPoint(MainBB);

    contextStack.push_back(contextInfo(Builder->saveIP(), contextId, {}, function, "mainw"));
    contextStack.back().variableMap = {};

    functionsWrapper->addFunction(name, function);

    std::cout << "Function " << name << " created successfully" << std::endl;
}




llvm::Value *parser::createVariable(std::string name, std::string value, std::size_t i) {

    llvm::Value *variable = parser::getVariable(name);
    if (variable != nullptr) {
        Builder->CreateStore(parser::evaluateValue(name, value, i), variable);
        return variable;
    }

    llvm::Type *varType = llvm::Type::getDoubleTy(*Context);
    variable = Builder->CreateAlloca(varType, nullptr, name);
    llvm::Value* val = parser::evaluateValue(name, value, i);
    Builder->CreateStore(val, variable);
    contextStack.back().variableMap[name] = variable;

    return variable;

    std::cout << "varaible initlization failed" << std::endl;
    return NULL;
}

std::string parser::parseFile() {
    bool calling = false;
    bool setting = false;
    bool readingArgs = false;
    std::string name;

    for (std::size_t i = 0; i < lexedCode.size(); ++i) {
        std::string& str = lexedCode[i];

        std::cout << "reading: " << str << std::endl;
        if (str == "\n") {
        }else if (str == "set") {
            str = lexedCode[++i];
            name = str;
            str = lexedCode[++i];
            parser::createVariable(name, str, i);
        }else if (str == "cond" || str == "endcond" || str == "else" || str == "condblock") {
            name = lexedCode[i++];
            std::string value = lexedCode[i];
            parser::evaluateConditional(name, value, i);
        }else if (str == "func") {
            name = lexedCode[i++];
            std::string value = lexedCode[i];
            parser::evaluateFunction(name, value, i);
        }

    }

    Builder->CreateRet(Builder->getInt32(0)); // return at the end of the main
    return "";
}

void parser::writeLine(std::string line) {
    std::string home = std::getenv("HOME");
    std::string fileOutputName = home + "/dev/lang";
    fileOutputName += filename + "bc";
    std::ofstream fileOUT(fileOutputName, std::ios::app); // open filename.txt in append mode

    fileOUT << line << std::endl; // append "some stuff" to the end of the file

    fileOUT.close(); // close the file
}