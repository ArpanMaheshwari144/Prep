// ============================================================
// COIN CHANGE — (LeetCode 322, Medium)   [DP #3 · min-count + reuse]  (take/not-take form)
// ============================================================
// coins[] (distinct denominations) + amount. amount banane ke liye MINIMUM kitne coins
// chahiye? har coin UNLIMITED baar use kar sakte. na ban sake -> -1.
//
//   coins=[1,2,5], amount=11  -> 3    (5+5+1)
//   coins=[2],     amount=3   -> -1   (nahi ban sakta)
//   coins=[1],     amount=0   -> 0    (0 amount -> 0 coins)
//   coins=[1,2,5], amount=7   -> 2    (5+2)
//
//   ★ SOCH: solve(amt) = har coin try -> 1 + solve(amt - coin) -> sabme se MIN.  reuse -> amt ghatao.
//   ★ base: amt==0 -> 0 · amt<0 -> INVALID (INT_MAX).
//   ★★ 3 TRAP (jahan phasa tha): (1) amt==0 base zaroori (2) best = INT_MAX se start (min dhoondh rahe)
//      (3) OVERFLOW: 1+solve(...) sirf tab jab solve != INT_MAX. end me: answer INT_MAX -> return -1.
//   (dono form: take/not-take (index) YA for-loop (saare coins) -- jo comfortable ho)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (TAKE / NOT-TAKE per coin index -- 2D dp[i][amount])
//  ★ har coin index i pe 2 CHOICE:
//     NOT-TAKE : coin i chhodo -> solve(i-1)              (agla coin, amount same)
//     TAKE     : coin i lo     -> 1 + solve(i, amount-coins[i])   (i SAME = reuse, amount ghata)
//     => min(notTake, take).
//  ★ base: amount<0 -> INT_MAX (INVALID) · i<0 -> (amount==0 ? 0 : INT_MAX)   [koi coin nahi bacha]
//     (i<0 base kyu, i==0 nahi: i==0 pe bhi coin[0] usable -> normal choice chalne do; base = sach me coin khatam = i<0.)
//  ★★ INT_MAX ka role: "is raaste se ban nahi sakta" ka signal -> final min me apne-aap HAAR jaata.
//  ★★ OVERFLOW guard: 1+solve(...) SIRF jab solve != INT_MAX (warna 1+INT_MAX overflow -> negative -> min galat).
//  ★ caller: answer INT_MAX -> return -1 (na-ban-sake).  memo = 2D dp[i][amount] (state = i + amount).
// ============================================================
int solve(vector<int> &coins, int amount, int i, vector<vector<int>> &dp)
{
    // ★★ amount==0 vs amount<0 = THEEK utna, na kam na zyada:
    //    amount==0 -> THEEK bana (11 ko theek 11) -> SUCCESS -> 0 coins.
    //    amount<0  -> 0 ke NEECHE gaya (coin bada le liya, overshoot) -> INVALID (ban-na = theek 0 pe rukna, cross nahi).
    if (amount < 0)
        return INT_MAX;                    // INVALID (0 cross -> zyada le liya)

    if (i < 0)                             // koi coin nahi bacha
        return amount == 0 ? 0 : INT_MAX;  //   amount THEEK 0 -> 0 coins · warna INVALID

    if (dp[i][amount] != -1)               // cache HIT
        return dp[i][amount];

    int notTake = solve(coins, amount, i - 1, dp);   // coin i chhoda -> agla coin

    int Take = INT_MAX;                              // default INVALID
    if (solve(coins, amount - coins[i], i, dp) != INT_MAX)          // overflow guard
        Take = 1 + solve(coins, amount - coins[i], i, dp);          // coin i liya (i same = reuse)

    return dp[i][amount] = min(notTake, Take);       // compute + STORE
}

int coinChange(vector<int> &coins, int amount)
{
    int n = coins.size();
    vector<vector<int>> dp(n + 1, vector<int>(amount + 1, -1));
    int ans = solve(coins, amount, n - 1, dp);
    return ans == INT_MAX ? -1 : ans;                // na-ban-sake -> -1
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
