; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [21 x i8] c"%f is less than ten\0A\00", align 1
@constStr.1 = private unnamed_addr constant [30 x i8] c"%f is more (or equal) to ten\0A\00", align 1
@constStr.2 = private unnamed_addr constant [25 x i8] c"Approximation of Pi: %f\0A\00", align 1

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
  store double 1.000000e+01, ptr %limit, align 8
  br label %cond

cond:                                             ; preds = %merge40, %entry
  %loadedVar = load double, ptr %iterations, align 8
  %loadedVar1 = load double, ptr %limit, align 8
  %FCMP = fcmp olt double %loadedVar, %loadedVar1
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  %tmp0 = alloca double, align 8
  store double %boolToDouble, ptr %tmp0, align 8
  %loadedVar2 = load double, ptr %tmp0, align 8
  %cond3 = fcmp oeq double %loadedVar2, 1.000000e+00
  br i1 %cond3, label %loop, label %merge

loop:                                             ; preds = %cond
  %loadedVar4 = load double, ptr %numerator, align 8
  %loadedVar5 = load double, ptr %denominator, align 8
  %DivisionTemp = fdiv double %loadedVar4, %loadedVar5
  %loadedVar6 = load double, ptr %numerator, align 8
  %loadedVar7 = load double, ptr %denominator, align 8
  %DivisionTemp8 = fdiv double %loadedVar6, %loadedVar7
  store double %DivisionTemp8, ptr %tmp0, align 8
  %loadedVar9 = load double, ptr %tmp0, align 8
  %term = alloca double, align 8
  store double %loadedVar9, ptr %term, align 8
  %loadedVar10 = load double, ptr %sign, align 8
  %loadedVar11 = load double, ptr %term, align 8
  %MultiplicationTemp = fmul double %loadedVar10, %loadedVar11
  %loadedVar12 = load double, ptr %sign, align 8
  %loadedVar13 = load double, ptr %term, align 8
  %MultiplicationTemp14 = fmul double %loadedVar12, %loadedVar13
  store double %MultiplicationTemp14, ptr %tmp0, align 8
  %loadedVar15 = load double, ptr %pi, align 8
  %loadedVar16 = load double, ptr %tmp0, align 8
  %AdditionTemp = fadd double %loadedVar15, %loadedVar16
  %tmp1 = alloca double, align 8
  store double %AdditionTemp, ptr %tmp1, align 8
  %loadedVar17 = load double, ptr %tmp1, align 8
  %loadedVar18 = load double, ptr %tmp1, align 8
  store double %loadedVar18, ptr %pi, align 8
  %loadedVar19 = load double, ptr %denominator, align 8
  %AdditionTemp20 = fadd double %loadedVar19, 2.000000e+00
  %loadedVar21 = load double, ptr %denominator, align 8
  %AdditionTemp22 = fadd double %loadedVar21, 2.000000e+00
  store double %AdditionTemp22, ptr %tmp0, align 8
  %loadedVar23 = load double, ptr %tmp0, align 8
  %loadedVar24 = load double, ptr %tmp0, align 8
  store double %loadedVar24, ptr %denominator, align 8
  store double -1.000000e+00, ptr %tmp0, align 8
  %loadedVar25 = load double, ptr %sign, align 8
  %loadedVar26 = load double, ptr %tmp0, align 8
  %MultiplicationTemp27 = fmul double %loadedVar25, %loadedVar26
  %loadedVar28 = load double, ptr %sign, align 8
  %loadedVar29 = load double, ptr %tmp0, align 8
  %MultiplicationTemp30 = fmul double %loadedVar28, %loadedVar29
  store double %MultiplicationTemp30, ptr %tmp1, align 8
  %loadedVar31 = load double, ptr %tmp1, align 8
  %loadedVar32 = load double, ptr %tmp1, align 8
  store double %loadedVar32, ptr %sign, align 8
  %loadedVar33 = load double, ptr %iterations, align 8
  %AdditionTemp34 = fadd double %loadedVar33, 1.000000e+00
  %loadedVar35 = load double, ptr %iterations, align 8
  %AdditionTemp36 = fadd double %loadedVar35, 1.000000e+00
  store double %AdditionTemp36, ptr %tmp0, align 8
  %loadedVar37 = load double, ptr %tmp0, align 8
  %loadedVar38 = load double, ptr %tmp0, align 8
  store double %loadedVar38, ptr %iterations, align 8
  br label %cond39

merge:                                            ; preds = %cond
  %loadedVar53 = load double, ptr %pi, align 8
  %0 = call i32 (...) @printf(ptr @constStr.2, double %loadedVar53)
  %tmp054 = alloca i32, align 4
  store i32 %0, ptr %tmp054, align 4
  ret i32 0

cond39:                                           ; preds = %loop
  %loadedVar41 = load double, ptr %pi, align 8
  %FCMP42 = fcmp olt double %loadedVar41, 3.140000e+00
  %zext43 = zext i1 %FCMP42 to i32
  %boolToDouble44 = sitofp i32 %zext43 to double
  %loadedVar45 = load double, ptr %pi, align 8
  %FCMP46 = fcmp olt double %loadedVar45, 3.140000e+00
  %zext47 = zext i1 %FCMP46 to i32
  %boolToDouble48 = sitofp i32 %zext47 to double
  store double %boolToDouble48, ptr %tmp0, align 8
  %loadedVar49 = load double, ptr %tmp0, align 8
  %cond50 = fcmp oeq double %loadedVar49, 1.000000e+00
  br i1 %cond50, label %then, label %else

then:                                             ; preds = %cond39
  %loadedVar51 = load double, ptr %pi, align 8
  %1 = call i32 (...) @printf(ptr @constStr, double %loadedVar51)
  store i32 %1, ptr %tmp0, align 4
  br label %merge40

else:                                             ; preds = %cond39
  %loadedVar52 = load double, ptr %pi, align 8
  %2 = call i32 (...) @printf(ptr @constStr.1, double %loadedVar52)
  store i32 %2, ptr %tmp0, align 4
  br label %merge40

merge40:                                          ; preds = %then, %else
  br label %cond
}

declare i32 @printf(...)
