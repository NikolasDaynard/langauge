#pragma once

#include <string>
#include <map>
#include <stack>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

struct Variable {
    llvm::Value *val;
    llvm::Type *type;

    // Default constructor
    Variable() : val(nullptr), type(nullptr) {}

    // Constructor that sets both value and type based on llvm::Value
    Variable(llvm::Value* v) : val(v), type(v != nullptr ? v->getType() : nullptr) {}
};


struct contextInfo {
    contextInfo(llvm::IRBuilder<>::InsertPoint ip, int cid, std::map<std::string, llvm::BasicBlock *> bbs, llvm::Function *func, std::string id)
        : insertionPoint(ip), contextId(cid), function(func), name(id), basicBlocks(bbs) {}

    llvm::IRBuilder<>::InsertPoint insertionPoint; // function's ip
    llvm::Function *function; // the function ref
    std::string name;
    llvm::BasicBlock *mergeRet = nullptr; // where to return on a endcond
    int contextId;
    std::map<std::string, llvm::BasicBlock *> basicBlocks;
    std::map<std::string, Variable> variableMap; // holds all vars, and context id
};

class parser {
private:
    std::vector<std::string> lexedCode;
    functionWrapper *functionsWrapper;

    std::string filename;
    llvm::Module *Module;
    llvm::IRBuilder<> *Builder;
    llvm::LLVMContext *Context;

    int functionNests = 0;
    int contextId = 0;
    llvm::FunctionCallee currentFunction;
    std::vector<llvm::Value *> currentArgs;
    std::vector<contextInfo> contextStack;
    
    std::map<std::string, llvm::Value *> strings;
    std::map<std::string, int> functionPositions; // stores function positions in the lexedCode
public:
    llvm::Value *createVariable(std::string name, std::string value, std::size_t i);
    Variable getVariable(const std::string& name);
    llvm::Value* getLoadVariable(const std::string& name);
    llvm::Value *evaluateValue(std::string name, std::string value, std::size_t i);
    void evaluateConditional(std::string name, std::string value, std::size_t i);
    void evaluateFunction(std::string name, std::string value, std::size_t i);
    int handleString(std::size_t i);

    std::string parseFile();
    void writeLine(std::string line);

    // Constructor with parameters
    parser(std::vector<std::string> newCode, std::string newFilename, llvm::Module *Mod, llvm::IRBuilder<> *Build, llvm::LLVMContext *Con) {
        lexedCode = newCode;
        Module = Mod;
        Builder = Build;
        Context = Con;
        filename = newFilename;
        functionsWrapper = new functionWrapper(Module, Builder);
        contextStack.push_back(contextInfo(Builder->saveIP(), contextId, {}, Builder->GetInsertBlock()->getParent(), "main"));
    }
    
    ~parser() {
        // delete Module; (not responsible (LETS GO))
    }
};