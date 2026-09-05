// ============================================================
// EDIT DISTANCE — (LeetCode 72, Hard)   [DP #8 · 2D on 2 strings, LCS cousin]
// ============================================================
// word1 ko word2 me badalne ke MINIMUM operations. 3 op allowed:
//   INSERT (char daalo) · DELETE (char hatao) · REPLACE (char badlo). har op = 1 cost.
//
//   "horse", "ros"          -> 3
//   "intention", "execution"-> 5
//   "", ""                  -> 0
//   "abc", "abc"            -> 0
//   "a", "ab"               -> 1    (ek insert)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
int solve(int i, int j, string s1, string s2, vector<vector<int>> &dp)
{
    if (i < 0)
    {
        return j + 1;
    }

    if (j < 0)
    {
        return i + 1;
    }

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    if (s1[i] == s2[j])
    {
        return dp[i][j] = 0 + solve(i - 1, j - 1, s1, s2, dp);
    }

    return dp[i][j] = 1 + min({solve(i - 1, j, s1, s2, dp), // delete one char
                            solve(i, j - 1, s1, s2, dp),  // insert
                            solve(i - 1, j - 1, s1, s2, dp)}); // replace
}

int longestCommonSubsequence(string s1, string s2)
{
    int m = s1.size();
    int n = s2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
    return solve(m - 1, n - 1, s1, s2, dp);
}

int minDistance(string w1, string w2)
{

    return longestCommonSubsequence(w1, w2);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(string a, string b, int exp, int t)
{
    int got = minDistance(a, b);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check("horse", "ros", 3, 1);
    check("intention", "execution", 5, 2);
    check("", "", 0, 3);
    check("abc", "abc", 0, 4);
    check("a", "ab", 1, 5);
    return 0;
}
