; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [10 x i8] c"string %f\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %var = alloca double, align 8
  store double 0.000000e+00, ptr %var, align 8
  %var1 = alloca ptr, align 8
  store ptr @constStr, ptr %var1, align 8
  %loadedVar = load ptr, ptr %var1, align 8
  %loadedVar1 = load double, ptr %var, align 8
  %0 = call i32 (...) @printf(ptr %loadedVar, double %loadedVar1)
  %tmp0 = alloca i32, align 4
  store i32 %0, ptr %tmp0, align 4
  ret i32 0
}

declare i32 @printf(...)
