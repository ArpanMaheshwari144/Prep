// ============================================================
// VALID PARENTHESIS with '*'  (LC-678)
// ============================================================
// string s me sirf '(' , ')' , '*'.  '*' = '(' YA ')' YA khaali (empty) ban sakta.
// batao: kya string VALID parenthesis ban SAKTI hai? (har '(' ka matching ')' , order sahi)
//
//
// ---- TEST CASES (s -> expected) ----
//   "()"        -> true
//   "(*)"       -> true
//   "(*))"      -> true
//   ")("        -> false
//   "(((**"     -> false
//   "((*)"      -> true
//   ""          -> true
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool solve(string &s, int index, int open, vector<vector<int>> &dp)
{
    if (index == s.size())
    {
        return open == 0;
    }

    if (dp[index][open] != -1)
    {
        return dp[index][open];
    }

    if (s[index] == '(')
    {
        return dp[index][open] = solve(s, index + 1, open + 1, dp);
    }
    else if (s[index] == ')')
    {
        if (open > 0)
        {
            return dp[index][open] = solve(s, index + 1, open - 1, dp);
        }
        else
        {
            return false;
        }
    }

    bool open_for_astrisk = solve(s, index + 1, open + 1, dp);

    bool close_for_astrisk;
    if (open > 0)
        close_for_astrisk = solve(s, index + 1, open - 1, dp);
    else
        close_for_astrisk = false;

    bool none = solve(s, index + 1, open, dp);

    return dp[index][open] = open_for_astrisk || close_for_astrisk || none;
}

bool checkValidString(string s)
{
    int n = s.size();
    vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
    return solve(s, 0, 0, dp);
}

// ---- test helper (ise mat chhed) ----
void check(bool got, bool exp, const string &label)
{
    cout << label << " -> got " << (got ? "true" : "false")
         << " | exp " << (exp ? "true" : "false")
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check(checkValidString("()"), true, "case1");
    check(checkValidString("(*)"), true, "case2");
    check(checkValidString("(*))"), true, "case3");
    check(checkValidString(")("), false, "case4");
    check(checkValidString("(((**"), false, "case5");
    check(checkValidString("((*)"), true, "case6");
    check(checkValidString(""), true, "case7");
    return 0;
}
