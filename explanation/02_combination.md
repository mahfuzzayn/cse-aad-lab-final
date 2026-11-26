# 02 — Combination (Generate All Combinations of Size r)

**Category 01** | Technique: Recursion with a Start Index

---

## 1. The Problem

Given `n` items and a size `r`, print **every way of choosing `r` of them**, where **order does not matter**.

For `{1, 2, 3, 4}` and `r = 2`:

```
1 2
1 3
1 4
2 3
2 4
3 4
```

Total count = C(n, r) = `4! / (2! · 2!)` = `6`.

**Input**
```
n r
a1 a2 ... an
```

**Output:** all C(n, r) combinations in lexicographic order.

---

## 2. The Intention

Combination is the twin of permutation. The difference teaches a key idea:

- Permutation: order matters → `{1,2}` and `{2,1}` are **different**
- Combination: order does not matter → they are the **same**, print once

To avoid printing the same group twice, we must **force an order** on ourselves: once we pick item `i`, the next pick must come *after* `i`. That single rule kills all duplicates. This "start index" idea reappears in subset-sum, team selection, and many DP problems.

---

## 3. The Core Idea (No Code Yet)

The enemy is duplicates. `{2,1}`, `{1,2}` — same set, two orders. Rule:

> **Each next chosen item must have a bigger index than the previously chosen one.**

So every combination is generated exactly once, in its sorted form.

Think of it as reading the items **left to right** and, at each item, deciding *pick it* or *skip it* — but you may never go back. Since you only move forward, no rearrangement is possible.

The count formula falls out naturally:
C(n, r) = number of ways to mark `r` "picked" positions among `n` = `n! / (r!(n−r)!)`.

---

## 4. The Whiteboard Process (Recursion Tree)

Input: `{1, 2, 3, 4}`, `r = 2`. Tree levels = how many items picked so far. The number on a branch = index we start scanning from.

```
                      start=0, have [ ]
            ┌────────────┬────────────┬────────────┐
          pick 1       pick 2       pick 3        pick 4
            │            │            │             │
        [1], start=2  [2], start=3  [3], start=4  [4], start=5
        ┌───┴───┐      ┌──┴──┐        │             │
      pick2  pick3   pick3 pick4     pick4        r=2 → PRINT "4 ..."? 
        │      │       │    │          │            no item left → dead end
     [1 2]  [1 3]   [2 3] [2 4]     [3 4]
     PRINT  PRINT   PRINT PRINT     PRINT
        │
     also pick 4 under [1]:
     [1 2] [1 3] [1 4] [2 3] [2 4] [3 4]   <- all 6
```

Full picture for the `[1 ...]` branch:

| Path | Build | Result |
|---|---|---|
| pick 1, pick 2 | `[1 2]` | print |
| pick 1, pick 3 | `[1 3]` | print |
| pick 1, pick 4 | `[1 4]` | print |
| pick 2, pick 3 | `[2 3]` | print |
| pick 2, pick 4 | `[2 4]` | print |
| pick 3, pick 4 | `[3 4]` | print |
| pick 4, then nothing left | `[4]` | dead end, r never reached |

Two stopping rules:

1. `len(cur) == r` → **success**, print
2. `start` ran past the end of the array → nothing left to pick, dead end (loop simply doesn't execute)

### Why no duplicates can ever appear

Suppose we picked index 3 (`value 4`) — can we later pick index 1 (`value 2`)? No: the loop for the next level starts at `start = 4`. The start index only ever grows. Rearrangement is impossible by construction.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| picked r items → print | `if ((int)cur.size() == r)` |
| only look forward from `start` | loop `i = start .. n-1` |
| pick item i | `cur.push_back(a[i])` |
| next level starts after i | recursive call with `i + 1` |
| backtrack | `cur.pop_back()` |

Note: unlike permutation there is **no `taken[]` array** — the start index alone already prevents reuse.

**C++** (`problems/02_combination.cpp`)

```cpp
void comb(int start) {
    if ((int)cur.size() == r) {           // chose r items
        for (int i = 0; i < r; i++) cout << cur[i] << " \n"[i == r - 1];
        return;
    }
    for (int i = start; i < n; i++) {     // only items after 'start'
        cur.push_back(a[i]);              // choose
        comb(i + 1);                      // explore, next start = i+1
        cur.pop_back();                   // backtrack
    }
}
```

**Python** (`problems_python/02_combination.py`) — same structure.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(r · C(n, r))` | C(n, r) results, each printed in O(r) |
| Space | `O(r)` | recursion depth + `cur` |

The internal nodes of the tree only add a constant factor — total nodes ≈ `2^n` at most, which is ≤ `r · C(n,r)` plus small overhead.

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `r = 0` | one combination (the empty one) — loop prints nothing meaningful, so program exits silently with no lines |
| `r > n` | impossible → no output |
| `r = n` | exactly one line: the whole array |
| `n = 0` | no output |
| unsorted input | sorted first so output is lexicographic |

---

## 8. Viva Questions

**Q: Difference between permutation and combination?**
Permutation = arrangement (order matters, count `nPr = n!/(n−r)!`). Combination = selection (order irrelevant, count `nCr = n!/(r!(n−r)!)`). `{1,2}` = `{2,1}` as a combination.

**Q: Why doesn't combination need the `taken[]` array?**
The start index `i+1` guarantees we never revisit earlier indices, so an item can never be picked twice.

**Q: How many nodes does the recursion tree have?**
Every node is a subset of the remaining suffix — bounded by `2^n` total, with exactly C(n, r) leaves that print.

**Q: What is C(n, r) + C(n, r−1)?**
`C(n+1, r)` — Pascal's rule; it's the "pick or skip the first item" split: skipping gives C(n, r), picking gives C(n, r−1).

**Q: How would you generate ALL subsets (any size) instead?**
Remove the `size == r` stop: print `cur` at every node of the tree. That visits all `2^n` subsets.
