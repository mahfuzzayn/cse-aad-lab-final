# 07 — Topological Sort (Kahn's Algorithm / BFS)

**Category 02** | Technique: In-degree counting + Queue

---

## 1. The Problem

A **directed acyclic graph (DAG)** has `n` vertices and `m` edges `u → v` meaning "u must come before v". Print an ordering of all vertices such that **every edge points forward**.

Use case: course prerequisites, build systems, task scheduling, spreadsheet cell evaluation.

**Input**
```
n m
u1 v1     (m lines: edge u → v)
...
```
**Output:** a valid topological order (vertices 1..n), or `-1` if a cycle exists (no valid order).

---

## 2. The Intention

Topological sort answers "*in what order should things happen?*" It teaches thinking about graphs through a local property — **in-degree** (number of incoming edges) — instead of paths. It is also the standard way to **detect cycles in a directed graph*: if the ordering stalls, a cycle is the reason.

---

## 3. The Core Idea (No Code Yet)

An edge `u → v` says "u before v". So a vertex with **zero incoming edges** has no obligations — nothing must precede it. It is safe to output *now*.

The algorithm:

1. Compute `indeg[v]` = number of edges entering `v`
2. Put every vertex with `indeg == 0` into a **queue** (they are all "ready")
3. Repeat until the queue is empty:
   - take a vertex `u` out, **output it**
   - it is now "done" — so every edge `u → v` leaving it is *satisfied*: decrease `indeg[v]`
   - if some `v` drops to 0 → all its prerequisites are done → push it into the queue
4. If you output all `n` vertices → valid order. If not (queue emptied early) → **cycle**

### Why a cycle stalls the queue

Every vertex on a cycle has an incoming edge *from inside the cycle*. Those edges can never be "satisfied" — the cycle waits on itself forever. Their in-degrees never reach 0, so they never enter the queue, so `order.size() < n` at the end.

---

## 4. The Whiteboard Process (Full Trace)

Input: the CLRS example (6 courses):

```
6 6
6 3
6 1
5 1
4 3
2 3
1 2
```

Draw the graph:

```
   5 ──→ 1 ──→ 2
   4 ──→ 3 ←──/   (2 → 3)
   6 ──→ 3, 6 ──→ 1
```

### Step 1 — in-degree table

| vertex | 1 | 2 | 3 | 4 | 5 | 6 |
|--------|---|---|---|---|---|---|
| indeg  | 2 | 1 | 3 | 0 | 0 | 0 |

(1 ← 6 and ← 5; 2 ← 1; 3 ← 6, ← 4, ← 2; 4, 5, 6 have no incoming edges)

### Step 2 — seed the queue with indeg 0

Queue: `[4, 5, 6]`

### Step 3 — process (BFS)

| Pop | Output so far | Edges relaxed | indeg after |
|-----|---------------|----------------|-------------|
| 4 | `4` | 4→3 : indeg[3] 3→2 | 1:2 2:1 3:2 5:0 6:0 |
| 5 | `4 5` | 5→1 : indeg[1] 2→1 | 1:1 2:1 3:2 6:0 |
| 6 | `4 5 6` | 6→3 (2→1), 6→1 (1→0) → **1 ready** | 1:0 2:1 3:1 |
| 1 | `4 5 6 1` | 1→2 : indeg[2] 1→0 → **2 ready** | 2:0 3:1 |
| 2 | `4 5 6 1 2` | 2→3 : indeg[3] 1→0 → **3 ready** | 3:0 |
| 3 | `4 5 6 1 2 3` | none | — |

Queue empty. `6 == 6` vertices output → **answer: 4 5 6 1 2 3**.

Check against every edge: 4→3 ✓(4 before 3), 5→1 ✓, 6→3 ✓, 6→1 ✓, 2→3 ✓, 1→2 ✓. Valid.

Note the order isn't unique — `5 4 6 1 2 3` is also valid. Any order the algorithm produces is correct; which one you get depends on queue insertion order.

### Cycle example

```
3 3
1 2
2 3
3 1
```
indeg = 1:1, 2:1, 3:1 → queue starts **empty** → output size 0 ≠ 3 → print `-1`.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| in-degree column | `indeg[v]++` while reading edges |
| "ready" vertices | queue seeded with all `indeg[i]==0` |
| output & satisfy edges | pop u, for each v in adj[u]: `--indeg[v]` |
| newly ready | `if (indeg[v] == 0) q.push(v)` |
| cycle check | `order.size() != n → -1` |

**C++** (`problems/07_topological_sort.cpp`)

```cpp
queue<int> q;
for (int i = 1; i <= n; i++) if (indeg[i] == 0) q.push(i);
while (!q.empty()) {
    int u = q.front(); q.pop();
    order.push_back(u);
    for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
}
```

**Python** (`problems_python/07_topological_sort.py`) — same with `collections.deque`.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n + m)` | every vertex queued once, every edge relaxed once |
| Space | `O(n + m)` | adjacency list + indeg + queue |

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `m = 0` (no edges) | all indeg 0 → prints `1 2 ... n` |
| cycle | output size < n → prints `-1` |
| self-loop `u → u` | indeg[u] ≥ 1 forever → `-1` (a self-loop is a 1-cycle) |
| duplicate edges | counted multiple times in indeg, relaxed multiple times — consistent, still correct |
| multiple valid orders | any one is printed (deterministic for a fixed input) |

---

## 8. Viva Questions

**Q: Does topological sort work on any directed graph?**
No — only DAGs. A cycle makes an order impossible; the algorithm detects it by running out of vertices.

**Q: Is the topological order unique?**
Not necessarily. It's unique iff at every step exactly one vertex has in-degree 0 (a "chain-like" DAG).

**Q: Kahn's vs DFS-based topological sort?**
DFS: push each vertex on finish time, reverse at the end. Kahn: peel in-degree-0 vertices. Same `O(n+m)`; Kahn detects cycles *for free* (count check), DFS needs extra rec-stack marking.

**Q: Where is topological sort used?**
Build systems (make), course prerequisite scheduling, resolving package/import dependencies, spreadsheet evaluation order, instruction scheduling.

**Q: Can you find *the* vertices forming a cycle?**
Yes — at the end, the vertices that never reached indeg 0 are exactly the ones on/behind cycles.

**Q: What is a DAG?**
Directed Acyclic Graph — directed edges, no directed cycle. Every DAG has at least one topological order, and at least one source (indeg 0) and one sink (outdeg 0).
