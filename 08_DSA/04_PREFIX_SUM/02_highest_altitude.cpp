// ============================================================
// FIND THE HIGHEST ALTITUDE — Prefix Sum block #2
// ============================================================
// gain[i] = ek point se agle point tak altitude me NET change.
// shuruaat altitude = 0. har point ki altitude = ab tak ke gains ka running SUM.
// SABSE ZYADA altitude (jo kabhi pahuncha) return karo.
//
// (hint: running-sum chalate raho, saath me MAX track karo. start (0) bhi ek candidate hai.)
//
// Tests (// expected):
//   [-5,1,5,0,-7]        -> 1    (altitudes: 0,-5,-4,1,1,-6 -> max 1)
//   [-4,-3,-2,-1,4,3,2]  -> 0    (kabhi 0 se upar nahi gaya -> start 0 hi max)
//   [1,2,3]              -> 6
//   [44,32,-9,52,23,-50,50,33,-84,47,-14,48]  -> 175
// ============================================================
// ---- ARPAN KI APPROACH ----
//  sum=0, maxi=0 se start. har gain[i] ko running sum me add karte jao (yeh current altitude hai).
//  saath-saath maxi = max(maxi, sum) update karo. start (0) already maxi me hai isliye woh bhi candidate.
//  end me maxi return -- sabse oonchi altitude jo kabhi pahuncha.
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int largestAltitude(vector<int> &gain)
{
    int sum = 0;
    int maxi = 0;
    for (int i = 0; i < gain.size(); i++)
    {
        sum += gain[i];
        maxi = max(maxi, sum);
        // cout << sum << endl;
    }
    return maxi;
}

int main()
{
    vector<int> a1 = {-5, 1, 5, 0, -7};
    vector<int> a2 = {-4, -3, -2, -1, 4, 3, 2};
    vector<int> a3 = {1, 2, 3};
    vector<int> a4 = {44, 32, -9, 52, 23, -50, 50, 33, -84, 47, -14, 48};

    cout << largestAltitude(a1) << " (expected 1)\n";
    cout << largestAltitude(a2) << " (expected 0)\n";
    cout << largestAltitude(a3) << " (expected 6)\n";
    cout << largestAltitude(a4) << " (expected 175)\n";
    return 0;
}
