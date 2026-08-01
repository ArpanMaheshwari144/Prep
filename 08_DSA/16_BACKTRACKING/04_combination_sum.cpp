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

// ---- APPROACH ----  (DONO form is file me: solve()=INCLUDE/EXCLUDE · solve2()=FOR-LOOP. same output.)
//  COMMON: REUSE allowed (recurse me index/i SAME rehta, +1 nahi) · target ghatao · target==0 -> record · target<0 -> prune.
//
//  FORM 1 — solve()  INCLUDE/EXCLUDE (2-branch):
//     base: target<0 -> return ; index>=size -> (target==0 ? record) -> return.
//     INCLUDE: push cand[index] -> solve(index SAME = REUSE, target-cand[index]) -> pop.
//     EXCLUDE: solve(index+1, target) bina push -> is element CHHODA, aage badho.
//
//  FORM 2 — solve2()  FOR-LOOP (start-loop):
//     base: target<0 -> return ; target==0 -> record + return.
//     for(i=start..n): push cand[i] -> solve2(i = REUSE, target-cand[i]) -> pop.
//
//  ★ FARAK: form1 = element pe liya/chhoda (2 call) · form2 = loop se har candidate branch. dono me reuse (index/i SAME).
// same subsets jaisa code, bas target aa gaya -- ab aur kuch nahi
// FORM 1 — INCLUDE/EXCLUDE (2-branch): har element pe liya(index same=reuse) / chhoda(index+1).
void solve(vector<int> &candidates, int target, vector<vector<int>> &ans, int index, vector<int> &temp)
{
    if (target < 0)
        return;

    if (index >= candidates.size())
    {
        if (target == 0)
        {
            ans.push_back(temp);
            
        }
        return;
        
    }

    temp.push_back(candidates[index]);
    solve(candidates, target - candidates[index], ans, index, temp);
    temp.pop_back();

    solve(candidates, target, ans, index + 1, temp);
}

// FORM 2 — FOR-LOOP (start-loop): loop se har candidate pe branch; recurse 'i' (reuse); target ghatao.
void solve2(vector<int> &candidates, int target, vector<vector<int>> &ans, int index, vector<int> &temp)
{
    if (target < 0)
        return;

    if (target == 0)
    {
        ans.push_back(temp);
        return;
    }

    for (int i = index; i < candidates.size(); i++)
    {
        temp.push_back(candidates[i]);                            // CHOOSE
        solve2(candidates, target - candidates[i], ans, i, temp); // EXPLORE (i -> reuse)
        temp.pop_back();                                          // UN-CHOOSE
    }
}

vector<vector<int>> combinationSum(vector<int> &candidates, int target)
{
    vector<vector<int>> ans;
    vector<int> temp;


    solve(candidates, target, ans, 0, temp);
    // solve2(candidates, target, ans, 0, temp);
    
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
