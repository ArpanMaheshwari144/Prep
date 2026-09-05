// ============================================================
// PARTITION EQUAL SUBSET SUM   (LC-416)   -- REDO_1 (blank)
// ============================================================
// nums[] (positive ints). Kya array ko DO barabar-sum subset me
// baant sakte hain?  return true/false.
//
//   [1,5,11,5] -> true   (S=22 -> har hissa 11: {11}=11, {1,5,5}=11)
//
// ---- TEST (nums -> expected) ----
//   [1,5,11,5]      -> true
//   [1,2,3,5]       -> false     (S=11 odd)
//   [1,1]           -> true
//   [1,2,5]         -> false
//   [2,2,2,2]       -> true
//   [3,3,3,4,5]     -> true
//   [1]             -> false
//   [2,4,6,8]       -> true
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool solve(vector<int> &nums, int target, int index, vector<vector<int>> &dp)
{
    if (target == 0)
    {
        return true;
    }

    if (index < 0)
    {
        return false;
    }

    if (index == 0)
        return nums[0] == target;

    if (dp[index][target] != -1)
    {
        return dp[index][target];
    }

    bool notTake = solve(nums, target, index - 1, dp);
    bool take = false;
    if (nums[index] <= target)
    {
        take = solve(nums, target - nums[index], index - 1, dp);
    }
    return dp[index][target] = take || notTake;
}

bool subsetSum(vector<int> &nums, int target, int n, vector<vector<int>> &dp)
{
    return solve(nums, target, n - 1, dp);
}

bool canPartition(vector<int> &nums)
{
    int S = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        S += nums[i];
    }

    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>((S / 2) + 1, -1));

    if (S % 2 != 0)
    {
        return false;
    }
    return subsetSum(nums, S / 2, n, dp);
}

// ---- test helper (ise mat chhed) ----
void check(bool got, bool exp, const string &label)
{
    cout << label << " -> got " << (got ? "true" : "false")
         << " | exp " << (exp ? "true" : "false")
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {1, 5, 11, 5};
    check(canPartition(a1), true, "case1");
    vector<int> a2 = {1, 2, 3, 5};
    check(canPartition(a2), false, "case2");
    vector<int> a3 = {1, 1};
    check(canPartition(a3), true, "case3");
    vector<int> a4 = {1, 2, 5};
    check(canPartition(a4), false, "case4");
    vector<int> a5 = {2, 2, 2, 2};
    check(canPartition(a5), true, "case5");
    vector<int> a6 = {3, 3, 3, 4, 5};
    check(canPartition(a6), true, "case6");
    vector<int> a7 = {1};
    check(canPartition(a7), false, "case7");
    vector<int> a8 = {2, 4, 6, 8};
    check(canPartition(a8), true, "case8");
    return 0;
}
