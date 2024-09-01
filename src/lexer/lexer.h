#pragma once

#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

/* datatype containing a line of code */
struct Line {
    std::string line;
};

/* compiles a source file in parsable wbc file */
class lexer {
private:
    std::string filename;
    int functionNests = 0;
public:
    /* returns lexed code */
    std::vector<std::string> parseFile();
    std::string encodeLine(std::string line);
    std::string postfixToLLVM(const std::vector<std::string>& postfix);
    void writeLine(std::string line);
    std::string readLine();

    // Constructor with parameters
    lexer(std::string newFilename) {
        filename = newFilename;
    }
    
    ~lexer() {
        // delete Module; (not responsible (LETS GO))
    }
};
