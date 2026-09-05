// ============================================================
// MAXIMUM SUBARRAY   (LC-53 — REDO_1, Kadane)
// ============================================================
// nums[] (positive + negative dono ho sakte). CONTIGUOUS subarray ka
// MAXIMUM sum lauta do. (kam se kam ek element lena hi hai.)
//
//   [-2,1,-3,4,-1,2,1,-5,4]  -> 6   ([4,-1,2,1])
//   [1]                      -> 1
//   [5,4,-1,7,8]             -> 23  (poora array)
//
// ---- TEST CASES ----
//   [-2,1,-3,4,-1,2,1,-5,4]  -> 6
//   [1]                      -> 1
//   [5,4,-1,7,8]             -> 23
//   [-1]                     -> -1     (single negative)
//   [-3,-1,-2]               -> -1     (all negative -> kam-negative)
//   [-2,-1]                  -> -1
//   [2,-1,2,-1,2]            -> 4
//   [8,-19,5,-4,20]          -> 21     (5-4+20)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxSubArray(vector<int> &nums)
{
    int sum = 0;
    int maxSum = INT_MIN;
    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i];
        maxSum = max(maxSum, sum);
        if (sum < 0)
        {
            sum = 0;
        }
    }
    return maxSum;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    check(maxSubArray(a1), 6, "case1");
    vector<int> a2 = {1};
    check(maxSubArray(a2), 1, "case2");
    vector<int> a3 = {5, 4, -1, 7, 8};
    check(maxSubArray(a3), 23, "case3");
    vector<int> a4 = {-1};
    check(maxSubArray(a4), -1, "case4");
    vector<int> a5 = {-3, -1, -2};
    check(maxSubArray(a5), -1, "case5");
    vector<int> a6 = {-2, -1};
    check(maxSubArray(a6), -1, "case6");
    vector<int> a7 = {2, -1, 2, -1, 2};
    check(maxSubArray(a7), 4, "case7");
    vector<int> a8 = {8, -19, 5, -4, 20};
    check(maxSubArray(a8), 21, "case8");
    return 0;
}
