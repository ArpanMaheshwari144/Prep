// ============================================================
// FIND PIVOT INDEX                     (Pattern: Prefix Sum — PURE, no map)
// ============================================================
// Ek array `nums` diya hai. Woh INDEX return karo jahan:
//   left ke saare elements ka sum  ==  right ke saare elements ka sum
// (us index ka apna element kisi side mein nahi ginte.)
// Leftmost aisa index return karo; koi na ho to -1.
//
// Example:
//   nums = [1, 7, 3, 6, 5, 6]   ->  3
//          (left of idx3 = 1+7+3 = 11,  right = 5+6 = 11)
//   nums = [1, 2, 3]            -> -1   (koi index aisa nahi)
//   nums = [2, 1, -1]          ->  0   (left = 0,  right = 1 + -1 = 0)
//
// Idea (PURE prefix, map ki zaroorat NAHI):
//   total = poore array ka sum.  leftSum chalao.
//   har i pe: rightSum = total - leftSum - nums[i].
//             agar leftSum == rightSum → i mil gaya.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int pivotIndex(vector<int> &nums)
    {
        int n = nums.size();
        int totalSum = 0;
        for (int i = 0; i < n; i++)
        {
            totalSum += nums[i];
        }

        int leftSum = 0; //  [1, 7, 3, 6, 5, 6]   ->  3
        for (int i = 0; i < n; i++)
        {
            int rightSum = totalSum - leftSum - nums[i];
            if (leftSum == rightSum)
            {
                return i;
            }
            leftSum += nums[i];
        }
        return -1;
    }
};

int main()
{
    Solution s;
    vector<int> nums = {1, 7, 3, 6, 5, 6};
    cout << s.pivotIndex(nums) << endl; // expected: 3
    return 0;
}
