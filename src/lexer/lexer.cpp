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

std::vector<std::string> separateTokens(const std::string& line) {
    std::string temp = "";
    std::vector<std::string> output;
    bool inString = false;
    
    for(char ch : line) {
        if (ch == '\"') {
            inString = !inString;
        }

        if (ch == ' ' && !inString) {
            if (!temp.empty()) {  // Avoid adding empty tokens
                output.push_back(temp);
                temp = "";
            }
        } else {
            temp += ch;
        }
    }
    
    // Add the last token if any
    if (!temp.empty()) {
        output.push_back(temp);
    }
    
    return output;
}

std::vector<std::string> lexer::parseFile() {
    std::string home = std::getenv("HOME");
    std::ifstream file(home + "/dev/lang/" + filename);
    remove((home + "/dev/lang/" + filename + "bc").c_str()); // delete old wbc

    std::string line;
    std::vector<std::string> lexedCode;

    while(true) {
        if (!getline(file, line)) {
            std::cout << std::endl;
            file.close();
            break;
        }

        line = lexer::encodeLine(line);

        std::cout << "----+++-----" << std::endl;
        std::cout << line << std::endl;
        lexer::writeLine(line);

        for (std::string token : separateTokens(line)) {
            lexedCode.push_back(token);
        }
        lexedCode.push_back("\n"); // add a new line char for termination 
    }

    return lexedCode;
}

void lexer::writeLine(std::string line) {
    std::string home = std::getenv("HOME");

    std::ofstream fileOUT(home + "/dev/lang/" + filename + "bc", std::ios::app); // open filename.txt in append mode

    fileOUT << line << std::endl; // append "some stuff" to the end of the file

    fileOUT.close(); // close the file
}

std::string lexer::encodeLine(std::string line) {
    line = line + ";"; // manually insert semi-colon

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

    if (line.length() >= 2) {
        if (line.c_str()[0] == '-' && line.c_str()[1] == '-') {
            return "";
        }
    }

    std::string keyword = std::string("");

    bool acceptingParams = false;
    bool settingVar = false; // var = z
    bool readingString = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char ch = line[i];

        if (!isalnum(ch) || i == line.length() - 1){

            if (ch == '\"') { // string nonsense
                readingString = !readingString;
                if (!readingString) {
                    keyword = "\"" + keyword + "\"";
                }
            }else if (readingString) {
                keyword = keyword + ch;
                continue;
            }

            // math keyword insertion
            if (ch == '+') {
                keyword = " add " + keyword;
            }else if (ch == '-') {
                keyword = " sub " + keyword;
            }else if (ch == '*') {
                keyword = " mul " + keyword;
            }else if (ch == '/') {
                keyword = " div " + keyword;
            }
            
            std::cout << i << std::endl;
            std::cout << keyword << std::endl;

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
                }else if (ch == ')') {
                    acceptingParams = false;
                }else if(ch == '=') {
                    settingVar = true;
                    acceptingParams = true;
                    result = "set " + keyword;
                }
            }

            keyword = "";
        }else {
            keyword = keyword + ch;
        }
    }

    return result;
}