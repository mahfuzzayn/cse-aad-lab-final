# 13 — Bellman-Ford Algorithm (Shortest Path with Negative Weights)

**Category 03** | Technique: Edge relaxation, repeated n−1 times (no greedy, no heap)

---

## 1. The Problem

Single-source shortest paths like Dijkstra — but now edges may have **negative weights**, and we must **detect a negative cycle** (a loop whose total weight is negative, which makes "shortest path" meaningless because you can loop forever and shrink the cost).

**Input**
```
n m src
u1 v1 w1     (m DIRECTED edges u → v, any sign of w)
...
```
**Output:** `i:dist` for all vertices, `-1` for unreachable; or just `-1` (first line) if a negative cycle is reachable from the source.

---

## 2. The Intention

Bellman-Ford is the honest workhorse: no clever data structures, just **the same relaxation, repeated enough times**. It teaches two deep ideas:

1. Why Dijkstra's greedy *must* fail with negatives (there is no safe "lock" anymore)
2. A bound from pure logic: *a shortest path visits at most n−1 edges, so n−1 rounds of relaxing everything must suffice*

It's also the gateway to DP-on-graphs (it *is* a DP over path length) and to Johnson's all-pairs algorithm.

---

## 3. The Core Idea (No Code Yet)

### The key bound

In a graph with `n` vertices and **no negative cycle**, a shortest path from src to any vertex is **simple** — it never repeats a vertex — so it uses **at most n−1 edges**.

### The algorithm

```
dist[src] = 0, everything else ∞
repeat (n−1) times:
    for EVERY edge (u → v, w):
        relax: if dist[u] + w < dist[v] then dist[v] = dist[u] + w
```

