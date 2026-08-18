// ============================================================
// MAX PROFIT — BUY ONCE, SELL LATER   (pehle BRUTE, phir OPTIMIZE)
// ============================================================
// prices[] diya — har din ka stock ka daam.
// Ek din KHAREEDO, aur kisi BAAD wale din BECHO (buy-din < sell-din).
// Sabse zyada profit batao. Agar koi profit possible nahi -> 0.
//
// >>> STEP 1: pehle BRUTE likh. STEP 2: baad me optimize karenge. <<<
//   brute recipe: NOUN = "pair (buy-din, sell-din)" jahan buy < sell
//                 -> saare pair bana (do loop: i buy, j = i+1..n sell)
//                 -> profit = prices[j] - prices[i]
//                 -> sabse bada profit rakho (aur 0 se neeche mat jaao).
//
// ---- TEST CASES ----
//   [7,1,5,3,6,4]  -> 5     (kharido 1 pe, becho 6 pe)
//   [7,6,4,3,1]    -> 0     (daam girta hi gaya, koi profit nahi)
//   [1,2]          -> 1
//   [5]            -> 0     (ek hi din, bech nahi sakte)
//   [2,4,1]        -> 2     (kharido 2, becho 4)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxProfit(vector<int> &prices)
{
    int ans = INT_MIN;
    int minSoFar = INT_MAX;
    for (int i = 0; i < prices.size(); i++)
    {
        minSoFar = min(minSoFar, prices[i]); // prices[j]
        ans = max(ans, prices[i] - minSoFar);
    }
    return ans < 0 ? 0 : ans;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {7, 1, 5, 3, 6, 4};
    check(maxProfit(a1), 5, "case1");
    vector<int> a2 = {7, 6, 4, 3, 1};
    check(maxProfit(a2), 0, "case2");
    vector<int> a3 = {1, 2};
    check(maxProfit(a3), 1, "case3");
    vector<int> a4 = {5};
    check(maxProfit(a4), 0, "case4");
    vector<int> a5 = {2, 4, 1};
    check(maxProfit(a5), 2, "case5");
    return 0;
}
