#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "lexer.h"
#include "../parseHelpers.h"

std::string lexer::parseFile() {
    std::string home = std::getenv("HOME");
    std::ifstream file(home + "/dev/lang/examples/hello.w");

    std::string line;

    while(true) {
        if (!getline(file, line)) {
            std::cout << std::endl;
            file.close();
            return line;
        }

        line = lexer::encodeLine(line);
        std::cout << line << std::endl;
    }
}

std::string lexer::encodeLine(std::string line) {
    // breaks code down into

    // resultVar function functionName 3
    // call functionName param1 param2 
    // var variableName
    // foo add foo bar // sub div mul too, setting result into first param
    // set foo value
    
    ltrim(line); // clear whitespace
    rtrim(line);
    line = removeWhitespaceNotInString(line);

    std::string keyword = std::string("");

    for (char ch : line) {
        if (!isalnum(ch)){
            std::cout << keyword << std::endl;
            keyword = "";
        }else {
            keyword = keyword + ch;
        }
    }

    return line;
}