#pragma once

#include <string>
#include <map>
#include <stack>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

struct functionInfo {
    functionInfo(llvm::IRBuilder<>::InsertPoint ip, std::map<std::string, llvm::BasicBlock *> bbs, llvm::Function *func, std::string id)
        : insertionPoint(ip), function(func), name(id), basicBlocks(bbs) {}

    llvm::IRBuilder<>::InsertPoint insertionPoint; // function's ip
    llvm::Function *function; // the function ref
    std::string name;
    std::map<std::string, llvm::BasicBlock *> basicBlocks;
};

class parser {
private:
    std::vector<std::string> lexedCode;
    functions *function;

    std::string filename;
    llvm::Module *Module;
    llvm::IRBuilder<> *Builder;
    llvm::LLVMContext *Context;

    std::map<std::string, llvm::Value*> variableMap; // holds all vars

    int functionNests = 0;
    llvm::FunctionCallee currentFunction;
    std::vector<llvm::Value *> currentArgs;
    std::stack<functionInfo> functionStack;
public:
    llvm::Value *createVariable(std::string name, std::string value, std::size_t i);
    llvm::Value* getVariable(const std::string& name);
    llvm::Value *evaluateValue(std::string name, std::string value, std::size_t i);
    void evaluateConditional(std::string name, std::string value, std::size_t i);

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
        functionStack.push(functionInfo(Builder->saveIP(), {}, nullptr, "main"));
    }
    
    ~parser() {
        // delete Module; (not responsible (LETS GO))
    }
};