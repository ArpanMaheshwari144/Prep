// ============================================================
// HOUSE ROBBER — REDO   [LC-198, DP #2]  (spaced redo, memory se)
// ============================================================
// nums[] = har ghar me paisa. 2 ADJACENT loot nahi sakte. MAX paisa?
//   [1,2,3,1] -> 4 · [2,7,9,3,1] -> 12 · [5] -> 5 · [2,1,1,2] -> 4
//   (solve(i) = max( nums[i]+solve(i-2), solve(i-1) ); base i<0 -> 0, i==0 -> nums[0]; memo)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(vector<int> &nums, int i, vector<int> &dp)
{
    if (i < 0)
    {
        return 0;
    }

    if (i == 0)
    {
        return nums[0];
    }

    if (dp[i] != -1)
    {
        return dp[i];
    }

    int notTake = 0 + solve(nums, i - 1, dp);
    int Take = nums[i] + solve(nums, i - 2, dp);

    return dp[i] = max(notTake, Take);
}

int rob(vector<int> &nums)
{
    int n = nums.size();
    vector<int> dp(n + 1, -1);
    return solve(nums, n - 1, dp);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, int exp, int t)
{
    int got = rob(in);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, 2, 3, 1}, 4, 1);
    check({2, 7, 9, 3, 1}, 12, 2);
    check({5}, 5, 3);
    check({2, 1, 1, 2}, 4, 4);
    return 0;
}
