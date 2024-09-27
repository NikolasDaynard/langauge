; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [5 x i8] c"test\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr)
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr)
  ret i32 0
}

define i32 @foo(double %0, ...) local_unnamed_addr {
main:
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr)
  %AdditionTemp2 = fadd double %0, 3.000000e+00
  %FCMP = fcmp oeq double %AdditionTemp2, 3.000000e+00
  br i1 %FCMP, label %then, label %merge

then:                                             ; preds = %main
  %2 = tail call i32 (...) @printf(ptr nonnull @constStr)
  br label %merge

merge:                                            ; preds = %main, %then
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
