# 01 — Permutation (Generate All Permutations)

**Category 01** | Technique: Recursion + Backtracking

---

## 1. The Problem

Given `n` items, print **every possible arrangement** of all of them, one per line.

A permutation is an arrangement where **order matters**. For `{1, 2, 3}`:

```
1 2 3
1 3 2
2 1 3
2 3 1
3 1 2
3 2 1
```

Total count = `n!` (n factorial) = `3! = 3 × 2 × 1 = 6`.

**Input**
```
n
a1 a2 ... an
```

**Output:** all `n!` permutations in lexicographic (dictionary) order.

---

## 2. The Intention

This is the **foundation of brute force**. Before learning clever algorithms, you must learn to *enumerate every possibility*. Permutation teaches:

- How recursion builds a solution **one position at a time**
- The **choose → explore → un-choose** cycle (backtracking)
- Why the total work is `n!` — the size of the search space itself

Once you understand this pattern, N-Queens, combinations, and every "try all possibilities" problem becomes the same skeleton.

---

## 3. The Core Idea (No Code Yet)

Think of `n` empty slots in a row:

```
[ _ ][ _ ][ _ ]        <- n slots to fill
```

Ask one simple question at **each slot**: *"Which unused item can I place here?"*

- Slot 1: any of the `n` items → `n` choices
- Slot 2: any item except the one already used → `n-1` choices
- Slot 3: `n-2` choices
- ... last slot: `1` choice

That is exactly why the count is `n × (n-1) × ... × 1 = n!`.

To make this work we need a memory of "who is already used" — a **taken/check array**. That is the whole trick.

---

## 4. The Whiteboard Process (Recursion Tree)

Input: `{1, 2, 3}`. Draw a tree. Each level = one slot. Each branch = one choice.

```
                          [ ]                     <- slot 0, nothing chosen
            ┌─────────────┼─────────────┐
           1│            2│            3│         <- try each unused item
            │             │             │
          [1]           [2]           [3]         <- slot 1
         ┌──┴──┐       ┌──┴──┐       ┌──┴──┐
        2│    3│      1│    3│      1│    2│
         │     │       │     │       │     │
       [1,2] [1,3]   [2,1] [2,3]   [3,1] [3,2]   <- slot 2 (last)
         │     │       │     │       │     │
       123   132     213   231     312   321     <- PRINT
```

Read the tree left to right — the output is automatically in dictionary order, because at every slot we try the smallest unused item first.

### Trace table for one path

| Step | Slot | Taken array | Current build | Action |
|------|------|-------------|---------------|--------|
| 1 | 0 | `[F F F]` | `[ ]` | pick 1 → taken = `[T F F]` |
| 2 | 1 | `[T F F]` | `[1]` | 1 is used, pick 2 → `[T T F]` |
| 3 | 2 | `[T T F]` | `[1 2]` | 1,2 used, pick 3 → `[T T T]` |
| 4 | 3 | `[T T T]` | `[1 2 3]` | all slots filled → **print 1 2 3** |
| 5 | — | undo 3 → `[T T F]` | `[1 2]` | **backtrack** |
| 6 | — | undo 2 → `[T F F]` | `[1]` | backtrack, try 3 next at slot 1... |

The **undo step** is backtracking: when a recursion returns, we free the item so the *next* branch can use it.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| n empty slots | recursion depth reaches `n` → print `cur` |
| "which unused item can go here?" | loop `i = 0..n-1`, skip if `taken[i]` |
| mark choice | `taken[i] = true; cur.push_back(a[i])` |
| explore deeper | recursive call `perm()` |
| erase choice (backtrack) | `cur.pop_back(); taken[i] = false` |

**C++** (`problems/01_permutation.cpp`)

```cpp
void perm() {
    if ((int)cur.size() == n) {            // all slots filled
        for (int i = 0; i < n; i++) cout << cur[i] << " \n"[i == n - 1];
        return;
    }
    for (int i = 0; i < n; i++) {          // try every item
        if (taken[i]) continue;            // skip used ones
        taken[i] = true;                   // choose
        cur.push_back(a[i]);
        perm();                            // explore
        cur.pop_back();                    // un-choose (backtrack)
        taken[i] = false;
    }
}
```

**Python** (`problems_python/01_permutation.py`) — identical structure with `taken` list and `cur` list.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n · n!)` | there are `n!` leaves, printing each costs `O(n)` |
| Space | `O(n)` | recursion depth + `cur` + `taken` |

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `n = 0` or negative | program prints nothing and exits |
| `n = 1` | prints the single item once (`1! = 1`) |
| unsorted input | we `sort()` first so output is lexicographic |
| duplicate values in input | treated as *positions*: each printed line corresponds to one arrangement of positions (e.g. `2 2` prints `2 2` twice). Standard for this exercise |

---

## 8. Viva Questions

**Q: Why is the output count `n!`?**
Slot 1 has `n` options, slot 2 has `n-1`, ... slot n has 1. Product = `n!`.

**Q: What is backtracking?**
Choosing an option, recursing, then *undoing* the choice so sibling branches start from a clean state. Here: push → recurse → pop.

**Q: Why does the output come sorted without sorting each line?**
Because at every slot we try items in increasing index order, and we sorted the input first — the leftmost branch is always the smallest.

**Q: Can you do it without recursion?**
Yes — `std::next_permutation` repeatedly from the sorted arrangement until it wraps around. Same `n!` outputs.

**Q: What breaks if we forget `taken[i] = false`?**
Every branch after the first would see items as permanently used — you would print only one permutation.
