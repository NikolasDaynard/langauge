; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() local_unnamed_addr {
entry:
  %0 = tail call i32 (...) @printf(double 2.000000e+00, double 2.800000e+01, double 6.000000e+00, double 1.000000e+00)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
