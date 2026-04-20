#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // prevIdx = pichle liye element ka index (-1 = kuch nahi liya)
    int solve(vector<int>& nums, int i, int prevIdx, vector<vector<int>>& dp) {
        if(i >= nums.size()){
            return 0;
        }

        if(prevIdx !=-1 && dp[i][prevIdx] != -1){
            return dp[i][prevIdx];
        }

        int pick = INT_MIN;
        if(prevIdx == -1 || nums[i] > nums[prevIdx]){
            pick = 1 + solve(nums, i+1, i, dp);
        }
        int not_pick = solve(nums, i+1, prevIdx, dp);
        int ans = max(pick, not_pick);

        if(prevIdx != -1){
            dp[i][prevIdx] = ans;
        }
        return ans;
    }

    int lengthOfLIS(vector<int>& nums) {
        int n = nums.size();
        // dp[i][prevIdx+1] — prevIdx -1 se start toh +1 shift
        vector<vector<int>> dp(n, vector<int>(n + 1, -1));
        return solve(nums, 0, -1, dp);
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {10, 9, 2, 5, 3, 7, 101, 18};
    cout << "Test 1: " << sol.lengthOfLIS(t1);
    cout << (sol.lengthOfLIS(t1) == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {0, 1, 0, 3, 2, 3};
    cout << "Test 2: " << sol.lengthOfLIS(t2);
    cout << (sol.lengthOfLIS(t2) == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {7, 7, 7, 7};
    cout << "Test 3: " << sol.lengthOfLIS(t3);
    cout << (sol.lengthOfLIS(t3) == 1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1, 2, 3, 4, 5};
    cout << "Test 4: " << sol.lengthOfLIS(t4);
    cout << (sol.lengthOfLIS(t4) == 5 ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {5};
    cout << "Test 5: " << sol.lengthOfLIS(t5);
    cout << (sol.lengthOfLIS(t5) == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
