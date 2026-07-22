// ============================================================
// MAXIMUM ABSOLUTE SUM OF ANY SUBARRAY — (LeetCode 1749, Medium)   [library-transfer test]
// ============================================================
// int array diya. kisi bhi CONTIGUOUS subarray ka sum lo -> uske ABSOLUTE value
// (|sum|) me se sabse BADA return karo.
//
//   [1,-3,2,3,-4]        -> 5    ([2,3] = 5)
//   [2,-5,1,-4,3,-2]     -> 8    ([-5,1,-4] = -8, |−8| = 8)
//   [1,2,3]              -> 6
//   [-1,-2,-3]           -> 6    (|-6|)
//   [5]                  -> 5
//
// (struggle-first, no peek. HINT-soch: aaj wala "max AND min dono track" yaad kar --
//  yahaan Kadane se maxSum + minSum dono nikaalo, answer inke |absolute| ka max.
//  dekh ye tere library ka near-transfer hai ya naya lagta.)
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
