#pragma once
#include <string>
#include "src/codegen.cpp"

class codegen {
public:
    int parse(std::string filename);
    codegen();
};