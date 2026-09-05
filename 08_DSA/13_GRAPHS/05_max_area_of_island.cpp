// ============================================================
// MAX AREA OF ISLAND — (LeetCode 695, Medium)   [GRAPHS #5 · grid DFS return-value]
// ============================================================
// grid me 0 (paani) / 1 (land). island = 4-direction connected 1's. (islands jaisa hi.)
// Batao SABSE BADE island ka AREA (kitne cells). koi island na ho -> 0.
//
//        1 1 0            islands: {(0,0),(0,1),(1,0)} = 3 cells,  {(2,2)} = 1 cell
//        1 0 0        ->  max area = 3
//        0 0 1
//
//   ISLANDS (LC-200) jaisa -- bas farak: count nahi, har island ka AREA nikaalo -> max track.
//     DFS ab VOID nahi -> AREA return kare (1 + 4-dir DFS ke returns).  [= tree recursion jo VALUE return karti]
//
//   Example:
//     [[1,1,0],[1,0,0],[0,0,1]]  -> 3
//     [[1,1],[1,1]]              -> 4
//     [[0,0],[0,0]]              -> 0
//     [[1]]                      -> 1
// ============================================================
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int DFS(vector<vector<int>> &grid, int i, int j, int m, int n)
{
    // out-of-bounds YA paani/visited ('0') -> ruk jao (yehi '0'-check island ko contain karta)
    if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0)
    {
        return 0;
    }

    grid[i][j] = 0; // SINK (visited) -- islands jaisa
    int neeche = DFS(grid, i + 1, j, m, n); // neeche wale hisse ka area
    int upar = DFS(grid, i - 1, j, m, n);   // upar
    int right = DFS(grid, i, j + 1, m, n);  // right
    int left = DFS(grid, i, j - 1, m, n);   // left

    return 1 + left + right + upar + neeche; // khud(1) + chaaron taraf ka area = is island ka total
}

int maxAreaOfIsland(vector<vector<int>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();
    int ans = 0; // area hamesha >=0 -> 0 se start (koi island nahi -> 0 hi rahega)
    for (int i = 0; i < m; i++) // har cell scan
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1) // naya land -> is island ka area nikaalo -> MAX rakho
            {
                ans = max(ans, DFS(grid, i, j, m, n));
            }
        }
    }
    return ans; // ans 0 se start tha -> koi island na mila -> 0 (special-check ki zaroorat nahi)
}

int main()
{
    vector<vector<int>> g1 = {{1, 1, 0}, {1, 0, 0}, {0, 0, 1}};
    vector<vector<int>> g2 = {{1, 1}, {1, 1}};
    vector<vector<int>> g3 = {{0, 0}, {0, 0}};
    vector<vector<int>> g4 = {{1}};

    // ── VISUAL (g1; 1=land; area = ek island ke cells ki count) ──
    //   1 1 0
    //   1 0 0     -> island1 area=3 (top-left L), island2 area=1 -> MAX 3
    //   0 0 1

    cout << maxAreaOfIsland(g1) << " (expected 3)\n";
    cout << maxAreaOfIsland(g2) << " (expected 4)\n";
    cout << maxAreaOfIsland(g3) << " (expected 0)\n";
    cout << maxAreaOfIsland(g4) << " (expected 1)\n";
    return 0;
}
