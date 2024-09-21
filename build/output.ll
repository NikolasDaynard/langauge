; ModuleID = 'test'
source_filename = "test"

@constStr = private unnamed_addr constant [17 x i8] c"hello from other\00", align 1
@constStr.1 = private unnamed_addr constant [5 x i8] c"test\00", align 1

define i32 @main() {
entry:
  %0 = call i32 @mainBodyFunc()
  ret i32 0
}

define private i32 @mainBodyFunc() {
entry:
  %tmp0 = alloca double, align 8
  %0 = call i32 (...) @test()
  store i32 %0, ptr %tmp0, align 4
  ret i32 0
}

define i32 @other(...) {
ma2in:
  %tmp0 = alloca double, align 8
  %0 = call i32 (...) @printf(ptr @constStr)
  store i32 %0, ptr %tmp0, align 4
  ret i32 0
}

declare i32 @printf(...)

define i32 @test(...) {
ma2in:
  %tmp0 = alloca double, align 8
  %0 = call i32 (...) @other()
  store i32 %0, ptr %tmp0, align 4
  %1 = call i32 (...) @printf(ptr @constStr.1)
  store i32 %1, ptr %tmp0, align 4
  ret i32 0
}
