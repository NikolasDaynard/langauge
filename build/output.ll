; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [21 x i8] c"%f is less than ten\0A\00", align 1
@constStr.1 = private unnamed_addr constant [25 x i8] c"Approximation of Pi: %f\0A\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %pi = alloca double, align 8
  store double 0.000000e+00, ptr %pi, align 8
  %numerator = alloca double, align 8
  store double 4.000000e+00, ptr %numerator, align 8
  %denominator = alloca double, align 8
  store double 1.000000e+00, ptr %denominator, align 8
  %sign = alloca double, align 8
  store double 1.000000e+00, ptr %sign, align 8
  %iterations = alloca double, align 8
  store double 0.000000e+00, ptr %iterations, align 8
  %limit = alloca double, align 8
  store double 3.000000e+01, ptr %limit, align 8
  br label %cond

cond:                                             ; preds = %merge23, %entry
  %tmp0 = alloca double, align 8
  %loadedNum = load double, ptr %iterations, align 8
  %loadedNum1 = load double, ptr %limit, align 8
  %FCMP = fcmp olt double %loadedNum, %loadedNum1
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  store double %boolToDouble, ptr %tmp0, align 8
  %loadedNum2 = load double, ptr %tmp0, align 8
  %cond3 = fcmp oeq double %loadedNum2, 1.000000e+00
  br i1 %cond3, label %loop, label %merge

loop:                                             ; preds = %cond
  %loadedNum4 = load double, ptr %numerator, align 8
  %loadedNum5 = load double, ptr %denominator, align 8
  %DivisionTemp = fdiv double %loadedNum4, %loadedNum5
  store double %DivisionTemp, ptr %tmp0, align 8
  %term = alloca double, align 8
  %loadedNum6 = load double, ptr %tmp0, align 8
  store double %loadedNum6, ptr %term, align 8
  %loadedNum7 = load double, ptr %sign, align 8
  %loadedNum8 = load double, ptr %term, align 8
  %MultiplicationTemp = fmul double %loadedNum7, %loadedNum8
  store double %MultiplicationTemp, ptr %tmp0, align 8
  %tmp1 = alloca double, align 8
  %loadedNum9 = load double, ptr %pi, align 8
  %loadedNum10 = load double, ptr %tmp0, align 8
  %AdditionTemp = fadd double %loadedNum9, %loadedNum10
  store double %AdditionTemp, ptr %tmp1, align 8
  %loadedNum11 = load double, ptr %tmp1, align 8
  store double %loadedNum11, ptr %pi, align 8
  %loadedNum12 = load double, ptr %denominator, align 8
  %AdditionTemp13 = fadd double %loadedNum12, 2.000000e+00
  store double %AdditionTemp13, ptr %tmp0, align 8
  %loadedNum14 = load double, ptr %tmp0, align 8
  store double %loadedNum14, ptr %denominator, align 8
  store double -1.000000e+00, ptr %tmp0, align 8
  %loadedNum15 = load double, ptr %sign, align 8
  %loadedNum16 = load double, ptr %tmp0, align 8
  %MultiplicationTemp17 = fmul double %loadedNum15, %loadedNum16
  store double %MultiplicationTemp17, ptr %tmp1, align 8
  %loadedNum18 = load double, ptr %tmp1, align 8
  store double %loadedNum18, ptr %sign, align 8
  %loadedNum19 = load double, ptr %iterations, align 8
  %AdditionTemp20 = fadd double %loadedNum19, 1.000000e+00
  store double %AdditionTemp20, ptr %tmp0, align 8
  %loadedNum21 = load double, ptr %tmp0, align 8
  store double %loadedNum21, ptr %iterations, align 8
  br label %cond22

merge:                                            ; preds = %cond
  %tmp035 = alloca double, align 8
  %loadedNum36 = load double, ptr %pi, align 8
  %0 = call i32 (...) @printf(ptr @constStr.1, double %loadedNum36)
  store i32 %0, ptr %tmp035, align 4
  ret i32 0

cond22:                                           ; preds = %loop
  %loadedNum24 = load double, ptr %pi, align 8
  %FCMP25 = fcmp olt double %loadedNum24, 3.140000e+00
  %zext26 = zext i1 %FCMP25 to i32
  %boolToDouble27 = sitofp i32 %zext26 to double
  store double %boolToDouble27, ptr %tmp0, align 8
  %loadedNum28 = load double, ptr %tmp0, align 8
  %cond29 = fcmp oeq double %loadedNum28, 1.000000e+00
  br i1 %cond29, label %then, label %else

then:                                             ; preds = %cond22
  br label %cond30

else:                                             ; preds = %cond22
  br label %merge23

merge23:                                          ; preds = %else, %merge33
  br label %cond

cond30:                                           ; preds = %then
  br i1 true, label %then31, label %else32

then31:                                           ; preds = %cond30
  %loadedNum34 = load double, ptr %pi, align 8
  %1 = call i32 (...) @printf(ptr @constStr, double %loadedNum34)
  store i32 %1, ptr %tmp0, align 4
  br label %merge33

else32:                                           ; preds = %cond30
  br label %merge33

merge33:                                          ; preds = %then31, %else32
  br label %merge23
}

declare i32 @printf(...)
