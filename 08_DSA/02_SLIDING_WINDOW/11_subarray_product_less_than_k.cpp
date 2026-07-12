// ============================================================
// SUBARRAY PRODUCT LESS THAN K — Sliding Window  (fresh, counting twist)
// ============================================================
// nums[] (positive ints) aur int k diya. kitne CONTINUOUS subarrays hain jinka
// PRODUCT (guna) STRICTLY k se kam hai? -> COUNT lauta.
//   nums=[10,5,2,6], k=100 -> 8
//     ([10],[5],[2],[6],[10,5],[5,2],[2,6],[5,2,6]) -> 8 subarrays product<100
//
// (approach yahan LIKHA NAHI -- code TU likhega. Arpan ki approach BAAD me,
//  jab tu code kare. copy-pen pe trace pehle.)
//
// Tests (nums, k -> expected):
//   [10,5,2,6], k=100   -> 8
//   [1,2,3], k=0        -> 0    (koi subarray product<0 nahi, sab >=1)
//   [1,1,1], k=2        -> 6    ([1]x3, [1,1]x2, [1,1,1]x1 = 6)
//   [10,9,10,4,3,8,3,3,6,2,10,10,9,3], k=19 -> 18
//   [1,2,3,4], k=10     -> 7
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int numSubarrayProductLessThanK(vector<int> &nums, int k)
{
    if (k == 0)
    {
        return 0;
    }

    int count = 0;
    int prod = 1;
    int i = 0, j = 0;
    while (j < nums.size())
    {
        prod *= nums[j];
        while (prod >= k)
        {
            prod /= nums[i];
            i++;
        }
        count += j - i + 1;
        j++;
    }
    return count;
}

int main()
{
    vector<int> a = {10, 5, 2, 6};
    vector<int> b = {1, 2, 3};
    vector<int> c = {1, 1, 1};
    vector<int> d = {10, 9, 10, 4, 3, 8, 3, 3, 6, 2, 10, 10, 9, 3};
    vector<int> e = {1, 2, 3, 4};

    cout << numSubarrayProductLessThanK(a, 100) << " (expected 8)\n";
    cout << numSubarrayProductLessThanK(b, 0) << " (expected 0)\n";
    cout << numSubarrayProductLessThanK(c, 2) << " (expected 6)\n";
    cout << numSubarrayProductLessThanK(d, 19) << " (expected 18)\n";
    cout << numSubarrayProductLessThanK(e, 10) << " (expected 7)\n";
    return 0;
}
