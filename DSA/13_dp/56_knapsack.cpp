#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    int solve(vector<int>& weights, vector<int>& values, int i, int W, vector<vector<int>>& dp) {
        if(i >= weights.size()){
            return 0;
        }

        if(W == 0){
            return 0;
        }

        if(dp[i][W] != -1){
            return dp[i][W];
        }

        int pick = INT_MIN;
        if(weights[i] <= W){   
            pick = values[i] + solve(weights, values, i+1, W-weights[i], dp);
        }
        int not_pick = solve(weights, values, i+1, W, dp);
        return dp[i][W] = max(pick, not_pick);
    }

    int knapsack(vector<int>& weights, vector<int>& values, int W) {
        int n = weights.size();
        vector<vector<int>> dp(n, vector<int>(W + 1, -1));
        return solve(weights, values, 0, W, dp);
    }
};

int main() {
    Solution sol;

    vector<int> w1 = {1, 2, 3};
    vector<int> v1 = {6, 10, 12};
    cout << "Test 1: " << sol.knapsack(w1, v1, 5);
    cout << (sol.knapsack(w1, v1, 5) == 22 ? "  PASS" : "  FAIL") << endl;

    vector<int> w2 = {2, 3, 4, 5};
    vector<int> v2 = {3, 4, 5, 6};
    cout << "Test 2: " << sol.knapsack(w2, v2, 5);
    cout << (sol.knapsack(w2, v2, 5) == 7 ? "  PASS" : "  FAIL") << endl;

    vector<int> w3 = {10};
    vector<int> v3 = {100};
    cout << "Test 3: " << sol.knapsack(w3, v3, 5);
    cout << (sol.knapsack(w3, v3, 5) == 0 ? "  PASS" : "  FAIL") << endl;

    vector<int> w4 = {1, 1, 1};
    vector<int> v4 = {10, 20, 30};
    cout << "Test 4: " << sol.knapsack(w4, v4, 2);
    cout << (sol.knapsack(w4, v4, 2) == 50 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
