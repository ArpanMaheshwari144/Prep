// ============================================================
// LONGEST COMMON SUBSEQUENCE (LCS) — (LeetCode 1143, Medium)   [DP #7 · 2D on 2 strings]
// ============================================================
// do string s1, s2. dono me COMMON subsequence (order same, beech skip allowed) ki sabse
// lambi LENGTH lautao.
//
//   "abcde", "ace"      -> 3    ("ace")
//   "abc", "abc"        -> 3
//   "abc", "def"        -> 0    (kuch common nahi)
//   "bl", "yby"         -> 1    ("b")
//   "ezupkr", "ubmrapg" -> 2    ("ur")
//
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
int solve(int i, int j, string s1, string s2, vector<vector<int>> &dp)
{
    if (i < 0 || j < 0)
    {
        return 0;
    }

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    if (s1[i] == s2[j])
    {
        return dp[i][j] = 1 + solve(i - 1, j - 1, s1, s2, dp);
    }

    return dp[i][j] = 0 + max(solve(i - 1, j, s1, s2, dp), solve(i, j - 1, s1, s2, dp));
}

int longestCommonSubsequence(string s1, string s2)
{
    int m = s1.size();
    int n = s2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return solve(m - 1, n - 1, s1, s2, dp);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(string a, string b, int exp, int t)
{
    int got = longestCommonSubsequence(a, b);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check("abcde", "ace", 3, 1);
    check("abc", "abc", 3, 2);
    check("abc", "def", 0, 3);
    check("bl", "yby", 1, 4);
    check("ezupkr", "ubmrapg", 2, 5);
    return 0;
}
