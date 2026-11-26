import sys

sys.setrecursionlimit(1000000)


def comb(start):
    if len(cur) == r:
        print(*cur)
        return
    for i in range(start, n):
        cur.append(a[i])
        comb(i + 1)
        cur.pop()


data = sys.stdin.read().split()
n, r = int(data[0]), int(data[1])
a = sorted(map(int, data[2:2 + n]))
cur = []
if n > 0 and 0 <= r <= n:
    comb(0)
