// ============================================================
// MATRIX CHAIN MULTIPLICATION (MCM) — interval-DP   [naya pattern, khud bharo]
// ============================================================
// matrices ki chain multiply karni hai. result to same, par ORDER (parenthesization) se
// COST (scalar multiplications) badalti. MINIMUM total multiplications lautao.
//
// dims array p[] deta hai: matrix A_i = p[i-1] x p[i].  (n matrices = p.size()-1)
//   eg p=[10,30,5,60] -> A1=10x30, A2=30x5, A3=5x60.
//
// ---- YAAD (khud bola): two_max ka WALL jaisa + interval-DP ----
//   solve(i, j):  chain A_i..A_j ko ek matrix banane ka MIN cost.
//     base : i == j  -> ek hi matrix -> 0
//     wall : k = i .. j-1  -> ( A_i..A_k ) x ( A_{k+1}..A_j )
//            cost = solve(i,k) + solve(k+1,j) + p[i-1]*p[k]*p[j]   // combine = a*b*c
//     -> in sab k ka MIN.   memo dp[i][j].   caller: solve(1, n).
//   (combine: left block = p[i-1] x p[k], right = p[k] x p[j] -> multiply = p[i-1]*p[k]*p[j])
//
//   p=[10,30,5,60]        -> 4500     ((A1A2)A3 = 1500+3000)
//   p=[40,20,30,10,30]    -> 26000
//   p=[10,20,30]          -> 6000     (1 hi tarika: 10*20*30)
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
