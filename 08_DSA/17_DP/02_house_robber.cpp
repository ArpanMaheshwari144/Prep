// ============================================================
// HOUSE ROBBER — (LeetCode 198, Medium)   [DP #2 · take/skip choice]
// ============================================================
// nums[] = har ghar me kitna paisa. tu chor. par 2 ADJACENT ghar loot nahi sakte
// (alarm baj jaayega). MAXIMUM kitna paisa loot sakta -> return.
//
//   [1,2,3,1]     -> 4    (ghar 0 + ghar 2 = 1+3)
//   [2,7,9,3,1]   -> 12   (2 + 9 + 1)
//   [5]           -> 5
//   [2,1,1,2]     -> 4    (2 + 2)
//
//   ★ SOCH: har ghar i pe 2 CHOICE ->
//       LOOT   ghar i : nums[i] + solve(i-2)   (i-1 skip karna padta, adjacent nahi)
//       SKIP   ghar i :           solve(i-1)   (ghar i chhoda, i-1 available)
//     => solve(i) = max( nums[i] + solve(i-2),  solve(i-1) )
//   ★ base: i<0 -> 0  (ya i==0 -> nums[0]).
//   ★ DP: overlap karta -> memoize (dp[i]!=-1 check + store), climbing-stairs jaisa.
// ============================================================

// ---- APPROACH ----  (take/SKIP choice + max -- climbing-stairs se aage: har step pe CHOICE)
//  ★ har ghar i pe 2 CHOICE:
//     LOOT i : nums[i] + solve(i-2)   (adjacent nahi -> i-1 skip karna padta)
//     SKIP i :          solve(i-1)    (ghar chhoda -> i-1 available)
//     => solve(i) = max(LOOT, SKIP).
//  ★ base: i<0 -> 0 · i==0 -> nums[0].
//  ★ solve(n-1) call: n-1 = array ka LAST index (0..n-1) -> "last ghar tak max". ye TOP-DOWN hi hai (bottom-up nahi).
//     (climbing me 'n' step-count 1..n tha; yahan array-index -> n-1. sirf naming ka farak.)
//  ★ memo wahi 3-pattern: dp[] -1 init · dp[i]!=-1 -> return · compute + store.
//  ★ bottom-up: dp[i] = max(nums[i]+dp[i-2], dp[i-1]) -- recursion ka i-1/i-2 pass = wahi table index.
// ============================================================
int solve(vector<int> &nums, int i, vector<int> &dp)
{
    if (i < 0)
        return 0;

    if (i == 0)
        return nums[0];

    if (dp[i] != -1)
    {
        return dp[i];
    }

    int LOOT = nums[i] + solve(nums, i - 2, dp);
    int SKIP = 0 + solve(nums, i - 1, dp);

    return dp[i] = max(LOOT, SKIP);
}

int rob(vector<int> &nums)
{
    int n = nums.size();
    vector<int> dp(n + 1, -1);
    return solve(nums, n - 1, dp);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, int exp, int t)
{
    int got = rob(in);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, 2, 3, 1}, 4, 1);
    check({2, 7, 9, 3, 1}, 12, 2);
    check({5}, 5, 3);
    check({2, 1, 1, 2}, 4, 4);
    return 0;
}
