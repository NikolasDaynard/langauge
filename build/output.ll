; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [10 x i8] c"not equal\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %tmp0 = alloca double, align 8
  store double 2.000000e+00, ptr %tmp0, align 8
  %var = alloca double, align 8
  %loadedNum = load double, ptr %tmp0, align 8
  store double %loadedNum, ptr %var, align 8
  %loadedNum1 = load double, ptr %var, align 8
  %FCMP = fcmp oeq double 3.000000e+00, %loadedNum1
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  store double %boolToDouble, ptr %tmp0, align 8
  %loadedNum2 = load double, ptr %tmp0, align 8
  %loadedNum3 = load double, ptr %tmp0, align 8
  %cond = fcmp oeq double %loadedNum3, 1.000000e+00
  br i1 %cond, label %then, label %else

then:                                             ; preds = %entry
  %loadedNum4 = load double, ptr %var, align 8
  %AdditionTemp = fadd double %loadedNum4, 1.000000e+00
  store double %AdditionTemp, ptr %tmp0, align 8
  %loadedNum5 = load double, ptr %tmp0, align 8
  store double %loadedNum5, ptr %var, align 8
  %va2 = alloca double, align 8
  store double 3.000000e-01, ptr %va2, align 8
  br label %merge

else:                                             ; preds = %entry
  %0 = call i32 (...) @printf(ptr @constStr)
  store i32 %0, ptr %tmp0, align 4
  br label %merge

merge:                                            ; preds = %else, %then
  ret i32 0
}

declare i32 @printf(...)
