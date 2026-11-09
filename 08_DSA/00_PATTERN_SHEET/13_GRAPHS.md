## PATTERN 13 — GRAPHS (BFS/DFS + visited)


> [!IMPORTANT]
> **KAB:** edges / adjacency list / grid diya · "kitne group?" · "A se B pahunch sakte?" · "kam se kam kitne step?"

> [!IMPORTANT]
> **BROAD IDEA:** graph = nodes + edges. TREE bhi ek graph hai (special) -> Graph = Tree + 2 cheez:
>
> (1) CYCLE ho sakti -> VISITED chahiye (warna A->B->C->A infinite loop)
>
> (2) pointers ki jagah ADJACENCY LIST.

> [!IMPORTANT]
> **TREE se transfer:** BFS = queue (level-order jaisa) · DFS = recursion (left/right ki jagah adj[node]).
> bas 'visited' add.

### INPUT-FORM pehchano (pehle ye -- kab adj-list banani, kab NAHI):

```
      unordered_map<int, vector<int>> adj;    // ya  vector<int> adj[n];
      edges[] + undirected -> adj[u]+=v; adj[v]+=u;  DONO taraf      (path-exists, CC, dijkstra{v,w})
      edges[] + directed   -> sirf adj[u]+=v; + indegree[v]++        (course-schedule/topo)
      GRID (2D) diya       -> adj-list NAHI -> padosi = 4-dir `dirs`, cell khud node
                              (islands, rotting, max-area)
      graph[]/node.neighbors already -> build SKIP, seedha traverse  (bipartite, clone-graph)
```

### DECISION-GUIDE (koi graph-Q se pehle):

```
   Q1  OUTER-LOOP chahiye? (SABSE bada -- yehi trick)
        kitne group / poora graph / disconnected -> HAAN:
             for (i : all nodes)  if (!vis[i]) { count++; BFS(i); }   // sirf unvisited pe: naya group
                                                                      //   (CC, Islands)
        ek source -> target / ek hi component -> NAHI: sirf source se EK traversal   (Path-Exists)
```

```
   Q2  DFS ya BFS? (connectivity ke liye dono same)
        BFS = shortest-path / level (unweighted) · DFS = recursion, code chhota
        BFS (queue):                          DFS (recursion):
            vis[start] = true;                    void dfs(int node) {
            q.push(start);                            vis[node] = true;
            while (!q.empty()) {                      for (int nbr : adj[node])
                int node = q.front();                     if (!vis[nbr]) dfs(nbr);
                q.pop();                              }
                for (int nbr : adj[node])
                    if (!vis[nbr]) {
                        vis[nbr] = true;   // PUSH pe mark: warna ek node queue me do baar
                        q.push(nbr);
                    }
            }
        GRID pe: adj[node] ki jagah 4-direction (Q3 dirs); visited = cell ko SINK ('0'/'2').
```

> [!WARNING]
> ★ EDGE (BFS grid / word ladder): visited ANDAR DAALTE waqt mark karo, nikaalte waqt nahi
> (warna ek node kai baar queue me).

> [!WARNING]
> ★ EDGE (queue / stack / heap se nikaalna): "auto &it = q.front(); q.pop();" = TOOTA reference (pop ke baad it kachra).
> copy lo (auto it = q.front()) ya pehle padho phir pop. tests sanyog se pass ho jaate, isliye chhupa rehta (rotting REDO_1).

```
   Q2b BFS ke ANDAR: PLAIN ya LEVEL-ORDER? (rotting me chhoota tha)
        "reach / connected / count / islands"   -> PLAIN BFS (level-count nahi)
        "kitne STEPS / MINUTES / shortest-dist"  -> LEVEL-ORDER, ek queue-size snapshot = ek level = ek step:
             while (!q.empty()) { int sz = q.size(); while (sz--) { ... push nbrs } steps++; }
             // sz pehle pakdo; steps++ level ke baad, andar nahi   (rotting, min-steps-in-grid, word-ladder)
```

```
   Q3  neighbours kaise? edges -> adj[node] · grid -> 2 TRICK (ratta, apne aap nahi aate):
          (a) vector<vector<int>> dirs = {{1,0},{-1,0},{0,1},{0,-1}};   // neeche, upar, daaye, baaye
              for (auto &d : dirs) { int nr = r + d[0], nc = c + d[1]; ... }
          (b) if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == ?)   // bounds PEHLE, grid baad:
              //   bahar index pe && pehle hi ruk jaata. HAR grid-Q me; '==?' per-Q (islands/rotting == '1')
```

> [!IMPORTANT]
> **visited HAMESHA (cycle rok) -> push/enter ke waqt mark.**

#### ── FAMILY: single-source traversal (ek source se ek BFS, target mila? koi OUTER-loop nahi) ──

---

### ▸ PATH EXISTS (LC-1971)  = single BFS (reachability)

