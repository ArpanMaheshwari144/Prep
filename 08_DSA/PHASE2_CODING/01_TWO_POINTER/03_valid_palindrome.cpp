// ============================================================
// VALID PALINDROME   —   Two Pointer (opposite ends)
// ============================================================
// String `s` palindrome hai? (aage se aur peeche se same) -> true / false.
//
// Example:
//   "racecar"  ->  true
//   "abba"     ->  true
//   "hello"    ->  false
//   "a"        ->  true
//
// INTUITION (soch — code TU likhega):
//   Do haath — ek shuruaat ke character pe, ek aakhir ke character pe.
//   Match karte hain? -> andar aao, agla pair dekho.
//   Beech mein ek bhi pair NA match kare -> palindrome NAHI (false).
//   Sab pair match -> palindrome HAI (true).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isPalindrome(string s)
{
    int left = 0;
    int right = s.size() - 1;
    while (left < right)
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
    cout << boolalpha << isPalindrome("racecar") << endl; // true
    cout << boolalpha << isPalindrome("hello") << endl;   // false
    return 0;
}
