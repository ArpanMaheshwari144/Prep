// Q: Stairs pe cost lagta hai — minimum cost mein top pe pahuncho (0 ya 1 se start)
#include <iostream>
#include <vector>
#include <climits>
using namespace std;
class Solution
{
public:
    int solve(vector<int> &cost, int i, vector<int> &dp)
    {
        // base case — stairs khatam ho gaye — 0 cost
        if (i >= cost.size())
        {
            return 0;
        }

        // pehle se solve hai toh return karo
        if (dp[i] != -1)
        {
            return dp[i];
        }

        // current stair ki cost + aage ek step ya do step mein se minimum
        int ans = cost[i] + min(solve(cost, i + 1, dp), solve(cost, i + 2, dp));
        // dp mein save karo
        return dp[i] = ans;
    }

    int minCostClimbingStairs(vector<int> &cost)
    {
        int n = cost.size();
        vector<int> dp(n + 1, -1);
        // 0th ya 1st stair se start — dono mein se minimum lo
        return min(solve(cost, 0, dp), solve(cost, 1, dp));
    }
};

int main()
{
    Solution sol;

    vector<int> c1 = {10, 15, 20};
    cout << "Test 1: " << sol.minCostClimbingStairs(c1);
    cout << (sol.minCostClimbingStairs(c1) == 15 ? "  PASS" : "  FAIL") << endl;

    vector<int> c2 = {1, 100, 1, 1, 1, 100, 1, 1, 100, 1};
    cout << "Test 2: " << sol.minCostClimbingStairs(c2);
    cout << (sol.minCostClimbingStairs(c2) == 6 ? "  PASS" : "  FAIL") << endl;

    vector<int> c3 = {0, 0, 0};
    cout << "Test 3: " << sol.minCostClimbingStairs(c3);
    cout << (sol.minCostClimbingStairs(c3) == 0 ? "  PASS" : "  FAIL") << endl;

    vector<int> c4 = {1, 2};
    cout << "Test 4: " << sol.minCostClimbingStairs(c4);
    cout << (sol.minCostClimbingStairs(c4) == 1 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
