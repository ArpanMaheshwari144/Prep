// ============================================================
// MIN PATH SUM   (2D DP — grid, min cost path)
// ============================================================
// m×n grid, har cell = cost. top-left -> bottom-right, sirf RIGHT/DOWN.
// MINIMUM total cost wala path ka sum return karo.
//
// SOCH (seed — unique-paths template):
//   cell pe 2 raaste: upar se ya left se -> jo SASTA wahi lo.
//   dp[i][j] = grid[i][j] + min(dp[i-1][j], dp[i][j-1])
//   BASE: pehli row/col = cumulative (ek hi raasta).
//
// Tests (// expected):
//   [[1,3,1],[1,5,1],[4,2,1]] -> 7
//   [[1,2,3],[4,5,6]]         -> 12   (1+2+3+6)
//   [[5]]                     -> 5
//   [[1,2],[1,1]]             -> 3    (1+1+1)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int solve(vector<vector<int>> &grid, int i, int j, int m, int n, vector<vector<int>> &dp)
{
    if (i >= m || j >= n)
    {
        return INT_MAX;
    }

    if (i == m - 1 && j == n - 1)
    {
        return grid[i][j];
    }

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int bottom = solve(grid, i + 1, j, m, n, dp);
    int right = solve(grid, i, j + 1, m, n, dp);
    return dp[i][j] = grid[i][j] + min(bottom, right);
}

int minPathSum(vector<vector<int>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();
    vector<vector<int>> dp(m, vector<int>(n, -1));
    return solve(grid, 0, 0, m, n, dp);
}

int main()
{
    vector<vector<int>> g1 = {{1, 3, 1}, {1, 5, 1}, {4, 2, 1}};
    vector<vector<int>> g2 = {{1, 2, 3}, {4, 5, 6}};
    vector<vector<int>> g3 = {{5}};
    vector<vector<int>> g4 = {{1, 2}, {1, 1}};

    cout << minPathSum(g1) << " (expected 7)\n";
    cout << minPathSum(g2) << " (expected 12)\n";
    cout << minPathSum(g3) << " (expected 5)\n";
    cout << minPathSum(g4) << " (expected 3)\n";
    return 0;
}
