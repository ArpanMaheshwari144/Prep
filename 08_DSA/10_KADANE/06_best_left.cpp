// ============================================================
// BEST SUBARRAY IN PREFIX 0..i  (bestLeft[])
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
//  bestLeft[i] = "0..i ke ANDAR best subarray" (kahin bhi khatam ho, bas i ke andar).
//  end[i] = EXACTLY i pe khatam best. to 0..i me best = end[0..i] ka sabse bada = RUNNING MAX.
//  step: temp = maxEndingAt(nums)  [end[] reuse] -> ans[i] = max(ans[i-1], temp[i]).
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> maxEndingAt(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(n, 0);
    ans = nums;

    for (int i = 1; i < n; i++)
    {
        // prefix sum hai bus yaha negive hia unk aage bmta negetove hata do rests isk matba nhai hia
        ans[i] = max(ans[i], nums[i] + ans[i - 1]);
    }
    return ans;
}

vector<int> bestLeftPrefix(vector<int> &nums)
{
    vector<int> ans(nums.size(), 0);
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
    return 0;
}
