// ============================================================
// VALID PALINDROME
// ============================================================
// Ek string (sirf lowercase letters). Check karo palindrome hai ya nahi
// (aage se aur peeche se same padhe).
//
// Tests (// expected):
//   "racecar"  -> 1 (true)
//   "hello"    -> 0
//   "a"        -> 1
//   "abba"     -> 1
//   "abca"     -> 0
//   ""         -> 1
// ============================================================
// ---- ARPAN KI APPROACH ----
//  2 pointer: left=0, right=end. jab tak left <= right.
//  s[left] != s[right] -> turant false. warna left++ , right--.
//  loop bina mismatch nikal gaya -> palindrome hai -> true.
// ============================================================

#include <iostream>
#include <string>
using namespace std;

bool isPalindrome(string s)
{
    int left = 0;
    int right = s.size() - 1;
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

int main()
{
    cout << isPalindrome("racecar") << " (expected 1)\n";
    cout << isPalindrome("hello") << " (expected 0)\n";
    cout << isPalindrome("a") << " (expected 1)\n";
    cout << isPalindrome("abba") << " (expected 1)\n";
    cout << isPalindrome("abca") << " (expected 0)\n";
    cout << isPalindrome("") << " (expected 1)\n";
    return 0;
}
