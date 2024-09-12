; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [7 x i8] c"it's 3\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %var = alloca double, align 8
  store double 3.000000e+00, ptr %var, align 8
  %tmp0 = alloca double, align 8
  %loadedNum = load double, ptr %var, align 8
  %FCMP = fcmp oeq double %loadedNum, 3.000000e+00
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  store double %boolToDouble, ptr %tmp0, align 8
  %loadedNum1 = load double, ptr %tmp0, align 8
  %cond = fcmp oeq double %loadedNum1, 1.000000e+00
  br i1 %cond, label %then, label %else

then:                                             ; preds = %entry
  %0 = call i32 (...) @printf(ptr @constStr)
  store i32 %0, ptr %tmp0, align 4
  br label %merge

else:                                             ; preds = %entry
  br label %merge

merge:                                            ; preds = %then, %else
  ret i32 0
}

declare i32 @printf(...)
