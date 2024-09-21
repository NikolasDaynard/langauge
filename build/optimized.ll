; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [39 x i8] c"Countdown starting from %f seconds...\0A\00", align 1
@constStr.1 = private unnamed_addr constant [25 x i8] c"%f seconds remaining...\0A\00", align 1
@constStr.2 = private unnamed_addr constant [12 x i8] c"Time's up!\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr, double 1.000000e+01)
  br label %loop.i

loop.i:                                           ; preds = %loop.i, %entry
  %seconds.01.int.i = phi i32 [ 10, %entry ], [ %SubTemp12.int.i, %loop.i ]
  %indvar.conv.i = sitofp i32 %seconds.01.int.i to double
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr.1, double %indvar.conv.i)
  %SubTemp12.int.i = add nsw i32 %seconds.01.int.i, -1
  %FCMP.not.i = icmp eq i32 %SubTemp12.int.i, 0
  br i1 %FCMP.not.i, label %mainBodyFunc.exit, label %loop.i

mainBodyFunc.exit:                                ; preds = %loop.i
  %2 = tail call i32 (...) @printf(ptr nonnull @constStr.2)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
