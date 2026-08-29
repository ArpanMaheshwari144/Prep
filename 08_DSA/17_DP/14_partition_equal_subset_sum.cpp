// ============================================================
// PARTITION EQUAL SUBSET SUM     (LC-416)
// ============================================================
// nums[] (positive ints). Batao kya array ko DO subset me baant
// sakte hain jinke element-SUM barabar hon?  return true/false.
//
//   [1,5,11,5]  -> true    ({1,5,5}=11  aur  {11}=11)
//   [1,2,3,5]   -> false   (total=11 odd -> aadha ho hi nahi sakta)
//
// ---- TEST (nums -> expected) ----
//   [1,5,11,5]      -> true
//   [1,2,3,5]       -> false
//   [1,1]           -> true      ({1} aur {1})
//   [1,2,5]         -> false     (total=8, par 4 ka subset nahi banta)
//   [2,2,2,2]       -> true      ({2,2} aur {2,2})
//   [3,3,3,4,5]     -> true      (total=18 -> {3,3,3}=9, {4,5}=9)
//   [1]             -> false
//   [2,4,6,8]       -> true      (total=20 -> {2,8}=10, {4,6}=10)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(vector<int> &wt, int W, int i, vector<vector<int>> &dp)
{
    if (W == 0) // target ban gaya -> SUCCESS (sabse pehle)
        return true;

    if (i < 0) // numbers khatam, target bacha -> nahi bana
        return false;

    if (i == 0) // aakhri number: sirf isse hi W ban sakta hai kya?
        return wt[0] == W;

    if (dp[i][W] != -1)
    {
        return dp[i][W];
    }

    int notTake = solve(wt, W, i - 1, dp); // number chhoda -> W same
    int take = 0;
    if (wt[i] <= W)
    {
        take = solve(wt, W - wt[i], i - 1, dp); // number liya -> W ghata
    }
    return dp[i][W] = (take || notTake); // koi ek raasta true -> true
}

bool knapsack(vector<int> &wt, int W, vector<vector<int>> dp, int n)
{
    return solve(wt, W, n - 1, dp);
}

bool subsetSum(vector<int> &nums, int target, vector<vector<int>> dp, int n)
{
    return knapsack(nums, target, dp, n);
}

// ---- APPROACH (= SUBSET-SUM, bas 2 line upar) -------------------
//   [1,5,11,5] -> total S = 22.  do BARABAR hisse -> har hissa 22/2 = 11.
//        koi subset 11 bana le   ->   {11}   ya   {1,5,5}
//        to baaki apne-aap 11 (22-11)  ->  DONO barabar!
//   => sawaal ban gaya: "kya subset S/2 (=11) tak pahunche?" = SUBSET-SUM.
//
//   DELTA (subsetSum ke upar sirf 2 line):
//      S ODD?  -> false      (21 ke do barabar tukde bante hi nahi)
//      warna   -> subsetSum(nums, S/2)
// ----------------------------------------------------------------
bool canPartition(vector<int> &nums)
{
    int S = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        S += nums[i];
    }

    int n = nums.size();
    vector<vector<int>> dp(n + 1, vector<int>((S / 2) + 1, -1));

    if (S % 2 != 0)
    {
        return false;
    }
    return subsetSum(nums, S / 2, dp, n);
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
    vector<int> a1 = {1, 5, 11, 5};
    check(canPartition(a1), true, "case1");
    vector<int> a2 = {1, 2, 3, 5};
    check(canPartition(a2), false, "case2");
    vector<int> a3 = {1, 1};
    check(canPartition(a3), true, "case3");
    vector<int> a4 = {1, 2, 5};
    check(canPartition(a4), false, "case4");
    vector<int> a5 = {2, 2, 2, 2};
    check(canPartition(a5), true, "case5");
    vector<int> a6 = {3, 3, 3, 4, 5};
    check(canPartition(a6), true, "case6");
    vector<int> a7 = {1};
    check(canPartition(a7), false, "case7");
    vector<int> a8 = {2, 4, 6, 8};
    check(canPartition(a8), true, "case8");
    return 0;
}
