// ============================================================
// MATRIX CHAIN MULTIPLICATION (MCM) — REDO 1   [blank redo — tu code kar]
// ============================================================
// matrices ki chain multiply karni hai. result to same, par ORDER (parenthesization)
// se COST (scalar multiplications) badalti. MINIMUM total multiplications lautao.
//
// INPUT p[] = chain ki dims. consecutive PAIR = ek matrix ka ROW x COL.  A_i = p[i-1] x p[i].
//   eg p=[10,30,5,60] -> A1=10x30, A2=30x5, A3=5x60.  (n matrices = p.size()-1)
//
//   p=[10,30,5,60]        -> 4500
//   p=[40,20,30,10,30]    -> 26000
//   p=[10,20,30]          -> 6000
//   p=[1,2,3,4]           -> 18
//   p=[10,20]             -> 0        (single matrix, koi multiply nahi)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(int i, int j, vector<int> &p, vector<vector<int>> &dp)
{
    if (i == j)
        return 0;

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int ans = INT_MAX;
    for (int k = i; k <= j - 1; k++)
    {
        ans = min(ans, solve(i, k, p, dp) + solve(k + 1, j, p, dp) + p[i - 1] * p[k] * p[j]);
    }
    return dp[i][j] = ans;
}

int matrixChain(vector<int> &p)
{
    int n = p.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    return solve(1, n - 1, p, dp);
}

void check(vector<int> p, int exp, int t)
{
    int got = matrixChain(p);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({10, 30, 5, 60}, 4500, 1);
    check({40, 20, 30, 10, 30}, 26000, 2);
    check({10, 20, 30}, 6000, 3);
    check({1, 2, 3, 4}, 18, 4);
    check({10, 20}, 0, 5);
    return 0;
}
