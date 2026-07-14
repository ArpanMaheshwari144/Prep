// ============================================================
// REVERSE VOWELS OF A STRING — Two Pointer  (REDO — paper pehle, code baad me)
// ============================================================
// string s. SIRF vowels (a,e,i,o,u -- lower & upper) ko aapas me REVERSE karo,
// baaki characters apni jagah. nayi string lautao.
//   "hello"   -> "holle"
//   "leetcode"-> "leotcede"

// ---- ARPAN KI APPROACH ----
//  same SWAP pattern. sirf vowels ko swap karna hai.
//  left/right pointer -> dono vowel (isVowel(s[left]) && isVowel(s[right])) -> swap + left++ right--.
//  left vowel nahi -> left++.  warna (right vowel nahi) -> right--.

//
// Tests (s -> expected):
//   "hello"     -> "holle"
//   "leetcode"  -> "leotcede"
//   "aA"        -> "Aa"
//   "bcdfg"     -> "bcdfg"
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
