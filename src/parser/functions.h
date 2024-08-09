#pragma once

#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "functions.h"

/* defines one line of code */
class functions {
private:
    std::optional<llvm::FunctionCallee> getStdlibFunction(std::string name);
    llvm::Module *Module;
    llvm::IRBuilder<> *Builder;
public:

    llvm::FunctionCallee getFunction(std::string name);
    void initStdLib();
    
    
    // Constructor with parameters
    functions(llvm::Module *Mod, llvm::IRBuilder<> *Build) {
        Module = Mod;
        Builder = Build;
        initStdLib();
    }
    
    ~functions() {
        // delete Module; (not responsible (LETS GO))
    }
};
