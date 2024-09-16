#pragma once
#include <algorithm> 
#include <cctype>
#include <locale>

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

bool isMathKeyword(std::string str) {
    return str == "add" || str == "sub" || str == "div" || str == "mul" || str == "cmp" || str == "les" || str == "pow";
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
