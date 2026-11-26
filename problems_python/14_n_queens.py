import sys

sys.setrecursionlimit(1000000)

n = int(sys.stdin.read().split()[0])
if n <= 0:
    print(0)
    sys.exit()
cnt = 0
first_sol = None
queen = [-1] * n
col_used = [False] * n
diag1 = [False] * (2 * n - 1)
diag2 = [False] * (2 * n - 1)


def solve(row):
    global cnt, first_sol
    if row == n:
        cnt += 1
        if first_sol is None:
            first_sol = queen[:]
        return
    for c in range(n):
        if col_used[c] or diag1[row + c] or diag2[row - c + n - 1]:
            continue
        queen[row] = c
        col_used[c] = True
        diag1[row + c] = True
        diag2[row - c + n - 1] = True
        solve(row + 1)
        col_used[c] = False
        diag1[row + c] = False
        diag2[row - c + n - 1] = False


solve(0)
print(cnt)
if first_sol is not None:
    for r in range(n):
        print("".join("Q" if first_sol[r] == c else "." for c in range(n)))
