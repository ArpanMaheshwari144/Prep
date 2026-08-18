// ============================================================
// LONGEST INCREASING SUBSEQUENCE (LIS) — (LeetCode 300, Medium)   [DP #4 · take/not-take + prev]
// ============================================================
// nums[] diya. sabse LAMBI STRICTLY INCREASING subsequence ki LENGTH lautao.
// (subsequence = order same, beech ke skip allowed. increasing = har agla > pichla.)
//
//   [10,9,2,5,3,7,101,18]  -> 4    ([2,3,7,101] ya [2,5,7,101])
//   [0,1,0,3,2,3]          -> 4    ([0,1,2,3])
//   [7,7,7,7]              -> 1    (strictly increasing -> ek hi)
//   [1,2,3,4,5]            -> 5
//   [5,4,3,2,1]            -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (TAKE / NOT-TAKE + prev-tracking -- coin-change family, ek twist)
//  state = (index, prevIndex): "index pe hain, pichla-LIYA element ka index = prevIndex".
//  NOT-TAKE : solve(index+1, prevIndex)                       (nums[index] chhoda -> prev wahi)
//  TAKE     : agar increasing rahe (prevIndex==-1 YA nums[index] > nums[prevIndex])
//                 -> 1 + solve(index+1, INDEX)                (liya -> ab prev = index)
//  => max(take, notTake).   base: index==n -> 0.
//  BASE saaf (LIS easy kyu): sirf index==n -> 0. koi INT_MAX/overflow/-1 nahi (coin-change se ulta).
//  MEMO twist: prevIndex -1 se shuru -> dp[index][prevIndex + 1] (offset +1 se index 0..n banaye).
// ============================================================
int solve(int index, int prevIndex, vector<int> &nums, vector<vector<int>> &dp)
{
    if (index == nums.size())
    {
        return 0;
    }

    if (dp[index][prevIndex + 1] != -1)
    {
        return dp[index][prevIndex + 1];
    }

    int notTake = 0 + solve(index + 1, prevIndex, nums, dp);

    int Take = 0;
    if (prevIndex == -1 || nums[index] > nums[prevIndex])
    {
        Take = 1 + solve(index + 1, index, nums, dp);
    }

    return dp[index][prevIndex + 1] = max(Take, notTake);
}

int lengthOfLIS(vector<int> &nums)
{
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    return solve(0, -1, nums, dp);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, int exp, int t)
{
    int got = lengthOfLIS(in);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({10, 9, 2, 5, 3, 7, 101, 18}, 4, 1);
    check({0, 1, 0, 3, 2, 3}, 4, 2);
    check({7, 7, 7, 7}, 1, 3);
    check({1, 2, 3, 4, 5}, 5, 4);
    check({5, 4, 3, 2, 1}, 1, 5);
    return 0;
}
