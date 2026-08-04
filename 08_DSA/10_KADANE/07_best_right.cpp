// ============================================================
// BEST SUBARRAY IN SUFFIX i..n-1  (bestRight[])
// ============================================================
// har index i ke liye: nums[i..n-1] ke ANDAR (i se aage) sabse-badi-sum contiguous subarray.
// = bestLeft ka MIRROR -- bas right se.
// ye values ka ARRAY lautao (size N).
//
//   [1,2,-7,8,6,-4]   -> [14, 14, 14, 14, 6, -4]
//   [5,4,-1,7,8]      -> [23, 18, 15, 15, 8]
//   [2,1,-99,3,3]     -> [6, 6, 6, 6, 3]
//   [-2,-1]           -> [-1, -1]
// ============================================================
// ---- APPROACH ----  (bestLeft ka MIRROR -- sab ulta, right se)
//  bestRight[i] = "i..n-1 ke ANDAR best subarray".
//  start[i] = EXACTLY i pe SHURU best = max(nums[i], nums[i] + start[i+1])   (loop right-to-left).
//  bestRight[i] = start[] ka running-max RIGHT se: ans[i] = max(ans[i+1], start[i]).
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> maxEndingAt(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    ans = nums;

    for (int i = n - 2; i >= 0; i--)
    {
        ans[i] = max(ans[i], nums[i] + ans[i + 1]);
    }
    return ans;
}

vector<int> bestRightSuffix(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(nums.size(), 0);
    vector<int> temp = maxEndingAt(nums);
    ans[n - 1] = temp[n - 1];
    for (int i = n - 2; i >= 0; i--)
    {
        ans[i] = max(ans[i + 1], temp[i]);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> nums, vector<int> exp, int t)
{
    vector<int> got = bestRightSuffix(nums);
    bool ok = (got == exp);
    cout << "T" << t << ": " << (ok ? "PASS" : "FAIL") << "  got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    check({1, 2, -7, 8, 6, -4}, {14, 14, 14, 14, 6, -4}, 1);
    check({5, 4, -1, 7, 8}, {23, 18, 15, 15, 8}, 2);
    check({2, 1, -99, 3, 3}, {6, 6, 6, 6, 3}, 3);
    check({-2, -1}, {-1, -1}, 4);
    return 0;
}