**SAWAAL**
```
n node (0 se n-1), bina disha wale (undirected) edges, ek source aur ek destination diye hain. Kya source se destination tak koi rasta hai? true ya false lautao.
  e.g. n=6, edges=[[0,1],[0,2],[3,5],[5,4],[4,3]], src=0, dst=5 -> false
  kyun: 0 ke saath sirf 1,2 jude; 5 alag group {3,4,5} me, beech me koi edge nahi
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** = tree LEVEL-ORDER ka skeleton + 3 ADD: (1) neighbours = adj[node] (edges se banao)
> (2) vis[] (tree me cycle nahi, graph me hai)  (3) level-loop NAHI: sirf reachability

> [!IMPORTANT]
> **CHAABI:** edges se `adj` banao (undirected = dono taraf), source se EK BFS; pop pe `node == destination` to true, queue khaali to false. Outer loop nahi, level-loop nahi.

> [!WARNING]
> `vis` PUSH ke saath hi mark karo, aur `vis[source] = true` bhi: warna cycle se ghoom ke ek node (source bhi) queue me do baar.

**CODE**
```cpp
bool validPath(int n, vector<vector<int>> &edges, int source, int destination) {
    unordered_map<int, vector<int>> adj;
    for (auto &it : edges) {
        int u = it[0];
        int v = it[1];
        adj[u].push_back(v);
        adj[v].push_back(u);                // undirected -> dono taraf
    }
    queue<int> q;
    vector<bool> vis(n, false);
    q.push(source);
    vis[source] = true;                     // ★ src bhi mark: warna cycle se ghoom ke src dobara push
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        if (node == destination)            // pop pe check: src == dst wala case bhi pakda jaata
            return true;
        for (auto &it : adj[node]) {
            if (!vis[it]) {
                vis[it] = true;             // ★ push ke saath hi mark: warna ek node queue me do baar
                q.push(it);
            }
        }
    }
    return false;                           // queue khaali -> koi path nahi (alag group)
}
```


#### ── FAMILY: COUNT components (outer-loop + FLOOD each; Islands = grid, CC = adj-list, bas neighbour badla) ──

### ▸ NUMBER OF ISLANDS (LC-200)  = Connected-Components on a GRID

**SAWAAL**
```
Ek grid di hai jisme '1' = zameen aur '0' = paani. Upar, neeche, daayein, baayein (4 disha) se jude '1' milke ek island bante hain. Kitne island hain, wo ginti lautao.
  e.g. [[1,1,0],[1,0,0],[0,0,1]] -> 2
  kyun: (0,0),(0,1),(1,0) ek group; (2,2) akela, uske 4 padosi paani; to 2
```

> [!IMPORTANT]
> **CHAABI:** har naya `'1'` = naya island: DFS se poora island `'0'` me dubo do (SINK = visited), phir `count++`.

> [!WARNING]
> base ek hi `if` sab `||` se: bounds PEHLE, `grid[i][j] == '0'` BAAD (bahar index pe grid padhne se pehle ruk jaata). `'0'` base me na ho to poora grid kha jaata.

**CODE**
```cpp
void DFS(vector<vector<char>> &grid, int i, int j, int m, int n) {
    if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == '0')   // ★ BASE ek hi if, sab || se
        return;                             // bounds PEHLE, '0' baad: bahar index pe grid padhne se
                                            //   pehle ruk jaata. '0' base me: warna poora grid kha jaata
    grid[i][j] = '0';                       // ★ SINK = visited, dobara na gino
    DFS(grid, i + 1, j, m, n);              // 4-dir: neeche
    DFS(grid, i - 1, j, m, n);              // upar
    DFS(grid, i, j + 1, m, n);              // right
    DFS(grid, i, j - 1, m, n);              // left
}

int numIslands(vector<vector<char>> &grid) {
    int m = grid.size();
    int n = grid[0].size();
    int count = 0;
    for (int i = 0; i < m; i++) {           // outer: har cell scan
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == '1') {        // har naya '1' = naya island
                DFS(grid, i, j, m, n);      // poora island dubo do
                count++;
            }
        }
    }
    return count;
}
```

---

### ▸ CONNECTED COMPONENTS (LC-323)  = ISLANDS par ADJ-LIST graph (grid nahi) + BFS

**SAWAAL**
```
n node (0 se n-1) aur bina disha wale edges diye hain. Graph me kitne alag-alag group (connected components) hain, wo ginti lautao.
  e.g. n=5, edges=[[0,1],[1,2],[3,4]] -> 2
  kyun: {0,1,2} ek group, {3,4} doosra; dono ke beech koi edge nahi
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** ISLANDS ka SAME skeleton (outer-loop + count), 2 farak: graph = ADJ-LIST, aur BFS (islands DFS)

> [!IMPORTANT]
> **CHAABI:** islands ka skeleton, grid ki jagah adj-list: outer loop har node, `!vis[i]` mila to `count++` + BFS (poora group mark). padosi = `adj[node]`.

> [!WARNING]
> `count++` aur BFS dono `if (!vis[i])` ke ANDAR, warna har node gina jaata (= n). start bhi push ke saath mark.

