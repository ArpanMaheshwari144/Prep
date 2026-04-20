#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
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

    int rob(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, -1);
        return solve(nums, 0, dp);
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {1, 2, 3, 1};
    cout << "Test 1: " << sol.rob(t1);
    cout << (sol.rob(t1) == 4 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {2, 7, 9, 3, 1};
    cout << "Test 2: " << sol.rob(t2);
    cout << (sol.rob(t2) == 12 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {5};
    cout << "Test 3: " << sol.rob(t3);
    cout << (sol.rob(t3) == 5 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {1, 2};
    cout << "Test 4: " << sol.rob(t4);
    cout << (sol.rob(t4) == 2 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
