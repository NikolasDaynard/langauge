; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [13 x i8] c"hellow world\00", align 1
@constStr.1 = private unnamed_addr constant [3 x i8] c"%f\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %0 = call i32 (...) @printf(ptr @constStr)
  %tmp0 = alloca i32, align 4
  store i32 %0, ptr %tmp0, align 4
  %1 = call i32 (...) @printf(ptr @constStr.1, double 3.000000e+00, double 4.000000e+00)
  store i32 %1, ptr %tmp0, align 4
  %loadedVar = load i32, ptr %tmp0, align 4
  %2 = call i32 (...) @printf(i32 %loadedVar)
  %tmp1 = alloca i32, align 4
  store i32 %2, ptr %tmp1, align 4
  ret i32 0
}

declare i32 @printf(...)
