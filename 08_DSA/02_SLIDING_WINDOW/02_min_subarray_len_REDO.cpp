// ============================================================
// MINIMUM SIZE SUBARRAY SUM — BLANK REDO (SW variable)
// ============================================================
// POSITIVE ints ka array + target diya. sabse CHHOTE contiguous subarray ki
// LENGTH nikaalo jiska sum >= target. koi na ho -> 0. O(n).
//
// (hint: variable window -> right badhao + sum me add.
//  jab tak sum >= target -> min-length record karo + LEFT se shrink (sum minus, left++). khud socho.)
//
// Tests (// expected):
//   target=7,  [2,3,1,2,4,3]        -> 2   ([4,3])
//   target=4,  [1,4,4]              -> 1
//   target=11, [1,1,1,1,1,1,1,1]    -> 0
//   target=15, [1,2,3,4,5]          -> 5
//   target=6,  [10,2,3]             -> 1
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int minSubArrayLen(int target, vector<int> &nums)
{
    int sum = 0;
    int size = INT_MAX;
    int i = 0, j = 0;
    while (j < nums.size())
    {
        sum += nums[j];
        while (sum >= target)
        {
            size = min(size, j - i + 1);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return size == INT_MAX ? 0 : size;
}

int main()
{
    vector<int> a1 = {2, 3, 1, 2, 4, 3};
    vector<int> a2 = {1, 4, 4};
    vector<int> a3 = {1, 1, 1, 1, 1, 1, 1, 1};
    vector<int> a4 = {1, 2, 3, 4, 5};
    vector<int> a5 = {10, 2, 3};

    cout << minSubArrayLen(7, a1) << " (expected 2)\n";
    cout << minSubArrayLen(4, a2) << " (expected 1)\n";
    cout << minSubArrayLen(11, a3) << " (expected 0)\n";
    cout << minSubArrayLen(15, a4) << " (expected 5)\n";
    cout << minSubArrayLen(6, a5) << " (expected 1)\n";
    return 0;
}
