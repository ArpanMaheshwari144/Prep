// ============================================================
// MAX CONSECUTIVE ONES III             (Pattern: Sliding Window - VARIABLE)
// ============================================================
// Ek binary array `nums` (sirf 0 aur 1) aur ek int `k` diya hai.
// Tu ZYADA SE ZYADA `k` zeros ko 1 mein FLIP kar sakta hai.
// Flip karne ke baad, sabse LAMBI consecutive 1's ki length return karo.
//
// Example:
//   nums = [1,1,1,0,0,0,1,1,1,1,0], k = 2   ->  6
//   nums = [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], k = 3   ->  10
//
// RETRY — SOLO, no hints. (Kal wala solution git history mein safe: commit 4686ab1)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int longestOnes(vector<int> &nums, int k)
    {
        int n = nums.size(); // 1,1,1,0,0,0,1,1,1,1,0
        int i = 0;
        int j = 0;
        int maxAns = INT_MIN;
        int zeroCount = 0;
        while (j < n)
        {
            if (nums[j] == 0)
            {
                zeroCount++;
            }
            if (zeroCount <= k)
            {
                maxAns = max(maxAns, j - i + 1);
            }
            if (zeroCount > k)
            {
                if (nums[i] == 0)
                {
                    zeroCount--;
                }
                i++;
                
            }
            j++;
        }
        return maxAns;
    }
};

int main()
{
    Solution s;
    vector<int> nums = {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0};
    int k = 2;
    cout << s.longestOnes(nums, k) << endl; // expected: 6
    return 0;
}
