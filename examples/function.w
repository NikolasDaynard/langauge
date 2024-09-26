other(num) {
    print("hello from other got %f", num)
}

test(arg1, arg2) {
    other(arg1)
    print("\ntest %f", arg1)
    print("\ntest %f", arg2)
}

other(2)
print("Test\n\n\n")
test(2, 3)