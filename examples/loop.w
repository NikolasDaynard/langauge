pi = 0
numerator = 4
denominator = 1
sign = 1
iterations = 0
limit = 10

while iterations < limit {
    term = numerator / denominator
    pi = pi + (sign * term)

    denominator = denominator + 2
    sign = sign * -1

    iterations = iterations + 1
    if pi < 3.14 {
        print("%f is less than ten\n", pi)
    else
        print("%f is more (or equal) to ten\n", pi)
    }
}

print("Approximation of Pi: %f\n", pi)
