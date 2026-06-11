// ============================================================
// VALID ANAGRAM   —   Hashing (count map)
// ============================================================
// Do strings s, t. t kya s ka ANAGRAM hai? (same letters + same count, alag order) -> true/false.
//
// Example:
//   s="anagram", t="nagaram" -> true
//   s="rat",     t="car"     -> false
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   s ke har char ka count map mein store karo.
//   phir t ke har char pe count-- karo (map se ghatao).
//   end mein: SAB count 0 -> anagram (true), warna false.
//   (different length / extra-char khud handle ho jaata: counts balance nahi -> false)
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
    cout << boolalpha << isAnagram("anagram", "nagaram") << endl; // expected: true
    cout << boolalpha << isAnagram("rat", "car") << endl;         // expected: false
    return 0;
}
