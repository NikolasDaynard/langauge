; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [7 x i8] c"str %f\00", align 1
@constStr.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@constStr.2 = private unnamed_addr constant [9 x i8] c"Num is 4\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr.1)
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr, double 3.000000e+00)
  %2 = tail call i32 (...) @printf(ptr nonnull @constStr, double 4.000000e+00)
  %3 = tail call i32 (...) @printf(ptr nonnull @constStr.2)
  ret i32 0
}

define i32 @foo(ptr %0, double %1, double %2, ...) local_unnamed_addr {
main:
  %3 = tail call i32 (...) @printf(ptr nonnull @constStr.1)
  %4 = tail call i32 (...) @printf(ptr %0, double 3.000000e+00)
  %5 = tail call i32 (...) @printf(ptr %0, double %1)
  %FCMP = fcmp oeq double %1, 4.000000e+00
  br i1 %FCMP, label %then, label %merge

then:                                             ; preds = %main
  %6 = tail call i32 (...) @printf(ptr nonnull @constStr.2)
  br label %merge

merge:                                            ; preds = %main, %then
  ret i32 0
}

define i32 @other(ptr %0, double %1, ...) local_unnamed_addr {
main:
  %2 = tail call i32 (...) @printf(ptr nonnull @constStr.1)
  %3 = tail call i32 (...) @printf(ptr %0, double %1)
  %FCMP = fcmp ogt double %1, 5.000000e+00
  br i1 %FCMP, label %then, label %merge

then:                                             ; preds = %main
  %AdditionTemp12 = fadd double %1, 1.000000e+00
  %4 = tail call i32 (ptr, double, ...) @other(ptr %0, double %AdditionTemp12)
  br label %merge

merge:                                            ; preds = %main, %then
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
