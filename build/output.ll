; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [9 x i8] c"test\0A%f\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr, double 0xC1A81E999A000000, double 3.000000e+00, double 0x41A81E99A0000000)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
