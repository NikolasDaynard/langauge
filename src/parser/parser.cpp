#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "functions.cpp"
#include "parser.h"

bool isMathKeyword(std::string str) {
    return str == "add" || str == "sub" || str == "div" || str == "mul";
}

void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Advance startPos to avoid infinite loop if 'to' contains 'from'
    }
}

llvm::Value* parser::getVariable(const std::string& name) {
    auto it = variableMap.find(name);
    if (it != variableMap.end()) {
        return it->second; // Return variable if found
    }

    return nullptr;
}

llvm::Value *parser::evaluateValue(std::string name, std::string value, std::size_t i) {
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') { 
        value = value.substr(1, value.size() - 2);
        replaceAll(value, "\\n", "\n"); // reinsert newlines
        llvm::Value *variable = Builder->CreateGlobalStringPtr(value.c_str(), "constStr");
        return variable;
    }

    if (std::isdigit(value.c_str()[0])) {
        llvm::Value *variable = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*Context), (float)std::stoi(value));
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
        while(lexedCode[i] != "\n") {
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
            }
            i++;
        }
        return val;
    }
    return nullptr;
}

llvm::Value *parser::createVariable(std::string name, std::string value, std::size_t i) {

    llvm::Value *variable = parser::getVariable(name);
    if (variable != nullptr) {
        //parser::evaluateValue(name, str);
        Builder->CreateStore(parser::evaluateValue(name, value, i), variable);
        return variable;
    }

    llvm::Type *varType = llvm::Type::getDoubleTy(*Context);
    variable = Builder->CreateAlloca(varType, nullptr, name);
    llvm::Value* val = parser::evaluateValue(name, value, i);
    Builder->CreateStore(val, variable);
    variableMap[name] = variable;

    return variable;

    std::cout << "varaible initlization failed" << std::endl;
    return NULL;
}

std::string parser::parseFile() {
    bool calling = false;
    bool setting = false;
    bool readingArgs = false;
    llvm::FunctionCallee currentFunction;
    std::vector<llvm::Value *> currentArgs;
    std::string name;

    for (std::size_t i = 0; i < lexedCode.size(); ++i) {
        std::string& str = lexedCode[i];

        std::cout << str << std::endl;
        if (str == "\n") {
            if (readingArgs) {
                Builder->CreateCall(currentFunction, currentArgs);
                currentArgs.clear(); // remove all args
            }

            readingArgs = false;
        }else if (str == "call") {
            str = lexedCode[++i];
            currentFunction = function->getFunction(str);
            readingArgs = true;
        }else if (str == "set") {
            str = lexedCode[++i];
            name = str;
            str = lexedCode[++i];
            parser::createVariable(name, str, i);
            // evaluates add sub and returns a var containing the result
            // while(parser::evaluateValue(name, str)); 
        }else if(readingArgs) {
            currentArgs.push_back(parser::evaluateValue(str, str, i)); // args don't have names
        }

    } 
    // for(std::pair<std::string, llvm::Value *> var : variableMap) {
        // Builder->CreateCall(function->getFunction("free"), var.second);
    // }
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