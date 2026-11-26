import sys

sys.setrecursionlimit(1000000)


def perm():
    if len(cur) == n:
        print(*cur)
        return
    for i in range(n):
        if taken[i]:
            continue
        taken[i] = True
        cur.append(a[i])
        perm()
        cur.pop()
        taken[i] = False


data = sys.stdin.read().split()
n = int(data[0])
a = sorted(map(int, data[1:1 + n]))
if n > 0:
    taken = [False] * n
    cur = []
    perm()
