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

std::string parser::parseFile() {
    bool calling = false;

    for (std::string str : lexedCode) {

        if (str == "call") {
            calling = true;
        }else if(calling) {
            function.getFunction(str);
        }

    } 
    return "";
}

void parser::writeLine(std::string line) {
    std::string home = std::getenv("HOME");

    std::ofstream fileOUT(home + "/dev/lang/examples/hello.wbc", std::ios::app); // open filename.txt in append mode

    fileOUT << line << std::endl; // append "some stuff" to the end of the file

    fileOUT.close(); // close the file
}