// ============================================================
// MINIMUM SIZE SUBARRAY SUM   (Sliding Window — VARIABLE window)
// ============================================================
// POSITIVE numbers ka array + ek target diya.
// SABSE CHHOTI length ka CONTIGUOUS subarray dhoondho jiska sum >= target.
// us length ko return karo. koi aisा subarray na ho -> 0 return karo.
//
// seed: VARIABLE window. right (j) se window GROW karo (sum += nums[j]).
//       jab tak sum >= target -> min-length record karo + left (i) se SHRINK (sum -= nums[i], i++).
//       case-logic TU soch.
//
// Tests (// expected):
//   target=7,  [2,3,1,2,4,3]        -> 2   ([4,3])
//   target=4,  [1,4,4]              -> 1   ([4])
//   target=11, [1,1,1,1,1,1,1,1]    -> 0   (poora sum 8 < 11)
//   target=6,  [10]                 -> 1
//   target=15, [1,2,3,4,5]          -> 5   (poora array)
// ============================================================

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int minSubArrayLen(int target, vector<int> &nums)
{
    int sum = 0;
    int minLen = INT_MAX;
    int i = 0, j = 0;
    while (j < nums.size())
    {
        sum += nums[j];
        while (sum >= target)
        {
            minLen = min(minLen, j - i + 1);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return minLen == INT_MAX ? 0 : minLen;
}

int main()
{
    vector<int> a1 = {2, 3, 1, 2, 4, 3};
    vector<int> a2 = {1, 4, 4};
    vector<int> a3 = {1, 1, 1, 1, 1, 1, 1, 1};
    vector<int> a4 = {10};
    vector<int> a5 = {1, 2, 3, 4, 5};

    cout << minSubArrayLen(7, a1) << " (expected 2)\n";
    cout << minSubArrayLen(4, a2) << " (expected 1)\n";
    cout << minSubArrayLen(11, a3) << " (expected 0)\n";
    cout << minSubArrayLen(6, a4) << " (expected 1)\n";
    cout << minSubArrayLen(15, a5) << " (expected 5)\n";
    return 0;
}
