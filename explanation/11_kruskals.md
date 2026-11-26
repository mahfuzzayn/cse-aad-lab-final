# 11 — Kruskal's Algorithm (Minimum Spanning Tree)

**Category 03** | Technique: Greedy (sort edges) + Union-Find (cycle detection)

---

## 1. The Problem

Same goal as Prim's (explanation 09): from a connected undirected weighted graph, select `n−1` edges forming a spanning tree of **minimum total weight**. Kruskal reaches it by a completely different route — a **global greedy** over edges.

**Input**
```
n m
u1 v1 w1     (m lines, undirected)
...
```
**Output:** chosen MST edges (ascending weight order), then total weight; `-1` if disconnected.

---

## 2. The Intention

Kruskal's is greedy at its boldest:

> "Sort **all** edges by weight. Take the globally cheapest edge that doesn't create a cycle. Repeat until n−1 edges."

The one obstacle is the phrase *doesn't create a cycle* — and solving it introduces the **Union-Find (Disjoint Set Union, DSU)** data structure, one of the most useful tools in all of algorithms. So this problem really teaches two things: a greedy MST proof, and DSU.

---

## 3. The Core Idea (No Code Yet)

### The greedy loop

```
sort edges by weight (ascending)
T = empty forest (every vertex alone)
for each edge (u, v, w) in sorted order:
    if u and v are already connected in T:
        skip it (would make a cycle)
    else:
        add it to T
stop when T has n−1 edges
```

Think of it as **merging islands**: every vertex starts as its own island; each accepted edge joins two islands into one. When one island covers everything, done.

### Union-Find in one breath

Two operations on a "who belongs to which set" structure:

