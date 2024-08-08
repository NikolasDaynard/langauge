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
    functions function;
public:
    /* returns path to new lexed code */
    std::string parseFile();
    void writeLine(std::string line);

    // Constructor with parameters
    parser(std::vector<std::string> newCode) {
        lexedCode = newCode;
    }
    
    ~parser() {
        // delete Module; (not responsible (LETS GO))
    }
};