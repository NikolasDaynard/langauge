; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [7 x i8] c"str %f\00", align 1
@constStr.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@constStr.2 = private unnamed_addr constant [9 x i8] c"Num is 4\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %0 = call i32 (ptr, double, double, ...) @foo(ptr @constStr, double 4.000000e+00, double 3.000000e+00)
  %tmp0 = alloca i32, align 4
  store i32 %0, ptr %tmp0, align 4
  ret i32 0
}

define i32 @foo(ptr %0, double %1, double %2, ...) {
main:
  %str = alloca ptr, align 8
  store ptr %0, ptr %str, align 8
  %num = alloca double, align 8
  store double %1, ptr %num, align 8
  %bool = alloca double, align 8
  store double %2, ptr %bool, align 8
  %loadedVar = load ptr, ptr %str, align 8
  %loadedVar1 = load ptr, ptr %str, align 8
  %3 = call i32 (ptr, double, ...) @other(ptr %loadedVar1, double 3.000000e+00)
  %tmp0 = alloca i32, align 4
  store i32 %3, ptr %tmp0, align 4
  %loadedVar2 = load ptr, ptr %str, align 8
  %loadedVar3 = load double, ptr %num, align 8
  %4 = call i32 (...) @printf(ptr %loadedVar2, double %loadedVar3)
  store i32 %4, ptr %tmp0, align 4
  br label %cond

cond:                                             ; preds = %main
  %loadedVar4 = load double, ptr %num, align 8
  %FCMP = fcmp oeq double %loadedVar4, 4.000000e+00
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  %loadedVar5 = load double, ptr %num, align 8
  %FCMP6 = fcmp oeq double %loadedVar5, 4.000000e+00
  %zext7 = zext i1 %FCMP6 to i32
  %boolToDouble8 = sitofp i32 %zext7 to double
  %tmp09 = alloca double, align 8
  store double %boolToDouble8, ptr %tmp09, align 8
  %loadedVar10 = load double, ptr %tmp09, align 8
  %cond11 = fcmp oeq double %loadedVar10, 1.000000e+00
  br i1 %cond11, label %then, label %else

then:                                             ; preds = %cond
  %5 = call i32 (...) @printf(ptr @constStr.2)
  store i32 %5, ptr %tmp09, align 4
  br label %merge

else:                                             ; preds = %cond
  br label %merge

merge:                                            ; preds = %then, %else
  ret i32 0
}

define i32 @other(ptr %0, double %1, ...) {
main:
  %string = alloca ptr, align 8
  store ptr %0, ptr %string, align 8
  %num = alloca double, align 8
  store double %1, ptr %num, align 8
  %2 = call i32 (...) @printf(ptr @constStr.1)
  %tmp0 = alloca i32, align 4
  store i32 %2, ptr %tmp0, align 4
  %loadedVar = load ptr, ptr %string, align 8
  %loadedVar1 = load double, ptr %num, align 8
  %3 = call i32 (...) @printf(ptr %loadedVar, double %loadedVar1)
  store i32 %3, ptr %tmp0, align 4
  br label %cond

cond:                                             ; preds = %main
  %loadedVar2 = load double, ptr %num, align 8
  %FCMP = fcmp ogt double %loadedVar2, 5.000000e+00
  %zext = zext i1 %FCMP to i32
  %boolToDouble = sitofp i32 %zext to double
  %loadedVar3 = load double, ptr %num, align 8
  %FCMP4 = fcmp ogt double %loadedVar3, 5.000000e+00
  %zext5 = zext i1 %FCMP4 to i32
  %boolToDouble6 = sitofp i32 %zext5 to double
  %tmp07 = alloca double, align 8
  store double %boolToDouble6, ptr %tmp07, align 8
  %loadedVar8 = load double, ptr %tmp07, align 8
  %cond9 = fcmp oeq double %loadedVar8, 1.000000e+00
  br i1 %cond9, label %then, label %else

then:                                             ; preds = %cond
  %loadedVar10 = load double, ptr %num, align 8
  %AdditionTemp = fadd double %loadedVar10, 1.000000e+00
  %loadedVar11 = load double, ptr %num, align 8
  %AdditionTemp12 = fadd double %loadedVar11, 1.000000e+00
  store double %AdditionTemp12, ptr %tmp07, align 8
  %loadedVar13 = load ptr, ptr %string, align 8
  %loadedVar14 = load double, ptr %tmp07, align 8
  %4 = call i32 (ptr, double, ...) @other(ptr %loadedVar13, double %loadedVar14)
  %tmp1 = alloca i32, align 4
  store i32 %4, ptr %tmp1, align 4
  br label %merge

else:                                             ; preds = %cond
  br label %merge

merge:                                            ; preds = %then, %else
  ret i32 0
}

declare i32 @printf(...)
