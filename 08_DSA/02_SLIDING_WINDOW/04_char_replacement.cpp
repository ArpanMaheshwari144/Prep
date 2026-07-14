// ============================================================
// LONGEST REPEATING CHARACTER REPLACEMENT   (Sliding Window + freq)
// ============================================================
// String s (sirf UPPERCASE letters) aur ek number k diya.
// Tu ZYADA SE ZYADA k characters ko kisi bhi doosre uppercase letter me
// BADAL sakta hai. Badalne ke baad, SABSE LAMBI substring ki LENGTH nikaalo
// jisme SAARE characters SAME ho.
//
// ---- ARPAN KI APPROACH ----
//  variable window + char freq map. j char count++, maxFreq = max(maxFreq, us count).
//  window VALID jab (size - maxFreq) <= k -> maxLen = max(maxLen, size) update.
//  agar (size - maxFreq) > k -> left se shrink (mp[s[i]]--, i++).
//  maxFreq kabhi ghatate nahi -> longest ke liye chal jaata (window sirf badhta/slide).
//
// Tests (// expected):
//   "ABAB",    k=2  -> 4   (2 badal ke "AAAA"/"BBBB")
//   "AABABBA", k=1  -> 4
//   "AAAA",    k=0  -> 4
//   "ABCDE",   k=1  -> 2
//   "AAAB",    k=0  -> 3
// ============================================================

#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

int characterReplacement(string s, int k)
{
    unordered_map<char, int> mp;
    int i = 0, j = 0, maxFreq = INT_MIN, maxLen = INT_MIN;
    while (j < s.size())
    {
        mp[s[j]]++;
        maxFreq = max(maxFreq, mp[s[j]]);
        if (j - i + 1 - maxFreq <= k)
        {
            maxLen = max(maxLen, j - i + 1);
        }

        while (j - i + 1 - maxFreq > k)
        {
            mp[s[i]]--;
            i++;
        }
        j++;
    }
    return maxLen;
}

int main()
{
    cout << characterReplacement("ABAB", 2) << " (expected 4)\n";
    cout << characterReplacement("AABABBA", 1) << " (expected 4)\n";
    cout << characterReplacement("AAAA", 0) << " (expected 4)\n";
    cout << characterReplacement("ABCDE", 1) << " (expected 2)\n";
    cout << characterReplacement("AAAB", 0) << " (expected 3)\n";
    return 0;
}
