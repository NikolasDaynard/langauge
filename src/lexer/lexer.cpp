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
#include <map>


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
    int functionNests = 0;

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
                token = "#" + token; // #" is a function string
                functionNests++;
            }
            if (functionNests != 0 && ch == ')') {
                token = token + "#"; // insert function end char
                functionNests--;
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
        if (isalnum(token[0]) || token[0] == '\"' || token[0] == '#') {
            output.push_back(token);
        } else if (precedence.find(token) != precedence.end()) {
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

    for (size_t i = 0; i < postfix.size(); ++i) {
        const std::string &token = postfix[i];
        std::cout << "parsing token" << token << std::endl;
        if ((isalnum(token[0]) || token[0] == '\"') && token.back() != '#') {
            evalStack.push(token);
        } else {
            if (token.front() == '#') { // call TODO: this is janky
                int functionNests = 1;
                std::string arguments = "";
                i++;

                while (true) {
                    std::cout << "res: " << postfix[i] << std::endl; 
                    
                    if (postfix[i].back() == '#') { // This has to be default case for blank "#" lines
                        arguments += " " + postfix[i].substr(0, postfix[i].length() - 1) + " end"; // invert because it's top elements
                        std::cout << "break on" << postfix[i].substr(0, postfix[i].length() - 1) << std::endl;
                        functionNests--;
                        if (functionNests == 0) {
                            break;
                        }
                    }else if (postfix[i].front() == '#') {
                        arguments += " call " + postfix[i].substr(1, postfix[i].length());
                        functionNests++;
                    }else{
                        arguments += " " + postfix[i]; // invert because it's top element
                    }

                    i++;
                }
                
                evalStack.push("call " + token.substr(1, token.length()) + arguments + "\n");
                // if this is true, there is no set, so pushing to the eval stack does nothing
                // Meaning, we have to do it ourselves, += just in case someone wants to do something with this
                if (originalVar == token) { 
                    result += evalStack.top();
                    std::cout << "func call: " << "call " + token.substr(1, token.length()) + arguments + "\n" << std::endl;
                }
                continue;
            }
            std::string rhs = evalStack.top(); evalStack.pop();
            std::string lhs = evalStack.top(); evalStack.pop();

            if (token != "=") {
                result += "set tmp" + std::to_string(tempVarCounter) + " ";
            }

            // technically this fails on the last set, but because lua doesn't have double assigment, we chillin 
            evalStack.push("tmp" + std::to_string(tempVarCounter++)); 

            result += (associations.find(token) != associations.end() ? associations.find(token)->second : "") + 
                    " " + lhs + " " + rhs + "\n";
        }
    }

    // for (std::string token : postfix) {
    // }
    return result;
}


std::string lexer::encodeLine(std::string line) {
    if (line == "") { return ""; };

    line = line + ";"; // manually insert semi-colon
    ltrim(line);
    rtrim(line);

    // replaceAllNotInString(line, "if", "if (");
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
