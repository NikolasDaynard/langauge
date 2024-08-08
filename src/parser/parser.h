#pragma once

#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

class parser {
private:
    std::string filename;
public:
    /* returns path to new lexed code */
    std::string parseFile();

    // Constructor with parameters
    parser(std::string newFilename) {
        filename = newFilename;
    }
    
    ~parser() {
        // delete Module; (not responsible (LETS GO))
    }
};