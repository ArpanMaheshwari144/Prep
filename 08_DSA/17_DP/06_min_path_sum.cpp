// ============================================================
// MINIMUM PATH SUM — (LeetCode 64, Medium)   [DP #6 · 2D grid, min-cost]
// ============================================================
// m x n grid, har cell me ek COST. (0,0) se (m-1,n-1) tak jaao, sirf RIGHT/DOWN.
// jis path ka SUM sabse KAM ho, wo sum lautao.
//
//   [[1,3,1],[1,5,1],[4,2,1]]  -> 7    (1->3->1->1->1)
//   [[1,2,3],[4,5,6]]          -> 12   (1->2->3->6)
//   [[5]]                      -> 5
//   [[1,2],[1,1]]              -> 3    (1->1->1)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
int solve(int i, int j, vector<vector<int>> &dp, vector<vector<int>> &grid)
{
    if (i < 0 || j < 0)
    {
        return INT_MAX;
    }

    if (i == 0 && j == 0)
        return grid[0][0];

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int up = solve(i - 1, j, dp, grid);
    int left = solve(i, j - 1, dp, grid);

    return dp[i][j] = grid[i][j] + min(up, left);
}

int uniquePaths(vector<vector<int>> &grid, int m, int n)
{
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return solve(m - 1, n - 1, dp, grid);
}

int minPathSum(vector<vector<int>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();
    return uniquePaths(grid, m, n);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<vector<int>> g, int exp, int t)
{
    int got = minPathSum(g);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({{1, 3, 1}, {1, 5, 1}, {4, 2, 1}}, 7, 1);
    check({{1, 2, 3}, {4, 5, 6}}, 12, 2);
    check({{5}}, 5, 3);
    check({{1, 2}, {1, 1}}, 3, 4);
    return 0;
}
