// ============================================================
// LONGEST CONSECUTIVE SEQUENCE            (LC-128 — TEST, interview-mode)
// ============================================================
// ek unsorted integer array diya hai. sabse LAMBI consecutive-elements
// ki sequence ki LENGTH return karo (elements consecutive integers hon,
// order/adjacency array me matter nahi karti).
//   [100,4,200,1,3,2] -> 4    (1,2,3,4 = consecutive, length 4)
//
// >>> INTERVIEW MODE: khud soch, bol-ke derive kar. koi hint neeche nahi. <<<
//
// ---- TEST CASES (nums -> expected) ----
//   [100,4,200,1,3,2]            -> 4     (1,2,3,4)
//   [0,3,7,2,5,8,4,6,0,1]        -> 9     (0,1,2,3,4,5,6,7,8)
//   [ ]                          -> 0     (khali)
//   [1]                          -> 1
//   [1,2,0,1]                    -> 3     (0,1,2 ; duplicate 1 ignore)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int longestConsecutive(vector<int> &nums)
{
    int ans = INT_MIN;
    unordered_set<int> st;
    for (auto &it : nums)
    {
        st.insert(it);
    }

    for (int i = 0; i < nums.size(); i++)
    {
        int num = nums[i];
        if (!st.count(num - 1))
        {
            int currNum = num;
            int count = 1;
            while (st.count(currNum + 1))
            {
                currNum++;
                count++;
            }
            ans = max(ans, count);
        }
    }
    return ans == INT_MIN ? 0 : ans;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {100, 4, 200, 1, 3, 2};
    check(longestConsecutive(a1), 4, "case1");

    vector<int> a2 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    check(longestConsecutive(a2), 9, "case2");

    vector<int> a3 = {};
    check(longestConsecutive(a3), 0, "case3");

    vector<int> a4 = {1};
    check(longestConsecutive(a4), 1, "case4");

    vector<int> a5 = {1, 2, 0, 1};
    check(longestConsecutive(a5), 3, "case5");
    return 0;
}
