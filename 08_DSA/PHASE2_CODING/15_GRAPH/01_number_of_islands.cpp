// ============================================================
// NUMBER OF ISLANDS   (Medium | Grid DFS/BFS — flood fill)
// ============================================================
// grid: '1'=land, '0'=water. Connected '1' (up/down/left/right) = ek island. KITNE islands?
//
//   1 1 0 0 0
//   1 1 0 0 0      -> 3 islands
//   0 0 1 0 0
//   0 0 0 1 1
//
//   all '0' -> 0   |   sab '1' connected -> 1
//
// APPROACH (jo samjha): scan grid -> unvisited '1' mile -> count++ -> DFS/BFS se saare connected '1' SINK ('0').
//   DFS: bounds-check + '1' hi expand + 4 direction.
// (tu code kar. fasa to "madad".)
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

// ---- TERA kaam: bhar (numIslands + DFS helper) ----
void dfs(vector<vector<char> > &grid, int i, int j)
{
    int row = grid.size();
    int col = grid[0].size();
    if (i < 0 || i >= row || j < 0 || j >= col || grid[i][j] == '0')
    {
        return;
    }

    grid[i][j] = '0';
    dfs(grid, i + 1, j); // top
    dfs(grid, i, j - 1); // left
    dfs(grid, i - 1, j); // bottom
    dfs(grid, i, j + 1); // right
}

int numIslands(vector<vector<char> > grid)
{
    int row = grid.size();
    int col = grid[0].size();
    int count = 0;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (grid[i][j] == '1')
            {
                count++;
                dfs(grid, i, j);
            }
        }
    }
    return count;
}

int main()
{
    vector<vector<char> > g1 = {
        {'1', '1', '0', '0', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '1', '0', '0'},
        {'0', '0', '0', '1', '1'}};
    cout << numIslands(g1) << endl; // expected 3

    vector<vector<char> > g2 = {
        {'1', '1', '1'},
        {'0', '1', '0'},
        {'1', '1', '1'}};
    cout << numIslands(g2) << endl; // expected 1

    vector<vector<char> > g3 = {
        {'0', '0'},
        {'0', '0'}};
    cout << numIslands(g3) << endl; // expected 0

    vector<vector<char> > g4 = {
        {'1', '0', '1'},
        {'0', '0', '0'},
        {'1', '0', '1'}};
    cout << numIslands(g4) << endl; // expected 4
    return 0;
}
