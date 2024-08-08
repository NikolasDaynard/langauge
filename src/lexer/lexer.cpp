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
    remove((home + "/dev/lang/examples/hello.wbc").c_str()); // delete old wbc

    std::string line;

    while(true) {
        if (!getline(file, line)) {
            std::cout << std::endl;
            file.close();
            return line;
        }

        line = lexer::encodeLine(line);

        std::cout << "----+++-----" << std::endl;
        std::cout << line << std::endl;
        lexer::writeLine(line);
    }
}

void lexer::writeLine(std::string line) {
    std::string home = std::getenv("HOME");

    std::ofstream fileOUT(home + "/dev/lang/examples/hello.wbc", std::ios::app); // open filename.txt in append mode

    fileOUT << line << std::endl; // append "some stuff" to the end of the file

    fileOUT.close(); // close the file
}

std::string lexer::encodeLine(std::string line) {
    // breaks code down into

    // resultVar function functionName 3
    // call functionName param1 param2 
    // var variableName
    // foo add foo bar // sub div mul too, setting result into first param
    // set foo value

    std::string result = "";
    
    ltrim(line); // clear whitespace
    rtrim(line);
    line = removeWhitespaceNotInString(line);

    std::string keyword = std::string("");

    bool acceptingParams = false;
    bool readingString = false;

    for (char ch : line) {
        if (!isalnum(ch)){

            if (ch == '\"') {
                readingString = !readingString;
                if (!readingString) {
                    keyword = "\"" + keyword + "\"";
                }
            }
            if (readingString) {
                continue;
            }

            std::cout << keyword << std::endl;

            if (ch == ')') {
                acceptingParams = false;
            }

            if (acceptingParams) {
                if  (ch == '(') {
                    result = result + " call " + keyword;
                    acceptingParams = true;
                }else {
                    result = result + " " + keyword;
                }
            }else{
                // if it is ( it is a function call
                if  (ch == '(') {
                    result = "call " + keyword;
                    acceptingParams = true;
                }
            }

            keyword = "";
        }else {
            keyword = keyword + ch;
        }
    }

    return result;
}