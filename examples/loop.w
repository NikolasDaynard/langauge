pi = 0
numerator = 4
denominator = 1
sign = 1
iterations = 0
limit = 30

while iterations < limit {
    term = numerator / denominator
    pi = pi + (sign * term)

    denominator = denominator + 2
    sign = sign * (1 - 2)

    iterations = iterations + 1
}

print("Approximation of Pi: %f\n", pi)

iterations = 0

while pi < 40 {
    iterations = iterations + 1
    if iterations == 30 {
        pi = 1000
    }
}