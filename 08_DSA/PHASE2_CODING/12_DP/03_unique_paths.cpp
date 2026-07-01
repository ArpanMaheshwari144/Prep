// ============================================================
// UNIQUE PATHS   (2D DP — grid)
// ============================================================
// Robot m×n grid ke TOP-LEFT pe. sirf RIGHT ya DOWN move. BOTTOM-RIGHT tak
// kitne UNIQUE paths? return count.
//
// SOCH (seed):
//   kisi cell pe aane ke 2 raaste: UPAR se ya LEFT se.
//   dp[i][j] = dp[i-1][j] + dp[i][j-1]
//   BASE: pehli row + pehli column = sab 1.
//
// Tests (// expected):
//   m=3, n=2  -> 3
//   m=3, n=7  -> 28
//   m=1, n=1  -> 1
//   m=3, n=3  -> 6
//   m=2, n=2  -> 2
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int solve(int i, int j, int m, int n, vector<vector<int>> &dp)
{
    if (i >= m || j >= n)
    {
        return 0;
    }

    if (i == m - 1 && j == n - 1)
    {
        return 1;
    }

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int bottom = solve(i + 1, j, m, n, dp);
    int right = solve(i, j + 1, m, n, dp);
    return dp[i][j] = bottom + right;
}

int uniquePaths(int m, int n)
{
    vector<vector<int>> dp(m, vector<int>(n, -1));
    return solve(0, 0, m, n, dp);
}

int main()
{
    cout << uniquePaths(3, 2) << " (expected 3)\n";
    cout << uniquePaths(3, 7) << " (expected 28)\n";
    cout << uniquePaths(1, 1) << " (expected 1)\n";
    cout << uniquePaths(3, 3) << " (expected 6)\n";
    cout << uniquePaths(2, 2) << " (expected 2)\n";
    return 0;
}
