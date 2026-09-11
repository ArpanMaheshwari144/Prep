// ============================================================
// MAXIMUM SUM CIRCULAR SUBARRAY — (LeetCode 918)  [REDO_1 · blank retrieval]
// ============================================================
// circular array (end->start wrap), max-sum NON-EMPTY contiguous subarray (wrap ho sakta).
//   [5,-3,5]         -> 10
//   [1,-2,3,-2]      -> 3
//   [3,-1,2,-1]      -> 4
//   [5,-2,5,3,-6,4]  -> 15
//   [-3,-2,-3]       -> -2   (all-negative EDGE -- khud handle)
// insight (khud yaad kar): wrap = total − MIN-subarray -> ans = max(maxKadane, total − minKadane).
//   ★ EDGE: sab negative -> total−minK = 0 (empty) galat -> maxK<0 ho to sirf maxK return.
//   ★ minK = Kadane ka MIN-version (prefix-sum se NAHI).
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

    int maxK = maxKadaneK(nums);
    int minK = minKadaneK(nums);

    // cout << maxK << " " << minK << endl;

    int circularSum = totalSum - minK;
    if (maxK > 0)
    {
        return max(maxK, circularSum);
    }
    return maxK;
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
    return 0;
}
