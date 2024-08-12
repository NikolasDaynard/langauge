; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [3 x i8] c"%p\00", align 1

define i32 @main() {
entry:
  %var2 = alloca i32, align 4
  store i32 6, ptr %var2, align 4
  %0 = call i32 (...) @printf(ptr @constStr, ptr %var2)
  ret i32 0
}

declare i32 @printf(...)
