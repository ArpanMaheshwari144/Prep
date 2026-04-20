# Template: Graphs

## DFS — Graph
```
void dfs(int node, adj, visited, result):
    visited[node] = true
    result.push_back(node)
    for(auto& neighbor : adj[node]):
        if(!visited[neighbor]):
            dfs(neighbor, adj, visited, result)
```

## BFS — Graph
```
queue mein start daal. visited[start] = true.
while queue khaali nahi:
    node = front. pop.
    result.push_back(node)
    for(auto& neighbor : adj[node]):
        if(!visited[neighbor]):
            visited[neighbor] = true
            queue.push(neighbor)
```

## DFS — Grid (Number of Islands)
```
dirs = {{1,0}, {0,1}, {-1,0}, {0,-1}}

void dfs(grid, vis, i, j, m, n):
    vis[i][j] = 1
    for(auto& dir : dirs):
        i_ = i + dir[0]
        j_ = j + dir[1]
        if(canVis(i_, j_, grid, vis, m, n)):
            dfs(grid, vis, i_, j_, m, n)

bool canVis(i, j, grid, vis, m, n):
    return i>=0 && j>=0 && i<m && j<n && grid[i][j]=='1' && !vis[i][j]
```

**IMPORTANT:** Graph mein HAMESHA visited track karo. Bina visited → infinite loop (cycle).

## Dijkstra — Shortest Path
```
dist(n, INT_MAX). dist[src] = 0.
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq.
pq.push({0, src}).

while(!pq.empty()):
    auto [d, node] = pq.top(). pq.pop().   ← COPY, & mat lagao!
    if(d > dist[node]) continue.
    for(auto& [neighbor, weight] : adj[node]):
        if(d + weight < dist[neighbor]):
            dist[neighbor] = d + weight
            pq.push({d + weight, neighbor})
return dist.
```

**IMPORTANT:** `auto [d,node]` — COPY karo, reference (&) nahi. Pop ke baad reference invalid.

## Topological Sort — Kahn's BFS
```
indegree nikaal: for(i) for(neighbor : adj[i]) indegree[neighbor]++
indegree 0 wale queue mein: q.push(i) — INDEX push, value nahi!
Queue se nikaal, result daal, neighbors ka indegree--
Indegree 0 ho → queue push
```