- **find(x)** — which island is x on? (follow parent pointers to the island's representative/root)
- **union(a, b)** — merge two islands (hang one root under the other)

Then "does edge (u,v) create a cycle?" becomes simply: `find(u) == find(v)`?

```
find(3) == find(5)?      (3)→(2)←(5)      same root (2) → CYCLE, skip
                         /
                        (1)

find(3) == find(8)?      (3)→(2)   (8)→(7)  roots differ → SAFE, union them
```

**Path compression** (`parent[x] = find(parent[x)]` while unwinding) is what makes finds nearly O(1).

### Why greedy is safe — the cycle property

If an edge is the *heaviest* on some cycle, some MST avoids it. When we skip an edge in Kruskal's, it's because its endpoints are already connected — the connecting path plus this edge forms a cycle where this edge is the heaviest (all earlier edges are ≤ its weight). So skipping loses nothing.

---

## 4. The Whiteboard Process (Full Trace — same CLRS graph as Prim's)

Edges sorted ascending (same input as 09):

| # | Edge | W | Action | Reason |
|---|------|---|--------|--------|
| 1 | 7—8 | 1 | ✔ take | joins {7},{8} |
| 2 | 3—9 | 2 | ✔ take | joins {3},{9} |
| 3 | 6—7 | 2 | ✔ take | joins {6},{7,8} |
| 4 | 1—2 | 4 | ✔ take | joins {1},{2} |
| 5 | 3—6 | 4 | ✔ take | joins {3,9},{6,7,8} |
| 6 | 3—4 | 7 | ✔ take | joins {3,9,6,7,8},{4} |
| 7 | 8—9 | 7 | ✘ skip | 8,9 both in {3,4,6,7,8,9} — cycle |
| 8 | 1—8 | 8 | ✔ take | joins {1,2},{3,4,6,7,8,9} |
| 9 | 4—5 | 9 | ✔ take | joins …,{5} → **8 edges = n−1, STOP** |
| 10 | 5—6 | 10 | — | never reached |

Track the island merges as a table (this is the part to practice on paper):

| After edge | Islands (components) |
|---|---|
| start | {1} {2} {3} {4} {5} {6} {7} {8} {9} |
| 7—8 (1) | {1}{2}{3}{4}{5}{6}{7,8}{9} |
| 3—9 (2) | {1}{2}{3,9}{4}{5}{6}{7,8} |
| 6—7 (2) | {1}{2}{3,9}{4}{5}{6,7,8} |
| 1—2 (4) | {1,2}{3,9}{4}{5}{6,7,8} |
| 3—6 (4) | {1,2}{3,6,7,8,9}{4}{5} |
| 3—4 (7) | {1,2}{3,4,6,7,8,9}{5} |
| 1—8 (8) | {1,2,3,4,6,7,8,9}{5} |
| 4—5 (9) | {1,2,3,4,5,6,7,8,9} — one island, STOP |

**MST total = 1+2+2+4+4+7+8+9 = 37** — identical to Prim's, as it must be (both optimal on the same graph).

Note the skipped edge 8—9 (7): at that moment 8 and 9 were already connected through 7, so accepting it would close the cycle 7—8—9—7 where it's tied-heaviest.

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| sorted edge list | `vector<tuple<int,int,int>>` of (w,u,v), `sort()` |
| find with path compression | `find(x){ return par[x]==x ? x : par[x]=find(par[x]); }` |
| everyone starts alone | `par[i] = i` |
| cycle test | `if (find(u) == find(v)) continue` |
| merge islands | `par[find(u)] = find(v)` |
| early stop | `mst.size() == n-1` |
| disconnected check | fewer than n−1 edges taken → `-1` |

**C++** (`problems/11_kruskals.cpp`) — the whole algorithm:

```cpp
sort(edges.begin(), edges.end());               // by weight
for (auto &[w, u, v] : edges) {
    int a = find(u), b = find(v);
    if (a == b) continue;                       // cycle → skip
    par[a] = b;                                 // union
    total += w;
    mst.push_back({u, v, w});
    if ((int)mst.size() == n - 1) break;        // tree complete
}
```

**Python** (`problems_python/11_kruskals.py`) — same, with a recursive `find`.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(m log m)` | sorting dominates; m unions/finds are near-constant (α(m) ≈ 4) |
| Space | `O(n + m)` | parent array + edge list |

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `n = 1` | n−1 = 0 edges needed → total 0 |
| disconnected | can't reach n−1 edges → `-1` |
| self-loop `u = v` | find(u)==find(v) always → skipped |
| parallel edges | the cheaper comes first in sort and joins; the second is skipped |
| equal weights | sort order between ties is arbitrary — different tie-breaks may give different MSTs, same total |

---

## 8. Kruskal's vs Prim's (learn this table cold)

| | Kruskal's | Prim's |
|---|---|---|
| Greedy scope | global (cheapest edge anywhere) | local (cheapest edge leaving the tree) |
| Structure kept | forest of many trees | one growing tree |
| Cycle avoidance | union-find test | built-in (never leaves the tree) |
| Needs connected input? | works on disconnected (produces a *spanning forest*; we detect and report) | must handle unreachable vertices |
| Best for | sparse graphs (m ≈ n) | dense graphs (m ≈ n²) |
| Time | `O(m log m)` | `O(m log m)` lazy / `O(n²)` array version |

Both are guaranteed optimal by the same cut/cycle properties.

---

## 9. Viva Questions

**Q: How does Kruskal detect cycles without DFS/BFS?**
Union-Find: an edge closes a cycle iff its endpoints already share a root.

**Q: What are the two Union-Find optimizations?**
Path compression (point every visited node straight at the root) and union by rank/size (attach the smaller tree under the larger). Together: `O(α(n))` per op — effectively constant.

**Q: Why is sorting by weight essential?**
The greedy proof (cycle property) relies on processing edges cheapest-first, so any skipped edge is never the unique cheapest way to connect something.

**Q: When would you prefer Kruskal over Prim?**
Sparse graphs — after sorting (m log m) the union-find phase barely runs. Prim with adjacency matrix `O(n²)` wins when m ≈ n².

**Q: Can Kruskal give a different tree than Prim but the same weight?**
Yes, whenever equal-weight edges allow multiple MSTs. Both totals are always the minimum.

**Q: What does Kruskal produce on a disconnected graph?**
A minimum spanning *forest* — one MST per connected component. Our program reports `-1` since a spanning tree doesn't exist.
