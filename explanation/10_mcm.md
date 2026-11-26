# 10 — Matrix Chain Multiplication (MCM)

**Category 03** | Technique: Dynamic Programming on intervals (the table + split-point table)

---

## 1. The Problem

You must multiply a **chain** of `n` matrices `A1 × A2 × ... × An`. The mathematical result is fixed no matter how you parenthesize — but the **number of scalar multiplications** changes dramatically.

Matrix `Ai` has dimensions `p[i-1] × p[i]` — so the input is just the `n+1` dimension array `p0 p1 ... pn` (adjacent matrices share the inner dimension, which is what makes the chain multiplicable at all).

Find the parenthesization with **minimum scalar multiplications**.

**Input**
```
n
p0 p1 ... pn
```
**Output:** minimum cost, then the optimal parenthesization (e.g. `((A1*A2)*A3)`).

---

## 2. The Intention

MCM is *the* **interval DP** problem. Its two-table structure (a cost table `m` + a split table `s`) is exactly what your faculty draws on the whiteboard. It also teaches a fact students never forget once they see a number:

> Same multiplication, different brackets, wildly different cost.

### The counting rule (the whole problem rests on it)

Multiplying a `q×r` matrix by an `r×s` matrix costs **q · r · s** scalar multiplications.

---

## 3. The Core Idea (No Code Yet)

### Why brackets matter — the classic demo

`A1` is 10×20, `A2` is 20x30, `A3` is 30×40 — p = [10, 20, 30, 40].

- `(A1×A2)×A3`: (10·20·30) + (10·30·40) = 6000 + 12000 = **18000**
- `A1×(A2×A3)`: (20·30·40) + (10·20·40) = 24000 + 8000 = **32000**

Same three matrices, same answer matrix — one way costs 78% more. Choosing brackets *is* the problem.

### The state

> `m[i][j]` = *cheapest cost to multiply the sub-chain `Ai ... Aj` (inclusive)**

Sub-chain `Ai...Aj` uses dimensions `p[i-1] ... p[j]` — a contiguous slice of `p`.

### The transition — the LAST multiplication

However you bracket `Ai...Aj`, there is a **final** multiplication:

```
(Ai ... Ak)  ×  (A(k+1) ... Aj)        for some k,  i ≤ k < j
   q×p[k]          p[k]×s              → costs q·p[k]·s = p[i-1]·p[k]·p[j]
```

The total cost splits into three independent pieces:

```
m[i][j] = min over k of:  m[i][k]  +  m[k+1][j]  +  p[i-1]·p[k]·p[j]
                          └─left──┘   └──right──┘   └──final mult──┘
```

`k` is the **split point**. Try every legal k, keep the best. Also record which k won in `s[i][j]` — that second table is what lets us print the parenthesization.

### Base case

`m[i][i] = 0` — one matrix, nothing to multiply.

### Filling order (the subtle part!)

`m[i][j]` needs shorter chains (`m[i][k]`, `m[k+1][j]`). So fill by **chain length** = 2, 3, ..., n — shortest chains first.

---

## 4. The Whiteboard Process (Full Trace)

CLRS classic: `n = 6`, `p = [30, 35, 15, 5, 10, 20, 25]`

| Matrix | Dimensions |
|---|---|
| A1 | 30×35 |
| A2 | 35×15 |
| A3 | 15×5 |
| A4 | 5×10 |
| A5 | 10×20 |
| A6 | 20×25 |

Draw two **upper-triangular** tables, `m` (cost) and `s` (split). Diagonal of `m` = 0.

### Length 2 chains (one multiplication each) — `m[i][i+1] = p[i-1]·p[i]·p[i+1]`

| Chain | Cost |
|---|---|
| m[1][2] = 30·35·15 | **15750** |
| m[2][3] = 35·15·5 | **2625** |
| m[3][4] = 15·5·10 | **750** |
| m[4][5] = 5·10·20 | **1000** |
| m[5][6] = 10·20·25 | **5000** |

### Length 3 chains — two candidate splits each

| Chain | k = i | k = i+1 | Best | s |
|---|---|---|---|---|
| m[1][3] | 0 + 2625 + 30·35·5 = 7875 | 15750 + 0 + 30·15·5 = 18000 | **7875** | 1 |
| m[2][4] | 0 + 750 + 35·15·10 = 6000 | 2625 + 0 + 35·5·10 = 4375 | **4375** | 3 |
| m[3][5] | 0 + 1000 + 15·5·20 = 2500 | 750 + 0 + 15·10·20 = 3750 | **2500** | 3 |
| m[4][6] | 0 + 5000 + 5·10·25 = 6250 | 1000 + 0 + 5·20·25 = 3500 | **3500** | 5 |

Worked cell `m[2][4]` in words: split after A2 → (A2)·(A3A4) costs `0 + 750 + 35·15·10 = 6000`; split after A3 → (A2A3)·(A4) costs `2625 + 0 + 35·5·10 = 4375`. Second wins → m[2][4] = 4375, s = 3.

### Length 4 chains

