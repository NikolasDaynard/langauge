; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [10 x i8] c"it's %f \0A\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %var = alloca double, align 8
  store double 3.000000e+00, ptr %var, align 8
  %test = alloca double, align 8
  store double 0.000000e+00, ptr %test, align 8
  br label %cond

cond:                                             ; preds = %loop, %entry
  %tmp0 = alloca double, align 8
  %loadedNum = load double, ptr %var, align 8
  %FCMP = fcmp oeq double %loadedNum, 3.000000e+00
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  store double %boolToDouble, ptr %tmp0, align 8
  %loadedNum1 = load double, ptr %tmp0, align 8
  %cond2 = fcmp oeq double %loadedNum1, 1.000000e+00
  br i1 %cond2, label %loop, label %merge

loop:                                             ; preds = %cond
  %loadedNum3 = load double, ptr %test, align 8
  %0 = call i32 (...) @printf(ptr @constStr, double %loadedNum3)
  store i32 %0, ptr %tmp0, align 4
  %loadedNum4 = load double, ptr %test, align 8
  %AdditionTemp = fadd double %loadedNum4, 1.000000e+00
  store double %AdditionTemp, ptr %tmp0, align 8
  %loadedNum5 = load double, ptr %tmp0, align 8
  store double %loadedNum5, ptr %test, align 8
  br label %cond

merge:                                            ; preds = %cond
  ret i32 0
}

declare i32 @printf(...)
