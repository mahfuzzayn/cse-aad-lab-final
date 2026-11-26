import sys

data = sys.stdin.read().split()
n = int(data[0])
W = float(data[1])
items = []
idx = 2
for _ in range(n):
    w = float(data[idx])
    v = float(data[idx + 1])
    idx += 2
    ratio = v / w if w > 0 else float("inf")
    items.append((ratio, w, v))
items.sort(key=lambda x: -x[0])
total = 0.0
cap = W
for _, w, v in items:
    if cap <= 0:
        break
    if w <= cap:
        total += v
        cap -= w
    else:
        total += v * (cap / w)
        cap = 0.0
print(f"{total:.2f}")
