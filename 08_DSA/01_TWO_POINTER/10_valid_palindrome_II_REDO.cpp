// ============================================================
// REDO — VALID PALINDROME II   (blank se, dekhe bina)
// ============================================================
// string s (lowercase). TRUE agar s palindrome hai, YA zyada-se-zyada
// EK character DELETE karke palindrome ban sakti hai.
//   "aba"    -> true
//   "abca"   -> true   ('c' delete -> "aba")
//   "abc"    -> false
//


//
// Tests (s -> expected):
//   "aba"        -> 1
//   "abca"       -> 1
//   "abc"        -> 0
//   "a"          -> 1
//   "deeee"      -> 1
//   "cbbcc"      -> 1
//   "abcdba"     -> 1
//   "caaab"      -> 0
// ============================================================

#include <iostream>
#include <string>
using namespace std;

bool isPalindrome(string s, int left, int right)
{
    while (left <= right)
    {
        if (s[left] != s[right])
        {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

bool validPalindrome(string s)
{
    int left = 0;
    int right = s.size() - 1;
    while (left <= right)
    {
        if (s[left] != s[right])
        {
            return isPalindrome(s, left + 1, right) || isPalindrome(s, left, right - 1);
        }
        left++;
        right--;
    }
    return true;
}

int main()
{
    cout << validPalindrome("aba") << " (expected 1)\n";
    cout << validPalindrome("abca") << " (expected 1)\n";
    cout << validPalindrome("abc") << " (expected 0)\n";
    cout << validPalindrome("a") << " (expected 1)\n";
    cout << validPalindrome("deeee") << " (expected 1)\n";
    cout << validPalindrome("cbbcc") << " (expected 1)\n";
    cout << validPalindrome("abcdba") << " (expected 1)\n";
    cout << validPalindrome("caaab") << " (expected 0)\n";
    return 0;
}
