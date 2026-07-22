// ============================================================
// MAXIMUM ABSOLUTE SUM OF ANY SUBARRAY — (LeetCode 1749, Medium)
// ============================================================
// int array diya. kisi bhi CONTIGUOUS subarray ka sum lo -> uske ABSOLUTE value
// (|sum|) me se sabse BADA return karo.
//
//   [1,-3,2,3,-4]        -> 5    ([2,3] = 5)
//   [2,-5,1,-4,3,-2]     -> 8    ([-5,1,-4] = -8, |-8| = 8)
//   [1,2,3]              -> 6
//   [-1,-2,-3]           -> 6    (|-6|)
//   [5]                  -> 5
//
// ---- ARPAN KI APPROACH ----
//  ★ FORMULA bilkul 152 (MAXIMUM PRODUCT SUBARRAY — (LeetCode 152, Medium)) jaisa -- bas GUNA (*) ki jagah PLUS (+). same 3 candidate, same freeze.
//    (candidate ke andar maxSum aur minSum hi feed hote -> nums[i]+maxSum, nums[i]+minSum.)
//  ★ FARAK: answer minSum (sabse chhota/negative sum) se bhi aa sakta -> ans = max(|maxSum|, |minSum|).
//  ★ DRY-RUN [2,-5,1,-4,3,-2] -> 8:
//      [-5,1,-4] ka sum = -8  -> ye minSum me aata (maxSum me nahi) -> |-8| = 8 = answer.
//      isiliye maxSum-minSum DONO track + abs DONO ka.
//  ★ 4 line:
//      temp   = max({ nums[i], nums[i]+maxSum, nums[i]+minSum })
//      minSum = min({ nums[i], nums[i]+maxSum, nums[i]+minSum })
//      maxSum = temp
//      ans    = max({ ans, abs(maxSum), abs(minSum) })
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxAbsoluteSum(vector<int> &nums)
{
    if (nums.size() == 1)
    {
        return nums[0];
    }

    int maxSum = nums[0]; // vector<int> c = {1, 2, 3};
    int minSum = nums[0];
    int ans = 0;
    for (int i = 1; i < nums.size(); i++)
    {
        int temp = max({nums[i], nums[i] + maxSum, nums[i] + minSum});
        minSum = min({nums[i], nums[i] + maxSum, nums[i] + minSum});
        maxSum = temp;
        ans = max({ans, abs(maxSum), abs(minSum)});
    }
    return ans;
}

int main()
{
    vector<int> a = {1, -3, 2, 3, -4};
    vector<int> b = {2, -5, 1, -4, 3, -2};
    vector<int> c = {1, 2, 3};
    vector<int> d = {-1, -2, -3};
    vector<int> e = {5};

    cout << maxAbsoluteSum(a) << " (expected 5)\n";
    cout << maxAbsoluteSum(b) << " (expected 8)\n";
    cout << maxAbsoluteSum(c) << " (expected 6)\n";
    cout << maxAbsoluteSum(d) << " (expected 6)\n";
    cout << maxAbsoluteSum(e) << " (expected 5)\n";
    return 0;
}
