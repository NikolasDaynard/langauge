; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [3 x i8] c"%f\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %0 = call i32 (...) @printf(ptr @constStr, double 2.434000e+03)
  %tmp0 = alloca i32, align 4
  store i32 %0, ptr %tmp0, align 4
  ret i32 0
}

declare i32 @printf(...)
