; ModuleID = 'output.ll'
source_filename = "test"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@constStr = private unnamed_addr constant [9 x i8] c"it equal\00", align 1
@constStr.2 = private unnamed_addr constant [4 x i8] c"nar\00", align 1

define i32 @main() local_unnamed_addr {
entry:
  %0 = tail call i32 (...) @printf(ptr nonnull @constStr)
  %1 = tail call i32 (...) @printf(ptr nonnull @constStr.2)
  ret i32 0
}

declare i32 @printf(...) local_unnamed_addr
