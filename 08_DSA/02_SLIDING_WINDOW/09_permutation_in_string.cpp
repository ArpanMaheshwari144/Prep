// ============================================================
// PERMUTATION IN STRING — Sliding Window  (REDO — fresh dimaag, paper pehle)
// ============================================================
// do string s1 aur s2. TRUE agar s2 me s1 ka koi PERMUTATION (anagram)
// substring ke roop me maujood hai.
//   s1="ab", s2="eidbaooo"  -> true
//   s1="ab", s2="eidboaoo"  -> false

// ---- ARPAN KI APPROACH ----
//  ye same hai FIND-ALL-ANAGRAMS wala -- dono me kaam ek hi hai.
//  farak sirf itna: wahan har match pe INDEX push karte the (poori list),
//  yahan bas EK match mile to seedha TRUE return (koi match na mile -> FALSE).
//  helper isAnagram (same length + char-count barabar) + p ki length ka fixed window -- wahi.

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

bool isAnagram(string s, string t)
{
    // cout << "-->" << s << " " << t << endl;
    if (s.size() != t.size())
    {
        return 0;
    }

    unordered_map<char, int> mp;
    for (auto &it : s)
    {
        mp[it]++;
    }

    for (auto &it : t)
    {
        mp[it]--;
    }

    for (auto &it : mp)
    {
        if (it.second != 0)
        {
            return false;
        }
    }
    return true;
}

bool findAnagrams(string s, string p)
{
    int i = 0, j = 0;
    while (j < s.size())
    {
        while (j - i + 1 > p.size())
        {
            i++;
        }

        if (j - i + 1 == p.size())
        {
            // cout << j - i + 1 << endl;
            if (isAnagram(s.substr(i, j - i + 1), p))
            {
                return true;
            }
        }
        j++;
    }
    return false;
}

bool checkInclusion(string s1, string s2)
{
    return findAnagrams(s2, s1);
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
