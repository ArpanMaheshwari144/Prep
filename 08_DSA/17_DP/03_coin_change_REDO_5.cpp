// ============================================================
// COIN CHANGE   (REDO_5 — blank test, DP revise)
// ============================================================
// coins[] (infinite har coin) + amount. Batao: amount banane ke liye
// MINIMUM kitne coins chahiye. Na ban sake -> return -1.
// (har coin baar-baar use kar sakte — unbounded.)
//
// >>> METHOD (yaad kar, blank se): <<<
//   - DP: har amount ke liye min-coins. base + transition + memo.
//   - dekhna nahi, khud likh. atko + KHUD poocho -> tab nudge.
//
// ---- TEST CASES ----
//   coins = {1,2,5},        amount = 11   -> 3     (5+5+1)
//   coins = {2},            amount = 3    -> -1    (ban hi nahi sakta)
//   coins = {1},            amount = 0    -> 0     (0 ke liye 0 coins)
//   coins = {2,5,10,1},     amount = 27   -> 4     (10+10+5+2)
//   coins = {186,419,83,408}, amount = 6249 -> 20
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(vector<int> &coins, int amount, int index, vector<vector<int>> &dp)
{
    if (amount < 0)
    {
        return INT_MAX;
    }

    if (index < 0)
    {
        return amount == 0 ? 0 : INT_MAX;
    }

    if (dp[index][amount] != -1)
    {
        return dp[index][amount];
    }

    int notTake = 0 + solve(coins, amount, index - 1, dp);

    int Take = INT_MAX;
    if (solve(coins, amount - coins[index], index, dp) != INT_MAX)
    {
        Take = 1 + solve(coins, amount - coins[index], index, dp);
    }
    return dp[index][amount] = min(notTake, Take);
}

int coinChange(vector<int> &coins, int amount)
{
    int n = coins.size();
    vector<vector<int>> dp(n + 1, vector<int>(amount + 1, -1));
    int ans = solve(coins, amount, n - 1, dp);
    return ans == INT_MAX ? -1 : ans;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> c1 = {1, 2, 5};
    check(coinChange(c1, 11), 3, "case1");

    vector<int> c2 = {2};
    check(coinChange(c2, 3), -1, "case2");

    vector<int> c3 = {1};
    check(coinChange(c3, 0), 0, "case3");

    vector<int> c4 = {2, 5, 10, 1};
    check(coinChange(c4, 27), 4, "case4");

    vector<int> c5 = {186, 419, 83, 408};
    check(coinChange(c5, 6249), 20, "case5");
    return 0;
}
