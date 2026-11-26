# 14 — N-Queens Problem (Backtracking)

**Category 03** | Technique: Backtracking with constraint checking

---

## 1. The Problem

Place `n` queens on an `n×n` chessboard so that **no two queens attack each other**. Queens attack along their row, column, and both diagonals. Count the solutions and show one board.

**Input:** `n`. **Output:** number of distinct solutions, then the first board found (rows of `Q` / `.`).

For `n = 4` there are **2** solutions; `n = 8` has **92**; `n = 2` and `n = 3` have **0**.

---

## 2. The Intention

N-Queens is the **flagship backtracking problem**. Permutation (explanation 01) generates all arrangements blindly; N-Queens adds the missing half of the skill: **pruning** — abandoning a branch the moment it violates a constraint, instead of building it to the end and checking.

The intellectual leap is realizing:

> **One queen per row** is free (just decide, row by row, *which column* each queen takes) — so the only real constraints to check are columns and diagonals.

---

## 3. The Core Idea (No Code Yet)

### Collapse the board to one decision per row

If every row must contain exactly one queen, a board is just an array:

```
queen[r] = column of the queen in row r
```

Search space: n choices per row, `n^n` raw arrangements — but we only ever try *safe* ones.

### The safety check for putting a queen at (row r, col c)

A previous queen at (row j, col queen[j]) attacks us iff:

1. `queen[j] == c` — same **column**
2. `|r − j| == |c − queen[j]|` — same **diagonal** (equal step down and across)

Rows can't clash by construction (we place one per row, moving downward).

### Diagonal indexing tricks (O(1) checks)

Every square belongs to:

- one "\" diagonal — cells on it share the sum `r + c` → index `r+c` (0 … 2n−2)
- one "/" diagonal — cells on it share the difference `r − c`, which is negative, so shift: `r − c + n − 1` (0 … 2n−2)

Keep three boolean arrays: `colUsed[n]`, `diag1[2n−1]` (sums), `diag2[2n−1]` (differences). A square is free iff all three flags are false. O(1) per check — no loops.

### The backtracking loop

```
solve(row):
    if row == n: found a full board → count it, remember it
    for each column c in 0..n-1:
        if (row, c) is safe:
            place queen: set the three flags
            solve(row + 1)              ← go deeper
            remove queen: clear the three flags   ← backtrack
```

The place/remove pair around the recursive call is the entire backtracking pattern.

### Pruning — the whole point

If row 0's queen and row 1's queen already share a diagonal, no arrangement of rows 2..n−1 can fix it. Backtracking abandons that subtree instantly. For n=8: from `8^8 ≈ 16.7M` blind arrangements down to ~2057 explored nodes — pruning removes ~99.99%.

---

## 4. The Whiteboard Process (n = 4 trace)

Board 4×4. Track the three marker arrays as the search goes. Dead ends are where the learning is:

```
Row 0: try c=0 → place Q at (0,0)
  Row 1: c=0 ✘ col | c=1 ✘ diag(\) | c=2 ✓ place (1,2) | c=3 ✘ diag(/)
    Row 2: c=0 ✘ diag | c=1 ✘ col... col free? col1 unused BUT (2,1)vs(0,0): |2-0|==|1-0| ✘ diag
           c=2 ✘ col | c=3 ✘ diag (|2-1|==|3-2|)  → DEAD END, all 4 columns fail
    backtrack: remove (1,2)
  Row 1 exhausted → DEAD END
backtrack: remove (0,0)

Row 0: try c=1 → place Q at (0,1)
  Row 1: c=3 ✓ (c=0: diag ✘, c=1 col ✘, c=2 diag ✘)
    Row 2: c=0 ✓ place (2,0)
      Row 3: c=2 ✓ place (3,2) → row == 4 → SOLUTION #1 ✔

  (board 1:)
  .Q..
  ...Q
  Q...
  ..Q.

  ... search continues after unplacing ...
Row 0: c=2 → (by symmetry with c=1 mirrored)
      → SOLUTION #2:
  ..Q.
  Q...
  ...Q
  .Q..
Row 0: c=3 → mirrors the c=0 failure → dead end
```

