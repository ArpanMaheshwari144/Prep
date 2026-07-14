// ============================================================
// REDO — VALID PALINDROME II   (blank se, dekhe bina)
// ============================================================
// string s (lowercase). TRUE agar s palindrome hai, YA zyada-se-zyada
// EK character DELETE karke palindrome ban sakti hai.
//   "aba"    -> true
//   "abca"   -> true   ('c' delete -> "aba")
//   "abc"    -> false
//

// ---- ARPAN KI APPROACH ----
//  isme kuch fancy nahi. two-pointer left/right se andar aao.
//  jab tak dono char SAME hain -> aage badho (left++, right--).
//  jaise hi MISMATCH mile -> bas yahi wo jagah hai jahan ek delete ka mauka lagta:
//    ya LEFT wala delete (left+1) karke dekho baaki palindrome hai kya,
//    ya RIGHT wala delete (right-1) karke dekho palindrome hai kya.
//  koi ek bhi TRUE -> answer true (isliye ||). koi mismatch aaya hi nahi -> already palindrome -> true.

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
