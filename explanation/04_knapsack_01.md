# 04 — 0/1 Knapsack (Dynamic Programming)

**Category 02** | Technique: Dynamic Programming (table method)

---

## 1. The Problem

Bag capacity `W`. `n` items, item `i` has weight `w_i` and value `v_i`. Take each item **at most once, whole** (that's the "0/1": take it or leave it). Maximize total value without exceeding `W`.

**Input**
```
n W
w1 v1
w2 v2
...
```

**Output:** maximum achievable value.

---

## 2. The Intention

This is **the** canonical DP teaching problem. Greedy fails here (see 03's trap example), so we must be smarter: solve many *small subproblems*, store their answers in a **table**, and build the big answer from small ones. Everything about DP — state, base case, transition, filling order — shows up on this one table your teacher draws on the whiteboard.

---

## 3. The Core Idea (No Code Yet)

### The state

Ask a precise question that has a short answer:

> `dp[i][j]` = *using only the first `i` items, with capacity `j`, what is the best value?*

The table has `(n+1)` rows and `(W+1)` columns — one row per "items considered so far", one column per "capacity left".

### The decision (the heart of the whole algorithm)

When item `i` arrives with capacity `j` available, there are only two possibilities:

```
                 item i fits? (w_i <= j)
                /                        \
             yes                          no
              │                            │
     ┌────────┴────────┐                skip it
     │                 │            dp[i][j] = dp[i-1][j]
   TAKE it           SKIP it
dp[i][j] =                      dp[i][j] = dp[i-1][j]
 v_i + dp[i-1][j - w_i]
     │
 take max of the two
```

That one line is the whole algorithm:

```
dp[i][j] = max( dp[i-1][j],  v_i + dp[i-1][j - w_i] )    (only if w_i <= j)
dp[i][j] = dp[i-1][j]                                   (if it doesn't fit)
```

### Base case

`dp[0][j] = 0` for every `j` — zero items means zero value. The first row is all zeros.

### The answer

`dp[n][W]` — bottom-right corner of the table (last row, last column).

---

## 4. The Whiteboard Process (Full Table Trace)

Input: `n = 4`, `W = 7`

| Item | Weight | Value |
|---|---|---|
| 1 | 1 | 1 |
| 2 | 3 | 4 |
| 3 | 4 | 5 |
| 4 | 5 | 7 |

Draw a table: rows = items 0..4, columns = capacity 0..7.

**Row 0** (no items): all zeros.

```
        j=0  j=1  j=2  j=3  j=4  j=5  j=6  j=7
i=0      0    0    0    0    0    0    0    0
```

**Row 1** (item 1: w=1, v=1). For every j ≥ 1 it fits: `max(above, 1 + dp[0][j-1]) = 1`.

```
i=1      0    1    1    1    1    1    1    1
```

**Row 2** (item 2: w=3, v=4). Fits when j ≥ 3:
- j=3: `max(1, 4 + dp[1][0]) = max(1, 4) = 4`
- j=4: `max(1, 4 + dp[1][1]) = max(1, 5) = 5`
- j=5: `max(1, 4 + dp[1][2]) = 5`
- j=6: `max(1, 4 + dp[1][3]) = 5`
- j=7: `max(1, 4 + dp[1][4]) = 5`

```
i=2      0    1    1    4    5    5    5    5
```

**Row 3** (item 3: w=4, v=5). Fits when j ≥ 4:
- j=4: `max(5, 5 + dp[2][0]) = 5`
- j=5: `max(5, 5 + dp[2][1]) = max(5, 6) = 6`
- j=6: `max(5, 5 + dp[2][2]) = 6`
- j=7: `max(5, 5 + dp[2][3]) = max(5, 9) = 9`

```
i=3      0    1    1    4    5    6    6    9
```

**Row 4** (item 4: w=5, v=7). Fits when j ≥ 5:
- j=5: `max(6, 7 + dp[3][0]) = 7`
- j=6: `max(6, 7 + dp[3][1]) = 8`
- j=7: `max(9, 7 + dp[3][2]) = 9`

```
i=4      0    1    1    4    5    7    8    9
```

**Answer = dp[4][7] = 9** (items 2 and 3: weight 3+4=7, value 4+5=9).

Reading rule while filling: **the value you compare against always comes from the row above** — either straight up (skip) or up-and-left by `w_i` (take).

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| table `(n+1) × (W+1)` | `vector<vector<int>> dp(n+1, vector<int>(W+1, 0))` |
| row 0 = zeros | the initializer already makes everything 0 |
| "doesn't fit → copy above" | `dp[i][j] = dp[i-1][j]` first |
| "fits → max(skip, take)" | `if (w[i] <= j) dp[i][j] = max(dp[i][j], dp[i-1][j-w[i]] + v[i])` |
| answer bottom-right | `dp[n][W]` |

**C++** (`problems/04_knapsack_01.cpp`)

```cpp
for (int i = 1; i <= n; i++) {
    for (int j = 0; j <= W; j++) {
        dp[i][j] = dp[i - 1][j];                       // skip item i
        if (w[i] <= j)                                  // it fits
            dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i]] + v[i]);  // take it
    }
}
```

**Python** (`problems/04_knapsack_01.py`) — identical double loop.

### Which items were taken? (common viva follow-up)

Walk the finished table *backwards*: start at `(n, W)`; if `dp[i][j] != dp[i-1][j]` then item `i` was taken — subtract `w_i` from `j` and go up a row; otherwise just go up a row. For the trace above: (4,7)=9 = (3,7)=9 → item 4 skipped; (3,7)=9 ≠ (2,7)=5 → item 3 taken, j=7−4=3; (2,3)=4 ≠ (1,3)=1 → item 2 taken, j=0; done → items {2,3}.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n · W)` | one pass over the whole table |
| Space | `O(n · W)` | the table itself (can be reduced to `O(W)` with one rolling row) |

Note `O(n·W)` is *pseudo-polynomial*: W is a numeric value, not an input size — doubling W doubles work even though the input only grew by one bit.

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `W = 0` | only column 0 exists → all zeros → answer 0 |
| `n = 0` | only row 0 → answer 0 |
| item weight 0 | always fits; `v_i` is free value (take if positive) |
| item heavier than W | never fits in any column → row copies the row above |
| duplicate items | treated as distinct copies — allowed |

---

## 8. Viva Questions

**Q: Define the DP state in one sentence.**
`dp[i][j]` = best value achievable using only the first `i` items within capacity `j`.

**Q: Why does greedy fail but DP succeed?**
Greedy commits to one locally-best choice; DP evaluates *both* branches (take/skip) for every (item, capacity) pair, so no possibility is lost.

**Q: Why fill row by row?**
Each row depends only on the previous row; row-major order guarantees `dp[i-1][...]` is complete before row `i` starts.

**Q: Can you reduce memory to one row?**
Yes — keep one row of size `W+1` and iterate `j` **from W down to 0** (descending, so the "row above" values aren't overwritten before use).

**Q: Is 0/1 knapsack NP-hard?**
The general problem is NP-hard; DP is pseudo-polynomial — polynomial in the *value* of W, not in the number of bits to write W.

**Q: What changes for the fractional version?**
Fractions make the per-kg exchange argument valid, so sorting by ratio (greedy) is optimal — see explanation 03.
