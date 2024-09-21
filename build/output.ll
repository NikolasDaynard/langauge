; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [39 x i8] c"Countdown starting from %f seconds...\0A\00", align 1
@constStr.1 = private unnamed_addr constant [25 x i8] c"%f seconds remaining...\0A\00", align 1
@constStr.2 = private unnamed_addr constant [12 x i8] c"Time's up!\0A\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %seconds = alloca double, align 8
  store double 1.000000e+01, ptr %seconds, align 8
  %loadedVar = load double, ptr %seconds, align 8
  %0 = call i32 (...) @printf(ptr @constStr, double %loadedVar)
  %tmp0 = alloca i32, align 4
  store i32 %0, ptr %tmp0, align 4
  br label %cond

cond:                                             ; preds = %loop, %entry
  %loadedVar1 = load double, ptr %seconds, align 8
  %FCMP = fcmp olt double 0.000000e+00, %loadedVar1
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  %loadedVar2 = load double, ptr %seconds, align 8
  %FCMP3 = fcmp olt double 0.000000e+00, %loadedVar2
  %zext4 = zext i1 %FCMP3 to i32
  %boolToDouble5 = sitofp i32 %zext4 to double
  %tmp06 = alloca double, align 8
  store double %boolToDouble5, ptr %tmp06, align 8
  %loadedVar7 = load double, ptr %tmp06, align 8
  %cond8 = fcmp oeq double %loadedVar7, 1.000000e+00
  br i1 %cond8, label %loop, label %merge

loop:                                             ; preds = %cond
  %loadedVar9 = load double, ptr %seconds, align 8
  %1 = call i32 (...) @printf(ptr @constStr.1, double %loadedVar9)
  store i32 %1, ptr %tmp06, align 4
  %loadedVar10 = load double, ptr %seconds, align 8
  %SubTemp = fsub double %loadedVar10, 1.000000e+00
  %loadedVar11 = load double, ptr %seconds, align 8
  %SubTemp12 = fsub double %loadedVar11, 1.000000e+00
  store double %SubTemp12, ptr %tmp06, align 8
  %loadedVar13 = load double, ptr %tmp06, align 8
  %loadedVar14 = load double, ptr %tmp06, align 8
  store double %loadedVar14, ptr %seconds, align 8
  br label %cond

merge:                                            ; preds = %cond
  %2 = call i32 (...) @printf(ptr @constStr.2)
  store i32 %2, ptr %tmp0, align 4
  ret i32 0
}

declare i32 @printf(...)
