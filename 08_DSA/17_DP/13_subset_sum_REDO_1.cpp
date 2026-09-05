// ============================================================
// SUBSET SUM (boolean)   (knapsack-family)   -- REDO_1 (blank)
// ============================================================
// nums[] (positive ints) + target. Kya koi SUBSET exactly
// `target` tak jode?  return true/false.
//
//   nums=[2,3,5], target=5  -> true   ({5} ya {2,3})
//
// ---- TEST (nums, target -> expected) ----
//   [2,3,5],     T=5   -> true
//   [1,2,7],     T=6   -> false
//   [1,5,11,5],  T=11  -> true
//   [3,34,4,12,5,2], T=9 -> true
//   [1,2,3],     T=0   -> true      (khaali subset = 0)
//   [2,4,6],     T=5   -> false
//   [7],         T=7   -> true
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

bool subsetSum(vector<int> &nums, int target)
{
    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>(target + 1, -1));
    return solve(nums, target, n - 1, dp);
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
    vector<int> a1 = {2, 3, 5};
    check(subsetSum(a1, 5), true, "case1");
    vector<int> a2 = {1, 2, 7};
    check(subsetSum(a2, 6), false, "case2");
    vector<int> a3 = {1, 5, 11, 5};
    check(subsetSum(a3, 11), true, "case3");
    vector<int> a4 = {3, 34, 4, 12, 5, 2};
    check(subsetSum(a4, 9), true, "case4");
    vector<int> a5 = {1, 2, 3};
    check(subsetSum(a5, 0), true, "case5");
    vector<int> a6 = {2, 4, 6};
    check(subsetSum(a6, 5), false, "case6");
    vector<int> a7 = {7};
    check(subsetSum(a7, 7), true, "case7");
    return 0;
}
