// ============================================================
// REVERSE VOWELS OF A STRING — Two Pointer  (fresh)
// ============================================================
// string s di hai. SIRF vowels (a,e,i,o,u -- lower & upper dono) ko aapas me
// REVERSE karo, baaki characters apni jagah par rahein. nayi string lautao.
//   "hello"   -> "holle"   (e,o swap; baaki same)
//   "leetcode"-> "leotcede"
//   "aA"      -> "Aa"
//

/*
---- ARPAN KI APPROACH ----
ye easy hai -- MOVE-ZEROES jaisa hi concept, bas thoda different. do pointer: ek start, ek end.
dekho dono vowel hain kya -> dono vowel to swap kar do, phir left++ AUR right--.
agar left vowel nahi -> left++.  else (right vowel nahi) -> right--.
bas itna hi, isme aur zyada khaas kuch nahi.
*/

// Tests (s -> expected):
//   "hello"     -> "holle"
//   "leetcode"  -> "leotcede"
//   "aA"        -> "Aa"
//   "bcdfg"     -> "bcdfg"    (koi vowel nahi -> same)
//   "AEIOU"     -> "UOIEA"
//   ""          -> ""
// ============================================================

#include <iostream>
#include <string>
using namespace std;

bool isVowel(char ch)
{
    if ((ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') ||
        (ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U'))
    {
        return true;
    }
    return false;
}

string reverseVowels(string s)
{
    int left = 0;
    int right = s.size() - 1;
    while (left < right)
    {
        if (isVowel(s[left]) && isVowel(s[right]))
        {
            swap(s[left], s[right]);
            left++;
            right--;
        }
        else if (!isVowel(s[left]))
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    return s;
}

int main()
{
    cout << reverseVowels("hello") << "  (expected holle)\n";
    cout << reverseVowels("leetcode") << "  (expected leotcede)\n";
    cout << reverseVowels("aA") << "  (expected Aa)\n";
    cout << reverseVowels("bcdfg") << "  (expected bcdfg)\n";
    cout << reverseVowels("AEIOU") << "  (expected UOIEA)\n";
    cout << reverseVowels("") << "  (expected <empty>)\n";
    return 0;
}
