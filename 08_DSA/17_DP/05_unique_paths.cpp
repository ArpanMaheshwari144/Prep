// ============================================================
// UNIQUE PATHS — (LeetCode 62, Medium)   [DP #5 · 2D DP (grid) intro]
// ============================================================
// m x n grid. robot TOP-LEFT (0,0) pe. sirf RIGHT ya DOWN chal sakta.
// BOTTOM-RIGHT (m-1, n-1) tak pahunchne ke kitne ALAG raaste? (count paths)
//
//   m=3, n=7  -> 28
//   m=3, n=2  -> 3
//   m=1, n=1  -> 1
//   m=3, n=3  -> 6
//   m=1, n=10 -> 1   (ek hi raasta -- seedha right)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (2D DP intro = climbing-stairs ka 2D cousin)
//  cell (i,j) pe pahunchne ke raaste = UPAR se (i-1,j) + LEFT se (i,j-1)
//     => solve(i,j) = solve(i-1,j) + solve(i,j-1).
//  base: (0,0) -> 1 (start) · i<0 || j<0 -> 0 (grid ke bahar, koi raasta nahi).
//  ★ climbing-stairs se connect: 1D dp[n] -> 2D dp[i][j]; ways(n-1)+ways(n-2) -> ways(i-1,j)+ways(i,j-1).
//  ★ memo ab 2D: dp[i][j].  caller: solve(m-1, n-1) (bottom-right se).
// ============================================================
int solve(int i, int j, vector<vector<int>> &dp)
{
    if (i < 0 || j < 0)
    {
        return 0;
    }

    if (i == 0 && j == 0)
        return 1;

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int up = solve(i - 1, j, dp);
    int left = solve(i, j - 1, dp);

    return dp[i][j] = up + left;
}

int uniquePaths(int m, int n)
{
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return solve(m - 1, n - 1, dp);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(int m, int n, int exp, int t)
{
    int got = uniquePaths(m, n);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check(3, 7, 28, 1);
    check(3, 2, 3, 2);
    check(1, 1, 1, 3);
    check(3, 3, 6, 4);
    check(1, 10, 1, 5);
    return 0;
}
