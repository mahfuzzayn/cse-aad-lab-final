# 09 — Prim's Algorithm (Minimum Spanning Tree)

**Category 02** | Technique: Greedy + Priority Queue (Min-Heap)

---

## 1. The Problem

Given a **connected, undirected, weighted** graph with `n` vertices and `m` edges, pick `n−1` edges that keep the graph connected (a **spanning tree**) with **minimum total weight** — the *minimum spanning tree* (MST).

Classic use case: connecting villages with the least total cable length.

**Input**
```
n m
u1 v1 w1    (m lines: undirected edge u—v of weight w)
...
```
**Output:** the MST edges (one per line: `u v w`), then total weight. `-1` if the graph is disconnected (no spanning tree exists).

---

## 2. The Intention

Prim's grows one tree "outward" — it's greedy in its purest visual form: *always swallow the cheapest edge that touches your tree.* It teaches how to combine a greedy invariant with a priority queue, and it pairs with Kruskal's (explanation 11) as the two canonical MST algorithms — same goal, opposite growth strategy.

---

## 3. The Core Idea (No Code Yet)

Maintain a set `T` of "tree vertices", starting with a single arbitrary vertex (we start at vertex 1). Then repeat `n−1` times:

> **Among all edges with one foot in `T` and one foot outside, take the cheapest one, and pull that outside vertex into `T`.**

```
   T (inside)          outside
   ●───────●      ✗   ●
   │  ╲               │
 cheap edge ✓     ✗   ●
   ▼ pull this outside vertex in
```

Why is this safe? **The cut property**: for any partition of the graph (T vs rest), the lightest edge crossing it belongs to *some* MST. So each greedy step never prices us out of an optimal solution.

### How the priority queue serves it

The "cheapest crossing edge" question is exactly a min-heap job:

- when a vertex `u` joins T, push every edge `u—v` (v not yet in T) into the heap as `(w, v, u)`
- the next tree edge = heap top (skip entries whose vertex already joined — they went stale)

Stale entries are normal — we push lazily and skip on pop.

---

## 4. The Whiteboard Process (Full Trace — CLRS graph)

Input: 9 vertices, 14 edges:

```
1—2:4   1—8:8   2—3:8   2—8:11   3—4:7   3—6:4   3—9:2
4—5:9   4—6:14  5—6:10  6—7:2    7—8:1   7—9:6   8—9:7
```

Draw the graph first (do this on paper — it's the "understand the problem" step). The full visual version is in `drawings/09_prims.excalidraw`. Edge list:

| Edge | W | Edge | W | Edge | W |
|------|---|------|---|------|---|
| 1—2 | 4 | 2—3 | 8 | 3—4 | 7 |
| 1—8 | 8 | 2—8 | 11 | 3—6 | 4 |
| 4—5 | 9 | 3—9 | 2 | 4—6 | 14 |
| 5—6 | 10 | 6—7 | 2 | 7—8 | 1 |
| 7—9 | 6 | 8—9 | 7 | | |

Tip for sketching: place vertices 1–8 as an outer ring (1 top-left going clockwise: 2, 3, 4, 5, 6, 7, 8) and put vertex 9 in the centre — then every edge is a short straight line.

Start at vertex **1**. Track: visited set, best edge picked each round.

| Round | Heap's cheapest crossing edge | Vertex pulled in | Edge added | Total |
|-------|-------------------------------|------------------|------------|-------|
| start | — | {1} | — | 0 |
| 1 | 1—2 (4) beats 1—8 (8) | 2 | 1—2 w=4 | 4 |
| 2 | 2—3 (8) beats 2—8 (11), 1—8 (8)* | 3 | 2—3 w=8 | 12 |
| 3 | 3—9 (2) — smallest anywhere | 9 | 3—9 w=2 | 14 |
| 4 | 3—6 (4) | 6 | 3—6 w=4 | 18 |
| 5 | 6—7 (2) | 7 | 6—7 w=2 | 20 |
| 6 | 7—8 (1) | 8 | 7—8 w=1 | 21 |
| 7 | 3—4 (7) | 4 | 3—4 w=7 | 28 |
| 8 | 4—5 (9) | 5 | 4—5 w=9 | 37 |

\* ties broken by heap order (weight, then vertex, then parent) — deterministic.

After round 8, all 9 vertices are in the tree → done, `n−1 = 8` edges.

**MST edges: 1—2(4), 2—3(8), 3—9(2), 3—6(4), 6—7(2), 7—8(1), 3—4(7), 4—5(9) — total 37.**

The invariant to *see*: after every round the visited set is one connected tree, and it only ever grows.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| start at vertex 1 | `pq.push({0, 1, -1})` — fake 0-weight entry |
| cheapest crossing edge | min-heap of `(w, u, parent)` |
| skip already-inside vertices | `if (vis[u]) continue` (stale entries) |
| pull u in, record edge | `vis[u]=true; mst.push_back({p,u,w})` |
| push u's outgoing edges | for each neighbour `v` not visited |
| stop when tree complete | `mst.size() == n` |
| disconnected check | counted visited < n → `-1` |

**C++** (`problems/09_prims.cpp`) — core loop:

```cpp
while (!pq.empty() && (int)mst.size() < n) {
    auto [w, u, p] = pq.top(); pq.pop();
    if (vis[u]) continue;          // stale entry
    vis[u] = true;
    total += w;
    if (p != -1) mst.push_back({p, u, w});
    for (auto [v, w2] : g[u]) if (!vis[v]) pq.push({w2, v, u});
}
```

**Python** (`problems_python/09_prims.py`) — same with `heapq`.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(m log m)` | each edge pushed once, each heap op `log m` |
| Space | `O(n + m)` | adjacency + heap |

With a decrease-key heap it's `O(m log n)`; the lazy version (push duplicates, skip stale) is simpler and asymptotically the same in m.

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `n = 1` | zero edges needed → total 0, no edges printed |
| disconnected graph | visits stall below n → `-1` |
| parallel edges (two u—v edges) | the cheaper one wins via the heap |
| self-loops | never cross the cut (both ends inside) → ignored |
| negative weights | fine — the cut property doesn't care about sign |

---

## 8. Prim's vs Kruskal's (the guaranteed viva question)

| | Prim's | Kruskal's |
|---|---|---|
| Strategy | grow one tree from a start vertex | sort all edges, add globally cheapest that doesn't cycle |
| Data structure | min-heap + visited array | sort + union-find (DSU) |
| Growth | one component always | forest merging into one tree |
| Best for | dense graphs | sparse graphs |
| Time | `O(m log m)` | `O(m log m)` (sort dominates) |
| Cycle check needed? | no (tree grows internally) | yes — union-find |

Both are greedy, both provably optimal via the cut property.

---

## 9. Viva Questions

**Q: What is a spanning tree? MST?**
Spanning tree: subgraph touching all n vertices with exactly n−1 edges and no cycle. MST: the spanning tree of minimum total weight.

**Q: State Prim's greedy invariant.**
At every step the partial solution is a subtree of *some* MST; the cheapest edge leaving it preserves that (cut property).

**Q: Why is the MST unique or not?**
Unique when all edge weights are distinct. Equal weights can give several MSTs (same total weight though).

**Q: Why can Prim's ignore stale heap entries?**
The first time a vertex is popped, it's via its cheapest crossing edge — any later entry for the same vertex is worse and the vertex is already in the tree.

**Q: Does the start vertex matter?**
For the total weight: no — any start gives an MST (possibly a different tree when ties exist).

**Q: Can Prim's produce a cycle?**
No — a popped vertex was outside the tree, and the popped edge connects it in; adding one new vertex per edge can't create a cycle.
