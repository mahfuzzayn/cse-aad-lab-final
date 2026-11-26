# 12 — Dijkstra's Algorithm (Single-Source Shortest Path)

**Category 03** | Technique: Greedy + Priority Queue (min-heap)

---

## 1. The Problem

Given a weighted graph (here undirected) and a **source** vertex, find the shortest total distance from the source to **every** vertex.

GPS navigation, network routing, currency arbitrage detection — this is *the* shortest path algorithm of daily life.

**Input**
```
n m src
u1 v1 w1     (m lines; here undirected, weight w ≥ 0)
...
```
**Output:** `i:dist` for every vertex 1..n on one line; `-1` in place of distance if unreachable.

---

## 2. The Intention

Dijkstra answers the question "*what is the cheapest way from HERE to everywhere?*" with one elegant greedy loop. It teaches:

- **relaxation** — the single most reused idea in shortest-path algorithms (Bellman-Ford and others are all "relax edges, but in different orders")
- why **non-negative weights** are a hard requirement, not a preference

---

## 3. The Core Idea (No Code Yet)

### The relaxation idea

Keep `dist[v]` = *best known distance so far* from src to v. Initially `dist[src] = 0`, everything else ∞.

**Relaxing an edge u→v with weight w:**

```
if dist[u] + w  <  dist[v]:
        dist[v] = dist[u] + w        "found a cheaper route through u"
```

The name is intuitive: the bound on dist[v] *relaxes* downward, never up.

### The greedy loop

```
repeat n times:
    pick the unvisited vertex u with the SMALLEST dist        ← greedy choice
    lock it: its distance is now FINAL
    relax every edge leaving u
```

**Why can we lock the smallest?** Because with non-negative weights, any other route to u must pass through a vertex already ≥ dist[u] — it can only add, never subtract. (This is exactly what breaks with negative edges: a later, cheaper detour could still appear.)

### The priority queue version

"Pick smallest unvisited" = min-heap of `(dist, vertex)`. Push a vertex whenever its distance improves; skip stale heap entries (a vertex may sit in the heap several times — only its best entry matters):

```
pop (d, u):
    if d != dist[u] → stale, skip
    else relax u's edges (pushing improvements)
```

---

## 4. The Whiteboard Process (Full Trace)

Input:

```
5 7 1
1 2 10
1 4 5
2 3 1
2 4 2
3 5 4
4 2 3
4 5 2
```

Graph (visual version in `drawings/12_dijkstra.excalidraw`) — note the **two parallel edges** between 2 and 4:

| Edge | W | Edge | W | Edge | W |
|------|---|------|---|------|---|
| 1—2 | 10 | 1—4 | 5 | 2—3 | 1 |
| 2—4 | 2 | 4—2 | 3 | 3—5 | 4 |
| 4—5 | 2 | | | | |

Sketch tip: put 1 top-left, 2 top-right, 4 bottom-left, 3 mid-right, 5 bottom-right; then 1—2 and 1—4 form a left frame, 2—3 and 3—5 a right frame, and the two 2—4 edges are the diagonals.

Initialise: `dist = [1:0, 2:∞, 3:∞, 4:∞, 5:∞]`, heap = {(0,1)}.

| Step | Pop (d,u) | Lock u (final) | Relaxations performed | dist after |
|------|-----------|----------------|------------------------|------------|
| 1 | (0, 1) | 1 → **0** | 1—2: `0+10 < ∞` → dist[2]=10; 1—4: `0+5 < ∞` → dist[4]=5 | 1:0 2:10 4:5 |
| 2 | (5, 4) | 4 → **5** | 4—1: 5+10 > 0 ✘; 4—2: `5+3=8 < 10` → dist[2]=**8**; 4—5: `5+2=7 < ∞` → dist[5]=**7** | 2:8 5:7 |
| 3 | (7, 5) | 5 → **7** | 5—3: `7+4=11 < ∞` → dist[3]=11; 5—4: 7+2 > 5 ✘ | 3:11 |
| 4 | (8, 2) | 2 → **8** | 2—1: ✘; 2—3: `8+1=9 < 11` → dist[3]=**9**; 2—4: `8+2 > 5` ✘ | 3:9 |
| 5 | (10, 2) stale | — | `10 ≠ dist[2]=8` → **skip** (the outdated entry) | — |
| 6 | (9, 3) | 3 → **9** | 3—2: 9+1 > 8 ✘; 3—5: 9+4 > 7 ✘ | — |

