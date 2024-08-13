// clang -S -emit-llvm main.c -o main.ll
#include <stdio.h>
#include "test.h"

int main() {
    double num = 4;
    // num = black_box(num);
    // printf("%f", num);
    black_box(num);
    if (num == 2) {
        printf("%f", num);
    }else {
        printf("test");
    }
    return 0;
}