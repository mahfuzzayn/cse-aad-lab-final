import sys
from collections import deque

data = sys.stdin.read().split()
n, m = int(data[0]), int(data[1])
g = [[] for _ in range(n + 1)]
indeg = [0] * (n + 1)
idx = 2
for _ in range(m):
    u, v = int(data[idx]), int(data[idx + 1])
    idx += 2
    g[u].append(v)
    indeg[v] += 1
q = deque(i for i in range(1, n + 1) if indeg[i] == 0)
order = []
while q:
    u = q.popleft()
    order.append(u)
    for v in g[u]:
        indeg[v] -= 1
        if indeg[v] == 0:
            q.append(v)
if len(order) != n:
    print(-1)
else:
    print(*order)
