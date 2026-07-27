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
// ---- APPROACH ----  (tu likh — pehle copy-pen pe trace, phir code)
//
//
//
// ============================================================
// NOTE: ye STUB hai — baad me fresh solve karna (jab graph ki baari aaye).
//       purani solved-version git history me safe hai (recover ho sakti).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void DFS(vector<vector<char>> &grid, int i, int j, int m, int n)
{
    // ★ out-of-bounds YA paani/visited ('0') -> ruk jao (yehi '0'-check island ko contain karta)
    if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == '0')
    {
        return;
    }

    grid[i][j] = '0'; // ★ SINK -> is land ko visited mark (dobara na gino)
    DFS(grid, i + 1, j, m, n); // neeche
    DFS(grid, i - 1, j, m, n); // upar
    DFS(grid, i, j + 1, m, n); // right
    DFS(grid, i, j - 1, m, n); // left
}

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
