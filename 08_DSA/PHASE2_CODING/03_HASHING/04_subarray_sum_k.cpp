// ============================================================
// SUBARRAY SUM EQUALS K                (Pattern: Hashing — prefix-sum + HashMap)
// ============================================================
// Ek array `nums` aur ek int `k`. Kitne CONTIGUOUS subarrays hain jinka
// sum EXACTLY k hai — woh COUNT return karo.  (nums mein NEGATIVE bhi ho sakte.)
//
// Examples:
//   nums = [1, 1, 1], k = 2   ->  2
//   nums = [1, 2, 3], k = 3   ->  2
//   nums = [0, 0, 0], k = 0   ->  6
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int n = nums.size();
        unordered_map<int, int> mp;
        int currSum = 0;
        mp[0] = 1;
        int ans = 0;
        for(int i=0;i<n;i++){
            currSum += nums[i]; //  nums = [1, 1, 1], k = 2   ->  2
            ans += mp[currSum - k];
            mp[currSum]++;
        }
        return ans;

    }
};

// ---- test harness ----
int main() {
    Solution s;
    vector<int> a = {1, 1, 1};
    cout << s.subarraySum(a, 2) << endl;   // expected: 2

    vector<int> b = {0, 0, 0};
    cout << s.subarraySum(b, 0) << endl;   // expected: 6
    return 0;
}
