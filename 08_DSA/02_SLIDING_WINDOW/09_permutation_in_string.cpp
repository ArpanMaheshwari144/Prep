// ============================================================
// PERMUTATION IN STRING — Sliding Window  (REDO — fresh dimaag, paper pehle)
// ============================================================
// do string s1 aur s2. TRUE agar s2 me s1 ka koi PERMUTATION (anagram)
// substring ke roop me maujood hai.
//   s1="ab", s2="eidbaooo"  -> true
//   s1="ab", s2="eidboaoo"  -> false


//
// Tests (s1, s2 -> expected):
//   "ab", "eidbaooo"   -> 1
//   "ab", "eidboaoo"   -> 0
//   "abc", "bbbca"     -> 1
//   "a", "a"           -> 1
//   "abc", "ab"        -> 0
//   "adc", "dcda"      -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// LC-438 find all anagrams ka EXACT same code (do map), farak sirf answer line:
//   438 me ans.push_back(i)  ->  yahan pehla match milte hi return true
bool checkInclusion(string s1, string s2)
{
    unordered_map<char, int> mp1;
    unordered_map<char, int> mp2;

    for (int i = 0; i < s1.size(); i++)
    {
        mp1[s1[i] - 'a']++;
    }

    int i = 0, j = 0;
    while (j < s2.size())
    {
        mp2[s2[j] - 'a']++;
        if (j - i + 1 == s1.size())
        {
            if (mp1 == mp2)
            {
                return true;
            }
            mp2[s2[i] - 'a']--;
            if (mp2[s2[i] - 'a'] == 0)
            {
                mp2.erase(s2[i] - 'a');
            }
            i++;
        }
        j++;
    }
    return false;
}

int main()
{
    cout << checkInclusion("ab", "eidbaooo") << " (expected 1)\n";
    cout << checkInclusion("ab", "eidboaoo") << " (expected 0)\n";
    cout << checkInclusion("abc", "bbbca") << " (expected 1)\n";
    cout << checkInclusion("a", "a") << " (expected 1)\n";
    cout << checkInclusion("abc", "ab") << " (expected 0)\n";
    cout << checkInclusion("adc", "dcda") << " (expected 1)\n";
    return 0;
}
