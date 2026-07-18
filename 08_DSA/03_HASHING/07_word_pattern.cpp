// ============================================================
// WORD PATTERN — Hashing  (fresh; isomorphic ka bhai — bijection)
// ============================================================
// string pattern (chars) aur string s (space-separated words).
// TRUE agar s, pattern ko FOLLOW kare: har pattern-char ek hi word se bandhe
// aur har word ek hi char se (one-to-one dono taraf, jaise isomorphic).
//   pattern="abba", s="dog cat cat dog"  -> true   (a->dog, b->cat)
//   pattern="abba", s="dog cat cat fish" -> false  (a to dog tha, aakhir fish)
//   pattern="aaaa", s="dog cat cat dog"  -> false  (a->dog phir a->cat clash)
//   pattern="abba", s="dog dog dog dog"  -> false  (a->dog, b->dog : 2 char same word me)

// ---- ARPAN KI APPROACH ----
//  ye ISOMORPHIC ka BHAI hai -- wahi 2-map BIJECTION. bas farak: wahan char<->char tha,
//  yahan char <-> STRING (word). aur kuch nahi.
//  s space-separated words ka hai -> stringstream(s) + while(ss >> word) se ek-ek word nikaalo.
//  ★ pattern pe alag FOR-loop NAHI (words hi drive kar rahe hain) -> ek counter i rakho:
//        if (i < pattern.size()) { ...mapping... i++ }   -> har word ke saath pattern[i] jodo.
//  mapping = isomorphic wahi: clash-check dono taraf, warna mp1[pattern[i]]=word, mp2[word]=pattern[i].
//  koi bhi taraf clash -> false.
//  ★ EDGE (length-guard): pattern.size() aur WORDS ki ginti barabar honi chahiye. words ko 'j' se gino;
//     end me pattern.size()==j -> true, warna false. (warna "a","dog cat" jaisा galat true de deta -- extra word chup-chaap skip.)

//
// Tests (pattern, s -> expected):
//   "abba", "dog cat cat dog"   -> 1
//   "abba", "dog cat cat fish"  -> 0
//   "aaaa", "dog cat cat dog"   -> 0
//   "abba", "dog dog dog dog"   -> 0
//   "abc",  "b c a"             -> 1
//   "ab",   "dog dog"           -> 0
//   "a",    "cat"               -> 1
// ============================================================

#include <bits/stdc++.h>
#include <sstream>
using namespace std;

bool wordPattern(string pattern, string s) // "abba", "dog cat cat dog"   -> 1
{
    unordered_map<char, string> mp1;
    unordered_map<string, char> mp2;
    stringstream ss(s);
    string word;
    int i = 0; // pattern count
    int j = 0; // word count
    while (ss >> word)
    {
        if (i < pattern.size())
        {
            // ISOMORPHIC CODE (wahi 2-map clash-check, bas char<->WORD)
            // ★ || KYUN (&& nahi): do taraf ka clash-check -- mp1 (char->word) side AUR mp2 (word->char) side.
            //   in dono me se KOI BHI EK clash mila -> galat -> return false. isliye OR (||).
            //   (&& hota to DONO taraf ek saath clash chahiye hota -> galat; ek hi taraf ka clash bhi false hona chahiye.)
            if (!mp1.empty() && !mp2.empty() &&
                    (mp1.count(pattern[i]) == 1 && mp1[pattern[i]] != word) ||
                (mp2.count(word) == 1 && mp2[word] != pattern[i]))
            {
                return false;
            }
            else
            {
                mp1[pattern[i]] = word;
                mp2[word] = pattern[i];
            }
            i++;
        }
        j++;
    }
    return (pattern.size() == j) ? true : false;
}

int main()
{
    cout << wordPattern("abba", "dog cat cat dog") << " (expected 1)\n";
    cout << wordPattern("abba", "dog cat cat fish") << " (expected 0)\n";
    cout << wordPattern("aaaa", "dog cat cat dog") << " (expected 0)\n";
    cout << wordPattern("abba", "dog dog dog dog") << " (expected 0)\n";
    cout << wordPattern("abc", "b c a") << " (expected 1)\n";
    cout << wordPattern("ab", "dog dog") << " (expected 0)\n";
    cout << wordPattern("a", "cat") << " (expected 1)\n";
    cout << wordPattern("a", "dog cat") << " (expected 0)\n";
    cout << wordPattern("abc", "dog cat") << " (expected 0)\n";
    return 0;
}
