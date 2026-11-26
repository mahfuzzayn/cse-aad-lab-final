import sys
import heapq

data = sys.stdin.read().split()
n, m = int(data[0]), int(data[1])
g = [[] for _ in range(n + 1)]
idx = 2
for _ in range(m):
    u, v, w = int(data[idx]), int(data[idx + 1]), int(data[idx + 2])
    idx += 3
    g[u].append((v, w))
    g[v].append((u, w))
if n <= 0:
    print(0)
    sys.exit()
vis = [False] * (n + 1)
pq = [(0, 1, -1)]
total = 0
mst = []
while pq and len(mst) < n:
    w, u, p = heapq.heappop(pq)
    if vis[u]:
        continue
    vis[u] = True
    total += w
    if p != -1:
        mst.append((p, u, w))
    for v, w2 in g[u]:
        if not vis[v]:
            heapq.heappush(pq, (w2, v, u))
cnt = sum(1 for i in range(1, n + 1) if vis[i])
if cnt < n:
    print(-1)
    sys.exit()
for u, v, w in mst:
    print(u, v, w)
print(total)
