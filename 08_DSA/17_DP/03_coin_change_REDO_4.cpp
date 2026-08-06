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

int coinChange(vector<int> &coins, int amount)
{
    // TODO: khud likho (take/not-take + 2 base + overflow guard + memo)
    return 0;
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
