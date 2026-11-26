# 03 — Fractional Knapsack (Greedy Method)

**Category 01** | Technique: Greedy

---

## 1. The Problem

A thief's bag holds `W` kg. There are `n` items; item `i` weighs `w_i` kg and is worth `v_i` taka. **You may take fractions of an item** (e.g. half a gold bar). Maximize the total value taken.

**Input**
```
n W
w1 v1
w2 v2
...
```

**Output:** maximum total value (2 decimals).

---

## 2. The Intention

This is **the** textbook example of the greedy method: at every step take the choice that looks best *right now*, and prove that this local greed leads to the global optimum.

It also exists to be contrasted with 0/1 Knapsack: the *one change* — "you may break items" vs "you may not" — moves the problem from a greedy `O(n log n)` solution to a DP `O(n·W)` solution. Examiners love this comparison (see the table at the bottom).

---

## 3. The Core Idea (No Code Yet)

Ask: *if I can take 1 kg of anything, which 1 kg is the best?* The one with the highest **value per kg** (the **ratio** `v/w`).

So the strategy is:

1. Compute ratio `v/w` for every item
2. **Sort items by ratio, descending**
3. Take items greedily:
   - if the item **fits completely** → take it whole
   - if it **doesn't fit** → take whatever fraction of it fills the remaining space, then **stop**

The last item is usually the only fractional one. That's why it's called fractional knapsack.

**Why must we stop after the fractional item?** Because the bag is then full — capacity 0.

---

## 4. The Whiteboard Process (Worked Example)

Classic input: `W = 50`, items:

| Item | Weight | Value | Ratio v/w |
|------|--------|-------|-----------|
| A | 10 | 60 | **6.0** |
| B | 20 | 100 | **5.0** |
| C | 30 | 120 | **4.0** |

### Step 1 — add the ratio column, re-sort by it

Already sorted: A (6.0), B (5.0), C (4.0).

### Step 2 — fill the bag, one row at a time

| Step | Item | Fits? | Taken | Value gained | Capacity left |
|------|------|-------|-------|--------------|---------------|
| 1 | A (10 kg, ratio 6) | 10 ≤ 50 ✔ whole | 10 kg | 60 | 50 − 10 = 40 |
| 2 | B (20 kg, ratio 5) | 20 ≤ 40 ✔ whole | 20 kg | 100 | 40 − 20 = 20 |
| 3 | C (30 kg, ratio 4) | 30 > 20 ✘ **fraction** | 20/30 = 2/3 | 120 × 2/3 = 80 | 0 |

### Step 3 — total

```
Total = 60 + 100 + 80 = 240.00
```

**Sanity check that greed is right here:** every kg taken from A earns 6 taka, from B earns 5, from C earns 4. Any solution that skips a high-ratio kg in favour of a low-ratio kg can be improved by swapping — so the sorted filling must be optimal.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| ratio column | `x.second / x.first` inside comparator |
| sort by ratio ↓ | `sort(..., cmp)` with `rx > ry` |
| "fits → take whole" | `if (w <= cap) { total += v; cap -= w; }` |
| "doesn't fit → fraction" | `total += v * (cap / w); cap = 0;` |
| bag full → stop | loop condition `cap > 0` |

**C++** (`problems/03_fractional_knapsack.cpp`) — the comparator guards `w == 0` (an item weighing nothing has infinite ratio: always take it, it's free value).

**Python** (`problems_python/03_fractional_knapsack.py`) — same logic; uses `float("inf")` for the zero-weight ratio.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n log n)` | dominated by the sort |
| Space | `O(n)` | the item list |

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `W = 0` | loop never runs → `0.00` |
| all items fit | every item taken whole, no fraction |
| item with weight 0 | ratio treated as +∞, taken first, adds free value |
| total weight < W | bag never fills; everything taken |
| item with value 0 | ratio 0, sorted last |

---

## 8. Fractional vs 0/1 (the viva favourite)

| | Fractional | 0/1 |
|---|---|---|
| May break items? | yes | no |
| Method | Greedy | Dynamic Programming |
| Time | `O(n log n)` | `O(n·W)` |
| Optimal? | yes (provably) | yes (DP explores all states) |
| Sorting by ratio works? | yes | **no** — greedy can be trapped |

**Why greedy fails on 0/1 — a concrete trap:** `W = 10`, items: `(w=6, v=60)` ratio 10, `(w=6, v=60)` ratio 10, `(w=10, v=95)` ratio 9.5.
Greedy-by-ratio takes one 6-kg item (value 60); the second 6-kg item doesn't fit whole (only 4 kg left, and 0/1 forbids fractions), the 10-kg item doesn't fit either → greedy total = **60**.
Optimal: take the single 10-kg item → total = **95**. The all-or-nothing rule is exactly what breaks the per-kg exchange argument, so 0/1 needs DP.

---

## 9. Viva Questions

**Q: Why is greedy optimal for fractional but not 0/1?**
Fractions let you compare *per kg*: any solution not filling with the best ratio can exchange kg and improve. In 0/1 you cannot exchange partially — taking an item is all-or-nothing, so local best can block the global best.

**Q: What is the exchange argument?**
Proof technique: take any optimal solution, show you can swap its contents with the greedy's choices without losing value — hence greedy is at least as good.

**Q: Do you ever need to sort by weight or value directly?**
No — only the ratio `v/w` matters. Sorting by value alone fails (a heavy high-value item wastes capacity); sorting by weight alone ignores value.

**Q: What if two items have equal ratio?**
Order between them doesn't change the total — any tie-break is fine (stable behaviour comes from the sort).
