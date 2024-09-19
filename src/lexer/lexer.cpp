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
            std::cout << std::endl; // \n
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
    if (line == "") { return; }
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
    std::string braceType = ""; // if{, while{, etc.

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

        if (token.substr(0, std::min(2, (int)token.length())) == "if") {
            token = token.substr(2, token.length());
            braceType = "if{";
        }else if (token.substr(0, std::min(5, (int)token.length())) == "while") {
            token = token.substr(5, token.length());
            braceType = "while{";
        }

        if (isalnum(ch) || ch == '.' || ch == '_') {
            token += ch;
        } else {
            if (ch == '-' && !isalnum(line[i - 1])) { // negative number (0 - num)
                tokens.insert(tokens.end(), {"(", "0", "-", std::string(1, line[++i]), ")"});
                continue;
            }

            if (ch == '(') { // function calls
                if (isalnum(line[i - 1])) {
                    if (line[line.length() - 2] == '{') {
                        std::cout << "function def" << std::endl;
                        token = "$" + token; // $ is a function def
                        functionNests++;
                    }else{
                        token = "#" + token; // # is a function string
                        functionNests++;
                    }
                }
            }
            if (ch == ')') {
                if (functionNests != 0) {
                    tokens.push_back(token);
                    tokens.push_back(",");
                    tokens.push_back("#");
                    token.clear();
                }
                functionNests--;
            }
            if (ch == '{') {
                // push last token
                ch = ' ';
                tokens.push_back(token);
                // make the new brace
                token = braceType;
                braceType = "";
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
        {"^", 6}, {"*", 5}, {"/", 4}, {"+", 3}, {"-", 2}, {"==", 1}, {"<", 1}, {"=", 0}, {"{", -1}, {"}", -2}
    };

    for (const std::string& token : tokens) {
        std::cout << "sy: " << token << std::endl;
        if ((isalnum(token[0]) || token[0] == '\"' || token[0] == '#' || token[0] == '$') && token.back() != '{') {
            output.push_back(token);
        } else if (precedence.find(token) != precedence.end()) {
            while (!operators.empty() && precedence[operators.top()] >= precedence[token]) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }else if (token.back() == '{') {
            while (!operators.empty()) {
                output.push_back(operators.top());
                operators.pop();
            }
            output.push_back(token);
        }else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.pop(); // Pop '('
        } else if (token == ",") { // separate args into seprate token blocks
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            output.push_back(token);
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

std::string lexer::postfixToLLVM(const std::vector<std::string>& postfix) {
    std::stack<std::string> evalStack;
    std::stack<std::string> functionNames;
    std::string result;
    std::string originalVar = postfix[0];
    int tempVarCounter = 0;  // Counter for temporary variables

    std::map<std::string, std::string> associations = {
        {"^", "pow"}, {"*", "mul"}, {"/", "div"}, {"+", "add"}, {"-", "sub"}, {"=", "set"}, {"==", "cmp"}, {"<", "les"}
    };

    for (size_t i = 0; i < postfix.size(); ++i) {
        const std::string &token = postfix[i];
        std::cout << "parsing token " << token << std::endl;

        if (token == ",") {
            continue;
        }
        if (token == "else") {
            result += token;
            continue;
        }

        if (token.back() == '#') {
            std::string args = "";
            bool isCond = false;
            while (true) {
                args = evalStack.top() + " " + args; // invert args
                evalStack.pop();
                if (evalStack.top() == "|endcond") {
                    isCond = true;
                    break;
                }else if (evalStack.top() == "|endfunc") {
                    isCond = false;
                    break;
                }
            }
            if (isCond) {
                result += "set tmp" + std::to_string(tempVarCounter) + " ";
                result += "call " + functionNames.top();
            }else {
                result += "func " + functionNames.top();
            }
            functionNames.pop();
            result += " " + args + " " + token.substr(0, token.length() - 1) + " end\n";
            evalStack.push("tmp" + std::to_string(tempVarCounter++)); 
            continue;
        }

        if ((isalnum(token[0]) || token[0] == '\"') && token.back() != '#' && token.back() != '{') {
            evalStack.push(token);
        } else {
            if (token.front() == '#' || token.front() == '$') {
                if (functionNames.size() == 0) { // if this is the first function call, safeguard the stack
                    if (token.front() == '#') {
                        evalStack.push("|endcond");
                    }else if (token.front() == '$') {
                        evalStack.push("|endfunc");
                    }
                }
                functionNames.push(token.substr(1, token.length()));
                continue;
            }
            if (token.back() == '{') {
                // add the while / if / for
                std::string prefix = "";
                for (char c : token) {
                    if (c != '{') { 
                        result += c; // push if or while to back of cond
                        prefix += c; // add to both so
                        // while 
                        // while cond tmp0
                    }else{
                        break;
                    }
                }
                result = prefix + " condblock do\n" + result;
                result +=  " cond " + evalStack.top();
                continue;
            }
            if (token == "}") {
                result += "endcond";
                continue;
            }

            std::string rhs = evalStack.top(); evalStack.pop();

            if (evalStack.top() == "|") { // pop safeguard
                evalStack.pop();
            }
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
