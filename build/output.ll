; ModuleID = 'hello'
source_filename = "hello"

@0 = private unnamed_addr constant [14 x i8] c"Hello, LLVM!\0A\00", align 1

define i32 @main() {
entry:
  %0 = call i32 (...) @printf(ptr @0)
  ret i32 0
}

declare i32 @printf(...)
