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
// ---- ARPAN KI APPROACH ----
//  COUNTER (stack ki zaroorat nahi -- sirf ( ) hain). do counter:
//    close_needed = jab '(' aaye -> close_needed++.
//    open_needed  = jab ')' aaye aur koi '(' unmatched na ho -> uske liye '(' chahiye.
//  ')' aaye to:
//    - close_needed > 0 -> close_needed-- (match ho gaya). aur jab close_needed 0 ho jaaye ->
//      matlab poora bracket valid ho gaya -> continue (aage kuch check nahi).
//    - warna else -> open_needed++ (is ')' ke liye koi '(' tha hi nahi).
//  answer = open_needed + close_needed (jo ')' pehle chahiye the + jo '(' aakhir me unmatched bache).
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
        else if (it == ')')
        {
            if (close_needed > 0)
            {
                close_needed--;
                if (close_needed == 0)
                {
                    continue;
                }
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
