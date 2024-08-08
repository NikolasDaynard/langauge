#pragma once

#include <iostream>
#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "functions.h"

std::optional<llvm::FunctionCallee> functions::getStdlibFunction(std::string name) {
    llvm::FunctionType *PrintfType = llvm::FunctionType::get(Builder->getInt32Ty(), true);
    llvm::FunctionCallee Printf = Module->getOrInsertFunction("printf", PrintfType);
    if (name == "print") {
        return Printf;
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