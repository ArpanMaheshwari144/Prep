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

bool checkInclusion(string s1, string s2)
{
    // TODO
}

int main()
{
    cout << checkInclusion("ab", "eidbaooo") << " (expected 1)\n";
    cout << checkInclusion("ab", "eidboaoo") << " (expected 0)\n";
    cout << checkInclusion("abc", "bbbca")   << " (expected 1)\n";
    cout << checkInclusion("a", "a")         << " (expected 1)\n";
    cout << checkInclusion("abc", "ab")      << " (expected 0)\n";
    cout << checkInclusion("adc", "dcda")    << " (expected 1)\n";
    return 0;
}
