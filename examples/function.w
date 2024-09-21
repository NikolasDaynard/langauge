other(num) {
    print("hello from other got %f", num)
}

test(arg1, arg2) {
    other(arg1)
    print("\ntest %f %f", arg1, arg2)
}

test(1, 2)