// ============================================================
// LONGEST CONSECUTIVE SEQUENCE   (Hashing — set, O(n))
// ============================================================
// Unsorted array diya. SABSE LAMBI consecutive elements ki sequence ki
// LENGTH nikaalo (e.g. [1,2,3,4] -> 4). sort NAHI (O(n) chahiye).
//
// ---- ARPAN KI APPROACH ----
//  saare nums ek SET me daalo. har num pe: agar (num-1) set me NAHI -> ye sequence ka START.
//  phir num+1, num+2... jab tak set me hai count karo -> length. max-length track.
//  start-check se har sequence sirf EK baar count -> O(n).
//
// Tests (// expected):
//   [100,4,200,1,3,2]        -> 4   ([1,2,3,4])
//   [0,3,7,2,5,8,4,6,0,1]    -> 9   (0..8)
//   [] (empty)               -> 0
//   [1,2,0,1]                -> 3   ([0,1,2])
//   [5]                      -> 1
// ============================================================

#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

int longestConsecutive(vector<int> &nums)
{
    int longestStreak = INT_MIN;
    unordered_set<int> st;
    for (auto &it : nums)
    {
        st.insert(it);
    }

    for (int i = 0; i < nums.size(); i++)
    {
        if (!st.count(nums[i] - 1))
        {
            int currNum = nums[i];
            int currentStreak = 1;

            while (st.count(currNum + 1))
            {
                currNum++;
                currentStreak++;
            }
            longestStreak = max(longestStreak, currentStreak);
        }
    }
    return longestStreak == INT_MIN ? 0 : longestStreak;
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
