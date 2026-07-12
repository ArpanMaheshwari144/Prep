// ============================================================
// LONGEST CONSECUTIVE SEQUENCE — Hashing  (blank test)
// ============================================================
// unsorted array diya. sabse LAMBI consecutive-numbers sequence ki LENGTH lautao.
// (sequence = lagataar badhte numbers, e.g. [1,2,3,4]). O(n) me karo (sort nahi).
//   nums=[100,4,200,1,3,2] -> 4   (1,2,3,4)
//
// ---- ARPAN KI APPROACH ----
//  saare nums ek SET me daalo. har num pe: agar (num-1) set me NAHI -> ye sequence ka START.
//  phir num+1, num+2... jab tak set me hai count karo -> max track. O(n).
//
// Tests (// expected):
//   [100,4,200,1,3,2]            -> 4
//   [0,3,7,2,5,8,4,6,0,1]        -> 9
//   []                           -> 0
//   [1]                          -> 1
//   [1,2,0,1]                    -> 3
//   [9,1,4,7,3,-1,0,5,8,-1,6]    -> 7   (-1,0,1..? -> actually 3,4,5,6,7,8,9 = 7)
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
    for (int i = 0; i < nums.size(); i++)
    {
        int num = nums[i]; // 100
        if (!st.count(num - 1)) // kya 99 set mei hai ye nahi agar hai to 100 start nahi agra nahi hia to 100 start
        {
            int currNum = num; // 100 start
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
    vector<int> a = {100, 4, 200, 1, 3, 2};
    vector<int> b = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    vector<int> c = {};
    vector<int> d = {1};
    vector<int> e = {1, 2, 0, 1};
    vector<int> f = {9, 1, 4, 7, 3, -1, 0, 5, 8, -1, 6};

    cout << longestConsecutive(a) << " (expected 4)\n";
    cout << longestConsecutive(b) << " (expected 9)\n";
    cout << longestConsecutive(c) << " (expected 0)\n";
    cout << longestConsecutive(d) << " (expected 1)\n";
    cout << longestConsecutive(e) << " (expected 3)\n";
    cout << longestConsecutive(f) << " (expected 7)\n";
    return 0;
}
