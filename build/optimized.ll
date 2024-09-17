; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [9 x i8] c"its less\00", align 1
@constStr.1 = private unnamed_addr constant [5 x i8] c"\0Atea\00", align 1
@constStr.2 = private unnamed_addr constant [5 x i8] c"\0Amhm\00", align 1
@constStr.3 = private unnamed_addr constant [25 x i8] c"Approximation of Pi: %f\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  br label %loop.i

loop.i:                                           ; preds = %loop.i, %entry
  %pi.04.i = phi double [ 0.000000e+00, %entry ], [ %AdditionTemp.i, %loop.i ]
  %iterations.03.i = phi double [ 0.000000e+00, %entry ], [ %AdditionTemp20.i, %loop.i ]
  %sign.02.i = phi double [ 1.000000e+00, %entry ], [ %MultiplicationTemp17.i, %loop.i ]
  %denominator.01.i = phi double [ 1.000000e+00, %entry ], [ %AdditionTemp13.i, %loop.i ]
  %DivisionTemp.i = fdiv double 4.000000e+00, %denominator.01.i
  %MultiplicationTemp.i = fmul double %sign.02.i, %DivisionTemp.i
  %AdditionTemp.i = fadd double %pi.04.i, %MultiplicationTemp.i
  %AdditionTemp13.i = fadd double %denominator.01.i, 2.000000e+00
  %MultiplicationTemp17.i = fneg double %sign.02.i
  %AdditionTemp20.i = fadd double %iterations.03.i, 1.000000e+00
  %FCMP.i = fcmp olt double %AdditionTemp20.i, 3.000000e+01
  br i1 %FCMP.i, label %loop.i, label %cond22.i

cond22.i:                                         ; preds = %loop.i
  %FCMP26.i = fcmp olt double %AdditionTemp20.i, 3.000000e+04
  br i1 %FCMP26.i, label %then.i, label %mainBodyFunc.exit

then.i:                                           ; preds = %cond22.i
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr)
  %FCMP36.i = fcmp olt double %AdditionTemp20.i, 4.000000e+05
  br i1 %FCMP36.i, label %then32.i, label %merge34.i

then32.i:                                         ; preds = %then.i
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr.1)
  br label %merge34.i

merge34.i:                                        ; preds = %then32.i, %then.i
  %2 = tail call i32 (...) @printf(ptr nonnull @constStr.2)
  br label %mainBodyFunc.exit

mainBodyFunc.exit:                                ; preds = %cond22.i, %merge34.i
  %3 = tail call i32 (...) @printf(ptr nonnull @constStr.3, double %AdditionTemp.i)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
