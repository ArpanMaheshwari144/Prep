// ============================================================
// MAX SUM SUBARRAY OF SIZE K   (Sliding Window — FIXED window)
// ============================================================
// Array + ek number k diya. SIZE k ke har CONTIGUOUS subarray ka sum dekho,
// unme se SABSE BADA sum return karo.
//
// ---- ARPAN KI APPROACH ----
//  fixed window: j se har element sum me add. jab window size (j-i+1) >= k ho ->
//  maxSum = max(maxSum, sum) update, phir left nums[i] hatao (sum -= nums[i]) + i++.
//  window k-size pe locked slide karta rehta -> last me maxSum.
//
// Tests (// expected):
//   [2,1,5,1,3,2], k=3   -> 9   (5+1+3)
//   [2,3,4,1,5],   k=2   -> 7   (3+4)
//   [1,1,1,1],     k=2   -> 2
//   [5],           k=1   -> 5
//   [1,2,3,4,5],   k=5   -> 15
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int maxSumK(vector<int> &nums, int k)
{
    int sum = 0;
    int maxSum = INT_MIN;
    int i = 0, j = 0;
    while (j < nums.size())
    {
        sum += nums[j];
        if (j - i + 1 >= k)
        {
            maxSum = max(maxSum, sum);
            sum -= nums[i];
            i++;
        }
        j++;
    }

    return maxSum;
}

int main()
{
    vector<int> a1 = {2, 1, 5, 1, 3, 2};
    vector<int> a2 = {2, 3, 4, 1, 5};
    vector<int> a3 = {1, 1, 1, 1};
    vector<int> a4 = {5};
    vector<int> a5 = {1, 2, 3, 4, 5};

    cout << maxSumK(a1, 3) << " (expected 9)\n";
    cout << maxSumK(a2, 2) << " (expected 7)\n";
    cout << maxSumK(a3, 2) << " (expected 2)\n";
    cout << maxSumK(a4, 1) << " (expected 5)\n";
    cout << maxSumK(a5, 5) << " (expected 15)\n";
    return 0;
}
