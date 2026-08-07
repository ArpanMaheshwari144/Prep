// ============================================================
// BURST BALLOONS (LC 312) — interval-DP   [MCM ka cousin; tu bharo]
// ============================================================
// balloons ki row. ek-ek phodo. balloon phodne pe coins = (left-padosi) * (khud) * (right-padosi),
// jahaan padosi = US WAQT ke zinda balloons (phoote hue gaayab). phodne ke baad left-right adjacent ho jaate.
// MAX total coins lautao.
//
// ---- APPROACH ----  (interval-DP = MCM ka WALL, sirf delta)
//  solve(i,j) = balloons A_i..A_j sab phodne ka MAX coins. (state = RANGE -> 2D dp[i][j].)
//     PAD: dono taraf 1 -> [1, ...balloons..., 1]. (kinaare ka gaayab padosi = 1, farak nahi.)
//     "pehle kaunsa phodun" -> phas jaata (padosi badalte, ranges dependent). ULTA socho:
//        k = is range ka SABSE LAST phoota balloon. tab andar sab ja chuke ->
//            k ke padosi = bahar ke FIXED boundaries p[i-1], p[j+1].
//            k se pehle: left [i..k-1] aur right [k+1..j] alag-alag phoote (k beech me khada tha).
//     for k=i..j:  coins = solve(i,k-1) + solve(k+1,j) + p[i-1]*p[k]*p[j+1]   -> in sab ka MAX.
//  MCM se DELTA: (1) MIN -> MAX  (2) k = REMOVED balloon (na wall-in-block) -> solve(i,k-1), NA solve(i,k)
//     (3) base i>j (na i==j) -> single balloon PHIR BHI phoodta  (4) pad + boundary p[i-1],p[j+1] (na p[i-1],p[k],p[j]).
//  real: coins/DP interview-classic. interval-DP FAMILY: MCM, stone-merge, optimal-BST.
//
//   balloons=[3,1,5,8]   -> 167
//   balloons=[1,5]       -> 10
//   balloons=[2,4,6]     -> 66
//   balloons=[7]         -> 7    (single: pad -> 1*7*1)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(int i, int j, vector<int> &p, vector<vector<int>> &dp)
{
    // base i>j: range me KOI balloon bacha hi nahi (khaali) -> phodne ko kuch nahi -> 0.
    //   i==j (single balloon) base NAHI: wo balloon abhi bhi PHOOTEGA (coin = p[i-1]*p[i]*p[i+1]),
    //   isliye loop me jaata hai. (MCM se ULTA: wahan single matrix = 0 cost = base tha.)
    if (i > j)
        return 0;

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int ans = INT_MIN;
    /*
        k = i..j kyun (na j-1): yahan k = jo balloon PHOODTA hai (REMOVED), na ki WALL.
        agar k = i  -> LEFT  = solve(i, i-1) KHAALI ho jaata.
        agar k = j  -> RIGHT = solve(j+1, j) KHAALI ho jaata.
        par khaali range koi problem nahi -> base i>j se 0 laut aata (koi balloon nahi = 0 coins) = VALID.
        isliye har balloon (i..j) ko "phoote-wala" maan sakte -> k poora i..j chalta.
        eg {1,3,2,5,1}: k=i ya k=j pe ek side khaali, base 0 usse sambhaal leta.
        (MCM se ULTA: wahan k = WALL, dono block me kam-se-kam 1 matrix chahiye ->
         k=j pe RIGHT block KHAALI = INVALID -> isliye MCM me k max = j-1.)
    */
    for (int k = i; k <= j; k++)
    {
        ans = max(ans, solve(i, k - 1, p, dp) + solve(k + 1, j, p, dp) + p[i - 1] * p[k] * p[j + 1]);
    }
    return dp[i][j] = ans;
}

int matrixChain(vector<int> &p, int n, vector<vector<int>> &dp)
{
    return solve(1, n, p, dp);
}

int burstBalloons(vector<int> &balloons)
{
    int n = balloons.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));

    // dondo taraf 1 pad kardoya kyui ahtra last index wala photo ukos thek karne ke liye
    balloons.insert(balloons.begin(), 1);
    balloons.push_back(1);
    return matrixChain(balloons, n, dp);
}

void check(vector<int> b, int exp, int t)
{
    int got = burstBalloons(b);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({3, 1, 5, 8}, 167, 1);
    check({1, 5}, 10, 2);
    check({2, 4, 6}, 66, 3);
    check({7}, 7, 4);
    return 0;
}
