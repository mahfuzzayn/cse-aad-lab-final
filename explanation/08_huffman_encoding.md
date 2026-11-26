# 08 — Huffman Encoding (Greedy)

**Category 02** | Technique: Greedy + Min-Heap + Binary Tree

---

## 1. The Problem

Given `n` symbols and how often each occurs (frequency), assign each symbol a **binary code** so that the total encoded length `Σ freq × code-length` is as small as possible, with **no code being a prefix of another** (prefix-free codes — needed so decoding is unambiguous).

**Input**
```
n
symbol1 freq1
symbol2 freq2
...
```
**Output:** each symbol with its code, then the total encoded length.

---

## 2. The Intention

Huffman coding is **data compression** — it's inside ZIP, JPEG, MP3, HTTP/2 HPACK. It teaches the deepest greedy idea in the course:

> **Rarest symbols get longest codes; frequent symbols get shortest codes.**

And it shows how a *greedy choice* (merge the two smallest nodes) provably lands in an optimal solution, plus how a min-heap and a binary tree work together.

---

## 3. The Core Idea (No Code Yet)

### Why prefix-free?

If `A = 0` and `B = 01`, the stream `01` is ambiguous (`B` or `A,?`). Prefix-free codes (no code starts another code) make each stream decode exactly one way. Putting symbols **only at the leaves** of a binary tree automatically guarantees prefix-freeness: the code is the path (left = 0, right = 1) from root to leaf, and no leaf can be on another leaf's path.

### Why frequencies matter

Total length = `Σ freq × (depth of leaf)`. To minimize it, push deep the symbols you rarely use, keep shallow the frequent ones.

### The greedy step

The two **rarest** symbols are safe to make **deepest siblings** — they'll share the maximum code length, differing only in the last bit. Merge them into one combined node with frequency `f1 + f2`, put it back, repeat. This is provably optimal (exchange argument: any optimal tree can be rearranged so the two rarest are siblings).

### The merge process (this IS the algorithm)

```
repeat n−1 times:
    take the two smallest-frequency nodes from a min-heap
    make them children of a new node with frequency = their sum
    insert the new node back
one node remains → root of the Huffman tree
```

---

## 4. The Whiteboard Process (Full Trace — CLRS example)

Input:

| symbol | a | b | c | d | e | f |
|--------|---|---|---|---|---|---|
| freq   | 5 | 9 | 12 | 13 | 16 | 45 |

### Round 1 — two smallest: a(5) and b(9)

Merge → node `ab(14)`:

```
      ab(14)
      /    \
    a(5)   b(9)
```
Pool now: c(12), d(13), **ab(14)**, e(16), f(45)

### Round 2 — two smallest: c(12) and d(13)

Merge → `cd(25)`:

```
      cd(25)
      /    \
    c(12)  d(13)
```
Pool: ab(14), e(16), cd(25), f(45)

### Round 3 — two smallest: ab(14) and e(16)

Merge → `abe(30)`:

```
       abe(30)
       /    \
     ab(14)  e(16)
     /  \
   a(5) b(9)
```
Pool: abe(30), cd(25), f(45)

### Round 4 — two smallest: cd(25) and abe(30)

Merge → `abcde(55)` (cd popped first → cd becomes the left child):

```
      abcde(55)
      /      \
   cd(25)    abe(30)
   /   \      /    \
  c     d   ab(14)  e
             /  \
            a    b
```
Pool: abcde(55), f(45)

### Round 5 — last two: f(45) and abcde(55) → root(100)

(f popped first → f becomes the left child)

```
                root(100)
               /         \
            f(45)      abcde(55)
                        /      \
                     cd(25)    abe(30)
                     /   \      /    \
                    c     d   ab(14)  e
                               /  \
                              a    b
```

### Read codes off the tree (left = 0, right = 1)

Which merged node becomes the left vs right child is a free choice — the program makes the *first-popped* (smaller) node the left child. Reading paths from the root gives:

| symbol | code | length | contribution |
|--------|------|--------|--------------|
| a | 1100 | 4 | 5×4 = 20 |
| b | 1101 | 4 | 9×4 = 36 |
| c | 100 | 3 | 12×3 = 36 |
| d | 101 | 3 | 13×3 = 39 |
| e | 111 | 3 | 16×3 = 48 |
| f | 0 | 1 | 45×1 = 45 |

**Total = 20+36+36+39+48+45 = 224 bits.**

Compare fixed-length coding: 6 symbols need 3 bits each → `100×3 = 300` bits. **Huffman saves 25%.**

Sanity check the intuition: f (most frequent, 45) got the shortest code (1 bit); a, b (rarest) got 4 bits each. Exactly the "frequent = short" principle.

---

## 5. From Idea to Code

The whole construction is array-friendly — no pointers needed:

- leaves are indices `0..n−1`, internal nodes get indices `n, n+1, ...`
- `l[i]`, `r[i]` = children of node i
- min-heap of `(freq, node)` pairs
- after merging, walk the tree (BFS/DFS) building each leaf's code string: append `'0'` going left, `'1'` going right

| Whiteboard idea | Code element |
|---|---|
| min-heap of frequencies | `priority_queue<pair<long long,int>, ..., greater<>>` |
| merge two smallest | pop twice, create node `cnt` |
| combined frequency | `w[cnt] = fu + fv` |
| heap push-back | `pq.push({w[cnt], cnt})` |
| read codes from tree | queue/stack walk appending `0`/`1` |
| total bits | `Σ freq[i] * code[i].size()` |

**C++** (`problems/08_huffman_encoding.cpp`) and **Python** (`problems_python/08_huffman_encoding.py`) produce identical output for the same input (both break ties by node index).

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n log n)` | n−1 merges, each heap op is `O(log n)` |
| Space | `O(n)` | tree arrays + heap + code strings |

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `n = 1` | no merge happens; single symbol gets code `0` (a tree needs at least one bit per symbol) |
| `n = 0` | no output |
| equal frequencies | ties broken by node index — deterministic; different tie-breaks give different (but equally optimal) codes |
| two symbols | codes `0` and `1` |

---

## 8. Viva Questions

**Q: Why must codes be prefix-free?**
Without it, a bit stream can decode multiple ways. Prefix-free codes make decoding unambiguous and instantaneous (stop as soon as you hit a leaf).

**Q: What is the greedy choice here?**
"Merge the two lowest-frequency nodes" — justified by an exchange argument: in some optimal tree the two rarest symbols are deepest siblings, so committing to that merge loses nothing.

**Q: What data structure makes it efficient?**
A min-heap (priority queue) — extract-min twice and insert once per merge, `O(log n)` each.

**Q: Is Huffman always optimal among prefix codes?**
Yes, for symbol-by-symbol (fixed input alphabet) coding given exact frequencies. It is *not* optimal in general — arithmetic coding and ANS can beat it.

**Q: How do you decode?**
Start at the root; read bits: 0 → left, 1 → right; on a leaf output the symbol and restart at root. Prefix-freeness guarantees correctness.

**Q: Why is the Huffman tree full (every internal node has 2 children)?**
If an internal node had one child, moving its subtree up shortens some codes without harming prefix-freeness — a non-full tree can't be optimal.

**Q: What is the relationship to fixed-length vs variable-length?**
Fixed-length (like ASCII, 8 bits) wastes space on skewed distributions; Huffman adapts lengths to frequencies, always ≤ fixed length + one bit per symbol.
