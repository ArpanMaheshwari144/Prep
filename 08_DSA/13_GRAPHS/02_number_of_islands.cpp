// ============================================================
// NUMBER OF ISLANDS — (LeetCode 200, Medium)   [GRAPHS #2 · grid-as-graph, BFS/DFS + visited]
// ============================================================
// grid diya hai '1' (LAND / zameen) aur '0' (WATER / paani) ka.
// Batao kitne ISLAND hain. Island = '1' ka ek connected group (4-direction: up/down/left/right se juda; diagonal NAHI).
//
//        1 1 0            1 1 1
//        1 0 0            0 1 0
//        0 0 1            1 1 1
//     -> 2 islands     -> 1 island (center se sab jude)
//     ({(0,0),(0,1),(1,0)} + {(2,2)})
//
//   ★ SOCH: grid bhi ek GRAPH hai -- har cell = node; uske 4 padosi (up/down/left/right) = edges.
//           (adjacency-list banane ki zaroorat nahi -- neighbours = row/col +-1 se mil jaate.)
//
//   Example:
//     [[1,1,0],[1,0,0],[0,0,1]]   -> 2
//     [[1,1,1],[0,1,0],[1,1,1]]   -> 1
//     [[0,0],[0,0]]               -> 0
//     [[1]]                       -> 1
// ============================================================
// ---- APPROACH ----  (grid = GRAPH; DFS se island sink + count)
//  ★ INTUITION: har cell = node, 4 padosi (up/down/left/right) = edges. har naye land se DFS -> poora island dubo -> count++.
//  1. double-loop har cell (i,j) pe ghoomo
//  2. cell == '1' (naya land)? -> count++  +  DFS(i,j)      (DFS poora island sink karegi)
//  3. return count
//
//  DFS(i,j) -- ek island ko poora '0' kar deta:
//     base    : out-of-bounds YA cell == '0' (paani/visited) -> return
//     mark    : grid[i][j] = '0'                 (★ SINK = visited)
//     recurse : 4 direction -> (i+1,j) (i-1,j) (i,j+1) (i,j-1)
//  ★★ TRAP (jo tune pakda): DFS base me '0'-check ZAROORI -- warna paani pe nahi rukta, poora grid kha jaata.
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

int numIslands(vector<vector<char>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();
    int count = 0;
    for (int i = 0; i < m; i++) // har cell scan (outer double-loop)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == '1') // naya (abhi tak un-sunk) land -> naya island
            {
                DFS(grid, i, j, m, n); // poora island dubo do (sab connected '1' -> '0')
                count++;
            }
        }
    }
    return count;
}

int main()
{
    vector<vector<char>> g1 = {{'1', '1', '0'}, {'1', '0', '0'}, {'0', '0', '1'}};
    vector<vector<char>> g2 = {{'1', '1', '1'}, {'0', '1', '0'}, {'1', '1', '1'}};
    vector<vector<char>> g3 = {{'0', '0'}, {'0', '0'}};
    vector<vector<char>> g4 = {{'1'}};

    cout << numIslands(g1) << " (expected 2)\n";
    cout << numIslands(g2) << " (expected 1)\n";
    cout << numIslands(g3) << " (expected 0)\n";
    cout << numIslands(g4) << " (expected 1)\n";
    return 0;
}
