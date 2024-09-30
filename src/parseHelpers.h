#pragma once
#include <algorithm> 
#include <cctype>
#include <locale>
#include <string>
#include <vector>
#include <map>

// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

inline std::string removeWhitespaceNotInString(std::string &str) {
    bool readingString = false;
    std::string result = "";
    for (char ch : str) {
        if (ch == '\"') {
            readingString = !readingString;
        }
        if (ch == ' ') {
            if (readingString) {
                result = result + " ";
            }
        }else{
            result = result + ch;
        }
    }
    return result;
}

void replaceAllNotInString(std::string &str, const std::string &from, const std::string &to) {
    size_t startPos = 0;
    bool inString = false;
    char quoteChar = '\0';

    while (startPos < str.length()) {
        if (!inString && (str[startPos] == '"' || str[startPos] == '\'')) {
            inString = true;
            quoteChar = str[startPos];
        } else if (inString && str[startPos] == quoteChar) {
            inString = false;
        }

        if (!inString && str.find(from, startPos) == startPos) {
            str.replace(startPos, from.length(), to);
            startPos += to.length(); // Advance startPos to avoid infinite loop if 'to' contains 'from'
        } else {
            ++startPos;
        }
    }
}

void replaceAll(std::string &str, const std::string &from, const std::string &to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Advance startPos to avoid infinite loop if 'to' contains 'from'
    }
}

struct token {
    int precedence;
    std::string name;
    std::string association;
    bool isMathKeyword;
    token(std::string n, int prec, bool isMath) {
        name = n;
        precedence = prec;
        isMathKeyword = isMath;
        association = "";
    }
    token(std::string n, std::string assoc, int prec, bool isMath) {
        name = n;
        precedence = prec;
        isMathKeyword = isMath;
        association = assoc;
    }
};

class tokenHolder {
public:
    std::map<std::string, token> tokens;
    std::vector<token> tokenVector;
    std::map<std::string, int> precedenceMap;
    std::map<std::string, std::string> associationMap;

    tokenHolder() {
        tokenVector = {
            token("^", "pow", 6, true),
            token("*", "mul", 5, true),
            token("/", "div", 4, true),
            token("+", "add", 3, true),
            token("-", "sub", 2, true),
            token("==", "cmp", 1, true),
            token("<", "les", 1, true),
            token(">", "grt", 1, true),
            token("=", "set", 0, true),
            token("{", -1, true),
            token("}", -2, true)
        };
        for (const auto& tok : tokenVector) {
            tokens.emplace(tok.name, tok);
            precedenceMap[tok.name] = tok.precedence;
            associationMap[tok.name] = tok.association;
        }
    }
};

bool isMathKeyword(std::string key, tokenHolder holder) {
    for (token tok : holder.tokenVector) {
        if (tok.association == key && tok.isMathKeyword) {
            return true;
        }
    }
    return false;
}
