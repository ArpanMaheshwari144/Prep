// ============================================================
// CLIMBING STAIRS   —   1D-DP  [pehla DP problem]
// ============================================================
// n seedhiyan. ek baar mein 1 ya 2 step. top tak pahunchne ke KITNE tareeke?
//
//   n=2 -> 2   (1+1, 2)
//   n=3 -> 3   (1+1+1, 1+2, 2+1)
//   n=4 -> 5
//   n=5 -> 8
//
// SIGNAL: "kitne tareeke / ways to reach" + har step pe choice -> DP.
//   RECURRENCE (jo samjha): ways(n) = ways(n-1) + ways(n-2)   [Fibonacci]
//     -> step n pe aaya ya n-1 se (+1) ya n-2 se (+2)
//   BASE: ways(1)=1, ways(2)=2
//   Approach: neeche se BUILD-UP (do pichhle values rakho, jodte jao) -- O(n).
//   No code -- tu likh (iterative do-pichhle-value, ya array, ya memoized recursion).
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

// ---- TERA kaam: yeh function bhar (1D-DP) ----
int solve(int n, vector<int> &dp)
{
    if (n == 1)
    {
        return 1;
    }
    if (n == 2)
    {
        return 2;
    }

    if(dp[n] != -1){
        return dp[n];
    }

    dp[n] = solve(n - 1, dp) + solve(n - 2, dp);
    return dp[n];
}
int climbStairs(int n)
{
    vector<int> dp(n+1, -1);
    return solve(n, dp);
}

int main()
{
    cout << climbStairs(2) << endl;  // expected 2
    cout << climbStairs(3) << endl;  // expected 3
    cout << climbStairs(4) << endl;  // expected 5
    cout << climbStairs(5) << endl;  // expected 8
    cout << climbStairs(1) << endl;  // expected 1
    cout << climbStairs(10) << endl; // expected 89
    return 0;
}
