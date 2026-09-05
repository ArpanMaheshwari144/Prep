// ============================================================
// VALID PARENTHESES   (LC-20 — REDO_1, stack open/close-match)
// ============================================================
// string s me sirf brackets: ( ) { } [ ]. Batao valid hai ya nahi.
// valid = har opening ka SAHI closing, sahi ORDER me (nested + sequence dono).
//
//   "()"      -> true
//   "()[]{}"  -> true
//   "(]"      -> false   (galat match)
//   "([)]"    -> false   (galat order — cross)
//   "{[]}"    -> true    (nested sahi)
//
// ---- TEST CASES ----
//   "()"          -> true
//   "()[]{}"      -> true
//   "(]"          -> false
//   "([)]"        -> false
//   "{[]}"        -> true
//   "("           -> false   (unmatched open)
//   ")"           -> false   (closing pe stack khaali)
//   ""            -> true    (khaali = valid)
//   "(((("        -> false
//   "((()))"      -> true
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isValid(string s)
{
    stack<int> st;
    for (auto &it : s)
    {
        if (it == '(' || it == '[' || it == '{')
        {
            st.push(it);
        }
        else
        {
            if (st.empty())
                return false;
            
            char ch = st.top();
            st.pop();
            if ((ch == '(' && it == ')') || (ch == '[' && it == ']') || (ch == '{' && it == '}'))
            {
                continue;
            }
            else
            {
                return false;
            }
        }
    }
    return st.empty();
}

// ---- test helper (ise mat chhed) ----
void check(bool got, bool exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check(isValid("()"), true, "case1");
    check(isValid("()[]{}"), true, "case2");
    check(isValid("(]"), false, "case3");
    check(isValid("([)]"), false, "case4");
    check(isValid("{[]}"), true, "case5");
    check(isValid("("), false, "case6");
    check(isValid(")"), false, "case7");
    check(isValid(""), true, "case8");
    check(isValid("(((("), false, "case9");
    check(isValid("((()))"), true, "case10");
    return 0;
}
