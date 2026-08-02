// ============================================================
// PERMUTATIONS — REDO   [LC-46, BACKTRACKING #2 · spaced 1-day lock]
// ============================================================
// nums[] (UNIQUE). SAARE permutations (har ORDER) lautao.
//
//   [1,2,3]  ->  [1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]   (3! = 6)
//   [0,1]    ->  [0,1],[1,0]
//   [1]      ->  [1]
//
//   ★ used[] track + loop 0-se (start nahi) + base temp.size()==n.
//     CHOOSE: used[i]=true, push -> EXPLORE -> UN-CHOOSE: pop, used[i]=false.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ★ INDEX/start KYU NAHI PASS kiya? -> permutation me ORDER matter -> har position pe koi bhi UNUSED
//   element (peeche wala bhi) chahiye -> loop HAMESHA 0-se -> "peeche mat jao" wali baat nahi -> start ki
//   zaroorat nahi, uski jagah used[] ("same element dobara mat lo").
//   (subsets/combinations me start = "peeche mat jao" = duplicate-rok, order-agnostic; yahan wo nahi chahiye.)
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

        used[i] = true;
        temp.push_back(nums[i]);
        solve(nums, ans, temp, used);

        used[i] = false;
        temp.pop_back();
    }
}

vector<vector<int>> permute(vector<int> &nums)
{
    vector<vector<int>> ans;
    vector<int> temp;
    vector<bool> used(nums.size(), false);
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
