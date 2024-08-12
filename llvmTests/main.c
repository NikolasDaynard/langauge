// clang -S -emit-llvm main.c -o main.ll
#include <stdio.h>

int main() {
    float num = 4;
    printf("%f", num);
}