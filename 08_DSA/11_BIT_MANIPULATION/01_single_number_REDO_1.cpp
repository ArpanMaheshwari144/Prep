// ═══════════════════════════════════════════════════════════════════════
//  SINGLE NUMBER  (LC 136)  — REDO
//  array me har number DO baar aata, sirf EK number ek baar. wahi lautao.
//  constraint: O(n) time + O(1) space (extra map / set NAHI).
//
//  INPUT-FORM: size >= 1 (odd). negative, zero, INT_MAX / INT_MIN bhi ho sakte.
//  TEST-CASES (input -> expected):
//   1) [2,2,1]                   -> 1
//   2) [4,1,2,1,2]               -> 4
//   3) [1]                       -> 1    (ek hi element)
//   4) [7,3,5,3,7]               -> 5    (beech me)
//   5) [-1,-1,-2]                -> -2   (negative)
//   6) [0,1,1]                   -> 0    (single = zero)
//   7) [5,0,0]                   -> 5    (pair = zero)
//   8) [0]                       -> 0
//   9) [9,8,9]                   -> 8
//  10) [2147483647,9,9]          -> 2147483647   (INT_MAX)
//  11) [-2147483648,3,3]         -> -2147483648  (INT_MIN)
//  12) [6,6,7,7,8,8,1]           -> 1    (sabse aakhri)
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int singleNumber(vector<int> &nums)
{
    int XORR = nums[0];
    for (int i = 1; i < nums.size(); i++)
    {
        XORR ^= nums[i];
    }
    return XORR;
}

// ---- test helper ----
void check(vector<int> nums, int exp, string name)
{
    int got = singleNumber(nums);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({2, 2, 1}, 1, "t1");
    check({4, 1, 2, 1, 2}, 4, "t2");
    check({1}, 1, "t3 single");
    check({7, 3, 5, 3, 7}, 5, "t4 beech me");
    check({-1, -1, -2}, -2, "t5 negative");
    check({0, 1, 1}, 0, "t6 single zero");
    check({5, 0, 0}, 5, "t7 pair zero");
    check({0}, 0, "t8 sirf zero");
    check({9, 8, 9}, 8, "t9");
    check({INT_MAX, 9, 9}, INT_MAX, "t10 INT_MAX");
    check({INT_MIN, 3, 3}, INT_MIN, "t11 INT_MIN");
    check({6, 6, 7, 7, 8, 8, 1}, 1, "t12 aakhri");
    return 0;
}
