; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [5 x i8] c"test\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %0 = call i32 (double, ...) @foo(double 1.000000e+00)
  %tmp0 = alloca i32, align 4
  store i32 %0, ptr %tmp0, align 4
  ret i32 0
}

define i32 @foo(double %0, ...) {
main:
  %num = alloca double, align 8
  store double %0, ptr %num, align 8
  %1 = call i32 (...) @printf(ptr @constStr)
  %tmp0 = alloca i32, align 4
  store i32 %1, ptr %tmp0, align 4
  ret i32 0
}

declare i32 @printf(...)
