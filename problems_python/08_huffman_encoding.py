import sys
import heapq

data = sys.stdin.read().split()
n = int(data[0])
if n <= 0:
    sys.exit()
sym = []
freq = []
idx = 1
for _ in range(n):
    sym.append(data[idx])
    freq.append(int(data[idx + 1]))
    idx += 2
w = freq + [0] * n
l = [-1] * (2 * n)
r = [-1] * (2 * n)
pq = [(freq[i], i) for i in range(n)]
heapq.heapify(pq)
cnt = n
while len(pq) > 1:
    fu, u = heapq.heappop(pq)
    fv, v = heapq.heappop(pq)
    w[cnt] = fu + fv
    l[cnt] = u
    r[cnt] = v
    heapq.heappush(pq, (w[cnt], cnt))
    cnt += 1
root = pq[0][1]
code = [""] * n
stack = [(root, "")]
while stack:
    u, c = stack.pop()
    if u < n:
        code[u] = c if c else "0"
        continue
    stack.append((l[u], c + "0"))
    stack.append((r[u], c + "1"))
total = 0
for i in range(n):
    print(sym[i], code[i])
    total += freq[i] * len(code[i])
print(total)
