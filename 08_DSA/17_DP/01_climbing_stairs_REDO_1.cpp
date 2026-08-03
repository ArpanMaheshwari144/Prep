// ============================================================
// CLIMBING STAIRS — REDO   [LC-70, DP #1]  (spaced redo, memory se)
// ============================================================
// n steps. ek baar 1 YA 2 step. top tak kitne tareeke?
//   n=2 -> 2 · n=3 -> 3 · n=4 -> 5 · n=1 -> 1 · n=5 -> 8
//   (ways(n) = ways(n-1) + ways(n-2); base n==0||n==1 -> 1; memo)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(int n, vector<int> &dp)
{
    if (n == 0 || n == 1)
    {
        return 1;
    }

    if (dp[n] != -1)
    {
        return dp[n];
    }

    return dp[n] = solve(n - 1, dp) + solve(n - 2, dp);
}

int climbStairs(int n)
{
    vector<int> dp(n + 1, -1);
    return solve(n, dp);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(int n, int exp, int t)
{
    int got = climbStairs(n);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check(2, 2, 1);
    check(3, 3, 2);
    check(4, 5, 3);
    check(1, 1, 4);
    check(5, 8, 5);
    return 0;
}
