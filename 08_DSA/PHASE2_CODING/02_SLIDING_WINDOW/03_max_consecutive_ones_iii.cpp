// ============================================================
// MAX CONSECUTIVE ONES III             (Pattern: Sliding Window - VARIABLE)
// ============================================================
// Ek binary array `nums` (sirf 0 aur 1) aur ek int `k` diya hai.
// Tu ZYADA SE ZYADA `k` zeros ko 1 mein FLIP kar sakta hai.
// Flip karne ke baad, sabse LAMBI consecutive 1's ki length return karo.
//
// Example:
//   nums = [1,1,1,0,0,0,1,1,1,1,0], k = 2   ->  6
//     (do 0 flip karo: [0,0,1,1,1,1] -> 6 length window of 1's)
//   nums = [0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1], k = 3   ->  10
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int longestOnes(vector<int> &nums, int k)
    {
        // TODO: tera code (Sliding Window - variable)
        int i = 0, j = 0, n = nums.size();
        int zeroCount = 0;
        int maxLen = 0;
        while (j < n)
        {
            if (nums[j] == 0)
            {
                zeroCount++;
            }
            if (zeroCount <= k)
            {
                maxLen = max(maxLen, j - i + 1);
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
        return maxLen;
    }
};

// ---- test harness (run karke verify — yeh solution nahi, bas runner) ----
int main()
{
    Solution s;
    vector<int> nums = {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0};
    int k = 2;
    cout << s.longestOnes(nums, k) << endl; // expected: 6
    return 0;
}