**CODE**
```cpp
int countComponents(int n, vector<vector<int>> &edges) {
    unordered_map<int, vector<int>> adj;
    for (auto &it : edges) {
        int u = it[0];
        int v = it[1];
        adj[u].push_back(v);
        adj[v].push_back(u);                // undirected -> dono taraf
    }
    queue<int> q;
    vector<bool> vis(n, false);             // sab unvisited
    int count = 0;                          // abhi koi group nahi gina
    for (int i = 0; i < n; i++) {           // ★ outer-loop: DISCONNECTED groups bhi cover
        if (!vis[i]) {                      // count++ AUR BFS dono iske ANDAR: warna har node gine (= n)
            q.push(i);
            vis[i] = true;                  // start bhi push ke saath mark
            count++;                        // ★ har UNVISITED node = naya component
            while (!q.empty()) {            // = PATH-EXISTS ka body
                int node = q.front();
                q.pop();
                for (auto &it : adj[node]) {   // neighbours = adj[node] (islands me 4-dir) = yahi farak
                    if (!vis[it]) {
                        vis[it] = true;
                        q.push(it);
                    }
                }
            }
        }
    }
    return count;
}
```

---

### ▸ MAX AREA OF ISLAND (LC-695)  = Islands, par MAX AREA (count nahi)

**SAWAAL**
```
Ek grid di hai jisme 1 = zameen aur 0 = paani (4 disha se jude 1 = ek island). Sabse bade island me kitne cell hain, wo lautao. Koi island na ho to 0.
  e.g. [[1,1,0],[1,0,0],[0,0,1]] -> 3
  kyun: island {(0,0),(0,1),(1,0)} = 3 cell, island {(2,2)} = 1 cell; max 3
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** ISLANDS ka SAME skeleton, 2 DELTA: (1) DFS void nahi, AREA return  (2) outer count++ nahi, MAX

> [!IMPORTANT]
> **CHAABI:** islands hi, 2 delta: DFS `void` nahi, area lautaye `1 + neeche + upar + right + left`; outer me `count++` nahi, `ans = max(ans, DFS(...))`.

> [!WARNING]
> base pe bounds PEHLE, `grid[i][j] == 0` baad, dono pe `return 0`; `1` sirf asli cell pe. `ans` 0 se (koi island nahi -> 0).

**CODE**
```cpp
int DFS(vector<vector<int>> &grid, int i, int j, int m, int n) {
    if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0)
        return 0;                           // bounds / paani -> 0 (bounds PEHLE)
    grid[i][j] = 0;                         // sink (visited)
    int neeche = DFS(grid, i + 1, j, m, n);
    int upar = DFS(grid, i - 1, j, m, n);
    int right = DFS(grid, i, j + 1, m, n);
    int left = DFS(grid, i, j - 1, m, n);
    return 1 + left + right + upar + neeche;   // ★ 1 = ye cell khud + chaaron taraf ka area
}

int maxAreaOfIsland(vector<vector<int>> &grid) {
    int m = grid.size();
    int n = grid[0].size();
    int ans = 0;                            // 0 se: area >= 0, koi island nahi -> 0
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 1)
                ans = max(ans, DFS(grid, i, j, m, n));   // har island ka area, sabse bada rakho
        }
    }
    return ans;
}
```


#### ── FAMILY: MULTI-SOURCE / level-BFS (saare sources EK saath queue; sz-snapshot = ek level = ek minute) ──

### ▸ ROTTING ORANGES (LC-994)  = grid MULTI-SOURCE BFS + levels

**SAWAAL**
```
Ek grid me 0 = khaali, 1 = taaza santra, 2 = sada santra. Har minute har sada santra apne 4 taraf ke taaze santron ko sada deta hai.
Saare santre sadne me kam se kam kitne minute lagenge, wo lautao. Kabhi na sad paayein to -1.
  e.g. [[2,1,1],[1,1,0],[0,1,1]] -> 4
  kyun: min 1: (0,1),(1,0) · min 2: (0,2),(1,1) · min 3: (2,1) · min 4: (2,2); sab sad gaye
