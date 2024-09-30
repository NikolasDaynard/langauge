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
#include "../parseHelpers.h"
#include "parser.h"

Variable parser::getVariable(const std::string& name) {
    auto it = contextStack.back().variableMap.find(name);
    if (it != contextStack.back().variableMap.end()) {
        return it->second; // Return variable if found
    }

    return nullptr;
}

llvm::Value *parser::evaluateValue(std::string name, std::string value, std::size_t i) {

    if (lexedCode[i] == "call") {
        std::cout << "evaling call" << std::endl;
        int orginalI = i;

        if (functionPositions.find(lexedCode[orginalI + 1]) != functionPositions.end() && functionPositions[lexedCode[orginalI + 1]] != -1) {
            i++;
            currentArgs.clear();
            while (true) {
                i++;
                if (lexedCode[i] == "end" || lexedCode[i - 1] == "end") {
                    break;
                }
                std::cout << "looped " + lexedCode[i] << std::endl;
                currentArgs.push_back(evaluateValue(lexedCode[i], lexedCode[i], i));
            }

            std::cout << "func " << lexedCode[orginalI + 1] << " was in the pos at " << functionPositions[lexedCode[orginalI + 1]] << std::endl; 
            int pos = functionPositions[lexedCode[orginalI + 1]];
            functionPositions[lexedCode[orginalI + 1]] = -1;
            int condBuffer = 0;

            for (std::size_t j = pos; j < lexedCode.size(); j++) {
                if (lexedCode[j] == "if" && lexedCode[j + 1] == "cond") {
                    condBuffer++;
                }
                if (lexedCode[j] == "endcond") {
                    if (condBuffer == 0) {
                        handleString(j);
                        break;
                    }
                    condBuffer--;
                }
                j = handleString(j);
            }

        }
        i = orginalI;
        i++;
        currentFunction = functionsWrapper->getFunction(lexedCode[orginalI + 1]);
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

    Variable val = parser::getVariable(value);
    if (val.val != nullptr) {
        return Builder->CreateLoad(val.type, val.val, "loadedVar");
    }

    if (isMathKeyword(value, tokenHold)) {
        std::cout << "oh dear god it's math" << std::endl;
        llvm::Value *first;
        llvm::Value *second;
        int originalIndex = i;
        if (isMathKeyword(lexedCode[i], tokenHold)) {
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
        }else if (lexedCode[originalIndex] == "les" || lexedCode[originalIndex] == "grt") { // <
            // Create the floating-point comparison
            llvm::Value *FCmp;
            if (lexedCode[originalIndex] == "les") {
                FCmp = Builder->CreateFCmpOLT(first, second, "FCMP");
            }else if (lexedCode[originalIndex] == "grt") {
                FCmp = Builder->CreateFCmpOGT(first, second, "FCMP");
            }

            // Convert the i1 result (boolean) to i32 (32-bit integer)
            llvm::Value *IntResult = Builder->CreateZExt(FCmp, llvm::Type::getInt32Ty(*Context), "zext");

            // Convert the i32 result to a double (64-bit floating point)
            llvm::Value *DoubleResult = Builder->CreateSIToFP(IntResult, llvm::Type::getDoubleTy(*Context), "boolToDouble");

            return DoubleResult;
        }else if (lexedCode[originalIndex] == "pow") {
            return Builder->CreateCall(functionsWrapper->getFunction("pow"), {first, second}); // TODO: this is undefined
        }
        i++;

        return val.val;
    }

    std::cout << "\033[33m" << value + " was not evaluated" << "\033[0m" << std::endl;

    return nullptr;
}

void parser::evaluateConditional(std::string name, std::string value, std::size_t i) {
    // Check for type (while if etc.)
    std::cout << "evalling" << name << value << lexedCode[i - 2] << std::endl;
    std::string conditionType = lexedCode[i - 2];
    if (conditionType == "if") {
        if (name == "condblock") {
            std::cout << "prev back " << contextStack.back().function->getName().str() << std::endl;

            // Step 5: Create the "then", "else", and "merge" blocks
            llvm::BasicBlock *CondBB = llvm::BasicBlock::Create(*Context, "cond", contextStack.back().function);
            llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(*Context, "then", contextStack.back().function);
            llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(*Context, "else", contextStack.back().function);
            llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(*Context, "merge", contextStack.back().function);

            std::map<std::string, Variable> map = contextStack.back().variableMap;
            contextStack.push_back({Builder->saveIP(), contextId, {{"cond", CondBB}, {"then", ThenBB}, {"else", ElseBB}, {"merge", MergeBB}}, contextStack.back().function, "ifEntry"});
            contextStack.back().variableMap = map;
            std::cout << "PUSHED NEW CONTEXTSTACK" << std::endl;

            Builder->CreateBr(CondBB);

            Builder->SetInsertPoint(CondBB);
        }else if (name == "cond") {
            std::cout << "prev back " << contextStack.back().function->getName().str() << std::endl;

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
            std::map<std::string, Variable> map = contextStack.back().variableMap;

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
        contextInfo savedContext = contextStack.back();
        contextStack.pop_back();
        std::cout << "POPPED CS" << std::endl;
        int mergeBlocksBack = 0;

        test:
        std::cout << mergeBlocksBack << " back" << std::endl;
        if (contextStack.size() > 0 && savedContext.basicBlocks.find("cond") == savedContext.basicBlocks.end() && savedContext.function->getName().str() != "mainBodyFunc") { // this is a function
            std::cout << "Popped " << savedContext.function->getName().str() << std::endl;
            std::cout << "context stack undersized " << savedContext.function->getName().str() << " w/ size " << contextStack.size() << std::endl;
            Builder->CreateRet(Builder->getInt32(0));
            Builder->SetInsertPoint(contextStack.back().insertionPoint.getBlock());
            return; 
        }
        if (contextStack.size() >= (1 + mergeBlocksBack)) {
            if (contextStack[contextStack.size() - (1 + mergeBlocksBack)].basicBlocks.find("merge")->first != "" && 
                contextStack[contextStack.size() - (1 + mergeBlocksBack)].basicBlocks.find("merge")->second->getName().str() != "") {
                
                mergeBlocksBack = mergeBlocksBack + 1;
                goto test;
            }
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
}

void parser::evaluateFunction(std::string name, std::string value, std::size_t i) {
    name = value;
    std::cout << "evaluating function " << name << " with return type " << value << std::endl;

    // Create the function type (example with void return type and no arguments)
    llvm::Type *returnType = llvm::Type::getInt32Ty(*Context);
    
    std::vector<llvm::Type *> argTypes;
    std::vector<std::string> varNames;
    int currentArg = 0;
    while (lexedCode[++i] != "end") { // first one is name
        argTypes.push_back(currentArgs.at(currentArg)->getType());
        varNames.push_back(lexedCode[i]);
        currentArg++;
    }

    llvm::FunctionType *functionType = llvm::FunctionType::get(returnType, argTypes, true);

    // Create the function with external linkage (can be changed to internal as needed)
    llvm::Function *function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, name, Module);

    llvm::BasicBlock *MainBB = llvm::BasicBlock::Create(*Context, "main", function);

    Builder->SetInsertPoint(MainBB);

    contextStack.push_back(contextInfo(Builder->saveIP(), contextId, {}, function, "main"));
    contextStack.back().variableMap = {};
    std::cout << "PUSHED NEW FUNC CONTEXTSTACK" << std::endl;

    llvm::Function::arg_iterator args = function->arg_begin();

    for (std::size_t id = 0; id < varNames.size(); ++id, ++args) {
        llvm::Value *arg = &*args;  // Get the argument value
        llvm::Value *variable = Builder->CreateAlloca(arg->getType(), nullptr, varNames[id]);
        Builder->CreateStore(arg, variable);
        contextStack.back().variableMap[varNames[id]] = variable;
        contextStack.back().variableMap[varNames[id]].type = arg->getType();
    }

    functionsWrapper->addFunction(name, function);

    std::cout << "Function " << name << " created successfully" << std::endl;
}




llvm::Value *parser::createVariable(std::string name, std::string value, std::size_t i) {
    Variable variable = parser::getVariable(name);

    if (variable.val != nullptr) {
        if (value == "call") { // assume it's right 
            Builder->CreateStore(parser::evaluateValue(name, value, i), variable.val);
            return variable.val;
        }else if (variable.type == parser::evaluateValue(name, value, i)->getType()) {
            Builder->CreateStore(parser::evaluateValue(name, value, i), variable.val);
            return variable.val;
        }
    }
    llvm::Value* val = parser::evaluateValue(name, value, i);

    variable = Builder->CreateAlloca(val->getType(), nullptr, name);
    Builder->CreateStore(val, variable.val);
    contextStack.back().variableMap[name] = variable;
    contextStack.back().variableMap[name].type = val->getType();
    return variable.val;

    std::cout << "varaible initlization failed" << std::endl;
    return NULL;
}

int parser::handleString(std::size_t i) {
    std::string name;
    std::string& str = lexedCode[i];

    std::cout << "reading: " << str << std::endl;

    if (str == "set") {
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
    return i;
}

std::string parser::parseFile() {
    int skippingCond = 0;

    for (std::size_t i = 0; i < lexedCode.size(); ++i) {
        if (skippingCond > 0) {
            if (lexedCode[i] == "if" && lexedCode[i + 1] == "cond") { // only iterate if already in func
                skippingCond++;
            }
            if (lexedCode[i] == "endcond") { skippingCond--; }
            continue;
        }

        if (lexedCode[i] == "func") {
            std::string value = lexedCode[i + 1];
            functionPositions[value] = i;
            skippingCond++;
            std::cout << "adding func to poses " << i << " " << value << std::endl;
            continue;
        }
        i = handleString(i);
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