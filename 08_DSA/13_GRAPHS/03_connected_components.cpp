// ============================================================
// NUMBER OF CONNECTED COMPONENTS — (LeetCode 323, Medium)   [GRAPHS #3 · adj-list BFS + visited + count]
// ============================================================
// n nodes hain (0 se n-1 tak). edges list di hai (undirected — [a,b] matlab a aur b jude).
// Batao graph me kitne ALAG-ALAG GROUP (connected components) hain.
//
//   "connected component" = nodes ka ek group jahan har node kisi na kisi raste se dusre tak pahunch sakta.
//   alag group = jinke beech koi rasta nahi.
//
//        0 — 1        3 — 4              -> 2 components  { {0,1,2}, {3,4} }
//            |
//            2
//
//   Example:
//     n=5, edges=[[0,1],[1,2],[3,4]]        -> 2
//     n=5, edges=[[0,1],[1,2],[2,3],[3,4]]  -> 1   (sab ek chain me jude)
//     n=4, edges=[]                          -> 4   (koi edge nahi -> har node akela = 4 group)
//     n=1, edges=[]                          -> 1
// ============================================================
// ---- APPROACH ----  (BFS + visited + outer-loop count)  = Islands ka adj-list version
//  ★ idea: har UNVISITED node = ek naya group -> count++ + BFS se poora group mark. outer-loop = disconnected cover.
//  1. edges -> adjacency list (undirected -> adj[u]+=v, adj[v]+=u).
//  2. for i = 0..n-1 -> agar vis[i] NAHI:
//       a. count++  +  q.push(i) + mark
//       b. BFS: while queue -> pop node -> uske unvisited neighbours mark + push
//  3. return count.
//  ★ TRAP (jo tune pakda): count++ AUR BFS dono if(!vis[i]) ke ANDAR -- warna har node gin leta (= n).
//    isolated node (n=4 no-edge -> 4) sahi tabhi jab har unvisited node KHUD ek naya group gine. [3rd/4th case]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int countComponents(int n, vector<vector<int>> &edges)
{
    unordered_map<int, vector<int>> adj;
    for (auto &it : edges)
    {
        int u = it[0];
        int v = it[1];

        adj[u].push_back(v); // u ka neighbour v
        adj[v].push_back(u); // v ka neighbour u  (bi-direction)
    }

    queue<int> q;
    vector<bool> vis(n, false);
    int count = 0;
    for (int i = 0; i < n; i++) // har node scan (outer-loop -> disconnected groups bhi cover)
    {
        if (!vis[i]) // ★ NAYA (unvisited) node -> naya group. sab kuch iske ANDAR (warna har node gin jaata = n)
        {
            q.push(i);
            vis[i] = true;
            count++; // naya group gino
            while (!q.empty()) // BFS: is POORE group ko visit-mark kar do
            {
                int node = q.front();
                q.pop();
                for (auto &it : adj[node])
                {
                    if (!vis[it])
                    {
                        vis[it] = true; // push ke waqt mark
                        q.push(it);
                    }
                }
            }
        }
    }
    return count; // kitne naye-group mile = utne components
}

int main()
{
    vector<vector<int>> e1 = {{0, 1}, {1, 2}, {3, 4}};
    vector<vector<int>> e2 = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};
    vector<vector<int>> e3 = {};
    vector<vector<int>> e4 = {};

    cout << countComponents(5, e1) << " (expected 2)\n";
    cout << countComponents(5, e2) << " (expected 1)\n";
    cout << countComponents(4, e3) << " (expected 4)\n";
    cout << countComponents(1, e4) << " (expected 1)\n";
    return 0;
}
