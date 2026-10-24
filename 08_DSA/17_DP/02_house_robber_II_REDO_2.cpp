// ============================================================
// HOUSE ROBBER II — (LeetCode 213, Medium)   REDO_2 — blank
// ============================================================
// nums[] = har ghar me paisa, 2 ADJACENT ghar loot nahi sakte.
// Ghar ek CIRCLE me lage hain -> PEHLA ghar aur AAKHRI ghar bhi
// aapas me ADJACENT hain (nums[0] aur nums[n-1] dono loot nahi sakte).
// MAXIMUM kitna paisa loot sakta -> return.
//
//   [2,3,2]               -> 3
//   [1,2,3,1]             -> 4
//   [1,2,3]               -> 3
//   [5]                   -> 5
//   [200,3,140,20,10]     -> 340
//   [2,7,9,3,1]           -> 11
//   [1,1]                 -> 1
//   [0,0,0]               -> 0
//   [4,1,2,7,5,3,1]       -> 14
//   [1,3,1,3,100]         -> 103
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- kaam: yeh function ----
int rob(vector<int> &nums)
{
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, int exp, int t)
{
    int got = rob(in);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({2, 3, 2}, 3, 1);
    check({1, 2, 3, 1}, 4, 2);
    check({1, 2, 3}, 3, 3);
    check({5}, 5, 4);
    check({200, 3, 140, 20, 10}, 340, 5);
    check({2, 7, 9, 3, 1}, 11, 6);
    check({1, 1}, 1, 7);
    check({0, 0, 0}, 0, 8);
    check({4, 1, 2, 7, 5, 3, 1}, 14, 9);
    check({1, 3, 1, 3, 100}, 103, 10);
    return 0;
}
