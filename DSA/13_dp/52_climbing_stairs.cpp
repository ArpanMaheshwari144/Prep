// Q: n stairs hain — ek ya do step le sakta hai — kitne tarike se top pahunchega?
#include <iostream>
#include <vector>
using namespace std;
class Solution {
public:
    // Helper — recursion + memo
    int solve(int n, vector<int>& dp) {
       // base case — 0 ya 1 step bacha toh 1 hi tarika
       if(n==0 || n==1) return 1;
       // pehle se solve kiya hai toh seedha return karo
       if(dp[n] != -1) return dp[n];

       // 1 step leke ya 2 step leke — dono ke tarike add karo
       int ans = solve(n-1, dp) + solve(n-2, dp);
       // dp mein save karo taaki dobara na solve karna pade
       return dp[n] = ans;
    }

    // Main function — dp bana ke solve call kar
    int climbStairs(int n) {
        vector<int> dp(n + 1, -1);
        return solve(n, dp);
    }
};

int main() {
    Solution sol;

    cout << "Test 1: n=1 → " << sol.climbStairs(1);
    cout << (sol.climbStairs(1) == 1 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 2: n=2 → " << sol.climbStairs(2);
    cout << (sol.climbStairs(2) == 2 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 3: n=3 → " << sol.climbStairs(3);
    cout << (sol.climbStairs(3) == 3 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 4: n=5 → " << sol.climbStairs(5);
    cout << (sol.climbStairs(5) == 8 ? "  PASS" : "  FAIL") << endl;

    cout << "Test 5: n=10 → " << sol.climbStairs(10);
    cout << (sol.climbStairs(10) == 89 ? "  PASS" : "  FAIL") << endl;

    return 0;
}
