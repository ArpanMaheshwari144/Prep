// ============================================================
// MAXIMUM SUM SUBARRAY OF SIZE K        (Pattern: Sliding Window - FIXED)
// ============================================================
// Ek array `nums` aur ek size `k` diya hai. Size-k ke kisi bhi
// CONTIGUOUS subarray ka MAXIMUM sum return karo.
//
// Example:
//   nums = [2, 1, 5, 1, 3, 2], k = 3   ->  9   (subarray [5,1,3])
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int maxSumSubarray(vector<int>& nums, int k) {
        int i = 0, j = 0, n = nums.size();
        int currSum = 0, maxSum = INT_MIN;
        while(j < n){
            currSum += nums[j];

            if(j-i+1 > k){
                currSum -= nums[i];
                i++;
            }
            if(j-i+1==k){
                maxSum = max(maxSum, currSum);
            }
            j++;
        }
        return maxSum;
    }
};

int main() {
    Solution s;
    vector<int> nums = {2, 1, 5, 1, 3, 2};
    int k = 3;
    cout << s.maxSumSubarray(nums, k) << endl;   // expected: 9
    return 0;
}
