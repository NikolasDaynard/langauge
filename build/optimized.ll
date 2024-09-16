; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [10 x i8] c"it's %f \0A\00", align 1

; Function Attrs: noreturn
define i32 @main() local_unnamed_addr #0 {
entry:
  br label %cond.i

cond.i:                                           ; preds = %cond.i, %entry
  %test.0.i = phi double [ 0.000000e+00, %entry ], [ %AdditionTemp.i, %cond.i ]
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr, double %test.0.i)
  %AdditionTemp.i = fadd double %test.0.i, 1.000000e+00
  br label %cond.i
}

declare i32 @printf(...) local_unnamed_addr

attributes #0 = { noreturn }
