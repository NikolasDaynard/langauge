#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

#include "lexer.h"

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
        std::cout << line << std::endl;

        line = lexer::encodeLine(line);
    }
}

std::string lexer::encodeLine(std::string line) {
    return line;
}