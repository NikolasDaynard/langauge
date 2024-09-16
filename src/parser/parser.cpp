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
    auto it = functionStack.back().variableMap.find(name);
    if (it != functionStack.back().variableMap.end()) {
        return it->second; // Return variable if found
    }

    return nullptr;
}

llvm::Value *parser::evaluateValue(std::string name, std::string value, std::size_t i) {
    if (lexedCode[i] == "call") {
        i++;
        currentFunction = function->getFunction(lexedCode[i]);
        currentArgs.clear();
        while (true) {
            i++;
            if (lexedCode[i] == "end") {
                break;
            }
            std::cout << "looped " + lexedCode[i] << std::endl;
            currentArgs.push_back(evaluateValue(lexedCode[i], lexedCode[i], i));
        }

        functionNests++;
    }

    if(functionNests != 0 && lexedCode[i] == "end") {
        llvm::Value *call = Builder->CreateCall(currentFunction, currentArgs);
        std::cout << "--" << std::endl;
        currentArgs.clear(); // remove all args
        functionNests--;
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
        }else if (lexedCode[originalIndex] == "pow") {
            return Builder->CreateCall(function->getFunction("pow"), {first, second}); // TODO: this is undefined
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
            std::cout << "prev back" << functionStack.back().function->getName().str() << std::endl;

            // Step 5: Create the "then", "else", and "merge" blocks
            llvm::BasicBlock *CondBB = llvm::BasicBlock::Create(*Context, "cond", functionStack.back().function);
            llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*Context, "then", functionStack.back().function);
            llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*Context, "else", functionStack.back().function);
            llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*Context, "merge", functionStack.back().function);

            std::map<std::string, llvm::Value *> map = functionStack.back().variableMap;
            functionStack.push_back({Builder->saveIP(), contextId, {{"cond", CondBB}, {"then", ThenBB}, {"else", ElseBB}, {"merge", MergeBB}}, functionStack.back().function, "ifEntry"});
            functionStack.back().variableMap = map;

            Builder->CreateBr(CondBB);

            Builder->SetInsertPoint(CondBB);
        }else if (name == "cond") {
            std::cout << "prev back" << functionStack.back().function->getName().str() << std::endl;

            // Step 4: Evaluate the condition (assuming evaluateValue already returns an llvm::Value *)
            llvm::Value *Condition = Builder->CreateFCmpOEQ(
                evaluateValue(value, value, i), 
                llvm::ConstantFP::get(llvm::Type::getDoubleTy(*Context), 1.0), 
                "cond"
            );

            // Step 6: Create a conditional branch based on the condition
            Builder->CreateCondBr(Condition, functionStack.back().basicBlocks.find("then")->second, functionStack.back().basicBlocks.find("else")->second);

            // Step 7: Populate the "then" block
            Builder->SetInsertPoint(functionStack.back().basicBlocks.find("then")->second);
        }
    }else if (conditionType == "while") {
        if (name == "condblock") {
            std::cout << "prev back" << functionStack.back().function->getName().str() << std::endl;

            // Step 5: Create the "then", "else", and "merge" blocks
            llvm::BasicBlock *CondBB = llvm::BasicBlock::Create(*Context, "cond", functionStack.back().function);
            llvm::BasicBlock *LoopBB = llvm::BasicBlock::Create(*Context, "loop", functionStack.back().function);
            llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*Context, "merge", functionStack.back().function);
            std::map<std::string, llvm::Value *> map = functionStack.back().variableMap;

            functionStack.push_back({Builder->saveIP(), contextId, {{"cond", CondBB}, {"loop", LoopBB}, {"merge", MergeBB}}, functionStack.back().function, "loopEntry"});
            functionStack.back().variableMap = map;

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
            Builder->CreateCondBr(Condition, functionStack.back().basicBlocks.find("loop")->second, functionStack.back().basicBlocks.find("merge")->second);

            // Step 7: Populate the "then" block
            Builder->SetInsertPoint(functionStack.back().basicBlocks.find("loop")->second); 
        }
    }else if (name == "endcond") {
        llvm::BasicBlock *MergeBB = functionStack.back().basicBlocks.find("merge")->second;

        for (std::pair<std::string, llvm::BasicBlock *> BB : functionStack.back().basicBlocks) {
            std::cout << "Reading BB: " << BB.first << std::endl;
            if (BB.first != "merge" && BB.first != "cond" && BB.first != "loop") {
                Builder->SetInsertPoint(BB.second);
                Builder->CreateBr(MergeBB);
            }else if (BB.first == "loop") {
                Builder->SetInsertPoint(BB.second);
                Builder->CreateBr(functionStack.back().basicBlocks.find("cond")->second);
            }
        }

        // Return to block
        Builder->SetInsertPoint(MergeBB);

        evaluateValue(value, value, i);
    }else if (name == "else") {
        llvm::BasicBlock *ElseBB = functionStack.back().basicBlocks.find("else")->second;
        Builder->SetInsertPoint(ElseBB);
    }
    if (name == "condblock") {
        // std::string str = lexedCode[++i];
        // name = str;
        // str = lexedCode[++i];
        // parser::createVariable(name, str, i);
    }
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
    functionStack.back().variableMap[name] = variable;

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
            parser:evaluateConditional(name, value, i);
        }else {
            // currentArgs.push_back(parser::evaluateValue(str, str, i)); // args don't have names
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