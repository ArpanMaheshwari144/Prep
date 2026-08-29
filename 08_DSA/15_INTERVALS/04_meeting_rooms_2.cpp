// ==============================================================
// MEETING ROOMS II  (GfG / LC-253)  -- INTERVALS + SWEEP LINE
// --------------------------------------------------------------
// KYA KARNA:
//   Do arrays: start[] aur end[]. start[i], end[i] = i-th meeting ka time.
//   MINIMUM kitne rooms chahiye taaki saari meetings ho sakein
//   (overlapping meetings ek room me nahi ho sakti).
//   Note: agar ek meeting ka start == doosri ka end -> same room reuse ho sakta.
//
// ASLI SAWAAL = kisi bhi ek PAL pe max kitni meetings ek saath chal rahi?
//               woh "peak overlap" = answer.
//
// APPROACH (SWEEP LINE / ordered-map -- event count):
//   soch: rooms ko OBJECT mat gino -> har pal ka +1/-1 gino. peak-overlap = answer.
//   map<int,int> mp = time -> delta.  har meeting: mp[start]++ (room bhara), mp[end]-- (room khaali).
//   ordered map keys KHUD sorted -> manual sort NAHI. iterate: overlap += delta, maxOverlap track.
//   tie (start==prev end): same key pe +1 aur -1 -> net 0 -> room reuse free me (jhoothi peak nahi).
//   [dusra: min-heap of end-times  |  teesra: 2-array sort + two-pointer (O(1) space)]  sab O(n log n)
//   [TP == sweep hi hai -- do pointer sorted starts/ends pe = jhaadu ko sarkaana. interview: technique-check]
//
// INPUT samajh: start=[2,9,6] end=[4,12,10] -> meetings [2,4] [9,12] [6,10]
//               t=9 pe [9,12] aur [6,10] dono chal rahi -> 2 room -> ans 2
//
// TEST CASES (input -> expected):
//   start=[1,10,7]  end=[4,15,10]  -> 1   (koi overlap nahi, ek room kaafi)
//   start=[2,9,6]   end=[4,12,10]  -> 2
//   start=[0,5,15]  end=[10,20,25] -> 2   ([0,10]&[5,20] overlap, [15,25]&[5,20] overlap)
//   start=[1]       end=[5]        -> 1   (single meeting)
//   start=[1,2,3]   end=[10,10,10] -> 3   (teeno saath overlap)
//   start=[10,20]   end=[20,30]    -> 1   (start==prev end -> reuse, ek room)
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
        if (maxOverlap < overlap)
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
