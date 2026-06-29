// ============================================================
// MAXIMUM SUBARRAY  (Kadane's)
// ============================================================
// Array nums (negatives bhi). CONTIGUOUS subarray ka MAX sum return karo.
// (kam se kam 1 element lena hi hai.)
//
// SOCH (seed): har number pe choice -> pichhle tukde me jodu YA yahin naya shuru karu?
//   pichhla jod bojh (negative) ban gaya -> drop, fresh start. + best yaad rakho.
//
// Tests (// expected):
//   [-2,1,-3,4,-1,2,1,-5,4]   -> 6     // [4,-1,2,1]
//   [1]                        -> 1
//   [5,4,-1,7,8]               -> 23    // poora
//   [-1,-2,-3]                 -> -1    // sabse kam-negative
//   [-2,-1]                    -> -1
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxSubArray(vector<int> &nums)
{
    int maxSum = nums[0];
    int sum = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i];
        maxSum = max(maxSum, sum);
        if (sum < 0)
        {
            sum = 0;
        }
    }
    return maxSum;
}

int main()
{
    vector<int> a1 = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    vector<int> a2 = {1};
    vector<int> a3 = {5, 4, -1, 7, 8};
    vector<int> a4 = {-1, -2, -3};
    vector<int> a5 = {-2, -1};
    vector<int> a6 = {-5, -3, -8};

    cout << maxSubArray(a1) << " (expected 6)\n";
    cout << maxSubArray(a2) << " (expected 1)\n";
    cout << maxSubArray(a3) << " (expected 23)\n";
    cout << maxSubArray(a4) << " (expected -1)\n";
    cout << maxSubArray(a5) << " (expected -1)\n";
    cout << maxSubArray(a6) << " (expected -3)\n";
    return 0;
}
