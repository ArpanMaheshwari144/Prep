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

int matrixChain(vector<int> &p)
{
    // TODO: khud likho (solve(i,j): base i==j->0, wall-loop k=i..j-1, combine p[i-1]*p[k]*p[j], min, memo dp[i][j]; caller solve(1,n))
    return 0;
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
