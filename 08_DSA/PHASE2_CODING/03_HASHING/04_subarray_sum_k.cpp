// ============================================================
// SUBARRAY SUM EQUALS K              (Pattern: HASHING — primary)
// ============================================================
// NOTE: Yeh HASHING ka question hai (asli kaam HashMap karta — count/lookup).
//       Prefix sum (running cumulative sum) yahan sirf ek TOOL ki tarah use
//       hua. Bridge problem (Hashing + Prefix), par dil HASHING ka hai —
//       isiliye Hashing folder mein rakha.
//
// Ek array `nums` aur ek int `k` diya hai. Kitne CONTIGUOUS subarrays
// hain jinka sum EXACTLY k hai — woh COUNT return karo.
// (nums mein negative bhi ho sakte — isliye sliding window FAIL, prefix+map.)
//
// Example:
//   nums = [1, 1, 1], k = 2      ->  2     ([1,1] do jagah)
//   nums = [1, 2, 3], k = 3      ->  2     ([1,2] aur [3])
//   nums = [1, -1, 0], k = 0     ->  3
//
// Idea: running prefix sum chalao. Har point pe poochho —
//       "(currentSum - k) pehle kabhi dekha?" (HashMap mein count).
//       Agar haan → utne subarrays yahan khatam hote. map[0]=1 se start (edge).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int subarraySum(vector<int> &nums, int k)
    {
        int n = nums.size();
        unordered_map<int, int> mp;
        mp[0] = 1;
        int currSum = 0;
        int ans = 0;
        for (int i = 0; i < n; i++)
        {
            currSum += nums[i];
            ans += mp[currSum - k];
            mp[currSum]++;
        }
        return ans;
    }
};

int main()
{
    Solution s;
    vector<int> nums = {1, 1, 1};
    cout << s.subarraySum(nums, 2) << endl; // expected: 2
    return 0;
}
