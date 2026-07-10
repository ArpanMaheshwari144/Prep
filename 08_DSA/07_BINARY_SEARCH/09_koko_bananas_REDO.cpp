// ============================================================
// KOKO EATING BANANAS — Binary Search on ANSWER (SPACED-REDO, blank)
// ============================================================
// piles[] = har pile me kitne kele. Koko ke paas h ghante hain.
// har ghante wo ek pile chunti aur k kele khaati (agar pile me k se kam -> baaki ghanta waste).
// MINIMUM eating-speed k lautao jisse wo h ghante me SAARE kele kha le.
//   piles=[3,6,7,11], h=8  -> 4
//
// (purana code MAT dekhna. copy-pen pe socho:
//  - speed k pe hours = sum( CEIL(piles[i]/k) )   <- ceil! (bache kele bhi poora ghanta)
//  - k ki range [1 .. max(piles)] pe BINARY SEARCH -> min k jahan hours(k) <= h.)
//
// Tests (// expected):
//   [3,6,7,11],        h=8  -> 4
//   [30,11,23,4,20],   h=5  -> 30
//   [30,11,23,4,20],   h=6  -> 23
//   [1,1,1,1],         h=4  -> 1
//   [1000000000],      h=2  -> 500000000
//   [3,6,7,11],        h=4  -> 11
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool solve(vector<int> &piles, int h, int mid)
{
    int hours = 0;
    for (int i = 0; i < piles.size(); i++)
    {
        // hours += ((piles[i] + mid - 1) / mid); // ceil nikalne ka formula

        hours += ceil((double)piles[i] / mid); // ceil nikalne ka formula
    }
    return hours <= h;
}

int minEatingSpeed(vector<int> &piles, int h)
{
    int low = 1;
    int high = *max_element(begin(piles), end(piles)); //   [3,6,7,11],  h=8  -> 4
    int ans = -1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (solve(piles, h, mid))
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
    vector<int> a = {3, 6, 7, 11};
    vector<int> b = {30, 11, 23, 4, 20};
    vector<int> c = {1, 1, 1, 1};
    vector<int> d = {1000000000};

    cout << minEatingSpeed(a, 8) << " (expected 4)\n";
    cout << minEatingSpeed(b, 5) << " (expected 30)\n";
    cout << minEatingSpeed(b, 6) << " (expected 23)\n";
    cout << minEatingSpeed(c, 4) << " (expected 1)\n";
    cout << minEatingSpeed(d, 2) << " (expected 500000000)\n";
    cout << minEatingSpeed(a, 4) << " (expected 11)\n";
    return 0;
}
