// ============================================================
// PALINDROMIC SUBSTRINGS (LC 647) — REDO_1  (blank retrieval, spaced)
// ============================================================
//
// String di hai. Kitne SUBSTRING (contiguous) palindrome hain? COUNT karo.
// Single char bhi palindrome. Same-dikhne wale par ALAG position wale ALAG count hote.
//
//   "abc"  -> 3    (a, b, c)
//   "aaa"  -> 6    (a,a,a, aa,aa, aaa)
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

void solve(int i, int j, string s, int &count)
{
    while (i >= 0 && j < s.size() && s[i] == s[j])
    {
        i--;
        j++;
        count++;
    }
}

int countSubstrings(string s)
{
    int count = 0;
    for (int i = 0; i < s.size(); i++)
    {
        solve(i, i, s, count);
        solve(i, i + 1, s, count);
    }
    return count;
}

// ------------------------------------------------------------
// TEST HARNESS
// ------------------------------------------------------------
int main()
{
    struct T
    {
        string s;
        int expected;
    };
    vector<T> tests = {
        {"abc", 3},   // sirf single chars
        {"aaa", 6},   // a,a,a + aa,aa + aaa
        {"a", 1},     // single char
        {"abba", 6},  // a,b,b,a + bb + abba
        {"abcba", 7}, // a,b,c,b,a + bcb + abcba
        {"aaaa", 10}, // 4 + 3 + 2 + 1
    };

    int pass = 0;
    for (int i = 0; i < (int)tests.size(); i++)
    {
        int got = countSubstrings(tests[i].s);
        bool ok = (got == tests[i].expected);
        cout << "Test " << (i + 1) << " (\"" << tests[i].s << "\"): got=" << got
             << " expected=" << tests[i].expected
             << "  -> " << (ok ? "PASS" : "FAIL") << "\n";
        pass += ok;
    }
    cout << "\n"
         << pass << "/" << tests.size() << " passed\n";
    return 0;
}
