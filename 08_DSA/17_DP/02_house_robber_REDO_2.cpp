// ============================================================
// HOUSE ROBBER   (REDO -- retention test, blank se)
// ============================================================
// Ghar ek line me hain, har ghar me kuch paisa (A[i]).
// Chori karni hai PAR do LAGATAAR (adjacent) ghar nahi loot sakte
// (alarm baj jaata). Max kitna paisa loot sakta hai?
//
// ---- TEST CASES ----
//   [2,7,9,3,1]   -> 12   (2 + 9 + 1)
//   [1,2,3,1]     -> 4    (1 + 3)
//   [2,1,1,2]     -> 4    (2 + 2)
//   [5]           -> 5
//   [10,1,1,10]   -> 20   (10 + 10)
//   []            -> 0
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(vector<int> &A, int i, vector<int> &dp)
{
    if (i < 0)
    {
        return 0;
    }

    if (dp[i] != -1)
    {
        return dp[i];
    }

    int take = A[i] + solve(A, i - 2, dp);
    int notTake = 0 + solve(A, i - 1, dp);
    return dp[i] = max(take, notTake);
}

int rob(vector<int> &A)
{
    int n = A.size();
    vector<int> dp(n + 1, -1);
    return solve(A, n - 1, dp);
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {2, 7, 9, 3, 1};
    check(rob(a1), 12, "case1");
    vector<int> a2 = {1, 2, 3, 1};
    check(rob(a2), 4, "case2");
    vector<int> a3 = {2, 1, 1, 2};
    check(rob(a3), 4, "case3");
    vector<int> a4 = {5};
    check(rob(a4), 5, "case4");
    vector<int> a5 = {10, 1, 1, 10};
    check(rob(a5), 20, "case5");
    vector<int> a6 = {};
    check(rob(a6), 0, "case6");
    return 0;
}
