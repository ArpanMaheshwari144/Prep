// ============================================================
// VALID ANAGRAM   (Hashing — frequency COUNT map)
// ============================================================
// Do string s, t di. Check karo t, s ka ANAGRAM hai ya nahi
// (same characters, same COUNT — bas order alag).
//
// seed: ek COUNT map { char -> kitni baar }.
//        s ke har char pe ++  ·  t ke har char pe --
//        end me: agar SAB count 0 (aur length same) -> anagram. warna nahi.
//        (ya: length alag -> turant false.)
//
// Tests (// expected):
//   "anagram","nagaram" -> 1 (true)
//   "rat","car"         -> 0
//   "a","ab"            -> 0   (length alag)
//   "listen","silent"   -> 1
//   "aacc","ccac"       -> 0   (count alag: aacc=2a2c, ccac=1a3c)
// ============================================================

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

bool isAnagram(string s, string t)
{
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
        if (it.second != 0) // mistake kardi maine > 1 kar diya tha
        {
            return false;
        }
    }
    return true;
}

int main()
{
    cout << isAnagram("anagram", "nagaram") << " (expected 1)\n";
    cout << isAnagram("rat", "car") << " (expected 0)\n";
    cout << isAnagram("a", "ab") << " (expected 0)\n";
    cout << isAnagram("listen", "silent") << " (expected 1)\n";
    cout << isAnagram("aacc", "ccac") << " (expected 0)\n";
    return 0;
}
