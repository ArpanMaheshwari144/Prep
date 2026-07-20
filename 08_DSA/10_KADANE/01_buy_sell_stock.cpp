// ============================================================
// BEST TIME TO BUY AND SELL STOCK — (LeetCode 121)   (fresh)
// ============================================================
// prices[i] = din i pe stock ka daam. ek baar KHAREEDO (buy) aur ek baar
// BECHO (sell) -- par sell hamesha buy ke BAAD wale din. max profit nikaalo.
// profit na ho paaye -> 0 return.
//
//   [7,1,5,3,6,4]   -> 5    (din1 pe 1 kharido, din4 pe 6 becho -> 5)
//   [7,6,4,3,1]     -> 0    (daam girta hi gaya -> koi profit nahi)
//   [1,2]           -> 1
//   [2,4,1]         -> 2    (2 kharido 4 becho; 1 baad me aaya par pehle nahi bech sakte)
//   [3,3,5,0,0,3,1,4] -> 4  (0 kharido, 4 becho)
//   [1]             -> 0
//
// ============================================================
// ---- ARPAN KI APPROACH ----
//  ★ ye TWO-POINTER nahi -- KADANE-family (running value + max update).
//     (rigorous link: din-ba-din DIFFERENCE array [p[i]-p[i-1]] ka MAX SUBARRAY SUM = ye. wahi Kadane.)
//  ★ IDEA (running-min): ek hi pass. do cheez track:
//        mini      = ab tak ka sabse SASTA din (yahan kharida hota).
//        maxProfit = ab tak ka best (aaj bech-te to kitna).
//     har din:  mini = min(mini, prices[i]);   maxProfit = max(maxProfit, prices[i] - mini);
//     (aaj becho -> aaj ka daam MINUS ab tak ka sabse sasta = aaj ka best profit.)
//  ★ sell hamesha buy ke BAAD -> apne-aap sahi (mini hamesha CURRENT se pehle ka min).
//  ★ maxProfit=INT_MIN safe: pehle hi din price-mini=0 -> 0 se kam kabhi return nahi (no-profit -> 0).
//  O(n) time, O(1) space.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxProfit(vector<int> &prices)
{
    int maxProfit = INT_MIN;
    int mini = INT_MAX;
    for (int i = 0; i < prices.size(); i++)
    {
        mini = min(mini, prices[i]);
        maxProfit = max(maxProfit, prices[i] - mini);
    }
    return maxProfit;
}

int main()
{
    vector<int> a = {7, 1, 5, 3, 6, 4};
    vector<int> b = {7, 6, 4, 3, 1};
    vector<int> c = {1, 2};
    vector<int> d = {2, 4, 1};
    vector<int> e = {3, 3, 5, 0, 0, 3, 1, 4};
    vector<int> f = {1};

    cout << maxProfit(a) << " (expected 5)\n";
    cout << maxProfit(b) << " (expected 0)\n";
    cout << maxProfit(c) << " (expected 1)\n";
    cout << maxProfit(d) << " (expected 2)\n";
    cout << maxProfit(e) << " (expected 4)\n";
    cout << maxProfit(f) << " (expected 0)\n";
    return 0;
}
