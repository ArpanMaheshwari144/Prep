// ============================================================
// PERMUTATIONS — (LeetCode 46, Medium)   [BACKTRACKING #2 · used[] + full-length]
// ============================================================
// nums[] (saare UNIQUE). SAARE permutations (har possible ORDER) lautao — koi bhi order.
//
//   [1,2,3]  ->  [1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]   (3! = 6)
//   [0,1]    ->  [0,1],[1,0]
//   [1]      ->  [1]
//
//   ★ subsets se FARAK:
//     - subsets: 'start' index (sirf aage) -> [1,2]==[2,1] repeat nahi.
//     - permutation: har position pe koi bhi UNUSED element -> used[] track, loop 0 se, used skip.
//   ★ base case: temp.size() == nums.size() (poora arrangement ban gaya) -> ans me daalo.
//   ★ CHOOSE: used[i]=true + push -> EXPLORE -> UN-CHOOSE: pop + used[i]=false.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (used[] track + FULL-LENGTH base — har position pe koi bhi bacha element)
//  ★ subsets se FARAK: subsets me 'start' (sirf aage) -> [1,2]==[2,1] nahi. permutation me ORDER matter -> har
//    position pe koi bhi UNUSED element -> loop 0 se + used[] se skip.
//  1. base: temp.size() == nums.size() -> poora arrangement -> ans me daalo -> RETURN.
//  2. for(i=0..n-1): used[i]? -> continue (skip already-liya).
//  3. CHOOSE   : used[i]=true; temp.push(nums[i]).
//  4. EXPLORE  : solve(...)   (koi 'start'/i+1 nahi -> phir 0 se, bas used skip).
//  5. UN-CHOOSE: temp.pop(); used[i]=false   (backtrack -> dono undo).
//  ★ n! permutations. used[] + temp DONO by-reference (shared).
//  ★★ INDEX/start KYU NAHI PASS kiya? -> order MATTER -> har position pe koi bhi UNUSED (peeche wala bhi)
//     -> loop HAMESHA 0-se -> "peeche mat jao" wali baat nahi -> 'start' ki zaroorat nahi, uski jagah used[].
//     (subsets/combinations me start = "peeche mat jao" (duplicate-rok, order-agnostic); yahan wo nahi chahiye.)
// ============================================================
void solve(vector<int> &nums, vector<vector<int>> &ans, vector<int> &temp, vector<bool> &used)
{
    if (temp.size() == nums.size())
    {
        ans.push_back(temp);
        return;
    }

    for (int i = 0; i < nums.size(); i++)
    {
        if (used[i] == true)
            continue;

        // choose
        used[i] = true;
        temp.push_back(nums[i]);

        // explore
        solve(nums, ans, temp, used);

        // unchoose
        temp.pop_back();
        used[i] = false;
    }
}

vector<vector<int>> permute(vector<int> &nums)
{
    vector<vector<int>> ans;
    vector<bool> used(nums.size(), false);
    vector<int> temp;
    solve(nums, ans, temp, used);
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = permute(in);
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  count=" << got.size() << " (exp " << exp.size() << ")\n";
}

int main()
{
    check({1, 2, 3}, {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}}, 1);
    check({0, 1}, {{0, 1}, {1, 0}}, 2);
    check({1}, {{1}}, 3);
    return 0;
}