**Why n−1 rounds are enough:** after round 1, every dist is correct for all paths using ≤ 1 edge. After round 2, correct for ≤ 2 edges. Induction: after round k, correct for ≤ k edges. With ≤ n−1 edges in any shortest path, round n−1 finishes the job. (The order of edges inside a round doesn't matter!)

**Early exit optimization:** if a whole round changes nothing, every distance is already final — stop.

### Negative cycle detection — one extra round

Run a **(n)-th** pass. If *any* edge can still be relaxed, some path keeps improving forever → a reachable negative cycle exists.

```
after n−1 rounds:
    if any edge (u,v,w) still has dist[u] + w < dist[v]:
        → negative cycle reachable from src
```

Why does that indicate a cycle? A shortest simple path would already be stable by round n−1; continued improvement means the improving route repeats a vertex — a negative cycle.

---

## 4. The Whiteboard Process (Full Trace)

Input:

```
5 8 1
1 2 6
1 3 7
2 3 8
2 4 -4
2 5 5
3 2 -2
4 1 2
4 5 7
```

Graph (visual version in `drawings/13_bellman_ford.excalidraw`):

| Edge | W | Edge | W | Edge | W | Edge | W |
|------|---|------|---|------|---|------|---|
| 1→2 | 6 | 2→3 | 8 | 3→2 | −2 | 4→5 | 7 |
| 1→3 | 7 | 2→4 | −4 | 4→1 | 2 | 2→5 | 5 |

Initialise: `dist = [1:0, 2:∞, 3:∞, 4:∞, 5:∞]`.

### Round 1 (edges relaxed in input order)

| Edge | Test | Result |
|---|---|---|
| 1→2 (6) | 0+6 < ∞ | dist[2] = **6** |
| 1→3 (7) | 0+7 < ∞ | dist[3] = **7** |
| 2→3 (8) | 6+8 = 14 > 7 | no |
| 2→4 (−4) | 6−4 = 2 < ∞ | dist[4] = **2** |
| 2→5 (5) | 6+5 = 11 < ∞ | dist[5] = **11** |
| 3→2 (−2) | 7−2 = 5 < 6 | dist[2] = **5** |
| 4→1 (2) | 2+2 = 4 > 0 | no |
| 4→5 (7) | 2+7 = 9 < 11 | dist[5] = **9** |

After round 1: `[0, 5, 7, 2, 9]`

### Round 2

| Edge | Test | Result |
|---|---|---|
| 1→2 (6) | 6 > 5 | no |
| 1→3 (7) | 7 = 7 | no |
| 2→3 (8) | 5+8 = 13 > 7 | no |
| 2→4 (−4) | 5−4 = 1 < 2 | dist[4] = **1** |
| 2→5 (5) | 5+5 = 10 > 9 | no |
| 3→2 (−2) | 7−2 = 5 = 5 | no |
| 4→1 (2) | 1+2 = 3 > 0 | no |
| 4→5 (7) | 1+7 = 8 < 9 | dist[5] = **8** |

After round 2: `[0, 5, 7, 1, 8]` — notice how dist[2] improving in round 1 *rippled* into 4 and 5 in round 2. That ripple is the entire reason multiple rounds exist.

### Round 3

Nothing improves: `[0, 5, 7, 1, 8]` → **early exit** (changed = false).

### Verification pass (the "extra" round)

Every edge stable → no negative cycle.

**Answer: `1:0 2:5 3:7 4:1 5:8`**

Reading off the routes: the best path to 2 is not the direct edge (6) but 1→3→2 = 7−2 = **5** — a negative edge *reversed the obvious choice*. From there 2→4 (−4) gives dist[4] = 1, and 2→5 (5) gives dist[5] = 10... improved by 4→5 (7) from dist[4]=1 → **8**. Negative edges everywhere, all handled.

### Negative cycle example

```
3 3 1
1 2 1
2 3 -3
3 1 1
```
The loop 1→2→3→1 costs 1−3+1 = **−1**. Round 1: dist = [0, 1, −2]. Round 2: 3→1: −2+1 = −1 < 0 → dist[1] = −1! A *source distance below zero* — proof of the cycle. After n−1 rounds the verification pass still finds improvements → program prints `-1`.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| edge list (not adjacency!) | `vector<tuple<int,int,int>> e` |
| n−1 rounds | `for (i = 1; i <= n-1; i++)` |
| relax every edge | loop over `e`, skip if `dist[u] == INF` |
| early exit | `if (!changed) break` |
| one verification pass | same loop once more; any improvement → print `-1` |
| unreachable guard | `dist[u] == INF` skips (prevents ∞+w nonsense) |

**C++** (`problems/13_bellman_ford.cpp`)

```cpp
for (int i = 1; i <= n - 1; i++) {
    bool changed = false;
    for (auto &[u, v, w] : e) {
        if (dist[u] == INF) continue;
        if (dist[u] + w < dist[v]) { dist[v] = dist[u] + w; changed = true; }
    }
    if (!changed) break;
}
```

**Python** (`problems_python/13_bellman_ford.py`) — identical.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n · m)` | n−1 passes × m edges (early exit often much faster in practice) |
| Space | `O(n + m)` | dist + edge list |

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| unreachable vertices | never relaxed (INF guard) → printed as `-1` |
| negative edges, no cycle | correct distances (the whole point) |
| negative cycle **reachable** from src | detected in verification pass → prints `-1` |
| negative cycle in an unreachable part | NOT flagged — it can't affect any answer (standard behaviour) |
| self-loop with negative weight | keeps relaxing forever → detected as a negative cycle |
| `m = 0` | only src = 0, rest `-1` |

---

## 8. Dijkstra vs Bellman-Ford (guaranteed viva question)

| | Dijkstra | Bellman-Ford |
|---|---|---|
| Weights | non-negative only | any |
| Strategy | greedy (lock smallest) | brute relaxation |
| Data structure | min-heap | plain edge list |
| Time | `O(m log m)` | `O(n·m)` |
| Detects negative cycles | no | yes |
| Graph type | directed or undirected | directed (undirected negative edge = negative 2-cycle!) |

---

## 9. Viva Questions

**Q: Why exactly n−1 iterations?**
Longest useful (simple) path has n−1 edges; each round extends the "correct within k edges" guarantee by one.

**Q: Why does Dijkstra break with negative weights but Bellman-Ford doesn't?**
Dijkstra locks the smallest tentative distance as final; a negative edge can later create a cheaper route, invalidating the lock. Bellman-Ford never locks — it relaxes until nothing changes.

**Q: How does the cycle detection work?**
If after n−1 rounds an edge can still relax, the improving path must revisit a vertex — a negative cycle.

**Q: Can you stop early?**
Yes — a round with no changes means all distances are final (implemented here).

**Q: Is Bellman-Ford dynamic programming?**
Yes — `dist[k][v]` (best path using ≤ k edges) is a DP over path length; the implemented version keeps only one row (rolling array).

**Q: Why is an undirected negative edge a problem?**
Traversing it back and forth (u→v→u) is already a negative cycle of length 2. Undirected graphs with negative edges need special handling.
