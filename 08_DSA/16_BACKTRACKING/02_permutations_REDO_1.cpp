// ============================================================
// PERMUTATIONS — (LeetCode 46, Medium)   [BACKTRACKING #2 · used[] · REDO_1]
// ============================================================
// nums[] (saare UNIQUE). SAARE permutations (har possible ORDER) lautao — koi bhi order.
//
//   [1,2,3]  ->  [1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]   (3! = 6)
//   [0,1]    ->  [0,1],[1,0]
//   [1]      ->  [1]
//
// FAMILY (nudge, baaki KHUD): backtracking. subsets se FARAK — yahan 'start' index
//   nahi, har position pe koi bhi UNUSED element. used[] track kar. derive kar.
//
// (check() order-independent hai — got aur exp dono sort karke compare, count bhi.)
// ============================================================
#include <bits/stdc++.h>
using namespace std;

void solve(vector<int> &nums, vector<int> &temp, vector<bool> &used, vector<vector<int>> &ans)
{
    if (temp.size() == nums.size())
    {
        ans.push_back(temp);
        return;
    }

    for (int i = 0; i < nums.size(); i++)
    {
        if (used[i])
            continue;

        temp.push_back(nums[i]);
        used[i] = true;

        solve(nums, temp, used, ans);

        temp.pop_back();
        used[i] = false;
    }
}

vector<vector<int>> permute(vector<int> &nums)
{
    vector<vector<int>> ans;
    vector<int> temp;
    vector<bool> used(nums.size(), false);
    solve(nums, temp, used, ans);
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
