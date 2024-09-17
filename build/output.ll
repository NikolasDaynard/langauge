; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [9 x i8] c"its less\00", align 1
@constStr.1 = private unnamed_addr constant [5 x i8] c"\0Atea\00", align 1
@constStr.2 = private unnamed_addr constant [5 x i8] c"\0Amhm\00", align 1
@constStr.3 = private unnamed_addr constant [25 x i8] c"Approximation of Pi: %f\0A\00", align 1

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

cond:                                             ; preds = %loop, %entry
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
  br label %cond

merge:                                            ; preds = %cond
  br label %cond22

cond22:                                           ; preds = %merge
  %tmp024 = alloca double, align 8
  %loadedNum25 = load double, ptr %iterations, align 8
  %FCMP26 = fcmp olt double %loadedNum25, 3.000000e+04
  %zext27 = zext i1 %FCMP26 to i32
  %boolToDouble28 = sitofp i32 %zext27 to double
  store double %boolToDouble28, ptr %tmp024, align 8
  %loadedNum29 = load double, ptr %tmp024, align 8
  %cond30 = fcmp oeq double %loadedNum29, 1.000000e+00
  br i1 %cond30, label %then, label %else

then:                                             ; preds = %cond22
  %0 = call i32 (...) @printf(ptr @constStr)
  store i32 %0, ptr %tmp024, align 4
  br label %cond31

else:                                             ; preds = %cond22
  br label %merge23

merge23:                                          ; preds = %else, %merge34
  %tmp041 = alloca double, align 8
  %loadedNum42 = load double, ptr %pi, align 8
  %1 = call i32 (...) @printf(ptr @constStr.3, double %loadedNum42)
  store i32 %1, ptr %tmp041, align 4
  ret i32 0

cond31:                                           ; preds = %then
  %loadedNum35 = load double, ptr %iterations, align 8
  %FCMP36 = fcmp olt double %loadedNum35, 4.000000e+05
  %zext37 = zext i1 %FCMP36 to i32
  %boolToDouble38 = sitofp i32 %zext37 to double
  store double %boolToDouble38, ptr %tmp024, align 8
  %loadedNum39 = load double, ptr %tmp024, align 8
  %cond40 = fcmp oeq double %loadedNum39, 1.000000e+00
  br i1 %cond40, label %then32, label %else33

then32:                                           ; preds = %cond31
  %2 = call i32 (...) @printf(ptr @constStr.1)
  store i32 %2, ptr %tmp024, align 4
  br label %merge34

else33:                                           ; preds = %cond31
  br label %merge34

merge34:                                          ; preds = %then32, %else33
  %3 = call i32 (...) @printf(ptr @constStr.2)
  store i32 %3, ptr %tmp024, align 4
  br label %merge23
}

declare i32 @printf(...)
