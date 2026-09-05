// ============================================================
// FIND IF PATH EXISTS IN GRAPH — (LeetCode 1971, Easy)   [GRAPHS #1 · single BFS/DFS + visited]
// ============================================================
// n nodes (0..n-1), undirected edges di hain. source aur destination diye.
// Batao: source se destination tak KOI rasta hai?  (true / false)
//
//     0 — 1 — 2            0 — 1        3 — 5
//      \_____/                           \ /
//     (0-1, 1-2, 2-0)                     4
//     src=0, dst=2 -> true         (0-1 juda; 3-4-5 ALAG group)
//                                  src=0, dst=5 -> false
//
//   Example:
//     n=3, edges=[[0,1],[1,2],[2,0]], src=0, dst=2         -> true
//     n=6, edges=[[0,1],[0,2],[3,5],[5,4],[4,3]], src=0, dst=5 -> false
//     n=1, edges=[], src=0, dst=0                          -> true   (khud se khud = path)
//     n=4, edges=[[0,1],[2,3]], src=0, dst=3               -> false
// ============================================================
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool validPath(int n, vector<vector<int>> &edges, int source, int destination)
{
    // STEP 1: edges -> ADJACENCY LIST (undirected -> dono taraf)
    unordered_map<int, vector<int>> adj;
    for (auto &it : edges)
    {
        int u = it[0];
        int v = it[1];

        adj[u].push_back(v); // u ka neighbour v
        adj[v].push_back(u); // v ka neighbour u  (bi-direction)
    }

    // STEP 2: BFS source se -> reachable node visit hote jaayenge
    queue<int> q;
    vector<bool> vis(n, false);
    q.push(source);
    vis[source] = true; // push ke waqt mark
    while (!q.empty())
    {
        int node = q.front();
        q.pop();

        if (node == destination)
            return true;

        for (auto &it : adj[node]) // unvisited neighbours -> mark + push
        {
            if (!vis[it])
            {
                vis[it] = true;
                q.push(it);
            }
        }
    }
    return false; // queue khali, dst tak nahi pahunche -> alag group me
}

int main()
{
    vector<vector<int>> e1 = {{0, 1}, {1, 2}, {2, 0}};
    vector<vector<int>> e2 = {{0, 1}, {0, 2}, {3, 5}, {5, 4}, {4, 3}};
    vector<vector<int>> e3 = {};
    vector<vector<int>> e4 = {{0, 1}, {2, 3}};

    // ── VISUAL (har test ka graph; index=node, values=neighbours) ──
    //  e1:  0-1-2 aur 2-0  (triangle, sab jude)         src0->dst2 = TRUE

    cout << validPath(3, e1, 0, 2) << " (expected 1)\n";
    cout << validPath(6, e2, 0, 5) << " (expected 0)\n";
    cout << validPath(1, e3, 0, 0) << " (expected 1)\n";
    cout << validPath(4, e4, 0, 3) << " (expected 0)\n";
    return 0;
}
