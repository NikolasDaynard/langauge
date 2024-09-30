#include <string>
#include <iostream>
#include <fstream>

class includer {
public:
    /* returns name to file */
    std::string linkExecutable(std::string file) {

    }
    void addFile(const std::string& file) {
        std::string home = std::getenv("HOME");
        // std::string fileDir = regex("./*/");
        
        std::string filePath = home + "/dev/lang/Wcache/files.txt";
        std::ofstream outFile(filePath, std::ios::app); 
        
        if (outFile.is_open()) {
            outFile << file << std::endl;
        } else {
            std::cerr << "Unable to open file: " << filePath << std::endl;
        }
    }
    includer () {

    }
};