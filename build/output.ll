; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [9 x i8] c"var: %d\0A\00", align 1
@constStr.1 = private unnamed_addr constant [10 x i8] c"var1: %d\0A\00", align 1
@constStr.2 = private unnamed_addr constant [12 x i8] c"test   more\00", align 1

define i32 @main() {
entry:
  %var = alloca i32, align 4
  store i32 2, ptr %var, align 4
  %var1 = alloca i32, align 4
  store i32 63, ptr %var1, align 4
  %loadedInt = load i32, ptr %var, align 4
  %0 = call i32 (...) @printf(ptr @constStr, i32 %loadedInt)
  %loadedInt1 = load i32, ptr %var1, align 4
  %1 = call i32 (...) @printf(ptr @constStr.1, i32 %loadedInt1)
  %2 = call i32 (...) @printf(ptr @constStr.2)
  ret i32 0
}

declare i32 @printf(...)
