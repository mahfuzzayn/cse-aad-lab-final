# 05 — Longest Common Subsequence (LCS)

**Category 02** | Technique: Dynamic Programming on two strings

---

## 1. The Problem

Given two strings, find the longest sequence that is a subsequence of **both**.

A subsequence keeps the left-to-right order but may skip characters: `ACE` is a subsequence of `ABCDE` (skip B, D).

Example: `ABCBDAB` and `BDCABA` → LCS length **4**, one LCS is `BCBA` (or `BDAB` — ties can exist).

**Input:** two strings on one line. **Output:** LCS length, then one LCS string.

---

## 2. The Intention

LCS is the gateway to **two-dimensional DP** — the state depends on a *pair* of prefixes, so the table is a grid. It powers real tools: `diff`, git merge, DNA sequence alignment, plagiarism checkers. If you can fill the LCS table by hand, you can fill *any* 2-D DP table.

---

## 3. The Core Idea (No Code Yet)

### The state

> `dp[i][j]` = *length of the LCS of the first `i` characters of `s` and the first `j` characters of `t`*

`s = ABCBDAB` → prefixes ` "", A, AB, ABC, ...`
`t = BDCABA` → prefixes ` "", B, BD, BDC, ...`

Table is `(n+1) × (m+1)`. Row 0 and column 0 (empty prefix) are all zeros — comparing against an empty string gives nothing in common.

### The decision — look at the **last** characters of the two prefixes

Let `x = s[i-1]` (last char of s-prefix) and `y = t[j-1]` (last char of t-prefix):

```
if x == y   (the characters MATCH):
    they can both join the LCS  →  dp[i][j] = dp[i-1][j-1] + 1

if x != y   (they DIFFER):
    at least one of them is not the LCS's last char, so drop one:
    dp[i][j] = max( dp[i-1][j] , dp[i][j-1] )
               (drop x)          (drop y)
```

That's the entire algorithm — match extends diagonally, mismatch takes the better of up/left.

**Why is the match case safe (why not also consider max with up/left)?** If the last chars match, there is always an optimal LCS that *ends* with that matched pair — extending `dp[i-1][j-1]` can never be worse. (Formal proof is the "cutting lemma" in CLRS.)

---

## 4. The Whiteboard Process (Full Table Trace)

`s = ABCBDAB` (rows, top to bottom), `t = BDCABA` (columns, left to right).

Write the empty row/column first, then letters:

```
          ""   B    D    C    A    B    A
    ""    0    0    0    0    0    0    0
    A     0    0    0    0    1    1    1
    B     0    1    1    1    1    2    2
    C     0    1    1    2    2    2    2
    B     0    1    1    2    2    3    3
    D     0    1    2    2    2    3    3
    A     0    1    2    2    3    3    4
    B     0    1    2    2    3    4    4
```

A few cells worked out in detail (remember: match → diagonal + 1; mismatch → max of up, left):

| Cell | Chars | Rule | Value |
|---|---|---|---|
| row A, col A | A = A match | `dp[up-left] + 1 = 0 + 1` | **1** |
| row B, col B | B = B match | `0 + 1` | **1** |
| row B, col C | B ≠ C | `max(up 1, left 1)` | **1** |
| row C, col C | C = C match | `dp[B][B] + 1 = 1 + 1` | **2** |
| row B(5th row), col B | B = B match | `dp[C][C] + 1 = 2 + 1` | **3** |
| row A(7th), col A | A = A match | `dp[D][B] + 1 = 3 + 1` | **4** |
| row B(8th), col B | B = B match | `dp[A][A] + 1 = 3 + 1` | **4** |

**Answer = bottom-right = 4.**

### Reconstructing the LCS string (arrow walk)

Start bottom-right `(8, 6)` and walk **backwards**:

- chars **match** → this char is in the LCS; record it; move **diagonally up-left**
- chars differ → move toward whichever neighbour holds the bigger value (up if `dp[i-1][j] ≥ dp[i][j-1]`, else left)

```
(8,6) B=B match → take B, go (7,5)
(7,5) A≠B, up(3) ≥ left(3) → go up (6,5)
(6,5) D≠B → up(2) < left(3) → go left (6,4)
(6,4) D≠A → up(2) ≥ left(2) → go up (5,4)
(5,4) B≠A → up(2) < left(3) → go left (5,3)
(5,3) B≠C → up(2) ≥ left(2) → go up (4,3)
(4,3) C=C match → take C, go (3,2)
(3,2) C≠D → up(1) ≥ left(1) → go up (2,2)
(2,2) B≠D → up(1) ≥ left(1) → go up (1,2)
(1,2) A≠D → up(0) < left(1) → go left (1,1)
(1,1) A=B match → take A, go (0,0) → STOP
```

Collected (backwards): `B C A` … plus the last A → reading in reverse: **`BCBA`** ✓

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| grid with empty row/col | `dp[n+1][m+1]` initialized 0 |
| match → diagonal + 1 | `if (s[i-1]==t[j-1]) dp[i][j]=dp[i-1][j-1]+1` |
| mismatch → max(up, left) | `else dp[i][j]=max(dp[i-1][j], dp[i][j-1])` |
| arrow walk backwards | while loop with the same three rules |
| collected reversed | `reverse(ans.begin(), ans.end())` |

**C++** (`problems/05_lcs.cpp`) — the reconstruction loop mirrors the fill rule exactly, so the code is just the whiteboard rules typed twice.

**Python** (`problems_python/05_lcs.py`) — identical.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n · m)` | every cell once, O(1) per cell |
| Space | `O(n · m)` | table (the string reconstruction needs the full table) |

Length-only variant can be `O(min(n,m))` space with a rolling row.

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| one string empty | row/col 0 → length 0, empty LCS line |
| no common character | length 0, empty line |
| identical strings | LCS = the string itself, length n |
| multiple LCSs of same length | program prints one of them (tie-break: prefer "up" on ties) |

---

## 8. Viva Questions

**Q: Subsequence vs substring?**
Subsequence may skip characters (order preserved); substring must be contiguous. `ACE` ⊂ `ABCDE` as subsequence only.

**Q: Why does a match always move diagonally?**
Both matched characters are the last of their prefixes, so the rest of the LCS must live in the two shortened prefixes — exactly `dp[i-1][j-1]`.

**Q: Why is this DP and not greedy?**
Skipping a matching pair can never help, but *which* matches to keep interacts across the whole table — no local rule decides; the table enumerates all prefix pairs.

**Q: Number of subproblems?**
`(n+1)(m+1)` — each solved in O(1) from smaller ones → optimal substructure + overlapping subproblems = DP.

**Q: Applications?**
diff/patch, git merge (myers diff), bioinformatics alignment, spell-check suggestions, plagiarism detection.

**Q: What would longest common SUBSTRING change?**
On mismatch write 0 instead of max(up,left); the answer is the max cell anywhere in the table, not the corner.
