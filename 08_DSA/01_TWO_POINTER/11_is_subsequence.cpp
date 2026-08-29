// ============================================================
// IS SUBSEQUENCE — Two Pointer  (REDO — paper pehle, code baad me)
// ============================================================
// do string s aur t. TRUE agar s, t ki SUBSEQUENCE hai (t me se kuch char
// HATA ke s ban jaaye, order same rahe).
//   s="abc", t="ahbgdc"  -> true
//   s="axc", t="ahbgdc"  -> false

// ---- ARPAN KI APPROACH ----
//  do string di hain. do pointer lo -> i for s, j for t.
//  s[i]==t[j] (char mil gaya) -> i++ AUR j++.  warna -> sirf j++ (t aage chalta rahega).
//  jab i, s ko paar kar le (i >= s.size()) -> saare s-char mil gaye -> TRUE. warna FALSE.

//
// Tests (s, t -> expected):
//   "abc",  "ahbgdc"   -> 1
//   "axc",  "ahbgdc"   -> 0
//   "",     "abc"      -> 1
//   "abc",  ""         -> 0
//   "aaa",  "aa"       -> 0
//   "ace",  "abcde"    -> 1
// ============================================================

#include <iostream>
#include <string>
using namespace std;

bool isSubsequence(string s, string t)
{
    int i = 0, j = 0;
    while (i < s.size() && j < t.size())
    {
        if (s[i] == t[j])
        {
            i++;
            j++;
        }
        else
        {
            j++;
        }
    }
    if (i >= s.size())
        return true;
    return false;
}

int main()
{
    cout << isSubsequence("abc", "ahbgdc") << " (expected 1)\n";
    cout << isSubsequence("axc", "ahbgdc") << " (expected 0)\n";
    cout << isSubsequence("", "abc") << " (expected 1)\n";
    cout << isSubsequence("abc", "") << " (expected 0)\n";
    cout << isSubsequence("aaa", "aa") << " (expected 0)\n";
    cout << isSubsequence("ace", "abcde") << " (expected 1)\n";
    return 0;
}
