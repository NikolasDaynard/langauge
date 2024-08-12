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

void initFunctions() {
    // llvm::Value *HelloWorld = Builder.CreateGlobalStringPtr("Hello, LLVM!\n");

    // llvm::FunctionType *PrintfType = llvm::FunctionType::get(Builder.getInt32Ty(), true);
    // llvm::FunctionCallee Printf = Module->getOrInsertFunction("printf", PrintfType);

    // Builder.CreateCall(Printf, {HelloWorld});
    // Builder.CreateRet(Builder.getInt32(0));
}

void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Advance startPos to avoid infinite loop if 'to' contains 'from'
    }
}

llvm::Value *parser::createVariable(std::string name, std::string value) {

    // check if it's a string
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') { 
        value = value.substr(1, value.size() - 2);
        replaceAll(value, "\\n", "\n"); // reinsert newlines
        llvm::Value *variable = Builder->CreateGlobalStringPtr(value.c_str(), "constStr");
        return variable;
    }

    if (std::isdigit(value.c_str()[0])) {
        std::cout << value << " is a number" << std::endl;
        
        llvm::Value *variable = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*Context), std::stoi(value));
        return variable;
    }

    llvm::Type *varType = llvm::Type::getInt32Ty(*Context);
    llvm::Value *variable = Builder->CreateAlloca(varType, nullptr, name);
    llvm::Value* constValue = llvm::ConstantInt::get(varType, 6);
    Builder->CreateStore(constValue, variable);

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

    for (std::string str : lexedCode) {
        std::cout << str << std::endl;
        if (str == "\n") {
            if (readingArgs) {
                Builder->CreateCall(currentFunction, currentArgs);

                currentArgs.clear(); // remove all args
            }

            calling = false;
            readingArgs = false;
        }else if (str == "call") {
            calling = true;
        }else if (str == "set") {
            printf("set HELLOW");
            setting = true;
        }else if(calling) {
            currentFunction = function->getFunction(str);
            readingArgs = true;
            calling = false;
        }else if(setting) {
            name = str;
            setting = false;
        }else if(readingArgs) {
            currentArgs.push_back(parser::createVariable(name, str));
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