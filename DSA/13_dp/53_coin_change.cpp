// Q: Minimum kitne coins lagenge given amount banane mein — nahi ban sakta toh -1
#include <iostream>
#include <vector>
#include <climits>
using namespace std;
class Solution {
public:
    // Helper — recursion + memo
    int solve(vector<int>& coins, int amount, vector<int>& dp) {
        // amount negative — ye path invalid hai
        if(amount < 0){
            return INT_MAX;
        }

        // base case — amount 0 — koi coin nahi chahiye
        if(amount == 0){
            return 0;
        }

        // pehle se solve kiya hai toh return karo
        if(dp[amount] != -1){
            return dp[amount];
        }

        int minAns = INT_MAX;
        // har coin try karo — jo minimum de wohi best
        for(int i=0;i<coins.size();i++){
            int ans = INT_MAX;
            // agar baaki amount solve ho sakta hai toh 1 coin add karo
            if(solve(coins, amount - coins[i], dp) != INT_MAX){
                ans =  1 + solve(coins, amount - coins[i], dp);
            }
            // sabme se minimum lo
            minAns = min(minAns, ans);
        }
        // dp mein save karo
        return dp[amount] = minAns;
    }

    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, -1);
        int ans = solve(coins, amount, dp);
        // agar answer INF hai toh possible nahi — -1 return
        return ans >= INT_MAX ? -1 : ans;
    }
};

int main() {
    Solution sol;

    vector<int> c1 = {1, 2, 5};
    cout << "Test 1: " << sol.coinChange(c1, 11);
    cout << (sol.coinChange(c1, 11) == 3 ? "  PASS" : "  FAIL") << endl;

    vector<int> c2 = {2};
    cout << "Test 2: " << sol.coinChange(c2, 3);
    cout << (sol.coinChange(c2, 3) == -1 ? "  PASS" : "  FAIL") << endl;

    vector<int> c3 = {1};
    cout << "Test 3: " << sol.coinChange(c3, 0);
    cout << (sol.coinChange(c3, 0) == 0 ? "  PASS" : "  FAIL") << endl;

    vector<int> c4 = {1, 3, 4};
    cout << "Test 4: " << sol.coinChange(c4, 6);
    cout << (sol.coinChange(c4, 6) == 2 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
