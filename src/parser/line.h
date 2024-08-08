#pragma once

#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_ostream.h>

/* defines one line of code */
class line {
public:
    
    
    // Constructor with parameters
    line(llvm::Module* Mod) {

    }
    
    ~line() {
        // delete Module; (not responsible (LETS GO))
    }
};