```

### purani galti (Aug sheet)

```
★ PURANI GALTI (Aug sheet): END me sirf return mins-1 tha -> grid me koi santra hi nahi to -1 aata, sahi 0. Isliye ab mins == 0 ? 0 : mins-1.
```

> [!IMPORTANT]
> **CHAABI:** saare `2` PEHLE queue me (multi-source = level 0); `sz = q.size()` snapshot = ek minute, `while (sz--)` ke BAHAR `mins++`; end me `1` bacha to -1.

> [!WARNING]
> aakhri level ke baad bhi `mins++` hota -> `mins - 1`; par koi santra hi nahi to `mins = 0` -> seedha 0 lautao (`mins - 1` = -1 galat). `grid = 2` push ke saath hi, warna doosra rotten bhi push karta.

**CODE**
```cpp
int orangesRotting(vector<vector<int>> &grid) {   // tree LEVEL-ORDER grid pe; har level = 1 minute
    queue<pair<int, int>> q;
    int m = grid.size();
    int n = grid[0].size();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 2)
                q.push({i, j});             // ★ MULTI-SOURCE = level-0 (saare rotten PEHLE)
        }
    }
    vector<vector<int>> dirs = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};   // 4-dir
    int mins = 0;
    while (!q.empty()) {
        int sz = q.size();                  // while se PEHLE pakdo: andar push se size badalta
        while (sz--) {                      // sz baar = sirf is minute wale rotten
            int row = q.front().first;
            int col = q.front().second;
            q.pop();                        // pehle row / col padho, PHIR pop
            for (auto &it : dirs) {
                int nR = it[0] + row;
                int nC = it[1] + col;
                if (nR >= 0 && nR < m && nC >= 0 && nC < n && grid[nR][nC] == 1) {   // bounds PEHLE
                    grid[nR][nC] = 2;       // ★ push ke saath hi 2: warna doosra rotten bhi push karta
                    q.push({nR, nC});
                }
            }
        }
        mins++;                             // poora level = 1 minute (while(sz--) ke BAHAR)
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 1)
                return -1;                  // fresh bacha -> impossible
        }
    }
    return mins == 0 ? mins : mins - 1;     // ★ last level extra -> -1. EDGE: koi santra hi nahi ->
}                                           //   mins = 0 -> mins-1 = -1 galat, sahi 0
```


#### ── FAMILY: TOPO-SORT / cycle-detect (DIRECTED; prereq + can-finish -> Kahn's = BFS + indegree) ──

### ▸ COURSE SCHEDULE (LC-207)  = TOPO-SORT (Kahn's), cycle-detect

**SAWAAL**
```
numCourses courses aur prerequisites diye hain, [a, b] ka matlab: a padhne se pehle b padhna zaroori hai. Kya saare course poore kiye ja sakte hain? true ya false lautao.
  e.g. numCourses=2, prereq=[[1,0],[0,1]] -> false
  kyun: 1 ke liye 0 pehle, 0 ke liye 1 pehle = CYCLE; dono ek doosre ka wait karte
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** "prereq + can-finish-ALL?" = koi CYCLE to nahi? cycle -> false
> [[1,0],[2,1],[3,2]]: har [a,b] = a lene se pehle b -> edge b -> a

> [!IMPORTANT]
> **CHAABI:** `[a, b]` = edge `b -> a`, `indegree[a]++`. indegree 0 wale queue me; nikaalte waqt `adj` ke har node ka indegree ghatao, 0 hua to push. `ans.size() == numCourses` = cycle nahi.

> [!WARNING]
> disha ulti mat karo: `u = it[1]` (pre), `v = it[0]` (course), edge pre -> course. cycle wale ka indegree KABHI 0 nahi -> queue me aate hi nahi -> size < n -> false.

**CODE**
```cpp
bool canFinish(int numCourses, vector<vector<int>> &prerequisites) {
    unordered_map<int, vector<int>> adj;
    vector<int> indegree(numCourses, 0);    // indegree = kitne prereq BACHE; 0 = ready
    for (auto &it : prerequisites) {
        int u = it[1];
        int v = it[0];                      // edge u -> v (pre -> course)
        adj[u].push_back(v);
        indegree[v]++;                      // ★ v ka ek prereq aur
    }
    queue<int> q;
    vector<int> ans;
    for (int i = 0; i < numCourses; i++) {  // har course check: kai ek saath start ho sakte
        if (indegree[i] == 0)
            q.push(i);                      // koi prereq nahi -> queue
    }
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        ans.push_back(node);
        for (auto &it : adj[node]) {
            indegree[it]--;                 // ek prereq pura
            if (indegree[it] == 0)
                q.push(it);                 // sab pure -> ready
        }
    }
    return ans.size() == numCourses;        // ★ cycle wale ka indegree KABHI 0 nahi -> queue me aate hi
}                                           //   nahi -> size < n -> false
```

---

### ▸ BUILD ORDER — ready me se SABSE CHHOTA pehle  = COURSE SCHEDULE (Kahn's) + queue ki jagah MIN-HEAP

