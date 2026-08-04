// ============================================================
// BEST SUBARRAY IN PREFIX 0..i  (bestLeft[])  -- BLANK REDO_1
// ============================================================
// har index i ke liye: nums[0..i] ke ANDAR (i tak) sabse-badi-sum contiguous subarray.
// (end[] se farak: end[i] me subarray i pe KHATAM hona zaroori tha; yahan i ke ANDAR
//  kahin bhi khatam ho sakti -- bas index i se aage nahi.)
// ye values ka ARRAY lautao (size N).
//
//   [1,2,-7,8,6,-4]   -> [1, 3, 3, 8, 14, 14]
//   [5,4,-1,7,8]      -> [5, 9, 9, 15, 23]
//   [2,1,-99,3,3]     -> [2, 3, 3, 3, 6]
//   [-2,-1]           -> [-2, -1]
// ============================================================
// ---- APPROACH ----  (end[] REUSE + running-max)
//  bestLeft[i] = "0..i ke ANDAR best" (kahin bhi khatam, bas i tak). end[i] = EXACTLY i pe khatam best.
//  to 0..i me best = end[0..i] ka sabse bada = RUNNING MAX.
//     bestLeft[i] = max( bestLeft[i-1] , end[i] )
//  ★ TRAP (test T5 [5,-2,-2,-1] ne pakda): running-max = AB TAK ka max -> bestLeft[i-1] chahiye,
//    NA sirf end[i-1] (wo sirf pichhla end, ab-tak-ka-max nahi). end=[5,3,1,0] -> bestLeft=[5,5,5,5].
//  step: temp = maxEndingAt(nums) [end[] reuse] -> ans[i] = max(ans[i-1], temp[i]).

#include <iostream>
#include <vector>
using namespace std;

vector<int> maxEndingAt(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    ans[0] = nums[0];
    for (int i = 1; i < n; i++)
    {
        ans[i] = max(nums[i], ans[i - 1] + nums[i]);
    }
    return ans;
}

vector<int> bestLeftPrefix(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    vector<int> temp = maxEndingAt(nums);
    ans[0] = temp[0];
    for (int i = 1; i < temp.size(); i++)
    {
        ans[i] = max(ans[i - 1], temp[i]);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> nums, vector<int> exp, int t)
{
    vector<int> got = bestLeftPrefix(nums);
    bool ok = (got == exp);
    cout << "T" << t << ": " << (ok ? "PASS" : "FAIL") << "  got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    check({1, 2, -7, 8, 6, -4}, {1, 3, 3, 8, 14, 14}, 1);
    check({5, 4, -1, 7, 8}, {5, 9, 9, 15, 23}, 2);
    check({2, 1, -99, 3, 3}, {2, 3, 3, 3, 6}, 3);
    check({-2, -1}, {-2, -1}, 4);
    check({5, -2, -2, -1}, {5, 5, 5, 5}, 5);
    return 0;
}
