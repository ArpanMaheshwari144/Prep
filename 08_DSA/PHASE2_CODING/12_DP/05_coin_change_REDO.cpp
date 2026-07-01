// ============================================================
// COIN CHANGE   —   [C] RECLAIM (min coins)   top-down memo
// ============================================================
// coins[] (unlimited use), amount. MIN coins to make amount. na bane -> -1.
//
// SOCH (seed — tera memo template):
//   solve(amt) = har coin try -> 1 + solve(amt - coin) -> sabka MIN.
//   BASE: amt==0 -> 0 | amt<0 -> invalid (bada/INT_MAX).
//   answer -1 tab jab koi combination na bane.
//
// Tests (// expected):
//   coins=[1,2,5],     amount=11 -> 3    (5+5+1)
//   coins=[2],         amount=3  -> -1   (na bane)
//   coins=[1],         amount=0  -> 0
//   coins=[1,2,5],     amount=0  -> 0
//   coins=[2,5,10,1],  amount=27 -> 4    (10+10+5+2)
// ============================================================

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int solve(vector<int> &coins, int amount, vector<int> &dp)
{
    if (amount == 0)
    {
        return 0;
    }

    if (amount < 0)
    {
        return INT_MAX;
    }

    if (dp[amount] != -1)
    {
        return dp[amount];
    }

    int mini = INT_MAX;
    for (int i = 0; i < coins.size(); i++)
    {
        if (solve(coins, amount - coins[i], dp) != INT_MAX)
            mini = min(mini, 1 + solve(coins, amount - coins[i], dp));
    }
    return dp[amount] = mini;
}

int coinChange(vector<int> &coins, int amount)
{
    vector<int> dp(amount + 1, -1);
    return solve(coins, amount, dp) == INT_MAX ? -1 : solve(coins, amount, dp);
}

int main()
{
    vector<int> c1 = {1, 2, 5};
    vector<int> c2 = {2};
    vector<int> c3 = {1};
    vector<int> c4 = {2, 5, 10, 1};

    cout << coinChange(c1, 11) << " (expected 3)\n";
    cout << coinChange(c2, 3) << " (expected -1)\n";
    cout << coinChange(c3, 0) << " (expected 0)\n";
    cout << coinChange(c1, 0) << " (expected 0)\n";
    cout << coinChange(c4, 27) << " (expected 4)\n";
    return 0;
}
