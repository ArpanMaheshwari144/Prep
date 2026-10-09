// ═══════════════════════════════════════════════════════════════════════
//  SUBARRAY PRODUCT LESS THAN K  (LC 713)  — REDO
//  nums[] (positive ints) aur int k diya. kitne CONTINUOUS subarrays hain jinka
//  PRODUCT (guna) STRICTLY k se kam hai? -> COUNT lautao.
//
//  INPUT-FORM: nums[i] >= 1, k >= 0.
//  TEST-CASES (nums, k -> expected):
//   1) [10,5,2,6], k=100                          -> 8
//   2) [1,2,3], k=0                               -> 0
//   3) [1,1,1], k=2                               -> 6
//   4) [10,9,10,4,3,8,3,3,6,2,10,10,9,3], k=19    -> 18
//   5) [1,2,3,4], k=10                            -> 7
//   6) [1], k=1                                   -> 0    (1 < 1 nahi)
//   7) [5], k=10                                  -> 1
//   8) [100], k=100                               -> 0
//   9) [1,1,1], k=1                               -> 0
//  10) [2,2,2,2], k=5                             -> 7
//  11) [1,2,3], k=7                               -> 6
//  12) [3,2,1,10], k=7                            -> 6
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int numSubarrayProductLessThanK(vector<int> &nums, int k)
{
    if (k == 0)
    {
        return 0;
    }

    int count = 0;
    int i = 0, j = 0, prod = 1;
    while (j < nums.size())
    {
        prod *= nums[j];
        while (prod >= k && i <= j)
        {
            prod /= nums[i];
            i++;
        }
        count += j - i + 1;
        j++;
    }
    return count;
}

// ---- test helper ----
void check(vector<int> nums, int k, int exp, string name)
{
    int got = numSubarrayProductLessThanK(nums, k);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({10, 5, 2, 6}, 100, 8, "t1");
    check({1, 2, 3}, 0, 0, "t2 k=0");
    check({1, 1, 1}, 2, 6, "t3 sab 1");
    check({10, 9, 10, 4, 3, 8, 3, 3, 6, 2, 10, 10, 9, 3}, 19, 18, "t4 lamba");
    check({1, 2, 3, 4}, 10, 7, "t5");
    check({1}, 1, 0, "t6 k=1");
    check({5}, 10, 1, "t7 single");
    check({100}, 100, 0, "t8 barabar");
    check({1, 1, 1}, 1, 0, "t9 k=1 sab 1");
    check({2, 2, 2, 2}, 5, 7, "t10");
    check({1, 2, 3}, 7, 6, "t11 poora array bhi");
    check({3, 2, 1, 10}, 7, 6, "t12 bada beech me");
    return 0;
}
