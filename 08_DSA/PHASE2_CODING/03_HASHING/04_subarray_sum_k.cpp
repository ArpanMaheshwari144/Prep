// ============================================================
// SUBARRAY SUM EQUALS K              (Pattern: HASHING — primary)
// ============================================================
// NOTE: Yeh HASHING ka question hai (asli kaam HashMap karta — count/lookup).
//       Prefix sum (running cumulative sum) yahan sirf ek TOOL ki tarah use
//       hua. Bridge problem (Hashing + Prefix), par dil HASHING ka hai.
//
// Ek array `nums` aur ek int `k` diya hai. Kitne CONTIGUOUS subarrays
// hain jinka sum EXACTLY k hai — woh COUNT return karo.
// (nums mein negative bhi ho sakte — isliye sliding window FAIL, prefix+map.)
//
// Example:
//   nums = [1, 1, 1], k = 2      ->  2     ([1,1] do jagah)
//   nums = [1, 2, 3], k = 3      ->  2     ([1,2] aur [3])
//   nums = [0, 0, 0], k = 0      ->  6     (edge — saare 0)
//
// REDO — SOLO, HASHING way (map). Intuition: running total chalao, har point pe
//   "(currentSum - k) pehle kitni baar dekha?" (HashMap count). map[0]=1 se start.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_map<int, int> mp;
        mp[0] = 1;
        int ans = 0;
        int currSum = 0;
        for(int i=0;i<n;i++){
            currSum += nums[i];
            ans += mp[currSum - k];
            mp[currSum]++;
        }
        return ans;
    }
};

int main() {
    Solution s;
    vector<int> a = {1, 1, 1};
    cout << s.subarraySum(a, 2) << endl;   // expected: 2

    vector<int> b = {0, 0, 0};
    cout << s.subarraySum(b, 0) << endl;   // expected: 6  (edge test)
    return 0;
}
