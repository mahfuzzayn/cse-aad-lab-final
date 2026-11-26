import sys

data = sys.stdin.read().split()
n, m, src = int(data[0]), int(data[1]), int(data[2])
e = []
idx = 3
for _ in range(m):
    u, v, w = int(data[idx]), int(data[idx + 1]), int(data[idx + 2])
    idx += 3
    e.append((u, v, w))
INF = float("inf")
dist = [INF] * (n + 1)
if n >= 1 and 1 <= src <= n:
    dist[src] = 0
    for _ in range(n - 1):
        changed = False
        for u, v, w in e:
            if dist[u] == INF:
                continue
            if dist[u] + w < dist[v]:
                dist[v] = dist[u] + w
                changed = True
        if not changed:
            break
    for u, v, w in e:
        if dist[u] == INF:
            continue
        if dist[u] + w < dist[v]:
            print(-1)
            sys.exit()
print(" ".join(f"{i}:{-1 if dist[i] == INF else dist[i]}" for i in range(1, n + 1)))
