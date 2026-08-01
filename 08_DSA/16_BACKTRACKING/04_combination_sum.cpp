// ============================================================
// COMBINATION SUM — (LeetCode 39, Medium)   [BACKTRACKING #4 · reuse allowed + target]
// ============================================================
// candidates[] (distinct positive). SAARE unique combinations lautao jinka SUM == target.
// ★ SAME number UNLIMITED baar use kar sakte (reuse allowed).
//
//   candidates=[2,3,6,7], target=7  ->  [[2,2,3],[7]]
//   candidates=[2,3,5],   target=8  ->  [[2,2,2,2],[2,3,3],[3,5]]
//   candidates=[2],       target=1  ->  []            (nahi ban sakta)
//   candidates=[2],       target=4  ->  [[2,2]]
//
//   ★ combinations (LC-77) se FARAK: yahan REUSE allowed -> recurse me 'i' pass karo (i+1 NAHI)
//     -> same element dobara aa sake.
//   ★ base: target==0 -> ek valid combo -> record + return.  target<0 -> return (prune, aage koi faida nahi).
//   ★ CHOOSE (push cand[i], target-cand[i]) -> EXPLORE (start = i, reuse) -> UN-CHOOSE (pop).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (= COMBINATIONS code REUSE, 2 tweak: reuse-'i' + target-base)
//  ★ combinations (LC-77) ka SAME skeleton, bas 2 farak:
//     1. REUSE allowed -> recurse me 'i' pass (i+1 NAHI) -> same element dobara aa sake.
//     2. size==k base ki jagah TARGET-base: target==0 -> record ; target<0 -> return (prune).
//  FLOW: CHOOSE (push cand[i], target -= cand[i]) -> EXPLORE (start=i, reuse) -> UN-CHOOSE (pop).
//  ★ target<0 prune upar -> candidates positive -> target ghatta jaata -> infinite nahi.
//  ★ start=i (i+1 nahi) -> same element repeat OK, par peeche wale nahi (duplicate combo rok).
// same subets jaise coide vus tragte aa gay ab bus or kcuh nahi
void solve(vector<int> &candidates, int target, vector<vector<int>> &ans, int index, vector<int> &temp)
{
    if (target < 0)
        return;

    if (target == 0)
    {
        ans.push_back(temp);
    }

    for (int i = index; i < candidates.size(); i++)
    {
        // liya
        temp.push_back(candidates[i]);
        solve(candidates, target - candidates[i], ans, i, temp); // explore
        temp.pop_back(); // undo
    }
}

vector<vector<int>> combinationSum(vector<int> &candidates, int target)
{
    vector<vector<int>> ans;
    vector<int> temp;
    solve(candidates, target, ans, 0, temp);
    return ans;
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
