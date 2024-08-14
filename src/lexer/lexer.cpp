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
#include <stack>


std::vector<std::string> separateTokens(const std::string& line) {
    std::string temp = "";
    std::vector<std::string> output;
    bool inString = false;
    
    for(char ch : line) {
        if (ch == '\"') {
            inString = !inString;
        }

        if ((ch == ' ' && !inString) || ch == '\n') {
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

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    bool readingString = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char ch = line[i];

        if (ch == '\"') {
            readingString = !readingString;
            token += ch;
            if (!readingString) {
                tokens.push_back(token);
                token.clear();
            }
            continue;
        }

        if (readingString) {
            token += ch;
            continue;
        }

        if (isalnum(ch) || ch == '.' || ch == '_') {
            token += ch;
        } else {
            if (ch == '(' && isalnum(line[i - 1])) { // function calls
                token += ch;
            }

            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            if (!isspace(ch)) {
                if (ch == '=' && i + 1 != line.length() && line[(int)std::min(line.length() - 1, i + 1)] == '=') { // special case for ==
                    tokens.push_back(std::string(1, ch) + std::string(1, line[i++]));
                }else{
                    tokens.push_back(std::string(1, ch));
                }
            }

        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

/** Evaluates the tokens to generate an optimized list of token placed in operational order */
std::vector<std::string> shuntingYard(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<std::string> operators;

    std::map<std::string, int> precedence = { // PEMDAS
        {"^", 6}, {"*", 5}, {"/", 4}, {"+", 3}, {"-", 2}, {"==", 1}, {"=", 0}
    };

    for (const std::string& token : tokens) {
        std::cout << "sy: " << token << std::endl;
        if (isalnum(token[0]) || token[0] == '\"') {
            output.push_back(token);
        } else if (precedence.find(token) != precedence.end() && token != "=") {
            while (!operators.empty() && precedence[operators.top()] >= precedence[token]) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.pop(); // Pop '('
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

std::string postfixToLLVM(const std::vector<std::string>& postfix) {
    std::stack<std::string> evalStack;
    std::string result;
    std::string originalVar = postfix[0];
    int tempVarCounter = 0;  // Counter for temporary variables

    std::map<std::string, std::string> associations = {
        {"^", "pow"}, {"*", "mul"}, {"/", "div"}, {"+", "add"}, {"-", "sub"}, {"=", "set"}, {"==", "cmp"}
    };

    for (const std::string& token : postfix) {
        if (isalnum(token[0]) || token[0] == '\"') {
            evalStack.push(token);
        } else {
            std::string rhs = evalStack.top(); evalStack.pop();
            std::string lhs = evalStack.top(); evalStack.pop();

            std::string tempVar = "tmp" + std::to_string(tempVarCounter++);
            result += "set " + tempVar + " " + 
                (associations.find(token) != associations.end() ? associations.find(token)->second : "") + 
                " " + lhs + " " + rhs + "\n";

            evalStack.push(tempVar);
        }
    }
    if (originalVar.back() != '(') { // not call
        std::string finalResult = evalStack.top();
        evalStack.pop();

        result += "set " + originalVar + " " + finalResult + "\n";
    }else{
        std::string finalResult;
        std::string arguments = "";

        while (evalStack.size() > 1) {
            finalResult = evalStack.top();
            evalStack.pop();
            std::cout << "res: " << finalResult << std::endl; 

            arguments = finalResult + " " + arguments; // invert because it's top element
        }
        originalVar.pop_back(); // remove '(' char
        result = result + "call " + originalVar + " " + arguments;

        result += "\n";
    }
    return result;
}


std::string lexer::encodeLine(std::string line) {
    if (line == "") { return ""; };

    line = line + ";"; // manually insert semi-colon
    ltrim(line);
    rtrim(line);

    // replaceAllNotInString(line, "if ", "");
    replaceAllNotInString(line, " then", "");
    line = removeWhitespaceNotInString(line);

    if (line.length() >= 2) {
        if (line.c_str()[0] == '-' && line.c_str()[1] == '-') {
            return "";
        }
    }

    std::vector<std::string> tokens = tokenize(line);
    for (std::string tok : tokens) {
        std::cout << "tok: " << tok << std::endl;
    }
    std::vector<std::string> postfix = shuntingYard(tokens);
    for (std::string pf : postfix) {
        std::cout << "pf: " << pf << std::endl;
    }
    std::string result = postfixToLLVM(postfix);
    
    std::cout << "op: " << result << std::endl;

    return result;
}
