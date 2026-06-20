// ============================================================
// VALID ANAGRAM   —   Arrays & Hashing   [COLD REDO]
// ============================================================
// Do strings s aur t. t, s ka anagram hai? (same letters + same counts, order alag)
//   s="anagram", t="nagaram"  -> true
//   s="rat",     t="car"      -> false
//   s="ab",      t="a"        -> false  (alag length)
//
// SIGNAL: "same letters + same counts, order ignore" -> Hashing (count map).
//   (blank se, apni soch. self-run Ctrl+Alt+N. no peeking.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isAnagram(string s, string t)
{
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

int main()
{
    cout << isAnagram("anagram", "nagaram") << endl; // expected 1
    cout << isAnagram("rat", "car") << endl;         // expected 0
    cout << isAnagram("ab", "a") << endl;            // expected 0  (alag length)
    cout << isAnagram("", "") << endl;               // expected 1  (dono khaali)
    cout << isAnagram("aacc", "ccac") << endl;       // expected 0  (counts alag)
    cout << isAnagram("listen", "silent") << endl;   // expected 1
    return 0;
}
