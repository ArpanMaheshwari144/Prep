// ============================================================
// BEST TIME TO BUY AND SELL STOCK   —   Arrays (single pass)
// ============================================================
// prices[] = har din ka stock daam. Ek din BUY, BAAD ke kisi din SELL.
// Maximum PROFIT return karo. (buy se pehle sell nahi; profit na ho to 0)
//
// Example:
//   {7, 1, 5, 3, 6, 4}  ->  5   (buy @1, sell @6)
//   {7, 6, 4, 3, 1}     ->  0   (har din girta — koi profit nahi)
//
// INTUITION (TU ne derive kiya — code TU likhega):
//   Ek pass — "ab tak ka sabse SASTA daam" (mini) yaad rakho.
//   Har din: profit = aaj ka daam - mini. Sabse bada profit yaad rakhte jao.
//   Aage badhte ho isliye mini hamesha aaj se PEHLE ka -> buy-before-sell apne aap.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxProfit(vector<int> &prices)
{
    int mini = INT_MAX;
    int maxi = INT_MIN;
    for(int i=0;i<prices.size();i++){
        mini = min(mini, prices[i]);
        maxi = max(maxi, prices[i] - mini);
    }
    return maxi;
}

int main()
{
    vector<int> a = {7, 1, 5, 3, 6, 4};
    cout << maxProfit(a) << endl; // expected: 5

    vector<int> b = {7, 6, 4, 3, 1};
    cout << maxProfit(b) << endl; // expected: 0
    return 0;
}
