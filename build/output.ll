; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [7 x i8] c"str %f\00", align 1
@constStr.1 = private unnamed_addr constant [4 x i8] c"\0A%f\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %0 = call i32 (ptr, double, double, ...) @foo(ptr @constStr, double 4.000000e+00, double 3.000000e+00)
  %tmp0 = alloca i32, align 4
  store i32 %0, ptr %tmp0, align 4
  ret i32 0
}

define i32 @foo(ptr %0, double %1, double %2, ...) {
main:
  %num = alloca ptr, align 8
  store ptr %0, ptr %num, align 8
  %str = alloca double, align 8
  store double %1, ptr %str, align 8
  %bool = alloca double, align 8
  store double %2, ptr %bool, align 8
  %loadedVar = load ptr, ptr %num, align 8
  %loadedVar1 = load double, ptr %str, align 8
  %3 = call i32 (...) @printf(ptr %loadedVar, double %loadedVar1)
  %tmp0 = alloca i32, align 4
  store i32 %3, ptr %tmp0, align 4
  %loadedVar2 = load double, ptr %bool, align 8
  %4 = call i32 (...) @printf(ptr @constStr.1, double %loadedVar2)
  store i32 %4, ptr %tmp0, align 4
  ret i32 0
}

declare i32 @printf(...)
