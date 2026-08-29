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
// ---- APPROACH ----   (= 05 (maxEndingAt) ka ARRAY le ke uska RUNNING-MAX)
//  TRICK: 05 ne "har index tak yahin-khatam best" ka ARRAY diya. bestLeft = us array ka AB-TAK-ka-max (0..i best).
//         running-max kabhi neeche nahi jaata.
//     temp = maxEndingAt(nums);              // 05 wala array
//     ans[0] = temp[0];
//     ans[i] = max( ans[i-1] , temp[i] );    // AB TAK ka max (ans[i-1], NA temp[i-1])
//  [1,2,-7,8,6,-4]:  temp=[1,3,-4,8,14,10]  ->  bestLeft=[1,3,3,8,14,14]
//  TRAP [5,-2,-2,-1]: temp=[5,3,1,0] -> [5,5,5,5]  (temp gira par ans TIKA -> ab-tak chahiye, sirf pichhla nahi)

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