**SAWAAL**
```
n module hain (0 se n-1). [a -> b] ka matlab: a banne ke baad hi b ban sakta hai. Ek baar me ek hi module banta hai.
Jo module ready hain (jinki saari dependency ban chuki), unme se sabse chhote number wala pehle banao. Poora kram lautao. Cycle ho to khaali list.
  e.g. n=4, [0->1]  ->  0 1 2 3
  kyun: shuru me ready = {0, 2, 3} (1 ko 0 chahiye) -> sabse chhota 0 -> ab 1 bhi ready = {1, 2, 3} -> 1 -> 2 -> 3
  (queue lagate to 0 2 3 1 aata: 1 baad me ready hua to line me peeche lagta)
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** PEHCHAAN: dependency -> TOPO. output line padho: "koi bhi valid order" -> queue · "sabse chhota /
> lexicographic" -> MIN-HEAP. topo ke KAI sahi order ([5->2,5->0,4->0,4->1,2->3,3->1]: "4 5 0 2 3 1"
> aur "5 4 2 0 3 1" dono sahi) -> khaas order = heap. queue = jo pehle ready · heap = ready me chhota
> BADLAAV bas itna: queue -> min-heap, front() -> top(). indegree / adj / ghatana / end-check SAME

> [!IMPORTANT]
> **CHAABI:** Course Schedule hi, bas `queue` -> `priority_queue<int, vector<int>, greater<int>>` (min-heap) aur `front()` -> `top()`: ready me se sabse chhota pehle. indegree / adj / end-check SAME.

> [!WARNING]
> `ans` KHAALI banao: `ans(n, 0)` + `push_back` = size 2n -> end check galat, hamesha `{}`. self-loop `[0->0]` pe indegree kabhi 0 nahi -> khaali.

**CODE**
```cpp
vector<int> buildOrder(int n, vector<vector<int>> &deps) {
    vector<int> indegree(n, 0);
    unordered_map<int, vector<int>> adj;
    for (auto &it : deps) {
        int u = it[0];
        int v = it[1];
        adj[u].push_back(v);
        indegree[v]++;
    }
    priority_queue<int, vector<int>, greater<int>> pq;   // ★ min-heap: sabse chhota top
    for (int i = 0; i < n; i++) {           // (id string ho -> heap <string>)
        if (indegree[i] == 0)
            pq.push(i);
    }
    vector<int> ans;                        // KHAALI se: ans(n,0) + push_back = size 2n, galat
    while (!pq.empty()) {
        int node = pq.top();                // n=4, [0->1]: heap {0,2,3} -> 0 -> 1 ready -> {1,2,3}
        pq.pop();                           //   = 0 1 2 3 (queue hota to 0 2 3 1 -> GALAT)
        ans.push_back(node);
        for (auto &it : adj[node]) {
            indegree[it]--;
            if (indegree[it] == 0)
                pq.push(it);
        }
    }
    if (ans.size() == n)
        return ans;
    return {};                              // loop / self-loop [0->0] -> indegree kabhi 0 nahi -> khaali
}
```

#### ── FAMILY: CLONE / deep-copy (BFS + map purana -> naya; map = cycle-rok + clone-dhundo) ──

### ▸ CLONE GRAPH (LC-133)  = BFS/DFS + map(old->new)

**SAWAAL**
```
Ek juda hua (connected), bina disha wala graph ka ek node diya hai. Poore graph ki nayi copy (deep copy) banao: naye node aur naye neighbours, aur copy ka wahi node lautao.
  e.g. adjList [[2,4],[1,3],[2,4],[1,3]] -> [[2,4],[1,3],[2,4],[1,3]] (sab naye node)
  kyun: square 1-2-3-4-1; har node ka naya clone, aur clone ke neighbours bhi clone hi, purane nahi
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** saare node ka clone map me (A->A', B->B'), phir clones ko CONNECT (A'--B'). 2 kaam:
> (a) clone BANANA (if ke ANDAR, sirf naye pe)  (b) JODNA push_back (if ke BAHAR, har edge pe)
> bina jodna -> loose clones -> adhoora graph

**SAAR**
```
VISUAL (A-B graph):
    original:  A --- B
    (1) clone map me daalo:  mp = { A->A' ,  B->B' }     // naye node, abhi LOOSE (jude nahi)
    (2) clones ko JODO:      A' --- B'                    // mp[A]->neighbors += mp[B]
    => naya deep-copy graph  A'-B'  taiyaar
```

> [!IMPORTANT]
> **CHAABI:** `map<purana, naya>`: (a) clone BANANA + `q.push` sirf naye pe, `if (mp.find(it) == mp.end())` ke ANDAR; (b) JODNA `mp[curr]->neighbors.push_back(mp[it])` `if` ke BAHAR, har edge pe.

> [!WARNING]
> seed `mp[node] = clone` BFS se pehle, warna cycle pe isi node ka doosra clone. check `mp.find` se karo, `mp[it]` likha to khaali entry ban jaati.

**CODE**
```cpp
Node *cloneGraph(Node *node) {
    if (!node)
        return nullptr;                     // khaali graph
    unordered_map<Node *, Node *> mp;       // purana (A) -> clone (A'); value hi clone
    queue<Node *> q;
    Node *clone = new Node();
    clone->val = node->val;
    mp[node] = clone;                       // ★ seed bhi map me: warna cycle pe isi node ka doosra clone
    q.push(node);
    while (!q.empty()) {
        Node *curr = q.front();
        q.pop();
        for (auto &it : curr->neighbors) {
            if (mp.find(it) == mp.end()) {  // (a) BANANA. find se check: mp[it] khaali entry bana deta
                Node *newClone = new Node();
                newClone->val = it->val;
                mp[it] = newClone;
                q.push(it);                 // push sirf naye pe: warna cycle me infinite loop
            }
            mp[curr]->neighbors.push_back(mp[it]);   // ★ (b) JODNA: A' ki list me B'
        }                                   // clone pehle se ho tab bhi edge jodni hai
    }
    return mp[node];                        // start node ka clone = naye graph ka shuru
}
// DFS: recursion + same map -> map me hai? return; nahi -> clone + map -> har nbr recurse + jodo
```


#### ── FAMILY: GRAPH-COLOURING / bipartite (2 group me baant sakte? odd-cycle? -> 2-COLOUR BFS) ──

