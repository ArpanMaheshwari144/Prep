// ============================================================
// COIN CHANGE — REDO (FOR-LOOP form)   [LC-322, DP #3]
// ============================================================
// coins[] (distinct) + amount. MINIMUM coins to make amount. har coin UNLIMITED. na ban sake -> -1.
//
//   [1,2,5], 11  -> 3   ·   [2], 3 -> -1   ·   [1], 0 -> 0   ·   [1,2,5], 7 -> 2   ·   [2,5,10,1], 27 -> 4
//
//   FOR-LOOP form: solve(amt) = har coin loop me try -> 1 + solve(amt - coin) -> sabme se MIN.
//     yahan 'i' index ki zaroorat nahi -> 1D dp[amount] kaafi.
//   base: amt==0 -> 0 · amt<0 -> INVALID (INT_MAX).
//   overflow: 1+solve(...) sirf jab solve != INT_MAX. end: answer INT_MAX -> -1.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (FOR-LOOP form -- SAME idea, bas likhawat alag)
//  take/not-take se FARAK: yahan SIRF "TAKE" hai -> for-loop me har coin lo -> min. NOT-TAKE alag likhne
//     ki zaroorat NAHI -- wo apne-aap ho jaata (backtracking ke for-loop/start-loop form jaisa: choice loop me).
//     [take/not-take me index i pe 2 branch; for-loop me saare coins loop -> "kaunsa coin lein" khud choose hota.]
//  solve(amt) = for coin in coins -> min(1 + solve(amt - coin)).  'i' index ki zaroorat NAHI -> 1D dp[amount].
//  base: amount<0 -> INT_MAX (INVALID) · amount==0 -> 0.
//  INT_MAX = "ban nahi sakta" signal -> min me apne-aap haar jaata.
//  OVERFLOW guard: 1+solve(...) SIRF jab solve != INT_MAX (warna 1+INT_MAX overflow).
//  caller: answer INT_MAX -> -1.
// ============================================================
int solve(vector<int> &coins, int amount, int i, vector<int> &dp)
{
    if (amount < 0)
        return INT_MAX;            // INVALID (amount cross)

    if (amount == 0)
        return 0;                  // ban gaya -> 0 coins

    if (dp[amount] != -1)          // cache HIT
        return dp[amount];

    int Take = INT_MAX;            // default INVALID
    for (int i = 0; i < coins.size(); i++)              // har coin TRY karo
    {
        if (solve(coins, amount - coins[i], i, dp) != INT_MAX)          // overflow guard
            Take = min(Take, 1 + solve(coins, amount - coins[i], i, dp)); // sabme se MIN (yehi "many coins" ka min)
    }

    return dp[amount] = Take;      // compute + STORE
}

int coinChange(vector<int> &coins, int amount)
{
    int n = coins.size();
    vector<int> dp(amount + 1, -1);
    int ans = solve(coins, amount, n - 1, dp);
    return ans == INT_MAX ? -1 : ans;                   // na-ban-sake -> -1
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
