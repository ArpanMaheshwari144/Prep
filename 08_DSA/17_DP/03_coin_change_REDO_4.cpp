// ============================================================
// COIN CHANGE — (LeetCode 322, Medium)   [REDO_4 — khud bharo · base-case LOCK]
// ============================================================
// coins[] (distinct denominations) + amount. amount EXACTLY banane ke liye MINIMUM
// kitne coins? har coin UNLIMITED baar. na ban sake -> -1.
//
//   coins=[1,2,5], amount=11  -> 3    (5+5+1)
//   coins=[2],     amount=3   -> -1
//   coins=[1],     amount=0   -> 0
//   coins=[1,2,5], amount=7   -> 2    (5+2)
//   coins=[2,5,10,1], amount=27 -> 4
// ============================================================
// FORM: take / not-take per coin index (2D dp[i][amount]).
//   base 1: amount < 0 -> ??      (overshoot = invalid)
//   base 2: i < 0      -> ??      (koi coin nahi bacha; amount==0 -> ? warna ?)
//   [ye 2 base baar-baar phasaya -- yahi lock karna hai]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(vector<int> &coins, int amount, int i, vector<vector<int>> &dp)
{
    if (amount < 0)
    {
        return INT_MAX;
    }

    if (i < 0)
    {
        return amount == 0 ? 0 : INT_MAX;
    }

    if (dp[i][amount] != -1)
    {
        return dp[i][amount];
    }

    int notTake = 0 + solve(coins, amount, i - 1, dp);
    int Take = INT_MAX;
    if (solve(coins, amount - coins[i], i, dp) != INT_MAX)
    {
        Take = 1 + solve(coins, amount - coins[i], i, dp);
    }
    return dp[i][amount] = min(notTake, Take);
}

int coinChange(vector<int> &coins, int amount)
{
    int n = coins.size();
    vector<vector<int>> dp(n + 1, vector<int>(amount + 1, -1));
    int ans = solve(coins, amount, n - 1, dp);
    return ans == INT_MAX ? -1 : ans;
}

void check(vector<int> coins, int amount, int exp, int t)
{
    int got = coinChange(coins, amount);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, 2, 5}, 11, 3, 1);
    check({2}, 3, -1, 2);
    check({1}, 0, 0, 3);
    check({1, 2, 5}, 7, 2, 4);
    check({2, 5, 10, 1}, 27, 4, 5);
    return 0;
}
