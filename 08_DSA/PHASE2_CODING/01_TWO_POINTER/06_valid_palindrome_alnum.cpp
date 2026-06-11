// ============================================================
// VALID PALINDROME (real interview version)   —   Two Pointer
// ============================================================
// String `s` palindrome hai? PAR:
//   - sirf LETTERS + DIGITS dekho (space, comma, ':' etc IGNORE karo)
//   - case ignore: 'A' == 'a'
//
// Example:
//   "A man, a plan, a canal: Panama"  ->  true
//   "race a car"                       ->  false
//   " "                                ->  true   (koi alnum nahi -> khaali -> palindrome)
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   Do haath: left = 0, right = end.   (tera simple palindrome jaisa hi)
//   FARAQ sirf 2:
//     1. SKIP: agar s[left] alphanumeric nahi -> left++ (aur dekho). same right ke liye.
//        (helper: isalnum(c) -> letter/digit hai kya?)
//     2. COMPARE case-ignore: tolower(s[left]) != tolower(s[right]) -> false.
//   Match -> dono andar (left++, right--). left < right tak chalao. Sab pass -> true.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isPalindrome(string s)
{
    int left = 0;
    int right = s.size() - 1;
    while (left < right)
    {
        if (!isalnum(s[left]))
        {
            left++;
        }
        else if (!isalnum(s[right]))
        {
            right--;
        }
        else if (tolower(s[left]) != tolower(s[right]))
        {
            return false;
        }
        else if (tolower(s[left]) == tolower(s[right]))
        {
            left++;
            right--;
        }
    }
    return true;
}

int main()
{
    cout << boolalpha << isPalindrome("A man, a plan, a canal: Panama") << endl; // true
    cout << boolalpha << isPalindrome("race a car") << endl;                     // false
    cout << boolalpha << isPalindrome(" ") << endl;                              // true
    return 0;
}
