// ============================================================
// HOUSE ROBBER   —   1D-DP  [DP #2]
// ============================================================
// Ghar ek line mein, har ghar mein paisа. RULE: do ADJACENT ghar nahi loot sakte (alarm).
// Max paisа kitna loot sakte?
//
//   [2,7,9,3,1] -> 12   (ghar 0+2+4 = 2+9+1)
//   [1,2,3,1]   -> 4    (ghar 0+2 = 1+3)
//   [5]         -> 5
//   [2,1,1,2]   -> 4    (ghar 0+3 = 2+2)
//
// SIGNAL: "max, par adjacent nahi le sakte" -> 1D-DP (take/skip choice).
//   RECURRENCE (jo samjha): har ghar i pe 2 choice ->
//     LOOT i -> money[i] + dp[i-2]    (i-1 nahi -> i-2 se)
//     SKIP i -> dp[i-1]
//     dp[i] = max( money[i] + dp[i-2],  dp[i-1] )
//   BASE: dp[0]=money[0], dp[1]=max(money[0],money[1])
//   No code -- tu likh (iterative dp array, ya do-pichhle-value, ya memoize).
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ---- TERA kaam: yeh function bhar (1D-DP) ----
int solve(vector<int> nums, int i, vector<int> &dp)
{
    if (i == 0)
    {
        return nums[0];
    }

    if (i == 1)
    {
        return max(nums[0], nums[i]);
    }

    if (dp[i] != -1)
    {
        return dp[i];
    }

    int loot = nums[i] + solve(nums, i - 2, dp);
    int noloot = solve(nums, i - 1, dp);

    return dp[i] = max(loot, noloot);
}
int rob(vector<int> nums)
{
    if (nums.empty())
        return 0;
    vector<int> dp(nums.size() + 1, -1);
    return solve(nums, nums.size() - 1, dp);
}

int main()
{
    cout << rob({2, 7, 9, 3, 1}) << endl; // expected 12
    cout << rob({1, 2, 3, 1}) << endl;    // expected 4
    cout << rob({5}) << endl;             // expected 5
    cout << rob({2, 1, 1, 2}) << endl;    // expected 4
    cout << rob({2, 7}) << endl;          // expected 7
    cout << rob({}) << endl;              // expected 0 (koi ghar nahi)
    return 0;
}
