// ============================================================
// MAXIMUM SUM CIRCULAR SUBARRAY — (LeetCode 918)  [REDO_2]
// ============================================================
// Ek circular array di hai — matlab aakhri element ke baad pehla element aa jaata hai.
// NON-EMPTY contiguous subarray ka MAXIMUM sum nikalo. Subarray wrap kar sakta hai
// (end se shuru ho ke start pe khatam).
//
// contiguous ka matlab wahi: lagatar elements. har element at most EK baar use hoga.
//
//   nums = [5,-3,5]          -> 10
//   nums = [1,-2,3,-2]       -> 3
//   nums = [3,-1,2,-1]       -> 4
//   nums = [5,-2,5,3,-6,4]   -> 15
//   nums = [-3,-2,-3]        -> -2
//   nums = [-2]              -> -2
//   nums = [3,-2,2,-3]       -> 3
//
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxKadaneK(vector<int> &nums)
{
    int sum = nums[0];
    int maxSum = nums[0];
    for (int i = 1; i < nums.size(); i++)
    {
        sum = max(sum + nums[i], nums[i]);
        maxSum = max(maxSum, sum);
    }
    return maxSum;
}

int minKadaneK(vector<int> &nums)
{
    int sum = nums[0];
    int minSum = nums[0];
    for (int i = 1; i < nums.size(); i++)
    {
        sum = min(sum + nums[i], nums[i]);
        minSum = min(minSum, sum);
    }
    return minSum;
}

int maxSubarraySumCircular(vector<int> &nums)
{
    int totalSum = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        totalSum += nums[i];
    }

    int maxSum = maxKadaneK(nums);
    int minSum = minKadaneK(nums);

    // cout << maxSum << " " << minSum << " " << totalSum << endl;

    int circularSum = totalSum - minSum;
    if (maxSum > 0)
    {
        return max(circularSum, maxSum);
    }
    return maxSum;
}

// ─── TESTS (haath mat lagana) ──────────────────────────
void check(vector<int> in, int exp, int t)
{
    int got = maxSubarraySumCircular(in);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({5, -3, 5}, 10, 1);
    check({1, -2, 3, -2}, 3, 2);
    check({3, -1, 2, -1}, 4, 3);
    check({5, -2, 5, 3, -6, 4}, 15, 4);
    check({-3, -2, -3}, -2, 5);
    check({-2}, -2, 6);
    check({3, -2, 2, -3}, 3, 7);
    check({2, -2, 2, -2}, 2, 8);
    check({-5, 3, 5, -2, 5, -3}, 11, 9);
    return 0;
}
