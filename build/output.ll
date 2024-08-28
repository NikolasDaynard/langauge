; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@constStr.1 = private unnamed_addr constant [2 x i8] c"4\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr, double 3.000000e+00)
  %tmp1.0.insert.ext = zext i32 %0 to i64
  %1 = bitcast i64 %tmp1.0.insert.ext to double
  %2 = tail call i32 (...) @printf(double 4.000000e+00)
  %tmp2.0.insert.ext = zext i32 %2 to i64
  %3 = bitcast i64 %tmp2.0.insert.ext to double
  %4 = tail call i32 (...) @printf(double %1, double 2.000000e+00, double %3, ptr nonnull @constStr.1)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
