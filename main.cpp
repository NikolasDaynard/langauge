#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

int main() {
    llvm::LLVMContext Context;
    llvm::Module* Module = new llvm::Module("hello", Context);

    llvm::IRBuilder<> Builder(Context);

    llvm::FunctionType *FuncType = llvm::FunctionType::get(Builder.getInt32Ty(), false);
    llvm::Function *MainFunction = llvm::Function::Create(FuncType, llvm::Function::ExternalLinkage, "main", Module);

    llvm::BasicBlock *EntryBB = llvm::BasicBlock::Create(Context, "entry", MainFunction);
    Builder.SetInsertPoint(EntryBB);

    llvm::Value *HelloWorld = Builder.CreateGlobalStringPtr("Hello, LLVM!\n");

    llvm::FunctionType *PrintfType = llvm::FunctionType::get(Builder.getInt32Ty(), true);
    llvm::FunctionCallee Printf = Module->getOrInsertFunction("printf", PrintfType);

    Builder.CreateCall(Printf, {HelloWorld});
    Builder.CreateRet(Builder.getInt32(0));

    llvm::verifyFunction(*MainFunction);

    std::error_code EC;
    llvm::raw_fd_ostream FileStream("output.ll", EC);
    if (EC) {
        llvm::errs() << "Error opening file: " << EC.message() << "\n";
        return 1;
    }

    Module->print(FileStream, nullptr);
    FileStream.close();

    delete Module;
    return 0;
}

