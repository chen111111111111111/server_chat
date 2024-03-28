data = [
    ("1-5", 7.56, -0.48),
    ("6-11", 5.16, -0.48),
    ("12-14", 3.00, -0.24),
    ("15-34", 2.16, -0.12),
    ("35-46", -0.24, -0.12),
    ("47-56", -1.72, -0.12),
    ("57", -2.92),
    ("58-68", -3.00, -0.12),
    ("69", -4.36),
    ("70-75", -4.44, -0.12),
    ("76", -5.20),
    ("77-102", -5.28, -0.12),
    ("103-143", -8.40, -0.12),
    ("144", -13.36),
    ("145-147", -13.44, -0.12),
    ("148", -13.76),
    ("149-156", -13.92, -0.12),
    ("157-161", -15.00, -0.24),
    ("162", -0.10)
]

result = []
for item in data:
    index_str, *values = item
    indices = range(int(index_str.split("-")[0]), int(index_str.split("-")[-1])+1)
    start = values[0]
    result.extend([start] * len(indices))
    if len(values) > 1:
        step = values[1]
        for i in range(1, len(indices)):
            start += step
            result[indices[i]-1] = start
    
    print(f"Data: {item} Length: {len(indices)}")

print(','.join([f"{value:.2f}" for value in result]))
print(f"Total data count: {len(result)}")
