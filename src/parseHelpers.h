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
