// ============================================================
// COIN CHANGE   —   1D-DP  [DP #3]   (JP classic)
// ============================================================
// coins[] + amount. Amount banаne ke MIN kitne coins? na ban sake -> -1.
// (har coin UNLIMITED baar use kar sakte)
//
//   coins=[1,2,5], amount=11 -> 3   (5+5+1)
//   coins=[2],     amount=3  -> -1  (2 se 3 ban hi nahi sakta)
//   coins=[1],     amount=0  -> 0   (0 banаne ko 0 coin)
//   coins=[1,2,5], amount=7  -> 2   (5+2)
//
// SIGNAL: "min coins/ways to make amount, coins reuse" -> 1D-DP (har amount pe saare coins try).
//   RECURRENCE (jo samjha):
//     dp[a] = amount 'a' banаne ke MIN coins.  dp[0] = 0.
//     dp[a] = har coin c (c <= a) pe -> min( dp[a-c] + 1 )
//       -> "ek coin c lagao -> baaki (a-c) ke liye dp[a-c] -> +1 (yeh coin)"
//   na bane -> dp[a] "infinity" reh jaata -> answer -1.
//   2 LOOP: bahar amount (1..amount), andar coins.
//   tip: dp ko bade number (jaise amount+1, "infinity" jaisа) se init karo, dp[0]=0.
//   No code -- tu likh (iterative table O(amount*coins)).
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// ---- TERA kaam: yeh function bhar (1D-DP table) ----
int coinChange(vector<int> coins, int amount)
{
}

int main()
{
    cout << coinChange({1, 2, 5}, 11) << endl; // expected 3
    cout << coinChange({2}, 3) << endl;        // expected -1
    cout << coinChange({1}, 0) << endl;        // expected 0
    cout << coinChange({1, 2, 5}, 7) << endl;  // expected 2
    cout << coinChange({2, 5, 10, 1}, 27) << endl; // expected 4 (10+10+5+2)
    cout << coinChange({186, 419, 83, 408}, 6249) << endl; // expected 20
    return 0;
}
