#pragma once

#include <iostream>
#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "functions.h"

void functions::initStdLib() {
    // llvm::FunctionType *PrintfType = llvm::FunctionType::get(Builder->getInt32Ty(), true);
    // llvm::FunctionCallee Printf = Module->getOrInsertFunction("printf", PrintfType);

    // llvm::FunctionType *FuncType = llvm::FunctionType::get(Builder->getInt32Ty(), {Builder->getInt32Ty()}, false);
    // llvm::Function *MyFunction = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, "my_function", Module);

    // // Get the function argument (an integer).
    // llvm::Function::arg_iterator Args = MyFunction->arg_begin();
    // llvm::Value *InputValue = Args++;

    // // Create the format string for `printf`.
    // llvm::Value *FormatStr = Builder->CreateGlobalStringPtr("%d\n");

    // // Call `printf` with the format string and the integer value.
    // Builder->CreateCall(Printf, {FormatStr, InputValue});

    // // Return the integer value.
    // Builder->CreateRet(InputValue);

    // llvm::verifyFunction(*MyFunction);
}

std::optional<llvm::FunctionCallee> functions::getStdlibFunction(std::string name) {
    llvm::FunctionType *retI32 = llvm::FunctionType::get(Builder->getInt32Ty(), true);
    llvm::FunctionType *retDouble = llvm::FunctionType::get(Builder->getDoubleTy(), true);
    if (name == "print") {
        llvm::FunctionCallee Printf = Module->getOrInsertFunction("printf", retI32);
        return Printf;
    }
    if (name == "free") {
        llvm::FunctionCallee Free = Module->getOrInsertFunction("free", retI32);
        return Free;
    }

    if (name == "pow") {
        llvm::FunctionCallee Pow = Module->getOrInsertFunction("pow", retDouble);
        return Pow;
    }

    return std::nullopt;
}

llvm::FunctionCallee functions::getFunction(std::string name) {
    // Try to get the function from the standard library
    std::optional<llvm::FunctionCallee> function = getStdlibFunction(name);

    if (function.has_value()) {
        std::cout << "Function found in the standard library." << std::endl;
        return function.value();
    } else {
        std::cout << "Function not found in the standard library." << std::endl;
    }
    return llvm::FunctionCallee();
}