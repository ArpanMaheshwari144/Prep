// ============================================================
// HOUSE ROBBER — (LeetCode 198, Medium)   [DP #2 · take/skip choice]
// ============================================================
// nums[] = har ghar me kitna paisa. tu chor. par 2 ADJACENT ghar loot nahi sakte
// (alarm baj jaayega). MAXIMUM kitna paisa loot sakta -> return.
//
//   [1,2,3,1]     -> 4    (ghar 0 + ghar 2 = 1+3)
//   [2,7,9,3,1]   -> 12   (2 + 9 + 1)
//   [5]           -> 5
//   [2,1,1,2]     -> 4    (2 + 2)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
int solve(vector<int> &nums, int i, vector<int> &dp)
{
    if (i < 0)
        return 0;

    if (i == 0)
        return nums[0];

    if (dp[i] != -1)
    {
        return dp[i];
    }

    int LOOT = nums[i] + solve(nums, i - 2, dp);
    int SKIP = 0 + solve(nums, i - 1, dp);

    return dp[i] = max(LOOT, SKIP);
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
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, 2, 3, 1}, 4, 1);
    check({2, 7, 9, 3, 1}, 12, 2);
    check({5}, 5, 3);
    check({2, 1, 1, 2}, 4, 4);
    return 0;
}
