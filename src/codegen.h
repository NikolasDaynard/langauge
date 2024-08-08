#pragma once

#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

class codegen {
private: 
    llvm::Module* Module;
    llvm::LLVMContext Context;
    llvm::IRBuilder<> Builder;

public:
    int print();
    int write();
    void black_box();
    int parse(std::string filename);

    llvm::Module* getModule();
    llvm::IRBuilder<>& getBuilder(); // return refrence to non-copyable class
    
    // Constructor with parameters
    codegen(llvm::Module* Mod) 
        : Module(Mod),
          Builder(Context) // Initialize Builder using Context
    {

        llvm::FunctionType *FuncType = llvm::FunctionType::get(Builder.getInt32Ty(), false);
        llvm::Function *MainFunction = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, "main", Module);

        llvm::BasicBlock *EntryBB = llvm::BasicBlock::Create(Context, "entry", MainFunction);
        Builder.SetInsertPoint(EntryBB);
    }
    
    ~codegen() {
        // delete Module; (not responsible (LETS GO))
    }
};
