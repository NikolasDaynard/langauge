other(string, num) {
    print("\n")
    print(string, num)
    if num > 5 {
        other(string, num + 1)
    }
}

foo(str, num, bool) {
    other(str, 3)
    print(str, num)
    if num == 4 {
        print("Num is 4")
    }
}

foo("str %f", 4, 3)