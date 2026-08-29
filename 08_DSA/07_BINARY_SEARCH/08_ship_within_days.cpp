// ============================================================
// CAPACITY TO SHIP PACKAGES WITHIN D DAYS — Binary Search on ANSWER
// ============================================================
// weights[] = packages (ORDER me ship karne hain, aage-peeche nahi kar sakte).
// D din diye. har din tu kuch packages ship kar sakta (order me), par ek din ka
// total weight ship-capacity se ZYADA nahi. MINIMUM capacity lauta jisse saare
// packages D din me ship ho jaayein.
//   weights=[1,2,3,4,5,6,7,8,9,10], D=5  -> 15
//
// ---- ARPAN KI APPROACH ----
//  Koko jaisa BS on ANSWER, par yahan answer = ship CAPACITY.
//  low  = max(weights)  (sabse bhaari package to ek din me jaana hi hai -> capacity usse kam nahi)
//  high = sum(weights)  (poora ek hi din me bhej do)
//  solve(mid): daysNeeded nikaalo -> sum chalao; agar sum+weights[i] > mid (capacity exceed)
//              -> naya din (day++, sum=0); warna sum me add karte jao. end me day <= days? -> feasible.
//  BS: solve(mid) true -> ans=mid, high=mid-1 (choti capacity try); warna low=mid+1. -> min feasible capacity.
//
// Tests (weights, D -> expected):
//   [1,2,3,4,5,6,7,8,9,10], D=5  -> 15
//   [3,2,2,4,1,4], D=3           -> 6
//   [1,2,3,1,1], D=4             -> 3
//   [1,2,3,4,5], D=5             -> 5   (har din 1 package -> max weight)
//   [10,10,10], D=1              -> 30  (sab ek din -> sum)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool solve(vector<int> &weights, int days, int mid)
{
    int day = 1;
    int sum = 0;
    for (int i = 0; i < weights.size(); i++)
    {
        if (sum + weights[i] > mid)
        {
            day++;
            sum = 0;
        }
        sum += weights[i];
    }
    return day <= days;
}

int shipWithinDays(vector<int> &weights, int days)
{
    int low = *max_element(begin(weights), end(weights));
    int high = accumulate(begin(weights), end(weights), 0);
    int ans = -1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (solve(weights, days, mid))
        {
            ans = mid;
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return ans;
}

int main()
{
    vector<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> b = {3, 2, 2, 4, 1, 4};
    vector<int> c = {1, 2, 3, 1, 1};
    vector<int> d = {1, 2, 3, 4, 5};
    vector<int> e = {10, 10, 10};

    cout << shipWithinDays(a, 5) << " (expected 15)\n";
    cout << shipWithinDays(b, 3) << " (expected 6)\n";
    cout << shipWithinDays(c, 4) << " (expected 3)\n";
    cout << shipWithinDays(d, 5) << " (expected 5)\n";
    cout << shipWithinDays(e, 1) << " (expected 30)\n";
    return 0;
}
