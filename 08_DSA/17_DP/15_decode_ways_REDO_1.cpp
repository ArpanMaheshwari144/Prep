// ============================================================
// DECODE WAYS — (LeetCode 91)  [REDO_1]
// ============================================================
// Ek string di hai jisme sirf ank (0-9) hain. Use decode karna hai is mapping se:
//
//      'A' -> 1   'B' -> 2   ...   'Z' -> 26
//
// Ginti karni hai ki poori string ko decode karne ke KITNE tareeke hain.
//
//      "12"    -> 2       ("AB" = 1,2   ya   "L" = 12)
//      "226"   -> 3       ("BBF" = 2,2,6  |  "BZ" = 2,26  |  "VF" = 22,6)
//      "06"    -> 0
//      "27"    -> 1
//
// Answer 32-bit int me aa jayega.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int solve(int i, string s, int n, vector<int> &dp)
{
    if (i == n)
    {
        return 1;
    }

    if (s[i] == '0')
    {
        return 0;
    }

    if (dp[i] != -1)
    {
        return dp[i];
    }

    int count = solve(i + 1, s, n, dp);
    if (i + 1 < n)
    {
        if (s[i] == '1' || (s[i] == '2' && s[i + 1] <= '6'))
        {
            count += solve(i + 2, s, n, dp);
        }
    }
    return dp[i] = count;
}

int numDecodings(string s)
{
    int n = s.size();
    vector<int> dp(n + 1, -1);
    return solve(0, s, n, dp);
}

// ------------------------------------------------------------
// TEST HARNESS (haath mat lagana)
// ------------------------------------------------------------
int pass_count = 0, total = 0;

void check(string s, int exp)
{
    int got = numDecodings(s);
    bool ok = (got == exp);
    pass_count += ok;
    total++;
    cout << (ok ? "PASS" : "FAIL")
         << "  s=\"" << s << "\""
         << string(max(0, 8 - (int)s.size()), ' ')
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    check("12", 2);
    check("226", 3);
    check("06", 0);
    check("10", 1);
    check("100", 0);
    check("2101", 1);
    check("11106", 2);
    check("1", 1);
    check("0", 0);
    check("27", 1);
    check("301", 0);
    check("1201234", 3);

    cout << "\n"
         << pass_count << "/" << total << " passed\n";
    return 0;
}
