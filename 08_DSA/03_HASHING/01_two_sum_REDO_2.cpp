// ============================================================
// TWO SUM   (LC-1 — REDO_2, hashmap complement)
// ============================================================
// nums[] + target. do INDICES lauta do jinke values ka sum == target.
// har input me exactly ek jodi hoti. ek element do baar use nahi.
// (order koi bhi — [0,1] ya [1,0] dono theek maana test me.)
//
//   nums=[2,7,11,15], target=9  -> [0,1]   (2+7)
//   nums=[3,2,4],     target=6  -> [1,2]   (2+4)
//   nums=[3,3],       target=6  -> [0,1]
//
// ---- TEST CASES (nums, target -> expected indices, order-free) ----
//   [2,7,11,15], 9   -> {0,1}
//   [3,2,4],     6   -> {1,2}
//   [3,3],       6   -> {0,1}
//   [1,5,3,8],   11  -> {2,3}     (3+8)
//   [-1,-2,-3,-4,-5], -8 -> {2,4} (-3 + -5)
//   [0,4,3,0],   0   -> {0,3}     (0+0)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> twoSum(vector<int> &nums, int target)
{
    vector<int> ans;
    unordered_map<int, int> mp;
    for (int i = 0; i < nums.size(); i++)
    {
        if (mp.find(target - nums[i]) != mp.end())
        {
            return {mp[target - nums[i]], i};
        }
        mp[nums[i]] = i;
    }
    return {-1, -1};
}

// ---- test helper (ise mat chhed) — order-free compare ----
void check(vector<int> got, vector<int> exp, const string &label)
{
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    bool ok = (got == exp);
    cout << label << " -> got {" << (got.size() ? to_string(got[0]) : "") << ","
         << (got.size() > 1 ? to_string(got[1]) : "") << "} | exp {"
         << exp[0] << "," << exp[1] << "}"
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {2, 7, 11, 15};
    check(twoSum(a1, 9), {0, 1}, "case1");
    vector<int> a2 = {3, 2, 4};
    check(twoSum(a2, 6), {1, 2}, "case2");
    vector<int> a3 = {3, 3};
    check(twoSum(a3, 6), {0, 1}, "case3");
    vector<int> a4 = {1, 5, 3, 8};
    check(twoSum(a4, 11), {2, 3}, "case4");
    vector<int> a5 = {-1, -2, -3, -4, -5};
    check(twoSum(a5, -8), {2, 4}, "case5");
    vector<int> a6 = {0, 4, 3, 0};
    check(twoSum(a6, 0), {0, 3}, "case6");
    return 0;
}
