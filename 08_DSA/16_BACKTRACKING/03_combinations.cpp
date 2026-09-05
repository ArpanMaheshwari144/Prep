// ============================================================
// COMBINATIONS — (LeetCode 77, Medium)   [BACKTRACKING #3 · start + fixed size k]
// ============================================================
// 1..n me se k numbers ke SAARE combinations (order matter NAHI) lautao.
//
//   n=4, k=2  ->  [1,2],[1,3],[1,4],[2,3],[2,4],[3,4]     (C(4,2) = 6)
//   n=1, k=1  ->  [1]
//   n=3, k=3  ->  [1,2,3]
//
//   family me jagah:
//     - SUBSETS      = start-aage, HAR node record (saari sizes, 2^n).
//     - COMBINATIONS = start-aage (subsets jaisa), record SIRF jab temp.size()==k (fixed size).
//     - PERMUTATION  = used[] + loop-0 (order matter).
//   base: temp.size() == k -> ans me daalo -> return.
//   loop i = start..n; choose i -> explore(i+1) -> un-choose.  (i+1 -> order nahi, aage hi)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
void solve(int n, int k, vector<vector<int>> &ans, int index, vector<int> &temp)
{
    // base case: temp me k elements aa gaye -> ek combination poora
    if (temp.size() == k)
    {
        ans.push_back(temp);
        return;
    }

    for (int i = index; i <= n; i++)  // 1..n numbers, 'index' se aage
    {
        temp.push_back(i);            // CHOOSE
        solve(n, k, ans, i + 1, temp); // EXPLORE (i+1 -> order nahi)
        temp.pop_back();              // UN-CHOOSE (backtrack)
    }
}

vector<vector<int>> subsets(int n, int k)
{
    vector<vector<int>> ans;
    vector<int> temp; // ye BAHAR hi banega -- agar helper me banate to har call naya banta (backtrack toot jaata)
    solve(n, k, ans, 1, temp);
    return ans;
}

vector<vector<int>> combine(int n, int k)
{
    return subsets(n, k);
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(int n, int k, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = combine(n, k);
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
    check(4, 2, {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}}, 1);
    check(1, 1, {{1}}, 2);
    check(3, 3, {{1, 2, 3}}, 3);
    return 0;
}
