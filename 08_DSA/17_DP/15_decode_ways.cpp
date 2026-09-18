// ============================================================
// DECODE WAYS  (LC 91)   [DP #15]
// ============================================================
//
// Ek digit-string di hai. Mapping: '1'=A, '2'=B, ... '26'=Z.
// Batao string ko KITNE alag tareeke se decode kiya ja sakta hai.
//
//   "226"  ->  3
//                "2 2 6"   = B B F
//                "22 6"    = V F
//                "2 26"    = B Z
//
// ★ GINTI chahiye — saari strings NAHI. (count, na ki list)
//
// TEST-CASES:
//   "12"      -> 2     ("1 2" = AB · "12" = L)
//   "226"     -> 3
//   "06"      -> 0     ★ leading zero se kuch decode hota hi nahi
//   "10"      -> 1     ("10" = J only; "1 0" invalid kyunki '0' akela kuch nahi)
//   "100"     -> 0     ★ "1 00" · "10 0" — dono me akela '0' bach jaata
//   "2101"    -> 1     ("2 10 1")
//   "11106"   -> 2     ("1 1 10 6" · "11 10 6")
//   "1"       -> 1
//   "0"       -> 0
//   "27"      -> 1     ★ 27 > 26 — do-digit wala chalta hi nahi
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

int solve(int i, string s, int n, vector<int> &dp)
{
    if (i == n)
    {
        return 1;      // poori string kat gayi = 1 valid tareeka mila
    }

    if (s[i] == '0')
    {
        return 0;      // '0' se koi decode SHURU hota hi nahi -> ye raasta mara
    }

    if (dp[i] != -1)
    {
        return dp[i];
    }

    int ans = solve(i + 1, s, n, dp);
    if (i + 1 < n)
    {
        if (s[i] == '1' || (s[i] == '2' && s[i + 1] <= '6'))
            ans += solve(i + 2, s, n, dp);
    }
    return dp[i] = ans;
}

int numDecodings(string s)
{
    vector<int> dp(s.size() + 1, -1);
    return solve(0, s, s.size(), dp);
}

// ------------------------------------------------------------
// TEST HARNESS
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

    cout << "\n"
         << pass_count << "/" << total << " passed\n";
    return 0;
}
