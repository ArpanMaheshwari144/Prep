// ============================================================
// VALID PALINDROME  — (LeetCode 125, FULL version)   (fresh)
// ============================================================
// ek string s di gayi -- isme kuch bhi ho sakta: letters (UPPER + lower),
// digits (0-9), spaces, punctuation (, : . ! ? etc).
//
// PALINDROME check karo, PAR:
//   • SIRF alphanumeric characters (a-z, A-Z, 0-9) consider karo.
//   • baaki sab (space, comma, colon, etc) IGNORE.
//   • case IGNORE karo (A == a).
//
// TRUE agar aise-filter-karne-ke-baad string aage-se aur peeche-se same padhe.
//
//   "A man, a plan, a canal: Panama"  -> true
//        (filter+lower -> "amanaplanacanalpanama" = palindrome)
//   "race a car"                      -> false   ("raceacar")
//   "0P"                              -> false   ('0' vs 'p')
//   " "                               -> true    (filter ke baad khaali -> palindrome maana jaata)
//   "a"                               -> true
//   ".,"                              -> true    (koi alphanumeric nahi -> khaali -> true)
//   "ab_a"                            -> true    ('_' ignore -> "aba")
//   "Madam"                           -> true
//
// (hint-help NAHI -- khud socho: 2 helper kaam aayenge -- ek "ye char alphanumeric hai?",
//  doosra "char ko lowercase karo". C++ me ye <cctype> me hote hain.)
// ============================================================

// ---- ARPAN KI APPROACH ----
//  ★ ye question EASY hai -- bas tolower + isalnum ka khel. baaki normal two-pointer (left=0, right=end).
//  ★ 2 nayi cheez normal-palindrome ke upar:
//     (1) CASE-IGNORE: compare karte waqt dono ko tolower karo -> 'A' aur 'a' same maano.
//     (2) SKIP non-alnum: space , : . etc ko chhodna hai -> isalnum se pakdo, us pointer ko aage/peeche khisko.
//  ★ ORDER (jo maine likha, kaam karta):
//     1. tolower(s[left]) == tolower(s[right])  -> match (dono alnum-same YA dono skippable-same) -> left++ right--.
//     2. warna !isalnum(s[left])  -> left non-alnum -> skip -> left++.
//     3. warna !isalnum(s[right]) -> right non-alnum -> skip -> right--.
//     4. warna (dono alnum PAR alag) -> return FALSE.
//  loop bina false cross ho gaya -> return TRUE. (koi alnum na ho / khaali -> true.)

#include <bits/stdc++.h>
using namespace std;

bool isPalindrome(string s)
{
    int left = 0;
    int right = s.size() - 1;

    while (left <= right)
    {
        if (tolower(s[left]) == tolower(s[right]))
        {
            left++;
            right--;
        }
        else if (!isalnum(s[left]))
        {
            left++;
        }
        else if (!isalnum(s[right]))
        {
            right--;
        }
        else{
            return false;
        }
    }
    return true;
}

int main()
{
    cout << isPalindrome("A man, a plan, a canal: Panama") << " (expected 1)\n";
    cout << isPalindrome("race a car") << " (expected 0)\n";
    cout << isPalindrome("0P") << " (expected 0)\n";
    cout << isPalindrome(" ") << " (expected 1)\n";
    cout << isPalindrome("a") << " (expected 1)\n";
    cout << isPalindrome(".,") << " (expected 1)\n";
    cout << isPalindrome("ab_a") << " (expected 1)\n";
    cout << isPalindrome("Madam") << " (expected 1)\n";
    return 0;
}
