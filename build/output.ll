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
  %0 = call i32 (double, ...) @foo(double 0.000000e+00)
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
  br label %cond

cond:                                             ; preds = %main
  %loadedVar = load double, ptr %num, align 8
  %AdditionTemp = fadd double %loadedVar, 3.000000e+00
  %loadedVar1 = load double, ptr %num, align 8
  %AdditionTemp2 = fadd double %loadedVar1, 3.000000e+00
  %tmp03 = alloca double, align 8
  store double %AdditionTemp2, ptr %tmp03, align 8
  %loadedVar4 = load double, ptr %tmp03, align 8
  %FCMP = fcmp oeq double %loadedVar4, 3.000000e+00
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  %tmp1 = alloca double, align 8
  store double %boolToDouble, ptr %tmp1, align 8
  %loadedVar5 = load double, ptr %tmp1, align 8
  %cond6 = fcmp oeq double %loadedVar5, 1.000000e+00
  br i1 %cond6, label %then, label %else

then:                                             ; preds = %cond
  %2 = call i32 (...) @printf(ptr @constStr)
  store i32 %2, ptr %tmp03, align 4
  br label %merge

else:                                             ; preds = %cond
  br label %merge

merge:                                            ; preds = %then, %else
  ret i32 0
}

declare i32 @printf(...)
