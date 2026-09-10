// ============================================================
// GENERATE PARENTHESES — (LeetCode 22, Medium)   [backtracking · constraint-guided]
// ============================================================
// n jode brackets diye. saare VALID (well-formed) combinations generate kar.
// GOAL: har string well-balanced ho.
//
//   n=1 -> ["()"]
//   n=2 -> ["(())","()()"]
//   n=3 -> ["((()))","(()())","(())()","()(())","()()()"]
//   (order matter nahi -- test order-independent compare karta.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
void solve(int left, int right, int n, string path, vector<string> &ans)
{
    if (left == n && right == n)
    {
        ans.push_back(path);
        return;
    }

    if (left < n)
    {
        solve(left + 1, right, n, path + '(', ans);
    }
    if (right < left)
    {
        solve(left, right + 1, n, path + ')', ans);
    }
}

vector<string> generateParenthesis(int n)
{
    vector<string> ans;
    string path;
    solve(0, 0, n, path, ans);
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
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
