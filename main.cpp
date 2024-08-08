#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "src/codegen.cpp"

int main(int argc, char *argv[]) {
    llvm::LLVMContext Context;
    llvm::Module* Module = new llvm::Module("test", Context);
    llvm::IRBuilder<> Builder(Context);

    codegen *gen = new codegen(Module);
    gen->parse("examples/hello.w");

    // gen->black_box();

    gen->write();
    gen->print();

    delete Module;

    if (argc > 1) { // run el codes
        int s;
        s = system("clang output.ll -o output");
        s = system("./output");
    }

    return 0;
}

