// ==============================================================
// MEETING ROOMS II  (LC-253)  -- REDO_2 (blank, khud likh)
// --------------------------------------------------------------
// KYA KARNA:
//   Do arrays: start[] aur end[]. start[i], end[i] = i-th meeting ka time.
//   MINIMUM kitne rooms chahiye taaki saari meetings ho sakein
//   (overlapping meetings ek room me nahi ho sakti).
//   Note: agar ek meeting ka start == doosri ka end -> same room reuse ho sakta.
//
// (approach yaad karke likh. atka -> sheet-glance -> phir dobara blank.)
//
// TEST CASES (input -> expected):
//   start=[1,10,7]  end=[4,15,10]  -> 1   (koi overlap nahi)
//   start=[2,9,6]   end=[4,12,10]  -> 2
//   start=[0,5,15]  end=[10,20,25] -> 2
//   start=[1]       end=[5]        -> 1   (single meeting)
//   start=[1,2,3]   end=[10,10,10] -> 3   (teeno saath overlap)
//   start=[10,20]   end=[20,30]    -> 1   (start==prev end -> reuse)
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

    int overlap = 0, maxOverlap = 0;
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
void check(vector<int> start, vector<int> end, int exp)
{
    int got = minMeetingRooms(start, end);
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
