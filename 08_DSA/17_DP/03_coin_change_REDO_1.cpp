// ============================================================
// COIN CHANGE — REDO (FOR-LOOP form)   [LC-322, DP #3]
// ============================================================
// coins[] (distinct) + amount. MINIMUM coins to make amount. har coin UNLIMITED. na ban sake -> -1.
//
//   [1,2,5], 11  -> 3   ·   [2], 3 -> -1   ·   [1], 0 -> 0   ·   [1,2,5], 7 -> 2   ·   [2,5,10,1], 27 -> 4
//
//   ★ FOR-LOOP form: solve(amt) = har coin loop me try -> 1 + solve(amt - coin) -> sabme se MIN.
//     yahan 'i' index ki zaroorat nahi -> 1D dp[amount] kaafi.
//   ★ base: amt==0 -> 0 · amt<0 -> INVALID (INT_MAX).
//   ★ overflow: 1+solve(...) sirf jab solve != INT_MAX. end: answer INT_MAX -> -1.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(vector<int> &coins, int amount, int i, vector<int> &dp)
{
    if (amount < 0)
        return INT_MAX;

    if (amount == 0)
    {
        return 0;
    }

    if (dp[amount] != -1)
    {
        return dp[amount];
    }

    int Take = INT_MAX;
    for (int i = 0; i < coins.size(); i++)
    {
        if (solve(coins, amount - coins[i], i, dp) != INT_MAX)
        {
            Take = min(Take, 1 + solve(coins, amount - coins[i], i, dp));
        }
    }

    return dp[amount] = Take;
}

int coinChange(vector<int> &coins, int amount)
{
    int n = coins.size();
    vector<int> dp(amount + 1, -1);
    return (solve(coins, amount, n - 1, dp) == INT_MAX ? -1 : solve(coins, amount, n - 1, dp));
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
