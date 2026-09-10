// ============================================================
// HOUSE ROBBER II — (LeetCode 213, Medium)   [REDO_1 · blank retrieval, 1-day spaced]
// ============================================================
// House Robber jaisa: nums[] = har ghar me paisa, 2 ADJACENT ghar loot nahi sakte.
// FARAK SIRF EK: ghar CIRCLE me -> PEHLA ghar aur AAKHRI ghar bhi ADJACENT
// (nums[0] aur nums[n-1] dono ek saath loot nahi sakte).
// MAXIMUM kitna paisa loot sakta -> return.
//
//   [2,3,2]            -> 3     (ghar 0 & 2 circle me adjacent -> dono nahi; best = ghar1 = 3)
//   [1,2,3,1]          -> 4     (ghar 0 + ghar 2 = 1+3)
//   [1,2,3]            -> 3
//   [5]                -> 5
//   [200,3,140,20,10]  -> 340
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
// TODO (Arpan): blank se dobara solve. rob() fill kar (helper jitne chahiye bana le).
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

    int SKIP = solve(nums, i - 1, dp);
    int LOOT = nums[i] + solve(nums, i - 2, dp);
    return dp[i] = max(SKIP, LOOT);
}

int rob1(vector<int> &nums)
{
    int n = nums.size();
    vector<int> dp(n + 1, -1);
    return solve(nums, n - 1, dp);
}

int rob(vector<int> &nums)
{
    int n = nums.size();
    vector<int> temp1, temp2;

    if (n == 1)
        return nums[0];

    for (int i = 0; i < n; i++)
    {
        if (i != 0)
        {
            temp1.push_back(nums[i]);
        }
        if (i != n - 1)
        {
            temp2.push_back(nums[i]);
        }
    }

    int ans1 = rob1(temp1);
    int ans2 = rob1(temp2);
    return max(ans1, ans2);
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
    check({2, 3, 2}, 3, 1);
    check({1, 2, 3, 1}, 4, 2);
    check({1, 2, 3}, 3, 3);
    check({5}, 5, 4);
    check({200, 3, 140, 20, 10}, 340, 5);
    return 0;
}
