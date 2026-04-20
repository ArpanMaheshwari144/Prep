#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // same as house robber problem same code as it is
    int solve(vector<int>& nums, int i, vector<int>& dp) {
        if(i >= nums.size()){
            return 0;
        }

        if(dp[i] != -1){
            return dp[i];
        }
    
        int pick = nums[i] + solve(nums, i+2, dp);
        int not_pick = solve(nums, i+1, dp);

        return dp[i] = max(pick, not_pick);
    }

    int maxSum(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, -1);
        return solve(nums, 0, dp);
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {3, 2, 7, 10};
    cout << "Test 1: " << sol.maxSum(t1);
    cout << (sol.maxSum(t1) == 13 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {5, 5, 10, 100, 10, 5};
    cout << "Test 2: " << sol.maxSum(t2);
    cout << (sol.maxSum(t2) == 110 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {3, 2, 5, 10, 7};
    cout << "Test 3: " << sol.maxSum(t3);
    cout << (sol.maxSum(t3) == 15 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1};
    cout << "Test 4: " << sol.maxSum(t4);
    cout << (sol.maxSum(t4) == 1 ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {5, 1};
    cout << "Test 5: " << sol.maxSum(t5);
    cout << (sol.maxSum(t5) == 5 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
