// ============================================================
// 0/1 KNAPSACK — (classic DP)   [DP #9 · pure take/not-take]
// ============================================================
// n items, har item ka weight[i] aur value[i]. ek bag jiski capacity W.
// items bag me daalo (har item EK BAAR = 0/1), total weight <= W rakhte hue
// MAXIMUM total value nikaalo.
//
//   wt=[1,2,3],   val=[6,10,12],  W=5  -> 22   (item 2+3: wt 5, val 22)
//   wt=[1,3,4,5], val=[1,4,5,7],  W=7  -> 9    (wt 3+4, val 4+5)
//   wt=[3],       val=[4],        W=2  -> 0    (fit nahi hua)
//   wt=[1,2,3],   val=[10,15,40], W=6  -> 65   (sab: wt 6, val 65)
//   wt=[2,3,4,5], val=[3,4,5,6],  W=5  -> 7    (wt 2+3, val 7)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (PURE take/not-take -- har item EK BAAR, chor-analogy: bag me max value)
//  state = (i, W): "item index i tak, bag me W capacity bachi".
//  NOT-TAKE : solve(i-1, W)                                  (item chhoda -> capacity same)
//  TAKE     : agar wt[i] <= W -> val[i] + solve(i-1, W-wt[i])  (liya -> value+ , weight ghata, item ONCE -> i-1)
//  => max(take, notTake).
//  ★ coin-change take/not-take se FARAK: item ONCE -> take me bhi i-1 (coin me REUSE -> i same tha).
//
//  ★★ BASE: i<0 vs i==0 (dono VALID, same jawab -- ye samajhne layak):
//     i<0 (yahan)  : index-0 ko NORMAL item maano (take/not-take chale). recursion i-1=-1 pe -> base 0.
//                    => i==0 ka case recursion + i<0-base ne APNE-AAP sambhaal liya (special-case NAHI chahiye). CLEANER.
//     i==0 (online): item-0 ko EXPLICIT handle (if(i==0) return wt[0]<=W?val[0]:0), recursion 1 level pehle rukti.
//                    same answer, bas zyada code.
//     => i<0 = "sach me khatam", index-0 = normal. coin-change wala hi principle (i<0 kyu, i==0 nahi).
//  DP: memo dp[i][W].
// ============================================================
int solve(vector<int> &wt, vector<int> &val, int W, int i, vector<vector<int>> &dp)
{
    if (i < 0)
        return 0;

    if (dp[i][W] != -1)
    {
        return dp[i][W];
    }

    int notTake = 0 + solve(wt, val, W, i - 1, dp);
    int take = 0;
    if (wt[i] <= W)
    {
        take = val[i] + solve(wt, val, W - wt[i], i - 1, dp);
    }
    return dp[i][W] = max(take, notTake);
}

int knapsack(vector<int> &wt, vector<int> &val, int W)
{
    int n = wt.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, -1));
    return solve(wt, val, W, n - 1, dp);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> wt, vector<int> val, int W, int exp, int t)
{
    int got = knapsack(wt, val, W);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, 2, 3}, {6, 10, 12}, 5, 22, 1);
    check({1, 3, 4, 5}, {1, 4, 5, 7}, 7, 9, 2);
    check({3}, {4}, 2, 0, 3);
    check({1, 2, 3}, {10, 15, 40}, 6, 65, 4);
    check({2, 3, 4, 5}, {3, 4, 5, 6}, 5, 7, 5);
    return 0;
}
