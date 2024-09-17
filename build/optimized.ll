; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [25 x i8] c"Approximation of Pi: %f\0A\00", align 1

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
  %FCMP.i = fcmp olt double %AdditionTemp20.i, 3.000000e+09
  br i1 %FCMP.i, label %loop.i, label %mainBodyFunc.exit

mainBodyFunc.exit:                                ; preds = %loop.i
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr, double %AdditionTemp.i)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
