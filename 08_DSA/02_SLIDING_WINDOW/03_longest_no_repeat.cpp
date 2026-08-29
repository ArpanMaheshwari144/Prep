// ============================================================
// LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS
//                              (Sliding Window — VARIABLE + SET)
// ============================================================
// Ek string di. SABSE LAMBI substring ki LENGTH nikaalo jisme koi
// character REPEAT na ho (sab unique).
//
// ---- ARPAN KI APPROACH ----
//  variable window + freq map (mp). j char ka count++ karo. jab tak current char
//  ka count > 1 (duplicate) -> left se shrink (mp[s[i]]--, i++) till duplicate gaya.
//  har step minLen = max(minLen, j-i+1). khaali string -> INT_MIN reh jaata -> 0.
//
// Tests (// expected):
//   "abcabcbb"  -> 3   ("abc")
//   "bbbbb"     -> 1   ("b")
//   "pwwkew"    -> 3   ("wke")
//   ""          -> 0
//   "abcdef"    -> 6   (sab unique)
//   "au"        -> 2
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int lengthOfLongestSubstring(string s)
{
    unordered_map<char, int> mp;
    int i = 0, j = 0, minLen = INT_MIN;
    while (j < s.size()) // "abcabcbb"  -> 3   ("abc")
    {
        mp[s[j]]++;
        while (mp[s[j]] > 1)
        {
            mp[s[i]]--;
            i++;
        }
        minLen = max(minLen, j - i + 1);
        j++;
    }
    return minLen == INT_MIN ? 0 : minLen;
}

int main()
{
    cout << lengthOfLongestSubstring("abcabcbb") << " (expected 3)\n";
    cout << lengthOfLongestSubstring("bbbbb") << " (expected 1)\n";
    cout << lengthOfLongestSubstring("pwwkew") << " (expected 3)\n";
    cout << lengthOfLongestSubstring("") << " (expected 0)\n";
    cout << lengthOfLongestSubstring("abcdef") << " (expected 6)\n";
    cout << lengthOfLongestSubstring("au") << " (expected 2)\n";
    return 0;
}
