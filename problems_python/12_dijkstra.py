import sys
import heapq

data = sys.stdin.read().split()
n, m, src = int(data[0]), int(data[1]), int(data[2])
g = [[] for _ in range(n + 1)]
idx = 3
for _ in range(m):
    u, v, w = int(data[idx]), int(data[idx + 1]), int(data[idx + 2])
    idx += 3
    g[u].append((v, w))
    g[v].append((u, w))
INF = float("inf")
dist = [INF] * (n + 1)
if n >= 1 and 1 <= src <= n:
    dist[src] = 0
    pq = [(0, src)]
    while pq:
        d, u = heapq.heappop(pq)
        if d != dist[u]:
            continue
        for v, w in g[u]:
            if dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                heapq.heappush(pq, (dist[v], v))
print(" ".join(f"{i}:{-1 if dist[i] == INF else dist[i]}" for i in range(1, n + 1)))
