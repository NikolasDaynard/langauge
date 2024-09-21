; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [21 x i8] c"%f is less than ten\0A\00", align 1
@constStr.1 = private unnamed_addr constant [30 x i8] c"%f is more (or equal) to ten\0A\00", align 1
@constStr.2 = private unnamed_addr constant [25 x i8] c"Approximation of Pi: %f\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  br label %loop.i

loop.i:                                           ; preds = %loop.i, %entry
  %pi.04.i = phi double [ 0.000000e+00, %entry ], [ %AdditionTemp.i, %loop.i ]
  %iterations.03.int.i = phi i32 [ 0, %entry ], [ %AdditionTemp36.int.i, %loop.i ]
  %sign.02.i = phi double [ 1.000000e+00, %entry ], [ %MultiplicationTemp30.i, %loop.i ]
  %denominator.01.i = phi double [ 1.000000e+00, %entry ], [ %AdditionTemp22.i, %loop.i ]
  %DivisionTemp8.i = fdiv double 4.000000e+00, %denominator.01.i
  %MultiplicationTemp14.i = fmul double %sign.02.i, %DivisionTemp8.i
  %AdditionTemp.i = fadd double %pi.04.i, %MultiplicationTemp14.i
  %AdditionTemp22.i = fadd double %denominator.01.i, 2.000000e+00
  %MultiplicationTemp30.i = fneg double %sign.02.i
  %AdditionTemp36.int.i = add nuw nsw i32 %iterations.03.int.i, 1
  %FCMP42.i = fcmp olt double %AdditionTemp.i, 3.140000e+00
  %constStr.constStr.1 = select i1 %FCMP42.i, ptr @constStr, ptr @constStr.1
  %0 = tail call i32 (...) @printf(ptr nonnull %constStr.constStr.1, double %AdditionTemp.i)
  %exitcond.not.i = icmp eq i32 %AdditionTemp36.int.i, 10
  br i1 %exitcond.not.i, label %mainBodyFunc.exit, label %loop.i

mainBodyFunc.exit:                                ; preds = %loop.i
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr.2, double %AdditionTemp.i)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
