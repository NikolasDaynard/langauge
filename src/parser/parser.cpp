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

llvm::Value *parser::createVariable(std::string name, std::string value) {

    // check if it's a string
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') { 
        value = value.substr(1, value.size() - 2);
        llvm::Value *variable = Builder->CreateGlobalStringPtr(value.c_str());
        return variable;
    }

    if (std::isdigit(value.c_str()[0])) {
        std::cout << value << " is a number" << std::endl;
        
        llvm::Value *variable = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*Context), std::stoi(value));
        return variable;
    }

    std::cout << "varaible initlization failed" << std::endl;
    return NULL;
}

std::string parser::parseFile() {
    bool calling = false;
    bool readingArgs = false;
    llvm::FunctionCallee currentFunction;
    std::vector<llvm::Value *> currentArgs;

    for (std::string str : lexedCode) {
        std::cout << str << std::endl;
        if (str == "\n") {
            if (readingArgs) {
                Builder->CreateCall(currentFunction, currentArgs);
                Builder->CreateRet(Builder->getInt32(0));
            }

            calling = false;
            readingArgs = false;
        }else if (str == "call") {
            calling = true;
        }else if(calling) {
            currentFunction = function->getFunction(str);
            readingArgs = true;
            calling = false;
        }else if(readingArgs) {
            // llvm::Value *FormatStr = Builder->CreateGlobalStringPtr("%s\n");
            // currentArgs.push_back(FormatStr);
            currentArgs.push_back(parser::createVariable("foo", str));
        }

    } 
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