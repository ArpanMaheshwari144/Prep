// ============================================================
// NUMBER OF CONNECTED COMPONENTS — (LeetCode 323, Medium)   [GRAPHS #1 · BFS/DFS + visited]
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
// ---- APPROACH ----  (BFS + visited + OUTER-LOOP count)
//  mechanic: ek BFS = ek POORA group light-up. outer-loop har anchhue node pe naya group ginta.
//  1. edges -> ADJACENCY LIST (undirected -> adj[u]+=v AUR adj[v]+=u).
//  2. visited[] BAHAR + count = 0.
//  3. for i=0..n-1:  vis[i]? -> skip.   nahi? -> count++ + BFS(i) [poora group visit].
//  4. return count.
//  ★★ KEY TRAP: outer-loop kyun -> ek BFS sirf EK group cover karti; disconnected tak khud nahi jaati.
//  ★ visited BAHAR (reset NAHI) · push pe mark (dobara queue me na aaye).
// ------------------------------------------------------------
//   DRY-RUN  (n=5, edges=[[0,1],[1,2],[3,4]]):   adj: 0->[1] 1->[0,2] 2->[1] 3->[4] 4->[3]
//     i=0  NEW  -> count=1, BFS visits {0,1,2}
//     i=1,2     -> visited, skip
//     i=3  NEW  -> count=2, BFS visits {3,4}
//     i=4       -> visited, skip                    => return 2
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int countComponents(int n, vector<vector<int>> &edges)
{
    // STEP 1: edges -> ADJACENCY LIST (har graph problem ka same first-step)
    unordered_map<int, vector<int>> adj;
    for (auto &it : edges)
    {
        int u = it[0];
        int v = it[1];

        adj[u].push_back(v); // u ka neighbour v
        adj[v].push_back(u); // v ka neighbour u  (undirected -> dono taraf)
    }

    queue<int> q;
    vector<bool> vis(n, false); // ★ visited BAHAR — ek hi, saare BFS share (reset nahi)
    int count = 0;

    // STEP 2: OUTER LOOP — har node check (disconnected group chhut na jaaye)
    for (int i = 0; i < n; i++)
    {
        if (!vis[i]) // ye node kisi gine-hue group me nahi -> NAYA component
        {
            count++;       // naya group gino
            q.push(i);     // yahin se BFS -> is group ke sab nodes visit ho jaayenge
            vis[i] = true; // push ke waqt mark

            // ---- normal BFS (Tree ke level-order jaisa, bas visited add) ----
            while (!q.empty())
            {
                int node = q.front();
                q.pop();
                for (auto &nbr : adj[node])
                {
                    if (!vis[nbr])
                    {
                        vis[nbr] = true; // ★ push ke waqt mark -> dobara queue me na aaye
                        q.push(nbr);
                    }
                }
            }
            // BFS khatam = is poore group ke nodes visited -> outer loop aage badhega
        }
    }
    return count; // kitne naye-group-start mile = utne connected components
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
