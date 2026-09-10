// ============================================================
// MAXIMUM SUM CIRCULAR SUBARRAY — (LeetCode 918, Medium)   [Kadane x2]
// ============================================================
// nums[] CIRCULAR array (end ke baad start). Maximum sum wala NON-EMPTY contiguous
// subarray dhundh -- subarray WRAP bhi kar sakta (end se start tak).
//
//   [5,-3,5]            -> 10   (wrap: last-5 + first-5, beech -3 chhoda)
//   [1,-2,3,-2]         -> 3    (no wrap)
//   [3,-1,2,-1]         -> 4
//   [5,-2,5,3,-6,4]     -> 15   (wrap: 4 + 5,-2,5,3)
//   [-3,-2,-3]          -> -2   (★ all-negative EDGE -- yahan tu khud sochega)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
int maxKadaneK(vector<int> &nums)
{
    int sum = 0;
    int maxSum = INT_MIN;
    for (int i = 0; i < nums.size(); i++)
    {
        sum = max(sum + nums[i], nums[i]);
        maxSum = max(maxSum, sum);
    }
    return maxSum;
}

int minKadaneK(vector<int> &nums)
{
    int sum = 0;
    int minSum = INT_MAX;
    for (int i = 0; i < nums.size(); i++)
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

    int circularSum = totalSum - minSum;
    if (maxSum > 0)
    {
        return max(circularSum, maxSum);
    }
    return maxSum;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
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
    check({-3, -2, -3}, -2, 5); // all-negative edge
    return 0;
}
