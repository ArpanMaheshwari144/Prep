// ============================================================
// TRIBONACCI (top-down MEMO)  — same DP template, 3 terms
// ============================================================
// T(n) = T(n-1) + T(n-2) + T(n-3).  base: T(0)=0, T(1)=1, T(2)=1.
//
// ★ WAHI 4-STEP TEMPLATE (Fibonacci jaisा, bas base 3 + recurrence me 3 terms):
//   1. BASE       : n==0 -> 0 ; n==1 || n==2 -> 1
//   2. MEMO CHECK : if (dp[n] != -1) return dp[n];
//   3. RECURRENCE : T(n-1) + T(n-2) + T(n-3)
//   4. STORE      : return dp[n] = ...;
//
// Tests (// expected):
//   trib(0) -> 0
//   trib(3) -> 2     (0,1,1,2)
//   trib(4) -> 4     (0,1,1,2,4)
//   trib(10) -> 149
//   trib(25) -> 1389537
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int trib(int n, vector<int> &dp)
{
    if (n == 0)
    {
        return 0;
    }
    if (n == 1 || n == 2)
    {
        return 1;
    }

    if (dp[n] != -1)
    {
        return dp[n];
    }

    dp[n] = trib(n - 1, dp) + trib(n - 2, dp) + trib(n - 3, dp);
    return dp[n];
}

int tribonacci(int n)
{
    vector<int> dp(n + 1, -1);
    return trib(n, dp);
}

int main()
{
    cout << tribonacci(0) << " (expected 0)\n";
    cout << tribonacci(3) << " (expected 2)\n";
    cout << tribonacci(4) << " (expected 4)\n";
    cout << tribonacci(10) << " (expected 149)\n";
    cout << tribonacci(25) << " (expected 1389537)\n";
    return 0;
}
