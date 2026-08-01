// ============================================================
// SUBSETS — REDO (for-loop / START-LOOP form)   [LC-78, BACKTRACKING #1 alt]
// ============================================================
// SAME problem (saare subsets), par doosri shakal: FOR-LOOP (start-loop) form.
//
//   [1,2,3]  ->  [], [1], [2], [3], [1,2], [1,3], [2,3], [1,2,3]   (2^3 = 8)
//   [0]      ->  [], [0]
//   [1,2]    ->  [], [1], [2], [1,2]
//
//   ★ start-loop idea (include/exclude se alag likhawat):
//     - HAR node pe current temp ans me daalo (alag base-case nahi -- har node = ek subset)
//     - for(i = start .. n-1):  CHOOSE (push nums[i]) -> EXPLORE (solve i+1) -> UN-CHOOSE (pop)
//     - 'start' aage badhta (i+1) -> peeche wale dobara na aaye
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp)
{
    ans.push_back(temp);
    for (int i = index; i < nums.size(); i++)
    {
        temp.push_back(nums[i]);
        solve(nums, ans, i + 1, temp);
        temp.pop_back();
    }
}

vector<vector<int>> subsets(vector<int> &nums)
{
    vector<vector<int>> ans;
    vector<int> temp; // ye BAHAR hi banega -- agar helper me banate to har call naya banta (backtrack toot jaata)
    solve(nums, ans, 0, temp);
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = subsets(in);
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
    check({1, 2, 3}, {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}}, 1);
    check({0}, {{}, {0}}, 2);
    check({1, 2}, {{}, {1}, {2}, {1, 2}}, 3);
    return 0;
}
