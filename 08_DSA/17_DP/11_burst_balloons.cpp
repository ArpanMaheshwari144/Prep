// ============================================================
// BURST BALLOONS (LC 312) — interval-DP   [MCM ka cousin; tu bharo]
// ============================================================
// balloons ki row. ek-ek phodo. balloon phodne pe coins = (left-padosi) * (khud) * (right-padosi),
// jahaan padosi = US WAQT ke zinda balloons (phoote hue gaayab). phodne ke baad left-right adjacent ho jaate.
// MAX total coins lautao.
//
// ---- IDEA (jo aaj samjha; code khud likhna) ----
//  1. dono taraa PHANTOM 1 pad karo -> [1, ...balloons..., 1]. (kinaare pe gaayab padosi = 1, farak nahi.)
//  2. "pehle kaunsa phodun" -> phas jaata (padosi badalte, ranges dependent). ULTA socho:
//     "is range me SABSE AAKHRI (LAST) kaunsa phoota?" -> wo balloon DEEWAR ban jaata:
//        - uske phodne ke waqt andar sab khatam -> uske padosi = range ke bahar wale FIXED boundaries.
//        - left-part aur right-part uski wajah se ALAG independent subproblem (deewar beech me rahi).
//  3. har balloon ko baari-baari "last/deewar" maano -> uska coin + left-part + right-part -> in sab ka MAX.
//     (min-nahi MAX. state = RANGE (i,j) -> 2D memo. bilkul MCM ka wall, sirf delta.)
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
    if (i > j)
        return 0;

    if (dp[i][j] != -1)
    {
        return dp[i][j];
    }

    int ans = INT_MIN;
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
