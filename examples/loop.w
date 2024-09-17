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
    sign = sign * (0 - 1)

    iterations = iterations + 1
    if pi < 3.14 {
        if 1 {
            print("%f is less than ten\n", pi)
        }
    }
}

-- if iterations < 30000 {
--     print("its less")
--     if iterations < 400000 {
--         print("\ntea")
--     }
--     print("\nmhm")
-- }
print("Approximation of Pi: %f\n", pi)

-- iterations = 0

-- while pi < 40 {
--     iterations = iterations + 1
--     if iterations == 30 {
--         pi = 1000
--     }
-- }
-- print("iterations? %f", iterations)