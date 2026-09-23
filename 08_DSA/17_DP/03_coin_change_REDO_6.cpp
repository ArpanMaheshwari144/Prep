// ============================================================
// COIN CHANGE   (REDO_6 — 23-Sep, blank page)
// ============================================================
// coins[] (har coin UNLIMITED baar) + amount. Batao: amount THEEK banane ke liye
// MINIMUM kitne coins chahiye. Na ban sake -> return -1.
//
// ------------------------------------------------------------

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

    int notTake = solve(coins, amount, i - 1, dp);
    int take = INT_MAX;
    if (solve(coins, amount - coins[i], i, dp) != INT_MAX)
    {
        take = 1 + solve(coins, amount - coins[i], i, dp);
    }
    return dp[i][amount] = min(take, notTake);
}

int coinChange(vector<int> &coins, int amount)
{
    int n = coins.size();
    vector<vector<int>> dp(n + 1, vector<int>(amount + 1, -1));
    int ans = solve(coins, amount, n - 1, dp);
    return ans == INT_MAX ? -1 : ans;
}

// ------------------------------------------------------------
int main()
{
    struct T
    {
        vector<int> coins;
        int amount;
        int expected;
        string kyun;
    };

    vector<T> tests = {
        {{1, 2, 5}, 11, 3, "classic — 5+5+1"},
        {{2}, 3, -1, "ban hi nahi sakta"},
        {{1}, 0, 0, "amount 0 -> 0 coin"},
        {{1, 3, 4}, 6, 2, "★ 3+3 — sabse bada pehle (4+1+1) uthaoge to 3 aayega"},
        {{2, 5, 10, 1}, 27, 4, "coins sorted nahi — 10+10+5+2"},
        {{5}, 5, 1, "ek coin, theek barabar"},
        {{3, 7}, 5, -1, "dono coin amount se bade/fit nahi"},
        {{1, 5, 6, 9}, 11, 2, "★ 5+6 — 9+1+1 nahi"},
        {{2, 4}, 7, -1, "saare even, amount odd"},
        {{186, 419, 83, 408}, 6249, 20, "★ bada — memo nahi to time out"},
        {{1}, 10000, 10000, "★ gehri recursion — sirf 1 ka coin"},
    };

    int pass = 0;
    for (int i = 0; i < (int)tests.size(); i++)
    {
        vector<int> copy = tests[i].coins;
        int got = coinChange(copy, tests[i].amount);
        bool ok = (got == tests[i].expected);
        pass += ok;
        cout << (ok ? "PASS  " : "FAIL  ")
             << "amount=" << tests[i].amount
             << "  expected=" << tests[i].expected
             << "  mila=" << got
             << "   [" << tests[i].kyun << "]\n";
    }
    cout << "\n"
         << pass << " / " << tests.size() << "\n";
    return 0;
}
