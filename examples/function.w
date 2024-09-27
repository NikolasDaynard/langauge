other(string, num) {
    print(string, num)
}

foo(str, num, bool) {
    other(str, 3)
    print(str, num)
    if num == 4 {
        print("Num is 4")
    }
}

foo("str %f", 4, 3)