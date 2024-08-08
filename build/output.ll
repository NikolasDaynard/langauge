; ModuleID = 'test'
source_filename = "test"

@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %0 = call i32 (...) @printf(ptr @0, i32 1)
  ret i32 0
}

declare i32 @printf(...)
