#include <string>
#include <iostream>
#include <fstream>

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "codegen.h"
#include "parser/parser.cpp"
#include "lexer/lexer.cpp"
#include "include/include.cpp"

int codegen::print() {
    Module->print(llvm::outs(), nullptr);
    return 0;
}

int codegen::write() {
    std::error_code EC;
    llvm::raw_fd_ostream FileStream("output.ll", EC);
    if (EC) {
        llvm::errs() << "Error opening file: " << EC.message() << "\n";
        return 1;
    }

    Module->print(FileStream, nullptr);
    FileStream.close();    

    return 0;
}

int codegen::parse(std::string filename) {
    // includer *inc = new includer();
    // inc->addFile("test");
    lexer *lex = new lexer(filename);
    std::vector<std::string> lexedCode = lex->parseFile();

    parser *parse = new parser(lexedCode, filename, Module, &Builder, &Context);
    std::string irFile = parse->parseFile();

    free(lex);
    free(parse);

    return 0;
}