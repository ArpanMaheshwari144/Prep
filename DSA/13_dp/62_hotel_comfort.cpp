#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // house robber ka variant
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

    int maxComfort(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n, -1);
        return solve(nums, 0, dp);
    }
};

int main() {
    Solution sol;

    vector<int> t1 = {2, 1, 4, 9};
    cout << "Test 1: " << sol.maxComfort(t1);
    cout << (sol.maxComfort(t1) == 11 ? "  PASS" : "  FAIL") << endl;

    vector<int> t2 = {5, 3, 4, 11, 2};
    cout << "Test 2: " << sol.maxComfort(t2);
    cout << (sol.maxComfort(t2) == 16 ? "  PASS" : "  FAIL") << endl;

    vector<int> t3 = {10};
    cout << "Test 3: " << sol.maxComfort(t3);
    cout << (sol.maxComfort(t3) == 10 ? "  PASS" : "  FAIL") << endl;

    vector<int> t4 = {3, 7};
    cout << "Test 4: " << sol.maxComfort(t4);
    cout << (sol.maxComfort(t4) == 7 ? "  PASS" : "  FAIL") << endl;

    vector<int> t5 = {6, 7, 1, 30, 8, 2, 4};
    cout << "Test 5: " << sol.maxComfort(t5);
    cout << (sol.maxComfort(t5) == 41 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
