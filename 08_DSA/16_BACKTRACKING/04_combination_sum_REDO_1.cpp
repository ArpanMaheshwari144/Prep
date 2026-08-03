// ============================================================
// COMBINATION SUM — REDO   [LC-39, BACKTRACKING #4 · spaced 1-day lock]
// ============================================================
// candidates[] (distinct positive). SAARE combos jinka SUM == target. SAME number UNLIMITED baar (reuse).
//
//   [2,3,6,7], t=7  -> [[2,2,3],[7]]   ·   [2,3,5], t=8 -> [[2,2,2,2],[2,3,3],[3,5]]
//   [2], t=1 -> []   ·   [2], t=4 -> [[2,2]]
//
//   combinations jaisa, 2 tweak: recurse 'i' (REUSE, i+1 nahi) + base = TARGET (0=record, <0=prune).
//     (dono form allowed: for-loop ya include/exclude — jo yaad aaye)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp, int target)
{
    if (target < 0)
        return;

    if (index >= nums.size())
    {
        if (target == 0)
        {
            ans.push_back(temp);
        }
        return;
    }

    // INCLUDE: element le liya
    temp.push_back(nums[index]);
    solve(nums, ans, index, temp, target - nums[index]); // aage badho
    temp.pop_back();               // aur usko hata diya (un-choose / backtrack)

    // EXCLUDE: element nahi liya
    solve(nums, ans, index + 1, temp, target);
}

vector<vector<int>> subsets(vector<int> &nums, int target)
{
    vector<vector<int>> ans;
    vector<int> temp; // ye BAHAR hi banega -- agar helper me banate to har call naya banta (backtrack toot jaata)
    solve(nums, ans, 0, temp, target);
    return ans;
}

vector<vector<int>> combinationSum(vector<int> &candidates, int target)
{
    return subsets(candidates, target);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> cand, int target, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = combinationSum(cand, target);
    for (auto &v : got)
        sort(v.begin(), v.end());
    for (auto &v : exp)
        sort(v.begin(), v.end());
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  count=" << got.size() << " (exp " << exp.size() << ")\n";
}

int main()
{
    check({2, 3, 6, 7}, 7, {{2, 2, 3}, {7}}, 1);
    check({2, 3, 5}, 8, {{2, 2, 2, 2}, {2, 3, 3}, {3, 5}}, 2);
    check({2}, 1, {}, 3);
    check({2}, 4, {{2, 2}}, 4);
    return 0;
}
