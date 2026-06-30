// ============================================================
// HOUSE ROBBER   —   COLD REDO ([C] -> rep 3, khud blank se)
// ============================================================
// Houses ki line, har house me paisa (nums[i]). ADJACENT 2 ghar nahi loot sakte.
// MAX total paisa return karo.
//
// SOCH (rep 3 — khud yaad kar, purana file mat dekh):
//   har ghar pe choice: LOOT karu (paisa[i] + i-2 tak ka best) YA SKIP karu (i-1 tak ka best).
//   dp[i] = max( LOOT , SKIP )
//
// Tests (// expected):
//   [1,2,3,1]     -> 4    (1 + 3)
//   [2,7,9,3,1]   -> 12   (2 + 9 + 1)
//   [5]           -> 5
//   [2,1,1,2]     -> 4    (2 + 2)
//   []            -> 0
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int solve(vector<int> &nums, int index, vector<int> &dp)
{
    if (index == 0)
    {
        return nums[0];
    }

    if (index == 1)
    {
        return max(nums[0], nums[1]);
    }

    if (dp[index] != -1)
    {
        return dp[index];
    }

    int loot = nums[index] + solve(nums, index - 2, dp);
    int noloot = 0 + solve(nums, index - 1, dp);

    return dp[index] = max(loot, noloot);
}

int rob(vector<int> &nums)
{
    if (nums.size() == 0)
    {
        return 0;
    }
    vector<int> dp(nums.size() + 1, -1);
    return solve(nums, nums.size() - 1, dp);
}

int main()
{
    vector<int> a1 = {1, 2, 3, 1};
    vector<int> a2 = {2, 7, 9, 3, 1};
    vector<int> a3 = {5};
    vector<int> a4 = {2, 1, 1, 2};
    vector<int> a5 = {};

    cout << rob(a1) << " (expected 4)\n";
    cout << rob(a2) << " (expected 12)\n";
    cout << rob(a3) << " (expected 5)\n";
    cout << rob(a4) << " (expected 4)\n";
    cout << rob(a5) << " (expected 0)\n";
    return 0;
}
