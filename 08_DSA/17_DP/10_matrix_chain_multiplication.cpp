// ============================================================
// MATRIX CHAIN MULTIPLICATION (MCM) — interval-DP   [naya pattern, khud bharo]
// ============================================================
// matrices ki chain multiply karni hai. result to same, par ORDER (parenthesization) se
// COST (scalar multiplications) badalti. MINIMUM total multiplications lautao.
//
// INPUT p[] = chain ki dims -> consecutive PAIR = ek matrix ka ROW x COL.  A_i = p[i-1] x p[i].  (n matrices = p.size()-1)
//   eg p=[10,30,5,60] -> A1=10x30, A2=30x5, A3=5x60.  (p[0]=A1 rows, p[1]=A1 cols=A2 rows, ...)
//
// ---- APPROACH ----  (interval-DP = two_max ka WALL + loop-har-wall + recurse + memo)
//  = two_max (Google) ka WALL hi hai. FARAK: two_max me left/right PRECOMPUTED (bestLeft/bestRight, O(1)) the;
//    yahan sub-chains OVERLAP karti -> precompute nahi -> har wall LOOP + har side RECURSE + memo (DP over RANGE).
//  solve(i,j) = chain A_i..A_j ko EK matrix banane ka MIN cost. (state = RANGE -> 2D dp[i][j].)
//     base : i==j -> ek hi matrix -> 0
//     wall : k = i..j-1 -> (A_i..A_k) x (A_{k+1}..A_j)
//              cost = solve(i,k) + solve(k+1,j) + p[i-1]*p[k]*p[j]   // combine (a*b*c)
//            -> in sab k ka MIN.  ans init INT_MAX (min-problem rule).
//  COMBINE: block A_i..A_k -> size p[i-1] x p[k]. wall pe LEFT=p[i-1]xp[k], RIGHT=p[k]xp[j] -> mult = p[i-1]*p[k]*p[j].
//     number-line: p[i-1]=poore-left ka baayaan chhor · p[k]=WALL · p[j]=poore-right ka dayaan chhor.
//  INDEX: 1-indexed -> caller solve(1, n) [n=p.size()-1]. i=0 mat karo (p[i-1]=p[-1] crash).
//  real-life: DB JOIN-order optimization · ML/graphics matrix-compute.
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
    // base i==j: chain me SIRF EK matrix bacha -> use akele multiply karne ko kuch nahi -> cost 0.
    //   (2+ matrix hon tabhi multiply hoti; akela matrix = koi kaam nahi = 0.)
    if (i == j)
        return 0;

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int ans = INT_MAX;
    /*
        k <= j-1 kyun (na j): wall chain ko (i..k) | (k+1..j) me todti hai.
        agar k = j hota -> RIGHT block (k+1..j) KHAALI ho jaata = invalid split.
        har side pe kam-se-kam 1 matrix chahiye -> isliye max k = j-1 (RIGHT = sirf A_j).
        aur RIGHT (k+1..j) manually nahi banate -> solve(k+1, j) recursion APNE AAP handle karta.
        eg {10,30,5,60}: k=j-1 pe right = single matrix, recursion sambhaal leta.
    */
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
