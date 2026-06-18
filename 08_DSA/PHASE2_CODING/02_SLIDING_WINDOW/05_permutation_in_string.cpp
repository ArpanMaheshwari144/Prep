// ============================================================
// PERMUTATION IN STRING   —   Sliding Window (fixed) + Hashing
// ============================================================
// s1, s2. Return true agar s2 mein s1 ka koi PERMUTATION (anagram) substring ho.
//
// Example:
//   s1 = "ab",  s2 = "eidbaooo"  ->  true   ("ba")
//   s1 = "ab",  s2 = "eidboaoo"  ->  false
//   s1 = "adc", s2 = "dcda"      ->  true   ("cda" / "dca")
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   1. s1Count = s1 ke chars ka count (FIXED reference).
//   2. s2 pe FIXED window (size = s1.length) slide karo, windowCount maintain karo:
//        right char add: windowCount[s2[right]]++
//        window > s1.length hua: left char hatao -> windowCount[s2[left]]-- , left++
//   3. Jab window size == s1.length:  windowCount == s1Count ? -> true
//   (Map ya size-26 array dono chalega. Array thoda fast compare.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool checkInclusion(string s1, string s2)
{
    int i = 0, j = 0;
    unordered_map<char, int> mp1;
    unordered_map<char, int> mp2;

    for (int i = 0; i < s1.size(); i++)
    {
        mp1[s1[i]]++;
    }

    while (j < s2.size())
    {
        mp2[s2[j]]++;
        if (j - i + 1 > s1.size())
        {
            mp2[s2[i]]--;
            if (mp2[s2[i]] == 0)
            {
                mp2.erase(s2[i]);
            }
            i++;
        }

        if (j - i + 1 == s1.size())
        {
            if (mp1 == mp2)
                return true;
        }
        j++;
    }
    return false;
}

int main()
{
    cout << boolalpha << checkInclusion("ab", "eidbaooo") << endl; // true
    cout << boolalpha << checkInclusion("ab", "eidboaoo") << endl; // false
    cout << boolalpha << checkInclusion("adc", "dcda") << endl;    // true
    return 0;
}