| Chain | Candidates k=… | Best | s |
|---|---|---|---|
| m[1][4] | k1: 0+4375+30·35·10 = 14875; k2: 15750+750+30·15·10 = 21000; k3: 7875+0+30·5·10 = 9375 | **9375** | 3 |
| m[2][5] | k2: 0+2500+35·15·20 = 13000; k3: 2625+1000+35·5·20 = 7125; k4: 4375+0+35·10·20 = 11375 | **7125** | 3 |
| m[3][6] | k3: 0+3500+15·5·25 = 5375; k4: 750+5000+15·10·25 = 9500; k5: 2500+0+15·20·25 = 10000 | **5375** | 3 |

### Length 5 chains

| Chain | Best | s |
|---|---|---|
| m[1][5] | k1: 0+7125+30·35·20 = 28125; k2: 15750+2500+30·15·20 = 27250; k3: 7875+1000+30·5·20 = **11875**; k4: 9375+0+30·10·20 = 15375 | **11875** | 3 |
| m[2][6] | k2: 0+5375+35·15·25 = 18500; k3: 2625+3500+35·5·25 = **10500**; k4: 4375+5000+35·10·25 = 18125; k5: 7125+0+35·20·25 = 24625 | **10500** | 3 |

### Length 6 — the answer cell

`m[1][6]`, candidates:

| k | Expression | Cost |
|---|---|---|
| 1 | 0 + m[2][6] + 30·35·25 = 10500 + 26250 | 36750 |
| 2 | m[1][2] + m[3][6] + 30·15·25 = 15750 + 5375 + 11250 | 32375 |
| 3 | m[1][3] + m[4][6] + 30·5·25 = 7875 + 3500 + 3750 | **15125** ← |
| 4 | m[1][4] + m[5][6] + 30·10·25 = 9375 + 5000 + 7500 | 21875 |
| 5 | m[1][5] + 0 + 30·20·25 = 11875 + 15000 | 26875 |

**Answer: m[1][6] = 15125, split s[1][6] = 3.**

### Reading the parenthesization from the s table

Recurse: `s[1][6]=3` → (A1A2A3)(A4A5A6); `s[1][3]=1` → (A1)(A2A3); `s[2][3]` → (A2)(A3); `s[4][6]=5` → (A4A5)(A6); `s[4][5]` → (A4)(A5). Result:

```
((A1*(A2*A3))*((A4*A5)*A6))     cost 15125
```

---

## 5. From Idea to Code

| Whiteboard idea | Code element |
|---|---|
| two triangular tables | `m`, `s` of size `(n+1)²` |
| fill by chain length | outer loop `len = 2..n` |
| every start i, end j = i+len−1 | two nested loops |
| try every split k | innermost loop, cost formula |
| record winner | `s[i][j] = k` on improvement |
| print brackets | recursive: split at `s[i][j]`, leaf prints `Ai` |

**C++** (`problems/10_mcm.cpp`)

```cpp
for (int len = 2; len <= n; len++)
    for (int i = 1; i + len - 1 <= n; i++) {
        int j = i + len - 1;
        m[i][j] = LLONG_MAX;
        for (int k = i; k < j; k++) {
            long long cost = m[i][k] + m[k+1][j] + (long long)p[i-1] * p[k] * p[j];
            if (cost < m[i][j]) { m[i][j] = cost; s[i][j] = k; }
        }
    }
```

**Python** (`problems_python/10_mcm.py`) — identical loops with `float("inf")` sentinel.

---

## 6. Complexity

| Measure | Value | Why |
|---|---|---|
| Time | `O(n³)` | three nested loops: lengths × starts × splits |
| Space | `O(n²)` | the two tables |

---

## 7. Edge Cases (handled)

| Case | Behaviour |
|---|---|
| `n = 1` | nothing to multiply → cost 0, prints `A1` |
| `n = 2` | forced order → p0·p1·p2, prints `(A1*A2)` |
| `n = 0` | prints 0 and exits |
| big dimensions | 64-bit (`long long`) prevents overflow in the cost |
| ties between splits | first (smallest k) wins — deterministic |

---

## 8. Viva Questions

**Q: Why is matrix multiplication associative but this problem still hard?**
Associativity fixes the *result*, not the *cost*. Cost depends on the intermediate shapes, so bracket choice matters.

**Q: What is the DP state for MCM?**
`m[i][j]` = min cost of the sub-chain Ai..Aj; `s[i][j]` = optimal split point.

**Q: Why fill the table by chain length?**
`m[i][j]` depends only on strictly shorter chains; processing by increasing length guarantees subproblems are ready.

**Q: How many ways to fully parenthesize n matrices?**
The Catalan number `C(n−1) = (2n−2)! / (n!(n−1)!)` — exponential, so brute force is hopeless.

**Q: Why is the final-multiplication viewpoint the key insight?**
Every bracketing has one final `×`; splitting there decomposes the problem into two independent sub-chains — that's the optimal substructure.

**Q: Which cell holds the answer and why isn't it m[n][n]?**
`m[1][n]` — the whole chain from A1 to An. `m[i][i]` is just the zero-cost diagonal.
