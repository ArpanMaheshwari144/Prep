// ============================================================
// SUBSET SUM (boolean)     (knapsack-family, take||not-take)
// ============================================================
// nums[] (positive ints) + ek target diya. Batao kya koi SUBSET
// exactly `target` tak jod sakta hai?  return true/false.
//
//   nums=[2,3,5], target=5  -> true    ({5}  ya  {2,3})
//   nums=[1,2,7], target=6  -> false   (koi subset 6 nahi banata)
//
// >>> knapsack ka bhai: har number pe TAKE (target ghata) ya SKIP.
//     answer BOOLEAN -> take || notTake.  base: T==0 true, i<0 false. <<<
//
// ---- TEST (nums, target -> expected) ----
//   [2,3,5],     T=5   -> true      ({5} ya {2,3})
//   [1,2,7],     T=6   -> false
//   [1,5,11,5],  T=11  -> true      ({11} ya {1,5,5})
//   [3,34,4,12,5,2], T=9 -> true    ({4,5} ya {3,4,2})
//   [1,2,3],     T=0   -> true      (khaali subset = 0)
//   [2,4,6],     T=5   -> false     (sab even, 5 odd -> nahi)
//   [7],         T=7   -> true
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH (0/1 knapsack ka BOOLEAN bhai) --------------------
//  har number pe 2 choice (bilkul knapsack): TAKE ya NOT-TAKE.
//    NOT-TAKE : solve(i-1, W)             -> number chhoda, target(W) same
//    TAKE     : wt[i]<=W ? solve(i-1, W-wt[i]) : skip  -> liya, target ghata
//    answer   : take || notTake           -> koi EK raasta true -> true
//  FARAK knapsack se: max-VALUE ki jagah TRUE/FALSE (target bana ya nahi).
//
//  BASE-ORDER (fiddly part -- ye SAHI order zaroori):
//    W==0  -> true   PEHLE  (target ban gaya; items bache/na-bache matter nahi)
//    i<0   -> false         (numbers khatam, target adhoora)
//    i==0  -> wt[0]==W       (last number: bas isse hi ban sakta?)
//  [W==0 sabse UPAR warna i==0 wale case me success miss ho jaata -- eg [5,3],T=3.]
// -----------------------------------------------------------------
int solve(vector<int> &wt, int W, int i, vector<vector<int>> &dp)
{
    if (W == 0)         // target ban gaya -> SUCCESS (sabse pehle)
        return true;

    if (i < 0)          // numbers khatam, target bacha -> nahi bana
        return false;

    if (i == 0)         // aakhri number: sirf isse hi W ban sakta hai kya?
        return wt[0] == W;

    if (dp[i][W] != -1)
    {
        return dp[i][W];
    }

    int notTake = solve(wt, W, i - 1, dp);          // number chhoda -> W same
    int take = 0;
    if (wt[i] <= W)
    {
        take = solve(wt, W - wt[i], i - 1, dp);     // number liya -> W ghata
    }
    return dp[i][W] = (take || notTake);            // koi ek raasta true -> true
}

bool knapsack(vector<int> &wt, int W)
{
    int n = wt.size();
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, -1));
    return solve(wt, W, n - 1, dp);
}

bool subsetSum(vector<int> &nums, int target)
{
    return knapsack(nums, target);
}

// ---- test helper (ise mat chhed) ----
void check(bool got, bool exp, const string &label)
{
    cout << label << " -> got " << (got ? "true" : "false")
         << " | exp " << (exp ? "true" : "false")
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {2, 3, 5};
    check(subsetSum(a1, 5), true, "case1");
    vector<int> a2 = {1, 2, 7};
    check(subsetSum(a2, 6), false, "case2");
    vector<int> a3 = {1, 5, 11, 5};
    check(subsetSum(a3, 11), true, "case3");
    vector<int> a4 = {3, 34, 4, 12, 5, 2};
    check(subsetSum(a4, 9), true, "case4");
    vector<int> a5 = {1, 2, 3};
    check(subsetSum(a5, 0), true, "case5");
    vector<int> a6 = {2, 4, 6};
    check(subsetSum(a6, 5), false, "case6");
    vector<int> a7 = {7};
    check(subsetSum(a7, 7), true, "case7");
    return 0;
}
