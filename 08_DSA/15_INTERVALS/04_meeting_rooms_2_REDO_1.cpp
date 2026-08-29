// ==============================================================
// MEETING ROOMS II  (GfG / LC-253)  -- REDO_1  (blank, khud likh)
// --------------------------------------------------------------
// KYA KARNA: do arrays start[] aur end[]. MINIMUM kitne rooms chahiye
//   taaki saari meetings ho sakein (overlapping = alag room).
//   Note: start == prev end -> reuse ho sakta.
//
// (approach yaad karke likh -- SWEEP LINE / ordered-map +1/-1 count.
//  atke to sheet-glance, phir dobara blank.)
//
// TEST CASES (input -> expected):
//   start=[1,10,7]  end=[4,15,10]  -> 1
//   start=[2,9,6]   end=[4,12,10]  -> 2
//   start=[0,5,15]  end=[10,20,25] -> 2
//   start=[1]       end=[5]        -> 1
//   start=[1,2,3]   end=[10,10,10] -> 3
//   start=[10,20]   end=[20,30]    -> 1   (tie -> reuse)
// ==============================================================

#include <bits/stdc++.h>
using namespace std;

int minMeetingRooms(vector<int> &start, vector<int> &end)
{
    map<int, int> mp;
    int n = start.size();

    for (int i = 0; i < n; i++)
    {
        mp[start[i]]++;
        mp[end[i]]--;
    }

    int maxOverlap = 0, overlap = 0;
    for (auto &it : mp)
    {
        overlap += it.second;
        if (overlap > maxOverlap)
        {
            maxOverlap = overlap;
        }
    }
    return maxOverlap;
}

// -------------------- test harness --------------------
void check(vector<int> s, vector<int> e, int exp)
{
    int got = minMeetingRooms(s, e);
    cout << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    check({1, 10, 7}, {4, 15, 10}, 1);
    check({2, 9, 6}, {4, 12, 10}, 2);
    check({0, 5, 15}, {10, 20, 25}, 2);
    check({1}, {5}, 1);
    check({1, 2, 3}, {10, 10, 10}, 3);
    check({10, 20}, {20, 30}, 1);
    return 0;
}
