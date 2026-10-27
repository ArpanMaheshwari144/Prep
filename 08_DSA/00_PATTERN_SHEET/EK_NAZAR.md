# EK NAZAR — OA / interview se pehli raat

> Sirf yaad dilane ke liye: har pattern ka KAB + CODE ka dhaancha + DHYAAN.
> Poora code (SAWAAL + TAREEKA + 174 problem) = [PATTERN_SHEET.md](PATTERN_SHEET.md). Roz ka redo wahi se.

---

## 01 TWO POINTER

**KAB:** array SORTED hai aur do number ki jodi chahiye · ya array me IN-PLACE kuch hatana / aage karna hai

**CODE (dono sire se):**
```cpp
int l = 0, r = n - 1;
while (l < r) {
    int sum = nums[l] + nums[r];
    if (sum == target) return {l, r};
    else if (sum < target) l++;      // chhota -> left aage
    else r--;                        // bada -> right peeche
}
```

**CODE (slow / fast, in-place):**
```cpp
int slow = 0;
for (int fast = 0; fast < n; fast++) {
    if (nums[fast] != 0) {           // jo RAKHNA hai
        swap(nums[slow], nums[fast]);
        slow++;
    }
}
```

**DHYAAN:**
- 3-sum me duplicate SKIP karo (2-sum me nahi)
- container / trapping / histogram me confuse -> sheet ka NAAM-SE-CONFUSE A

**POORA:** [PATTERN 1](PATTERN_SHEET.md#pattern-1--two-pointer)

---

## 13 GRAPHS

**KAB:** edges / adjacency list / grid diya · "kitne group?" · "A se B pahunch sakte?" · "kam se kam kitne step?"

**CODE (graph banao):**
```cpp
unordered_map<int, vector<int>> adj;
for (auto &e : edges) {
    adj[e[0]].push_back(e[1]);
    adj[e[1]].push_back(e[0]);       // undirected = DONO taraf (directed me sirf ek)
}
// GRID ho to adj nahi: dirs = {{-1,0},{1,0},{0,-1},{0,1}}, cell khud node
```

**CODE (BFS):**
```cpp
vector<bool> vis(n, false);
queue<int> q;
vis[src] = true;
q.push(src);
while (!q.empty()) {
    int node = q.front();
    q.pop();
    for (int nbr : adj[node]) {
        if (!vis[nbr]) {
            vis[nbr] = true;         // PUSH karte waqt mark
            q.push(nbr);
        }
    }
}
```

**CODE (kitne group):**
```cpp
int count = 0;
for (int i = 0; i < n; i++) {
    if (!vis[i]) {
        count++;                     // naya group
        BFS(i);
    }
}
```

**DHYAAN:**
- visited PUSH karte waqt mark karo, nikaalte waqt nahi (warna ek node kai baar queue me)
- `auto &it = q.front(); q.pop();` = toota reference -> pehle padho, phir pop
- BFS = level / kam se kam step · DFS = recursion, code chhota
- dependency / kram -> topo (indegree + queue) · weight wale raaste -> Dijkstra (min-heap + dist[])

**POORA:** [PATTERN 13](PATTERN_SHEET.md#pattern-13--graphs-bfsdfs--visited)
