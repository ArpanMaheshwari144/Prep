// ============================================================
// GENERATE PARENTHESES — (LeetCode 22)  [REDO_1 · blank retrieval]
// ============================================================
// n jode brackets -> saare VALID (well-formed) combinations.
//   n=1 -> ["()"]
//   n=2 -> ["(())","()()"]
//   n=3 -> ["((()))","(()())","(())()","()(())","()()()"]  (order matter nahi)
// insight (khud yaad kar): backtracking, 2 rule -> '(' jab open<n · ')' jab close<open.
//   ★ TRAP: dono branch pe `return` mat lagana (first return 2nd branch maar deta).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void solve(int left, int right, vector<string> &ans, string path, int n)
{
    if (left == n && right == n)
    {
        ans.push_back(path);
    }

    if (left < n)
    {
        solve(left + 1, right, ans, path + '(', n);
    }
    if (right < left)
    {
        solve(left, right + 1, ans, path + ')', n);
    }
}

vector<string> generateParenthesis(int n)
{
    vector<string> ans;
    string path;
    solve(0, 0, ans, path, n);
    return ans;
}

// ─── TESTS (haath mat lagana) ──────────────────────────
void check(int n, vector<string> exp, int t)
{
    vector<string> got = generateParenthesis(n);
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got.size() << " strings, exp=" << exp.size() << "\n";
}

int main()
{
    check(1, {"()"}, 1);
    check(2, {"(())", "()()"}, 2);
    check(3, {"((()))", "(()())", "(())()", "()(())", "()()()"}, 3);
    return 0;
}
