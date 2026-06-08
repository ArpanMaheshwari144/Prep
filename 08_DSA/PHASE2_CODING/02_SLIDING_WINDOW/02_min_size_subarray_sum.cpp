// ============================================================
// MINIMUM SIZE SUBARRAY SUM           (Pattern: Sliding Window - VARIABLE)
// ============================================================
// Ek array `nums` (positive integers) aur ek `target` diya hai.
// Us CONTIGUOUS subarray ki MINIMUM LENGTH return karo jiska
// sum >= target ho. Agar koi nahi, to 0 return karo.
//
// Example:
//   target = 7, nums = [2,3,1,2,4,3]   ->  2   (subarray [4,3], sum 7)
//   target = 4, nums = [1,4,4]         ->  1   (subarray [4])
//   target = 11, nums = [1,1,1,1,1]    ->  0   (koi subarray nahi)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int i = 0, j = 0, n = nums.size();
        int currSum = 0, minLen = INT_MAX;
        while(j < n){
            currSum += nums[j];
            while(currSum >= target){
                minLen = min(minLen, j-i+1);
                currSum -= nums[i];
                i++;
            }
            j++;
        }
        return minLen == INT_MAX ? 0 : minLen;


    }
};

int main() {
    Solution s;
    vector<int> nums = {2, 3, 1, 2, 4, 3};
    int target = 7;
    cout << s.minSubArrayLen(target, nums) << endl;   // expected: 2
    return 0;
}
