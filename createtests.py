tests = []

with open('files.txt', 'r') as f:
    for line in f:
        test = ['0' if i == '.' else i for i in line.strip().split(':')[0]]
        tests.append(test)

for idx, i in enumerate(tests):
    with open(f'defaults/{idx}.txt', 'w') as f:
        f.write(' '.join(i))