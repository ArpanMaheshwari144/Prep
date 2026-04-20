# Pattern 12: Graphs

## Ye kya hai?

Bhai dekh — Cities aur roads. Har city ek **node**, har road ek **edge**.

```
Delhi --- Mumbai --- Bangalore
  |                    |
Jaipur              Chennai
```

Tree bhi graph hai — lekin graph mein cycles ho sakte, multiple paths ho sakte.

## Graph kaise store karte hain? **Adjacency List:**

```
0 --- 1
|     |
3 --- 2

adj[0] = [1, 3]
adj[1] = [0, 2]
adj[2] = [1, 3]
adj[3] = [0, 2]
```

## Tree vs Graph:

```
Tree:  Ek root, koi cycle nahi, ek hi path
Graph: Koi bhi node se start, cycles ho sakte, multiple paths
```

## Ghoomne ke 2 tarike (same as Trees):

**DFS** — deep jao ek path mein, phir wapas (recursion)
**BFS** — level by level (queue)

**IMPORTANT FARK TREES SE:** Graph mein **visited track karna ZAROORI** — warna cycle mein infinite loop.

---

## DFS — Graph pe

```
dfs(node):
  visited[node] = true
  result mein daal
  for har neighbor:
    agar visited nahi → dfs(neighbor)
```

### Dry Run:
```
0 --- 1
|     |
3 --- 2

DFS(0): visit 0.
  neighbor 1 → DFS(1): visit 1.
    neighbor 0 → visited. Skip.
    neighbor 2 → DFS(2): visit 2.
      neighbor 1 → visited. Skip.
      neighbor 3 → DFS(3): visit 3.

Order: 0, 1, 2, 3
```

---

## BFS — Graph pe

```
Queue mein start daal. visited mark.
while queue khaali nahi:
  node nikaal
  result mein daal
  har neighbor: visited nahi → push + visited mark
```

### Dry Run:
```
0 --- 1
|     |
3 --- 2

Queue: [0]. visited={0}
0 nikala. Neighbors 1,3 push. Queue: [1,3]. visited={0,1,3}
1 nikala. Neighbor 2 push. Queue: [3,2]. visited={0,1,3,2}
3 nikala. Sab visited. Queue: [2].
2 nikala. Sab visited. Queue: [].

Order: 0, 1, 3, 2
```

---

## Number of Islands — DFS on Grid

**Real Life:** Satellite se dekh. Paani (0) aur zameen (1). Kitne alag islands?

```
1 1 0 0 0
1 1 0 0 0       → 3 islands
0 0 1 0 0
0 0 0 1 1
```

### Kaise solve:
1. Grid mein ghoom
2. '1' mili aur visited nahi → count++, DFS lagao
3. DFS saari connected '1' ko visited mark kare (4 directions)

### 4 Directions:
```
dirs = {{1,0}, {0,1}, {-1,0}, {0,-1}}   ← down, right, up, left

Naya position = current + direction:
  i_ = i + dir[0]
  j_ = j + dir[1]
```

### canVis function — DFS se pehle check:
```
Bounds check: i_ >= 0 && j_ >= 0 && i_ < m && j_ < n
Grid check:   grid[i_][j_] == '1' (zameen hai)
Visited check: !vis[i_][j_] (pehle nahi dekha)

TEENO chahiye — ek bhi miss → crash ya infinite loop
```

---

## Questions Jo Kiye

| # | Question | Type | Key trick |
|---|----------|------|-----------|
| 46 | DFS Graph | DFS | visited + recursion on neighbors |
| 47 | BFS Graph | BFS | queue + visited |
| 48 | Number of Islands | DFS Grid | 4 directions, canVis (bounds + grid + vis), count++ jab new '1' mile |

---

## Galtiyan Jo Hui (Number of Islands mein):

1. **grid[i][j] == 1 likhna '1' ki jagah** — grid char hai, '1' char hai, 1 int hai. Match nahi hoga.
2. **dirs[i][j] iterate karna** — dirs 2D array of directions hai, dirs[i][j] ek int hai. `for(auto& dir : dirs)` use karo.
3. **Same i,j pe DFS call** — naya position = i + dir[0], j + dir[1]. Same i,j → infinite recursion.
4. **Bounds check missing** — i_ negative ya grid se bahar → CRASH.
5. **canVis mein vis check missing** — already visited pe DFS → infinite loop.
6. **Count loop mein head badha diya** — variable alag rakh, original pointer lose mat kar.

---

## Q49: Cycle Detection — DFS + Parent Check

**Soch:** DFS se ghoom. Neighbor visited hai aur parent nahi → CYCLE!

```
dfs(node, parent):
  visited[node] = true
  isCycle = false
  for neighbor:
    !visited && neighbor != parent → dfs(neighbor, node). true aaya → isCycle=true
    visited && neighbor != parent → isCycle=true (CYCLE!)
  return isCycle
```

