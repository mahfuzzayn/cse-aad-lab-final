import sys

data = sys.stdin.read().split()
n = int(data[0])
a = list(map(int, data[1:1 + n]))
if n <= 0:
    print(0)
    sys.exit()
dp = [1] * n
par = [-1] * n
for i in range(1, n):
    for j in range(i):
        if a[j] < a[i] and dp[j] + 1 > dp[i]:
            dp[i] = dp[j] + 1
            par[i] = j
best = 0
for i in range(1, n):
    if dp[i] > dp[best]:
        best = i
seq = []
i = best
while i != -1:
    seq.append(a[i])
    i = par[i]
seq.reverse()
print(dp[best])
print(*seq)
