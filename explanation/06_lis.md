# 06 — Longest Increasing Subsequence (LIS)

**Category 02** | Technique: Dynamic Programming (1-D, O(n²) table method)

---

## 1. The Problem

Given a sequence of numbers, find the longest subsequence whose values **strictly increase** (keep left-to-right order, skip freely).

Example: `10 9 2 5 3 7 101 18` → LIS length **4**, one LIS is `2 5 7 101` (also `2 3 7 101`, `2 5 7 18`, ...).

**Input**
```
n
a1 a2 ... an
```
**Output:** LIS length, then one LIS.

---

## 2. The Intention

LIS is the classic **1-D DP** problem — the state is a single index, not a pair. It's also your reintroduction to it after learning it before: the trick worth remembering is *redefining the question so it has an answer you can build on*: not "LIS of the whole array" (too vague) but "LIS **ending exactly at** position i" (concrete). That redefinition habit is 90% of DP skill.

Applications: patience sorting, longest chain of envelopes, git rebase histories, stock trend analysis.

---

## 3. The Core Idea (No Code Yet)

### The state (the crucial redefinition)

> `dp[i]` = *length of the longest increasing subsequence that **ends exactly at** `a[i]`*

Why "ending at i"? Because then the transition is well-defined: any earlier element `a[j]` with `a[j] < a[i]` can be the element *just before* `a[i]` in a chain.

### The transition

```
dp[i] = 1 + max( dp[j] )   over all j < i with a[j] < a[i]
        (if no such j exists, dp[i] = 1 — the chain is just a[i] alone)
```

Read it as: *"who can I legally stand on top of?"* — only smaller earlier elements, and you want the tallest stack among them.

### Base case

`dp[i] = 1` for all i (every element alone is an increasing subsequence of length 1).

### The final answer

`max(dp[0..n-1])` — **not** necessarily `dp[n-1]`, because the best chain can end anywhere!

### Recovering the actual sequence

Keep `par[i]` = the index `j` you extended from. After the fill, start at the argmax index and follow parent pointers backwards; reverse at the end.

---

## 4. The Whiteboard Process (Full Trace)

Input: `a = [10, 9, 2, 5, 3, 7, 101, 18]`, n = 8.

Draw one row per element. For each i, scan everything to its left:

| i | a[i] | smaller elements to the left (j: a[j]<a[i]) | their dp | dp[i] | par[i] |
|---|------|---------------------------------------------|----------|-------|--------|
| 0 | 10 | — none | — | **1** | −1 |
| 1 | 9 | none (10 > 9) | — | **1** | −1 |
| 2 | 2 | none | — | **1** | −1 |
| 3 | 5 | j=2 (2) | dp=1 | **2** | 2 |
| 4 | 3 | j=2 (2) | dp=1 | **2** | 2 |
| 5 | 7 | j=2 (2), j=3 (5), j=4 (3) | 1, 2, 2 | **3** | 3 |
| 6 | 101 | all j=0..5 | 1,1,1,2,2,3 | **4** | 5 |
| 7 | 18 | j=0..5 (all but 101) | 1,1,1,2,2,3 | **4** | 5 |

Worked cells in words:

- **i=3 (value 5):** who is smaller to my left? Only `2` (dp 1). So I can stand on a chain of length 1 → `dp[3] = 1 + 1 = 2`, `par[3] = 2`.
- **i=5 (value 7):** smaller before me: `2`, `5`, `3` with dp 1, 2, 2. Best is 2 (either `2 5` or `2 3`) → `dp[5] = 3`, `par[5] = 3` (first max found).
- **i=6 (value 101):** everything is smaller; best dp before me is 3 → `dp[6] = 4`, `par[6] = 5`.

### Answer

`max(dp) = 4`, first reached at `i = 6`.

### Chain walk (follow par from i=6)

```
101 (par 5) → 7 (par 3) → 5 (par 2) → 2 (par −1) STOP
backwards: 2 5 7 101     ← reverse of the walk
```

**LIS = 2 5 7 101** ✓

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| dp[i] = 1 default | `vector<int> dp(n, 1)` |
| "who can I stand on?" | inner loop `j = 0..i-1`, condition `a[j] < a[i]` |
| take best predecessor | `if (dp[j] + 1 > dp[i]) { dp[i] = dp[j]+1; par[i] = j; }` |
| answer = max over all ends | scan dp for its maximum index `best` |
| chain walk | `for (i = best; i != -1; i = par[i])` then reverse |

**C++** (`problems/06_lis.cpp`)

```cpp
for (int i = 1; i < n; i++) {
    for (int j = 0; j < i; j++) {
        if (a[j] < a[i] && dp[j] + 1 > dp[i]) {
            dp[i] = dp[j] + 1;
            par[i] = j;
        }
    }
}
```

**Python** (`problems/06_lis.py`) — identical double loop.

Note `a[j] < a[i]` is **strict**: equal values don't extend the chain (strictly increasing). For non-decreasing change it to `<=`.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n²)` | double loop: for each i, scan all j < i |
| Space | `O(n)` | dp + par arrays |

There is a faster `O(n log n)` method (binary search over a "tails" array — patience sorting), but it does *not* come from this table and is harder to explain on a whiteboard; the O(n²) DP is the expected lab answer.

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `n = 0` | prints `0` and exits |
| `n = 1` | length 1, the element itself |
| strictly decreasing array | every dp[i] = 1 → length 1, prints `a[0]` (`best` stays 0, first maximum found) |
| all equal values | strict `<` → length 1 |
| duplicates mixed in | duplicates never chain together; each extends only from strictly smaller |

---

## 8. Viva Questions

**Q: Why must the state be "ending at i" instead of "LIS of the first i elements"?**
Because "LIS of prefix i" cannot transition: the best prefix-LIS may end with a huge value that blocks appending `a[i]`. Knowing *where* a chain ends is the information the next step needs.

**Q: Why is the answer max(dp) and not dp[n−1]?**
The longest chain can end at any position; nothing forces it through the last element.

**Q: How do you get the actual sequence, not just the length?**
Store the predecessor index in `par[]`, walk back from the argmax, reverse.

**Q: LIS vs LCS — how are they related?**
LIS of an array with distinct values = LCS of the array and its sorted version. (Sort `a`, remove duplicates, run LCS.)

**Q: How would O(n log n) work?**
Maintain `tails[k]` = smallest possible tail of an increasing subsequence of length k+1; each element binary-searches its position → `O(n log n)`. Length only; reconstruction needs extra bookkeeping.

**Q: What changes for longest *non-decreasing* subsequence?**
Comparison becomes `a[j] <= a[i]`.
