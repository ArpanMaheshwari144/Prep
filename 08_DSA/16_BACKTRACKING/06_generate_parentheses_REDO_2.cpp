// ============================================================
// GENERATE PARENTHESES (LC 22) — BACKTRACKING  |  REDO_2 (22-Sep, 12 din baad)
// REDO_1 10-Sep ko hua tha. Aaj KHAALI PAGE se dobara.
// Notes/sheet/purani file mat kholna. Atko to TAB peek karna.
// ============================================================
//
// PROBLEM:
//   n jode bracket diye hain. Saare VALID (well-formed) combination banao.
//
//   n=1  ->  ["()"]
//   n=2  ->  ["(())", "()()"]
//   n=3  ->  ["((()))", "(()())", "(())()", "()(())", "()()()"]
//
//   kram (order) maayne nahi rakhta -- test khud sort kar ke milata hai.
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

void solve(int open, int close, string path, vector<string> &ans, int n)
{
    if (open == n && close == n)
    {
        ans.push_back(path);
        return;
    }

    if (open < n)
    {
        solve(open + 1, close, path + '(', ans, n);
    }
    if (close < open)
    {
        solve(open, close + 1, path + ')', ans, n);
    }
}

vector<string> generateParenthesis(int n)
{
    vector<string> ans;
    string path;
    solve(0, 0, path, ans, n);
    return ans;
}

// ------------------------------------------------------------
int main()
{
    struct T
    {
        int n;
        vector<string> expected;
        string kyun;
    };

    vector<T> tests = {
        {0, {""}, "n=0 — ek khaali string (LeetCode yahi deta hai)"},
        {1, {"()"}, "sabse chhota"},
        {2, {"(())", "()()"}, "do shakal"},
        {3, {"((()))", "(()())", "(())()", "()(())", "()()()"}, "★ paanch — agar 4 aaye to dono branch pe return laga diya hai"},
        {4, {"(((())))", "((()()))", "((())())", "((()))()", "(()(()))", "(()()())", "(()())()", "(())(())", "(())()()", "()((()))", "()(()())", "()(())()", "()()(())", "()()()()"}, "★ 14 hone chahiye (Catalan) — ginti galat = koi branch kat raha hai"},
    };

    int pass = 0;
    for (int i = 0; i < (int)tests.size(); i++)
    {
        vector<string> got = generateParenthesis(tests[i].n);
        vector<string> exp = tests[i].expected;
        sort(begin(got), end(got));
        sort(begin(exp), end(exp));

        bool ok = (got == exp);
        pass += ok;

        cout << (ok ? "PASS  " : "FAIL  ")
             << "n=" << tests[i].n
             << "   expected " << exp.size() << " string"
             << ", mile " << got.size()
             << "   [" << tests[i].kyun << "]\n";

        if (!ok)
        {
            cout << "        mila: ";
            for (auto &x : got)
                cout << x << " ";
            cout << "\n        chahiye: ";
            for (auto &x : exp)
                cout << x << " ";
            cout << "\n";
        }
    }
    cout << "\n"
         << pass << " / " << tests.size() << "\n";
    return 0;
}
