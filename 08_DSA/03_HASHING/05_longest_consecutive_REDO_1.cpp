// ============================================================
// LONGEST CONSECUTIVE SEQUENCE — BLANK REDO (hashing / set)
// ============================================================
// UNSORTED array. SABSE LAMBI consecutive elements ki sequence ki LENGTH nikaalo.
//   e.g. [100,4,200,1,3,2] -> [1,2,3,4] -> 4.
// ★ sort NAHI karna. O(n) chahiye.
//
// ---- ARPAN KI APPROACH ----
//  saare nums ek SET me daalo. har num pe: agar (num-1) set me NAHI -> ye sequence ka START.
//  phir num+1, num+2... jab tak set me hai count -> max track. O(n).
//
// Tests (// expected):
//   [100,4,200,1,3,2]        -> 4
//   [0,3,7,2,5,8,4,6,0,1]    -> 9
//   [] (empty)               -> 0
//   [1,2,0,1]                -> 3
//   [5]                      -> 1
// ============================================================

#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

int longestConsecutive(vector<int> &nums)
{
    unordered_set<int> st;
    for (auto &it : nums)
    {
        st.insert(it);
    }

    int ans = INT_MIN;
    for (auto &num : nums) //{100, 4, 200, 1, 3, 2};
    {
        if (!st.count(num - 1))
        {
            int currNum = num;
            int count = 1;
            while (st.count(currNum + 1))
            {
                count++;
                currNum++;
            }
            ans = max(ans, count);
        }
    }
    return ans == INT_MIN ? 0 : ans;
}

int main()
{
    vector<int> a1 = {100, 4, 200, 1, 3, 2};
    vector<int> a2 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    vector<int> a3 = {};
    vector<int> a4 = {1, 2, 0, 1};
    vector<int> a5 = {5};

    cout << longestConsecutive(a1) << " (expected 4)\n";
    cout << longestConsecutive(a2) << " (expected 9)\n";
    cout << longestConsecutive(a3) << " (expected 0)\n";
    cout << longestConsecutive(a4) << " (expected 3)\n";
    cout << longestConsecutive(a5) << " (expected 1)\n";
    return 0;
}