### ▸ IS BIPARTITE (LC-785)  = 2-colour BFS + outer-loop (CC jaisa)

**SAWAAL**
```
Adjacency list wala graph diya hai. Kya saare nodes ko do group me aise baant sakte hain ki har edge ke dono sire alag group me hon? true ya false lautao.
  e.g. [[1,2,3],[0,2],[0,1,3],[0,2]] -> false
  kyun: 0-1-2 triangle: 0 red, 1 blue, 2 dono ka padosi, koi rang nahi bachta
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** har node ko rang 0/1; koi 2 JUDE node SAME rang na hon. odd cycle (triangle) -> false
> PEHCHAAN: "DO team me baanto + jo JUDE (dushman / edge) wo ek group me NAHI". output "team A / team B"
> = 2 rang. DSU ULTA hai: DSU me jude -> SAME group, yahan jude -> ALAG
> graph[i] = node i ke neighbours (adj-list KHUD di hai). [[1,3],[0,2],[1,3],[0,2]] -> true (square)
> [[1,2,3],[0,2],[0,1,3],[0,2]] -> false (0-1-2 triangle)

**SAAR**
```
INPUT: graph[i] = node i ke saare neighbours (adj-list KHUD di hai, banani nahi)
    [[1,3],[0,2],[1,3],[0,2]]  -> true        [[1,2,3],[0,2],[0,1,3],[0,2]] -> false
        0 --- 1                                    0 --- 1
        |     |      square = 4-cycle              | \   |      0-1-2 triangle = odd cycle
        3 --- 2      2 rang ho jaate               3 --- 2

DRY-RUN (triangle 0-1-2):
    0 ko 0 diya  -> nbr 1 = 1 · nbr 2 = 1
    curr 1 (rang 1) -> nbr 2 ka rang bhi 1 = SAME -> false        (odd cycle pakda gaya)
```

> [!IMPORTANT]
> **CHAABI:** har node rang 0/1, padosi ko `1 - color[node]`; padosi pehle se ranga + SAME rang = false. outer loop har node (CC jaisa), `color[i] == -1` pe hi BFS.

> [!WARNING]
> BFS shuru me `color[node] = 0` ZAROORI, warna `1 - (-1)` flip galat. outer me `-1` check PEHLE, rang wale pe dobara BFS nahi.

**CODE**
```cpp
bool BFS(vector<vector<int>> &graph, vector<int> &color, int node) {
    queue<int> q;
    q.push(node);
    color[node] = 0;                        // START ko rang ZAROORI, warna flip galat
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (auto &it : graph[node]) {
            if (color[it] == -1) {          // (a) uncolored -> ULTA rang + push
                color[it] = 1 - color[node];   // ★ 0 -> 1, 1 -> 0
                q.push(it);
            }
            else {
                if (color[it] == color[node])   // (b) pehle se rang + SAME = conflict
                    return false;
            }
        }
    }
    return true;                            // is component me conflict nahi
}

bool isBipartite(vector<vector<int>> &graph) {
    int m = graph.size();
    vector<int> color(m, -1);               // -1 = abhi rang nahi (0 aur 1 asli rang)
    for (int i = 0; i < m; i++) {           // ★ har node se: graph disconnected ho sakta
        if (color[i] == -1) {               // -1 check PEHLE: rang wale pe BFS dobara nahi
            if (BFS(graph, color, i) == false)
                return false;               // ek bhi component fail -> poora false
        }
    }
    return true;
}
```


#### ── FAMILY: WEIGHTED shortest-path (edges pe weight -> Dijkstra; kam edges != kam weight, BFS kaafi nahi) ──

### ▸ DIJKSTRA  = BFS template + 2 swap + relaxation

**SAWAAL**
```
n node, bina disha wale weight wale edges {u, v, w} aur ek source diya hai. Source se har node tak pahunchne ka sabse kam kul weight lautao.
  e.g. n=4, edges 0-1(4), 0-2(1), 1-2(2), 1-3(1), 2-3(5), src=0 -> [0,3,1,4]
  kyun: node 1: seedha 4 se sasta 0->2->1 = 3; node 3: 0->2->1->3 = 4 (0->2->3 = 6)
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** BFS ka WEIGHTED bhai: queue -> MIN-HEAP (chhoti dist pehle) · visited[] -> dist[] (INF, src=0) + RELAX
> visited kyun nahi: dist[] + STALE-skip = visited ka kaam; strict relax -> undirected me bhi loop nahi
> negative weight -> Dijkstra FAIL (dist ghatta rehta) -> Bellman-Ford. REAL: Maps / GPS, routing
> pq me {dist, node} PAIR: heap FIRST se sort -> HAMESHA {jispe-sort, saath-wala}

> [!WARNING]
> **★ EDGE:**
> {node, dist} ULTA likha to bhi test PASS ho jaate (relax baar-baar theek karta), par Dijkstra NAHI raha
> kyun {dist, node}: chhoti DIST wala node nikla = uski dist PAKKI (weight >= 0, aage sasta raasta aa hi nahi sakta)

