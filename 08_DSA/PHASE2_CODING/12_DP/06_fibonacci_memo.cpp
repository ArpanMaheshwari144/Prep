// ============================================================
// FIBONACCI (top-down MEMO)  — DP TEMPLATE cement karne ke liye
// ============================================================
// fib(n) = fib(n-1) + fib(n-2). base: fib(0)=0, fib(1)=1.
//
// ★ 4-STEP TEMPLATE (yaad rakh — har DP me yahi):
//   1. BASE CASE       : if (n <= 1) return n;
//   2. MEMO CHECK      : if (dp[n] != -1) return dp[n];
//   3. RECURRENCE      : fib(n-1) + fib(n-2)
//   4. STORE + return  : return dp[n] = ...;
//
// Tests (// expected):
//   fib(0) -> 0
//   fib(1) -> 1
//   fib(5) -> 5     (0,1,1,2,3,5)
//   fib(10) -> 55
//   fib(15) -> 610
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

int fib(int n, vector<int>& dp) {
    if(n <= 1){
        return n;
    }

    if(dp[n] != -1){
        return dp[n];
    }

    dp[n] = fib(n-1, dp) + fib(n-2, dp);
    return dp[n];
}

int fibonacci(int n) {
    vector<int> dp(n + 1, -1);   // size n+1 (index n tak chahiye)
    return fib(n, dp);
}

int main() {
    cout << fibonacci(0)  << " (expected 0)\n";
    cout << fibonacci(1)  << " (expected 1)\n";
    cout << fibonacci(5)  << " (expected 5)\n";
    cout << fibonacci(10) << " (expected 55)\n";
    cout << fibonacci(15) << " (expected 610)\n";
    return 0;
}
