// ============================================================
// MIN ADD TO MAKE PARENTHESES VALID — Stack / Counter  (fresh)
// ============================================================
// string s sirf '(' aur ')'. MINIMUM kitne brackets ADD karne padenge (kahin bhi)
// taaki string VALID ho jaaye (har '(' ka ')' match ho)? -> wo count lauta.
//   "())"   -> 1   (ek '(' add -> "(())")
//   "(((    -> 3   (teen ')' chahiye)
//   "()"    -> 0
//   ")("    -> 2
//
//
// Tests (s -> expected):
//   "())"      -> 1
//   "((("      -> 3
//   "()"       -> 0
//   ")("       -> 2
//   ""         -> 0
//   "()))(("   -> 4
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int minAddToMakeValid(string s)
{
    if (s.empty())
        return 0;

    int open_needed = 0;
    int close_needed = 0;

    for (auto &it : s)
    {
        if (it == '(') // cout << minAddToMakeValid("(())") << " (expected 0)\n";
        {
            close_needed++;
        }
        else
        {
            if (close_needed > 0)
            {
                close_needed--;
            }
            else
            {
                open_needed++;
            }
        }
    }
    return open_needed + close_needed;
}

int main()
{
    cout << minAddToMakeValid("())") << " (expected 1)\n";
    cout << minAddToMakeValid("(((") << " (expected 3)\n";
    cout << minAddToMakeValid("()") << " (expected 0)\n";
    cout << minAddToMakeValid(")(") << " (expected 2)\n";
    cout << minAddToMakeValid("") << " (expected 0)\n";
    cout << minAddToMakeValid("()))((") << " (expected 4)\n";
    cout << minAddToMakeValid("(())") << " (expected 0)\n";
    cout << minAddToMakeValid("(()") << " (expected 1)\n";
    cout << minAddToMakeValid("(()))") << " (expected 1)\n";
    return 0;
}
