// ============================================================
// FROG JUMP (top-down MEMO)  — fib-structure + CHOICE (ek step up)
// ============================================================
// Frog index 0 -> index n-1. har jagah se 1 ya 2 step jump.
// jump cost = |height farak|. MINIMUM total cost.
//
// ★ fib se ek STEP UP: ab ADD nahi -> 2 options me se MIN CHUNO (+ cost).
//   dp[i] = min( dp[i-1] + |h[i]-h[i-1]| , dp[i-2] + |h[i]-h[i-2]| )
//   base: dp[0] = 0.  (i-2 wala option sirf jab i >= 2)
//
// TEMPLATE wahi 4-step: base -> memo-check -> recurrence(min) -> store.
// (direction: index n-1 se peeche i-1/i-2 -> base ORIGIN i==0. tera direction-rule yaad?)
//
// Tests (// expected):
//   [10,30,40,20]        -> 30
//   [10,20,30,10]        -> 20
//   [30,10,60,10,60,50]  -> 40
//   [10]                 -> 0
//   [10,50]              -> 40
// ============================================================

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

int solve(int i, vector<int>& h, vector<int>& dp) {
    // TODO: tera code (4 steps — base i==0 -> 0, i-2 sirf jab i>1)
    return 0;
}

int frogJump(vector<int>& heights) {
    int n = heights.size();
    vector<int> dp(n, -1);
    return solve(n - 1, heights, dp);
}

int main() {
    vector<int> a1 = {10,30,40,20};
    vector<int> a2 = {10,20,30,10};
    vector<int> a3 = {30,10,60,10,60,50};
    vector<int> a4 = {10};
    vector<int> a5 = {10,50};

    cout << frogJump(a1) << " (expected 30)\n";
    cout << frogJump(a2) << " (expected 20)\n";
    cout << frogJump(a3) << " (expected 40)\n";
    cout << frogJump(a4) << " (expected 0)\n";
    cout << frogJump(a5) << " (expected 40)\n";
    return 0;
}