### {node, dist} ULTA likha to (trace)

```
  upar wale e.g. pe:
    {node, dist} ->  0 nikla -> node1 (dist 4) pehle nikla, kyunki node NUMBER chhota
                     -> 4 pakka nahi (asli 3: 0->2->1), node 3 ko 5 bataya
                     -> node2 nikla, node1 = 3 -> node1 DOBARA khula, node 3 = 4
                     = har node kai baar khulta, bade graph pe bahut dheema
    {dist, node} ->  0 -> (1, node2) -> (3, node1) sirf EK baar, seedha sahi -> (4, node3) khatam
```

> [!IMPORTANT]
> **CHAABI:** BFS + 2 swap: `queue` -> min-heap `{dist, node}` (dist PEHLE), `visited` -> `dist[]` (INF, src 0) + RELAX `wt + it.second < dist[it.first]` -> update, phir push.

> [!WARNING]
> pair `{dist, node}` hi likho: `{node, dist}` pe test PASS par har node kai baar khulta. stale skip `wt > dist[node]` strictly `>` (`>=` se src `{0, src}` khud skip -> sab INF).

**CODE**
```cpp
vector<int> dijkstra(int n, vector<vector<int>> &edges, int src) {
    unordered_map<int, vector<pair<int, int>>> adj;
    for (auto &it : edges) {
        int u = it[0];
        int v = it[1];
        int w = it[2];
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});           // undirected DONO taraf
    }
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;   // {dist,node}
    vector<int> dist(n, 1e9);               // INF se: pehla rasta hamesha chhota lage
                                            // ★ sawaal weight 1e9 ke paas de -> tabhi long long + INF = LLONG_MAX
    dist[src] = 0;                          // src khud 0
    pq.push({0, src});
    while (!pq.empty()) {
        int wt = pq.top().first;            // node tak ki dist
        int node = pq.top().second;
        pq.pop();
        // ★ > strictly (>= nahi): taaza entry ka wt == dist[node] hota. >= = src {0,src} khud skip -> sab INF
        if (wt > dist[node])                // STALE skip (sirf > : barabar wala taaza entry)
            continue;
        for (auto &it : adj[node]) {        // it.first = nbr, it.second = EDGE weight (dist[node] NAHI)
            if (wt + it.second < dist[it.first]) {   // ★ RELAX: naya rasta chhota? (< strict: barabar pe
                dist[it.first] = wt + it.second;     //   dobara push bekaar, loop ka khatra)
                pq.push({wt + it.second, it.first}); // pehle dist update, phir push
            }
        }
    }
    return dist;
}
```


#### ── FAMILY: DSU / Union-Find (groups / same-set + edges ek-ek MERGE; BFS = pura traverse, DSU = incremental) ──

### ▸ DSU (Union-Find)  = parent[] + find + unite

**SAWAAL**
```
n node (0 se n-1) aur bina disha wale edges diye hain. Union-Find (DSU) se batao ki kitne alag group hain.
  e.g. n=5, edges=[[0,1],[1,2],[3,4]] -> 2
  kyun: unite 0-1, 1-2 -> ek leader; 3-4 -> doosra leader; parent[i]==i wale 2 bache
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** KAB: groups / connected / same-set? · UNDIRECTED cycle-detect · provinces · redundant-conn
> · MST-Kruskal
> REAL: dost-circle ("A-B same circle?"), accounts merge

> [!IMPORTANT]
> **CHAABI:** `parent[i] = i` se shuru; `find` = root tak jao + path compression (`parent[x] = find(parent[x])`); `unite` = dono ROOT jodo (rank se); group ginti = `find(i) == i`.

> [!WARNING]
> `unite` me `x`-`y` khud mat jodo, ROOT jodo (`find(x)`, `find(y)`), warna group toot jaata. same group? bhi `find(x) == find(y)`, `parent` compare nahi.

**CODE**
```cpp
class DSU {
public:
    vector<int> parent, rnk;                // rnk = tree ki approx height (union-by-rank isi se)
    DSU(int n) {
        parent.resize(n);
        rnk.assign(n, 0);
        for (int i = 0; i < n; i++)
            parent[i] = i;                  // shuru me har node khud apna leader
    }
    int find(int x) {
        if (x == parent[x])
            return x;                       // khud ka parent = ROOT, yahin ruko
        return parent[x] = find(parent[x]); // ★ PATH COMPRESSION: agli baar seedha root
    }
    void unite(int x, int y) {
        int u_x = find(x);                  // ★ ROOT jodo, x-y khud nahi: warna group toot jaata
        int u_y = find(y);
        if (u_x == u_y)
            return;                         // already same set
        if (rnk[u_x] < rnk[u_y])            // UNION BY RANK: chhota bade ke neeche
            parent[u_x] = u_y;
        else if (rnk[u_y] < rnk[u_x])
            parent[u_y] = u_x;
        else {
            parent[u_y] = u_x;
            rnk[u_x]++;                     // barabar rank: jodne se ek level badha
        }
    }
};
// 2 opt (path-compression + union-by-rank) -> find / unite ~O(1)
// same group? -> find(x) == find(y)   (root compare, parent nahi: parent beech ka ho sakta)

