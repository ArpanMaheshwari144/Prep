// ============================================================
// COIN CHANGE — (LeetCode 322, Medium)   [DP #3 · min-count + reuse]
// ============================================================
// coins[] (distinct denominations) + amount. amount banane ke liye MINIMUM kitne coins
// chahiye? har coin UNLIMITED baar use kar sakte. na ban sake -> -1.
//
//   coins=[1,2,5], amount=11  -> 3    (5+5+1)
//   coins=[2],     amount=3   -> -1   (nahi ban sakta)
//   coins=[1],     amount=0   -> 0    (0 amount -> 0 coins)
//   coins=[1,2,5], amount=7   -> 2    (5+2)
//
//   ★ SOCH: solve(amount) = har coin try karo -> 1 + solve(amount - coin) -> sabme se MIN.
//       for coin in coins:  best = min(best, 1 + solve(amount - coin));
//     reuse allowed -> amount ghatao par coin-list wahi (index nahi ghatata).
//   ★ base: amount==0 -> 0 (kuch nahi chahiye).  amount<0 -> INVALID (bahut bada, INT_MAX/skip).
//   ★ DP: solve(amount) overlap karta -> memo dp[amount]. na-ban-sake -> -1 handle.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(vector<int> &coins, int amount, int i, vector<int> &dp)
{
    if (amount < 0)
    {
        return INT_MAX;
    }

    if (amount == 0)
    {
        return 0;
    }

    if (dp[amount] != -1)
    {
        return dp[amount];
    }

    // int notTake = 0 + solve(coins, amount, i - 1, dp);

    // int take = INT_MAX;
    // if (coins[i] <= amount)
    // {
    //     if (solve(coins, amount - coins[i], i, dp) != INT_MAX)
    //     {
    //         take = 1 + solve(coins, amount - coins[i], i, dp);
    //     }
    // }

    int ans = INT_MAX;
    for (int i = 0; i < coins.size(); i++)
    {
        if (solve(coins, amount - coins[i], i, dp) != INT_MAX)
        {
            ans = min(ans, 1 + solve(coins, amount - coins[i], i, dp));
        }
    }

    return dp[amount] = ans;
}

int coinChange(vector<int> &coins, int amount)
{
    int n = coins.size();
    vector<int> dp(amount + 1, -1);
    return solve(coins, amount, n - 1, dp) == INT_MAX ? -1 : solve(coins, amount, n - 1, dp);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
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
