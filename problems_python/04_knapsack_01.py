import sys

data = sys.stdin.read().split()
n, W = int(data[0]), int(data[1])
w = [0] * (n + 1)
v = [0] * (n + 1)
idx = 2
for i in range(1, n + 1):
    w[i] = int(data[idx])
    v[i] = int(data[idx + 1])
    idx += 2
dp = [[0] * (W + 1) for _ in range(n + 1)]
for i in range(1, n + 1):
    for j in range(W + 1):
        dp[i][j] = dp[i - 1][j]
        if w[i] <= j:
            dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i]] + v[i])
print(dp[n][W])