All locked. **Final: 1:0 2:8 3:9 4:5 5:7.**

Two things worth noticing in the trace:

- **dist[2] improved twice** (10 → 8): the route 1→4→2 beat the direct edge. That's why we can't just look at direct edges.
- **Step 5 was a stale pop**: the old (10, 2) entry was still in the heap after dist[2] became 8. The `d != dist[u]` check throws it away.

Sanity-check the greedy lock at step 2: when we locked vertex 4 at distance 5, could any cheaper path to 4 exist? Every other path leaves 1 via edge 1—2 (cost ≥ 10) or is the direct 5 — nothing can beat 5. Lock is safe.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| dist table, ∞ default | `vector<long long> dist(n+1, INF)` |
| source at 0, in heap | `dist[src]=0; pq.push({0, src})` |
| smallest unvisited | min-heap top |
| stale entry skip | `if (d != dist[u]) continue` |
| relaxation | `if (dist[u]+w < dist[v]) dist[v]=dist[u]+w, push` |
| unreachable at the end | still INF → print `-1` |

**C++** (`problems/12_dijkstra.cpp`)

```cpp
while (!pq.empty()) {
    auto [d, u] = pq.top(); pq.pop();
    if (d != dist[u]) continue;                 // stale
    for (auto [v, w] : g[u])
        if (dist[u] + w < dist[v]) {
            dist[v] = dist[u] + w;
            pq.push({dist[v], v});
        }
}
```

**Python** (`problems_python/12_dijkstra.py`) — same with `heapq`.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(m log m)` | every edge can cause one push; each heap op `log m` |
| Space | `O(n + m)` | dist + adjacency + heap |

Array version (no heap, scan for min each round): `O(n²)` — better for dense graphs.

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| unreachable vertex | dist stays INF → printed as `-1` |
| `m = 0` | only the source is 0, everything else `-1` |
| multiple edges between same pair | all relaxed; the cheapest effectively wins |
| self-loop | `dist[u]+w < dist[u]` false for w ≥ 0 → ignored |
| zero-weight edges | fine — they can't invalidate locks (non-negative) |
| **negative weights** | **NOT supported** — algorithm silently gives wrong answers; that's Bellman-Ford's job (explanation 13) |

---

## 8. Viva Questions

**Q: Why does Dijkstra fail on negative edges?**
The greedy lock assumes no future path can undercut a settled distance. A negative edge later on the route can do exactly that — the settled "final" answer becomes wrong.

**Q: What is relaxation?**
Attempting `dist[v] = min(dist[v], dist[u]+w)` over an edge — improving the best-known bound. All shortest-path algorithms differ only in the *order* they relax edges.

**Q: Why can a vertex appear multiple times in the heap, and is that a bug?**
Each improvement pushes a new entry; older ones become stale. Skipping via `d != dist[u]` is the standard "lazy deletion" — correct and simpler than decrease-key.

**Q: Dijkstra vs BFS?**
BFS finds shortest paths only when every edge weighs the same (then it *is* Dijkstra with all w=1, and the heap becomes a plain FIFO queue).

**Q: Dijkstra vs Bellman-Ford?**
Dijkstra: faster `O(m log n)`, no negative weights. Bellman-Ford: slower `O(nm)`, handles negatives and detects negative cycles.

**Q: How would you also print the shortest PATH, not just its length?**
Keep `parent[v] = u` at every successful relaxation; walk parents back from the target and reverse.

**Q: Is Dijkstra dynamic programming or greedy?**
Greedy in its choices (lock the minimum), but the dist table with improving subproblem answers has a strong DP flavour — both descriptions appear in textbooks.
