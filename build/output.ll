; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [24 x i8] c"hello from other got %f\00", align 1
@constStr.1 = private unnamed_addr constant [8 x i8] c"Test\0A\0A\0A\00", align 1
@constStr.2 = private unnamed_addr constant [9 x i8] c"\0Atest %f\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %0 = call i32 (double, ...) @other(double 2.000000e+00)
  %tmp0 = alloca i32, align 4
  store i32 %0, ptr %tmp0, align 4
  %1 = call i32 (...) @printf(ptr @constStr.1)
  store i32 %1, ptr %tmp0, align 4
  %2 = call i32 (double, double, ...) @test(double 2.000000e+00, double 3.000000e+00)
  store i32 %2, ptr %tmp0, align 4
  ret i32 0
}

define i32 @other(double %0, ...) {
main:
  %num = alloca double, align 8
  store double %0, ptr %num, align 8
  %loadedVar = load double, ptr %num, align 8
  %1 = call i32 (...) @printf(ptr @constStr, double %loadedVar)
  %tmp0 = alloca i32, align 4
  store i32 %1, ptr %tmp0, align 4
  ret i32 0
}

declare i32 @printf(...)

define i32 @test(double %0, double %1, ...) {
main:
  %arg1 = alloca double, align 8
  store double %0, ptr %arg1, align 8
  %arg2 = alloca double, align 8
  store double %1, ptr %arg2, align 8
  %loadedVar = load double, ptr %arg1, align 8
  %2 = call i32 (double, ...) @other(double %loadedVar)
  %tmp0 = alloca i32, align 4
  store i32 %2, ptr %tmp0, align 4
  %loadedVar1 = load double, ptr %arg1, align 8
  %3 = call i32 (...) @printf(ptr @constStr.2, double %loadedVar1)
  store i32 %3, ptr %tmp0, align 4
  %loadedVar2 = load double, ptr %arg2, align 8
  %4 = call i32 (...) @printf(ptr @constStr.2, double %loadedVar2)
  store i32 %4, ptr %tmp0, align 4
  ret i32 0
}
