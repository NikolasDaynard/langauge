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

llvm::Value* parser::getVariable(const std::string& name) {
    auto it = variableMap.find(name);
    if (it != variableMap.end()) {
        return it->second; // Return variable if found
    }
    // std::cout << name << " not ofund" << std::endl;
    // Handle case where variable is not found (e.g., throw an error or return nullptr)
    return nullptr;
}

llvm::Value *parser::createVariable(std::string name, std::string value, bool reading) {

    // check if it's a string
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') { 
        value = value.substr(1, value.size() - 2);
        replaceAll(value, "\\n", "\n"); // reinsert newlines
        llvm::Value *variable = Builder->CreateGlobalStringPtr(value.c_str(), "constStr");
        return variable;
    }

    if (std::isdigit(value.c_str()[0]) && reading) {
        // std::cout << value << " is a number" << std::endl;
        
        llvm::Value *variable = llvm::ConstantFP::get(llvm::Type::getDoubleTy(*Context), (float)std::stoi(value));
        return variable;
    }

    llvm::Value *variable = parser::getVariable(name);
    if (variable != nullptr) {
        return Builder->CreateLoad(Builder->getDoubleTy(), variable, "loadedNum");
    }

    if (reading) { // don't create if reading
        std::cout << "varaible initlization failed" << std::endl;
        return NULL;
    }

    llvm::Type *varType = llvm::Type::getDoubleTy(*Context);
    variable = Builder->CreateAlloca(varType, nullptr, name);
    llvm::Value* constValue = llvm::ConstantFP::get(varType, (float)std::stoi(value));
    Builder->CreateStore(constValue, variable);
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
            parser::createVariable(name, str, false);
        }else if(readingArgs) {
            currentArgs.push_back(parser::createVariable(str, str, true)); // args don't have names
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