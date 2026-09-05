// ============================================================
// VALID PALINDROME II — Two Pointer  (fresh, one-deletion allowed)
// ============================================================
// string s (lowercase). TRUE agar s palindrome hai, YA ZYADA-SE-ZYADA EK character
// DELETE karke palindrome ban sakti hai.
//   "aba"    -> true  (already palindrome)
//   "abca"   -> true  ('c' ya 'b' delete -> "aba"/"aca")
//   "abc"    -> false (ek delete se bhi nahi banta)
//
//
// Tests (s -> expected):
//   "aba"        -> 1
//   "abca"       -> 1
//   "abc"        -> 0
//   "a"          -> 1
//   "deeee"      -> 1     (ek 'd' delete -> "eeee")
//   "cbbcc"      -> 1     (ek 'c' delete -> "cbbc")
//   "abcdba"     -> 1     ('c' delete -> "abdba" = palindrome. [galti sudhri: pehle 0 likha tha])
//   "caaab"      -> 0     (ek delete se bhi nahi: c-hata->"aaab" nahi, b-hata->"caaa" nahi)
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
    // int left = 0;
    // int right = s.size() - 1;
    // int left_count = 0;
    // int right_count = 0;
    // while (left <= right)
    // {
    //     if (s[left] != s[right] && left_count == 0)
    //     {
    //         left++;
    //         left_count = 1;
    //     }
    //     else if (s[left] == s[right])
    //     {
    //         left++;
    //         right--;
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }

    // if (left >= right)
    //     return true;

    // left = 0;
    // right = s.size() - 1;
    // while (left <= right)
    // {
    //     if (s[left] != s[right] && right_count == 0)
    //     {
    //         right--;
    //         right_count = 1;
    //     }
    //     else if (s[left] == s[right])
    //     {
    //         left++;
    //         right--;
    //     }
    //     else
    //     {
    //         return false;
    //     }
    // }

    // if (left < right)
    //     return false;
    // return true;

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
    cout << validPalindrome("tebbem") << " (expected 0)\n"; // dono skip fail
    cout << validPalindrome("cbbcc") << " (expected 1)\n";  // delete-RIGHT wala
    return 0;
}
