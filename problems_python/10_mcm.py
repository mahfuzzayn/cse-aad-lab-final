import sys

sys.setrecursionlimit(1000000)

data = sys.stdin.read().split()
n = int(data[0])
if n <= 0:
    print(0)
    sys.exit()
p = list(map(int, data[1:2 + n]))
m = [[0] * (n + 1) for _ in range(n + 1)]
s = [[0] * (n + 1) for _ in range(n + 1)]
for length in range(2, n + 1):
    for i in range(1, n - length + 2):
        j = i + length - 1
        m[i][j] = float("inf")
        for k in range(i, j):
            cost = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j]
            if cost < m[i][j]:
                m[i][j] = cost
                s[i][j] = k


def print_order(i, j):
    if i == j:
        print(f"A{i}", end="")
        return
    print("(", end="")
    print_order(i, s[i][j])
    print("*", end="")
    print_order(s[i][j] + 1, j)
    print(")", end="")


print(m[1][n])
print_order(1, n)
print()
