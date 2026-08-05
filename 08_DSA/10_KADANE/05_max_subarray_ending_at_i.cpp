// ============================================================
// MAX SUBARRAY ENDING AT EACH INDEX i   (end[] array)
// ============================================================
// ek int array diya. har index i ke liye nikaalo: "index i pe KHATAM hone waali"
// sabse-badi-sum contiguous subarray ka sum. (i pe end hona ZAROORI, aage nahi jaana.)
// in saari values ka ARRAY lautao (size N).
//
//   [1,2,-7,8,6,-4]   -> [1, 3, -4, 8, 14, 10]
//   [5,4,-1,7,8]      -> [5, 9, 8, 15, 23]
//   [2,1,-99,3,3]     -> [2, 3, -96, 3, 6]
//   [-2,-1]           -> [-2, -1]
// ============================================================

// ---- APPROACH ----   (= PREFIX-SUM + max/reset  = MAX SUBARRAY SUM ko per-index STORE)
//  TRICK: prefix-sum jaisa running total (ans[i-1] + nums[i]); par jab NEGATIVE ho -> phenk do, fresh (Kadane).
//         GLOBAL max NAHI -> HAR index ka value STORE.
//     ans[0] = nums[0];
//     ans[i] = max( nums[i] , ans[i-1] + nums[i] );   // prefix JOD, ya phenk ke akela nums[i] -- jo bada
//  [1,2,-7,8,6,-4] -> [1,3,-4,8,14,10]   (i=3: pichhla ans -4 bojh -> phenko -> sirf nums[3]=8)

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

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> nums, vector<int> exp, int t)
{
    vector<int> got = maxEndingAt(nums);
    bool ok = (got == exp);
    cout << "T" << t << ": " << (ok ? "PASS" : "FAIL") << "  got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    check({1, 2, -7, 8, 6, -4}, {1, 3, -4, 8, 14, 10}, 1);
    check({5, 4, -1, 7, 8}, {5, 9, 8, 15, 23}, 2);
    check({2, 1, -99, 3, 3}, {2, 3, -96, 3, 6}, 3);
    check({-2, -1}, {-2, -1}, 4);
    return 0;
}
