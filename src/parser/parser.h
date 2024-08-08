#pragma once

#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

class parser {
private:
    std::vector<std::string> lexedCode;
    functions *function;

    llvm::Module *Module;
    llvm::IRBuilder<> *Builder;
    llvm::LLVMContext *Context;
public:
    llvm::Value *createVariable(std::string name, std::string value);
    std::string parseFile();
    void writeLine(std::string line);

    // Constructor with parameters
    parser(std::vector<std::string> newCode, llvm::Module *Mod, llvm::IRBuilder<> *Build, llvm::LLVMContext *Con) {
        lexedCode = newCode;
        Module = Mod;
        Builder = Build;
        Context = Con;
        function = new functions(Module, Builder);
    }
    
    ~parser() {
        // delete Module; (not responsible (LETS GO))
    }
};