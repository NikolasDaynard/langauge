; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [24 x i8] c"hello from other got %f\00", align 1
@constStr.1 = private unnamed_addr constant [8 x i8] c"Test\0A\0A\0A\00", align 1
@constStr.2 = private unnamed_addr constant [9 x i8] c"\0Atest %f\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr, double 2.000000e+00)
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr.1)
  %2 = tail call i32 (...) @printf(ptr nonnull @constStr, double 2.000000e+00)
  %3 = tail call i32 (...) @printf(ptr nonnull @constStr.2, double 2.000000e+00)
  %4 = tail call i32 (...) @printf(ptr nonnull @constStr.2, double 3.000000e+00)
  ret i32 0
}

define i32 @other(double %0, ...) local_unnamed_addr {
main:
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr, double %0)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr

define i32 @test(double %0, double %1, ...) local_unnamed_addr {
main:
  %2 = tail call i32 (...) @printf(ptr nonnull @constStr, double %0)
  %3 = tail call i32 (...) @printf(ptr nonnull @constStr.2, double %0)
  %4 = tail call i32 (...) @printf(ptr nonnull @constStr.2, double %1)
  ret i32 0
}