**Total: 2 solutions.** Note how each dead end was discovered at row 1 or 2 — long before the board filled up. That early abandonment is pruning.

### The marker arrays at the moment of solution #1

Queens at (0,1), (1,3), (2,0), (3,2):

| Array | Meaning | Set indices |
|---|---|---|
| colUsed | columns occupied | 1, 3, 0, 2 → all four |
| diag1 (`r+c`) | "\" diagonals | 1, 4, 2, 5 |
| diag2 (`r−c+3`) | "/" diagonals | 4, 1, 5, 2 |

Every queen sits on a *unique* column and unique diagonals of both kinds — that's exactly what "no attacks" means.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| board = one column per row | `vector<int> queen(n)` |
| three marker arrays | `colUsed`, `diag1`, `diag2` |
| safe ⇔ all three false | the `if` in the loop |
| place / explore / remove | set flags → `solve(row+1)` → clear flags |
| solution reached | `row == n` → `cnt++`, save first board |
| print board | row of `Q`/`.` per queen position |

**C++** (`problems/14_n_queens.cpp`) — the entire search:

```cpp
void solve(int row) {
    if (row == n) { cnt++; if (!found) { firstSol = queen; found = true; } return; }
    for (int c = 0; c < n; c++) {
        if (colUsed[c] || diag1[row + c] || diag2[row - c + n - 1]) continue;  // pruned
        queen[row] = c;
        colUsed[c] = diag1[row + c] = diag2[row - c + n - 1] = true;   // place
        solve(row + 1);                                                 // explore
        colUsed[c] = diag1[row + c] = diag2[row - c + n - 1] = false;  // remove
    }
}
```

**Python** (`problems_python/14_n_queens.py`) — same three arrays and recursion.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n!)` worst case (heavily pruned in practice) | row 0 has n options, row 1 ≤ n−1 safe ones, ... |
| Space | `O(n)` | recursion depth + the marker arrays |

Actual solution counts: n=4→2, n=5→10, n=6→4, n=7→40, n=8→92, n=9→352, n=10→724. Growth is far below n! — that's pruning at work.

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `n = 1` | 1 solution: single `Q` |
| `n = 2` or `n = 3` | 0 solutions (provably impossible) — prints `0`, no board |
| `n ≤ 0` | prints `0` and exits |
| large n (≥ 13) | still correct but slow — backtracking explores millions of nodes; the program is meant for the classic lab range n ≤ 12 |

---

## 8. Viva Questions

**Q: Why exactly one queen per row?**
Any valid board has exactly n queens on n rows, so at most one per row; permuting columns then covers every arrangement. The row constraint becomes automatic.

**Q: How do you check diagonals in O(1)?**
The "\" diagonal has constant `r+c`; the "/" diagonal has constant `r−c` (shifted by n−1 to index an array). Two boolean arrays replace the loop over previous queens.

**Q: What is pruning?**
Cutting a search branch as soon as a partial assignment already violates a constraint — never completing doomed boards.

**Q: Why is this backtracking and not brute force?**
Brute force would generate all `n^n` column assignments and check each fully. Backtracking checks constraints *incrementally* and abandons early — orders of magnitude fewer nodes.

**Q: How many solutions for the classic 8-queens?**
92 (12 fundamental up to symmetry).

**Q: How would you print ALL boards instead of one?**
Print inside the `row == n` base case instead of storing the first solution (output becomes large quickly).

**Q: What guarantees completeness — that we don't miss solutions by undoing?**
The loop at each row tries every safe column; undoing flags restores the exact pre-call state, so sibling branches explore a pristine board. Every safe sequence of choices is eventually visited.
