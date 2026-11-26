import sys

sys.setrecursionlimit(1000000)

data = sys.stdin.read().split()
n, m = int(data[0]), int(data[1])
edges = []
idx = 2
for _ in range(m):
    u, v, w = int(data[idx]), int(data[idx + 1]), int(data[idx + 2])
    idx += 3
    edges.append((w, u, v))
edges.sort()
par = list(range(n + 1))


def find(x):
    if par[x] == x:
        return x
    par[x] = find(par[x])
    return par[x]


total = 0
mst = []
for w, u, v in edges:
    a, b = find(u), find(v)
    if a == b:
        continue
    par[a] = b
    total += w
    mst.append((u, v, w))
    if len(mst) == n - 1:
        break
if n >= 1 and len(mst) != n - 1:
    print(-1)
    sys.exit()
for u, v, w in mst:
    print(u, v, w)
print(total)
