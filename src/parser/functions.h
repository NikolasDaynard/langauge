#pragma once

#include <string>
#include <map>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/BasicBlock.h>

#include "functions.h"

/* defines one line of code */
class functionWrapper {
private:
    std::optional<llvm::FunctionCallee> getStdlibFunction(std::string name);
    llvm::Module *Module;
    llvm::IRBuilder<> *Builder;
    std::map<std::string, llvm::Function *> functionMap;
public:

    llvm::FunctionCallee getFunction(std::string name);
    void initStdLib();
    void addFunction(std::string name, llvm::Function *func);
    
    // Constructor with parameters
    functionWrapper(llvm::Module *Mod, llvm::IRBuilder<> *Build) {
        Module = Mod;
        Builder = Build;
        functionMap = {};
        initStdLib();
    }
    
    ~functionWrapper() {
        // delete Module; (not responsible (LETS GO))
    }
};
