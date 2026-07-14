// ============================================================
// IS SUBSEQUENCE — Two Pointer  (fresh)
// ============================================================
// do string s aur t. TRUE agar s, t ki SUBSEQUENCE hai.
// subsequence = t me se kuch (ya koi nahi) characters HATA ke s ban jaaye,
// baaki characters ka ORDER same rahe.
//   s="abc", t="ahbgdc"  -> true   (a..b..c order me mil gaye)
//   s="axc", t="ahbgdc"  -> false  ('x' nahi mila)
//
// -> problem ko naye sire se padh. pattern baad me.

// ---- ARPAN KI APPROACH ----
//  do pointer: i for s, j for t.
//  s[i]==t[j] (char mil gaya) -> i++ AUR j++.  nahi mila -> sirf j++ (t aage scan karte raho).
//  loop tab tak jab tak dono range me. ANT me: agar i, s ke aakhir tak pahunch gaya (i >= s.size())
//     -> matlab s ke saare char order me mil gaye -> TRUE. warna FALSE.
//  (★ chhoti galti ki thi: s[i]==s[j] check kar raha tha -- sahi hai s[i]==t[j].)

//
// Tests (s, t -> expected):
//   "abc",  "ahbgdc"   -> 1
//   "axc",  "ahbgdc"   -> 0
//   "",     "abc"      -> 1     (khaali s har string ki subsequence)
//   "abc",  ""         -> 0
//   "aaa",  "aa"       -> 0     (s bada, mil hi nahi sakta)
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
    if(i >= s.size()){
        return true;
    }
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
