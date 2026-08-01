// ============================================================
// MAX SUBARRAY SUM (Kadane) — REDO   [LC-53, comfort redo]
// ============================================================
// nums[] diya. contiguous subarray ka MAXIMUM sum lautao. (kam se kam 1 element.)
//
//   [-2,1,-3,4,-1,2,1,-5,4]  -> 6    ([4,-1,2,1])
//   [1]                      -> 1
//   [5,4,-1,7,8]             -> 23
//   [-1]                     -> -1
//   [-2,-3,-1,-5]            -> -1   (saare negative -> sabse bada single)
//
//   ★ Kadane: har index pe -> "yahan tak ka best" = max(khud, pichla-best + khud).
//     global-max alag track. (running sum negative hua to reset = khud se shuru.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ─── YAHAN SOLVE KAR ────────────────────────────────────────
int maxSubArray(vector<int> &nums)
{
    int maxSum = INT_MIN;
    int sum = 0;
    for (auto &it : nums)
    {
        sum += it;
        maxSum = max(maxSum, sum);
        if (sum < 0)
        {
            sum = 0;
        }
    }
    return maxSum;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, int exp, int t)
{
    int got = maxSubArray(in);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({-2, 1, -3, 4, -1, 2, 1, -5, 4}, 6, 1);
    check({1}, 1, 2);
    check({5, 4, -1, 7, 8}, 23, 3);
    check({-1}, -1, 4);
    check({-2, -3, -1, -5}, -1, 5);
    return 0;
}
