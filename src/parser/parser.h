#pragma once

#include <string>
#include <map>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

class parser {
private:
    std::vector<std::string> lexedCode;
    functions *function;

    std::string filename;
    llvm::Module *Module;
    llvm::IRBuilder<> *Builder;
    llvm::LLVMContext *Context;

    std::map<std::string, llvm::Value*> variableMap; // holds all vars
public:
    llvm::Value *createVariable(std::string name, std::string value, bool reading);
    llvm::Value* getVariable(const std::string& name);
    llvm::Value *evaluateValue(std::string name, std::string value);
    
    std::string parseFile();
    void writeLine(std::string line);

    // Constructor with parameters
    parser(std::vector<std::string> newCode, std::string newFilename, llvm::Module *Mod, llvm::IRBuilder<> *Build, llvm::LLVMContext *Con) {
        lexedCode = newCode;
        Module = Mod;
        Builder = Build;
        Context = Con;
        filename = newFilename;
        function = new functions(Module, Builder);
    }
    
    ~parser() {
        // delete Module; (not responsible (LETS GO))
    }
};