int countComponents(int n, vector<vector<int>> &edges) {
    DSU dsu(n);
    for (auto &e : edges)
        dsu.unite(e[0], e[1]);              // har edge -> do node merge
    int count = 0;
    for (int i = 0; i < n; i++)
        if (dsu.find(i) == i)
            count++;                        // sirf root ka parent khud wahi = ek group
    return count;
}
```

---

### ▸ WORD LADDER (LC-127)  = BFS shortest-path (unweighted)

**SAWAAL**
```
beginWord, endWord aur ek wordList di hai. Har kadam pe sirf ek akshar badal sakte ho, aur beech ka har word wordList me hona chahiye.
beginWord se endWord tak sabse chhote raste me kitne word aate hain (beginWord bhi gino), wo lautao. Na pahunch sako to 0.
  e.g. hit -> cog, list=[hot,dot,dog,lot,log,cog] -> 5
  kyun: hit -> hot -> dot -> dog -> cog = 5 word (begin aur end dono gine)
```

> [!IMPORTANT]
> **PEHCHAAN / SAAR:** begin -> end, har step EK letter badlo, beech ka har word dict me. "min steps" -> BFS level-by-level
> word = NODE · edge = ek letter farak + dict me. graph IMPLICIT (words), islands / Kahn's wali family
> hit -> hot -> dot / lot -> dog / log -> cog = 5 WORDS (begin = 1, steps nahi)
> level track 2 tareeke: steps PAIR me (ye code) · ya size-loop + level++ (poora layer khatam pe)

### EXAMPLE (hit -> cog, level-wise)

```
★★ EXAMPLE (yahi yaad -> poora recall flash):  begin=hit, end=cog, dict={hot,dot,dog,lot,log,cog}
    Level 1:  hit
    Level 2:  hot                    (hit->hot)
    Level 3:  dot, lot               (hot->dot, hot->lot)
    Level 4:  dog, log               (dot->dog, lot->log)
    Level 5:  cog == endWord -> 5    [hit hot dot dog cog = 5 WORDS]
```

### TEMPLATE: TERA (steps PAIR me) vs MERA (size-loop)

```
TEMPLATE:
   TERA (level PAIR me)                              │  MERA (per-layer SIZE-loop)
   ──────────────────────────────────────────────────┼──────────────────────────────────────
   st = {wordList}   (lookup + visited)              │  st = {wordList}
   q<pair<string,int>>;  push {beginWord,1}          │  q<string>;  push beginWord;  level=1  // + st.erase(beginWord): begin dobara na aaye
   while(!q):                                        │  while(!q):
     word = front.first; steps = front.second; pop   │    sz = q.size();  repeat sz baar:
     word==endWord -> return steps                   │      word = pop
     for pos i:  original = word[i]                  │      word==endWord -> return level
       for ch 'a'..'z':  word[i] = ch                │      for pos i:  original = word[i]
         word in st? erase(word) +                   │        for ch 'a'..'z':  word[i] = ch
                     push {word, steps+1}            │          word in st? erase(word) + push word
       word[i] = original   (RESTORE)                │        word[i] = original   (RESTORE)
   return 0                                          │    level++   (poora layer khatam)
                                                     │  return 0
   FARAK sirf level-track: TERA steps PAIR me (push me steps+1) · MERA size-loop + baad me level++.
```

> [!IMPORTANT]
> **★ VISITED trick (Arpan):** dict-set se word ERASE karo jaise hi use karo -> alag visited-set NAHI chahiye
> (ek check "dict me hai?" + "visited nahi?" dono serve).

> [!IMPORTANT]
> **CHAABI:** word = node; har position pe `'a'..'z'` badal ke dict me mila = padosi. BFS `{word, steps}`, begin = 1; dict-set se ERASE karte hi use karo = visited (alag set nahi).

> [!WARNING]
> `word[i] = original` RESTORE har position ke baad, warna word bigad jaata. `st.erase(beginWord)` bhi, begin dobara na aaye.

**CODE**
```cpp
int ladderLength(string beginWord, string endWord, vector<string> &wordList) {
    unordered_set<string> st(begin(wordList), end(wordList));   // dict = lookup + visited dono
    queue<pair<string, int>> q;
    q.push({beginWord, 1});                 // begin = 1 word
    st.erase(beginWord);                    // begin dobara na aaye
    while (!q.empty()) {
        string word = q.front().first;
        int steps = q.front().second;
        q.pop();
        if (word == endWord)
            return steps;
        for (int i = 0; i < word.size(); i++) {
            char original = word[i];
            for (char ch = 'a'; ch <= 'z'; ch++) {
                word[i] = ch;
                if (st.find(word) != st.end()) {
                    st.erase(word);         // ★ use karte hi ERASE = visited, alag visited-set nahi
                    q.push({word, steps + 1});
                }
            }
            word[i] = original;             // ★ RESTORE zaroori (warna word bigad jaata)
        }
    }
    return 0;                               // endWord dict me nahi -> 0
}
```


