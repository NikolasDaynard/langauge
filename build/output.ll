; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [11 x i8] c"var 1: %f\0A\00", align 1

define i32 @main() {
entry:
  %var = alloca double, align 8
  store double 1.000000e+01, ptr %var, align 8
  %var1 = alloca double, align 8
  store double 2.000000e+00, ptr %var1, align 8
  %loadedNum = load double, ptr %var, align 8
  store double %loadedNum, ptr %var1, align 8
  %loadedNum1 = load double, ptr %var1, align 8
  %0 = call i32 (...) @printf(ptr @constStr, double %loadedNum1)
  ret i32 0
}

declare i32 @printf(...)
