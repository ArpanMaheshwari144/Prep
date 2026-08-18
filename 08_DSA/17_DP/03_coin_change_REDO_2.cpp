// ============================================================
// COIN CHANGE — REDO 2   [LC-322, DP #3]  (spaced redo, memory se)
// ============================================================
// coins[] + amount. MIN coins to make amount. har coin UNLIMITED. na ban sake -> -1.
//   [1,2,5],11 -> 3 · [2],3 -> -1 · [1],0 -> 0 · [1,2,5],7 -> 2 · [2,5,10,1],27 -> 4
//   (take/not-take YA for-loop. base amount==0->0, amount<0->INT_MAX. overflow guard. end INT_MAX->-1)
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

    int exclude = 0 + solve(coins, amount, i - 1, dp);
    int include = INT_MAX;
    if (solve(coins, amount - coins[i], i, dp) != INT_MAX)
    {
        include = 1 + solve(coins, amount - coins[i], i, dp);
    }

    return dp[i][amount] = min(include, exclude);
}

int coinChange(vector<int> &coins, int amount)
{
    int n = coins.size();
    vector<vector<int>> dp(n + 1, vector<int>(amount + 1, -1));
    int ans = solve(coins, amount, n - 1, dp);
    return ans == INT_MAX ? -1 : ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> coins, int amount, int exp, int t)
{
    int got = coinChange(coins, amount);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
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
