// ============================================================
// KOKO EATING BANANAS — Binary Search block #6 (BS on ANSWERS)
// ============================================================
// piles[] = har pile me kitne kele. Koko ke paas `h` ghante hain.
// har ghante wo EK pile chunti aur k kele khaati (pile < k -> poora kha ke ruk jaati, agla ghanta).
// MINIMUM speed k return karo jisse wo `h` ghante me SAARE kele kha le.
//
// ---- ARPAN KI APPROACH ----
//  BS index pe nahi, ANSWER (speed k) ki range [1 .. max-pile] pe.
//  solve(mid): hours = sum of ceil(pile/mid) (double-cast se), hours<=h to speed kaafi hai.
//  kaafi hai -> ans=mid aur choti try karo (high=mid-1), warna badi (low=mid+1).
//  min feasible k lautao.
//
// Tests (// expected):
//   [3,6,7,11],        h=8  -> 4
//   [30,11,23,4,20],   h=5  -> 30
//   [30,11,23,4,20],   h=6  -> 23
//   [1,1,1,1],         h=4  -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool solve(vector<int> &piles, int h, int mid)
{
    int hours = 0;
    for (int i = 0; i < piles.size(); i++)
    {
        hours += ceil((double)piles[i] / mid);
    }
    return hours <= h;
}

int minEatingSpeed(vector<int> &piles, int h)
{
    int low = 1;
    int high = *max_element(begin(piles), end(piles));
    int ans = 0;
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
    vector<int> a1 = {3, 6, 7, 11};
    vector<int> a2 = {30, 11, 23, 4, 20};
    vector<int> a3 = {1, 1, 1, 1};

    cout << minEatingSpeed(a1, 8) << " (expected 4)\n";
    cout << minEatingSpeed(a2, 5) << " (expected 30)\n";
    cout << minEatingSpeed(a2, 6) << " (expected 23)\n";
    cout << minEatingSpeed(a3, 4) << " (expected 1)\n";
    return 0;
}
