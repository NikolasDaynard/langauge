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
    functionInfo(llvm::IRBuilder<>::InsertPoint ip, int cid, std::map<std::string, llvm::BasicBlock *> bbs, llvm::Function *func, std::string id)
        : insertionPoint(ip), contextId(cid), function(func), name(id), basicBlocks(bbs) {}

    llvm::IRBuilder<>::InsertPoint insertionPoint; // function's ip
    llvm::Function *function; // the function ref
    std::string name;
    int contextId;
    std::map<std::string, llvm::BasicBlock *> basicBlocks;
    std::map<std::string, llvm::Value*> variableMap; // holds all vars, and context id
};

class parser {
private:
    std::vector<std::string> lexedCode;
    functions *function;

    std::string filename;
    llvm::Module *Module;
    llvm::IRBuilder<> *Builder;
    llvm::LLVMContext *Context;

    int functionNests = 0;
    int contextId = 0;
    llvm::FunctionCallee currentFunction;
    std::vector<llvm::Value *> currentArgs;
    std::vector<functionInfo> functionStack;
    std::map<std::string, llvm::Value *> strings;
public:
    llvm::Value *createVariable(std::string name, std::string value, std::size_t i);
    llvm::Value* getVariable(const std::string& name);
    llvm::Value* getLoadVariable(const std::string& name);
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
        functionStack.push_back(functionInfo(Builder->saveIP(), contextId, {}, nullptr, "main"));
    }
    
    ~parser() {
        // delete Module; (not responsible (LETS GO))
    }
};