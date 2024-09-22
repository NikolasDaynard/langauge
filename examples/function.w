other(num) {
    print("hello from other got %f", num)
}

test(arg1, arg2) {
    other(arg1)
    print("\ntest %f", arg1)
    print("recived %s", arg2)
}

test(1, "string")