**Dry Run:**
```
0-1-2-3-0

dfs(0,-1) → dfs(1,0) → dfs(2,1) → dfs(3,2)
  neighbor 0: visited && 0!=2 → CYCLE! return true.
  true propagate: 2→true, 1→true, 0→true ✅
```

**Galtiyan:**
1. **`dfs(it, parent)` likhna `dfs(it, node)` ki jagah** — purana parent pass, naya nahi.
2. **`== false` check karna `== true` ki jagah** — cycle true return karta, lekin false se compare → lost.
3. **else if outer if ke andar** — visited check kabhi nahi hoga.
4. **return true/false confusion** — simple rakho: isCycle variable, end mein return.

## Q50: Topological Sort — Kahn's Algorithm (BFS + In-degree)

**Problem:** Directed Acyclic Graph mein nodes ka order nikaal — prerequisites satisfy hon.

**Steps:**
1. In-degree nikaal har node ka (kitne nodes usse pehle aane chahiye)
2. In-degree 0 wale queue mein daal (ye pehle process honge)
3. Queue se nikaal, result mein daal, neighbors ka indegree--
4. Indegree 0 ho jaye → queue mein daal
5. Repeat

**Dry Run:**
```
0 → 1 → 3
0 → 2 → 3

indegree = [0, 1, 1, 2]

Queue: [0] (indegree 0)
0 nikala. indegree[1]--, indegree[2]--. indegree=[0,0,0,2]. Queue: [1,2]
1 nikala. indegree[3]--. indegree=[0,0,0,1]. Queue: [2]
2 nikala. indegree[3]--. indegree=[0,0,0,0]. Queue: [3]
3 nikala. Done.

Result: [0,1,2,3] ✅
```

**Galtiyan:**
1. **q.push(indegree[i]) likhna q.push(i) ki jagah** — value push, index nahi!
2. **Sirf node 0 push karna** — in-degree 0 wale multiple ho sakte
3. **indegree[it]-- bhoolna** — neighbor ka indegree kam nahi hoga, kabhi 0 nahi aayega
4. **In-degree calculate na karna** — BFS lagana kaafi nahi, in-degree zaroori

## Q51: Dijkstra — Shortest Path (Priority Queue + Greedy)

**Problem:** Weighted graph mein ek source se sab nodes tak shortest distance dhundh.

**Real Life:** Google Maps — sabse chhota rasta dhundh. Har road ki distance alag.

**BFS vs Dijkstra:**
```
BFS:      sab edges weight 1. Simple Queue.
Dijkstra: edges alag alag weight. Priority Queue (min-heap).
```

**Kaise kaam karta:**
1. dist array — sab INT_MAX, source = 0
2. Priority Queue mein {0, source} daal
3. Sabse chhoti distance wala nikaal
4. Uske neighbors dekh — agar chhota rasta mila → update + push

**Priority Queue kyun?** Sabse chhoti distance wala pehle process karna hai — min-heap se wo automatically pehle niklega.

**Dry Run:**
```
    1
A ----- B
|       |
4|      |2
|   3   |
C ----- D

dist = [0, MAX, MAX, MAX]  pq = [{0,A}]

Pop {0,A}:
  A→B: 0+1=1 < MAX → dist[B]=1. push {1,B}
  A→C: 0+4=4 < MAX → dist[C]=4. push {4,C}

Pop {1,B}:
  B→D: 1+2=3 < MAX → dist[D]=3. push {3,D}

Pop {3,D}:
  D→C: 3+3=6 > 4 → skip
  D→B: 3+2=5 > 1 → skip

Pop {4,C}:
  sab skip.

dist = [0, 1, 4, 3] ✅
```

**Template:**
```
dist(n, INT_MAX). dist[src] = 0.
pq.push({0, src}).

while(!pq.empty()):
    auto [d, node] = pq.top(). pq.pop().
    if(d > dist[node]) continue.    ← purana entry, skip
    for neighbors:
        newDist = d + weight
        if(newDist < dist[neighbor]):
            dist[neighbor] = newDist
            pq.push({newDist, neighbor})
```

**IMPORTANT — & vs copy:**
```
auto [d, node] = pq.top()    ← COPY. Pop ke baad safe. YE USE KARO.
auto &[d, node] = pq.top()   ← REFERENCE. Pop ke baad INVALID — crash!
```

**Galtiyan:**
1. **for(i=0; i<n; i++) sab nodes loop** — sirf current node ke neighbors dekho: `for(auto& it : adj[node])`
2. **pq.push({dist, it})** — it pair hai, int nahi. `it.first` (node) push karo.
3. **auto &[d,node] = pq.top()** — reference liya, pop kiya, reference invalid. & hatao.
4. **d > dist[node] check missing** — purane entries process honge, slow.

## Graphs ✅ DONE

---

---

## Ek Line Mein Yaad Rakh

> Graph DFS = **"Visited mark. Result daal. Neighbors loop — visited nahi toh DFS call."**
> Graph BFS = **"Queue mein daal. Nikaal, neighbors push — visited nahi toh."**
> Number of Islands = **"Grid ghoom. '1' mili → count++, DFS se sab connected '1' visited mark."**
