// ============================================================
// LONGEST SUBSTRING WITHOUT REPEATING CHARS — Sliding Window (SPACED-REDO, blank)
// ============================================================
// string s diya. sabse LAMBI substring ki LENGTH lautao jisme koi character
// REPEAT na ho.
//   "abcabcbb" -> 3   ("abc")
//   "bbbbb"    -> 1   ("b")
//   "pwwkew"   -> 3   ("wke")
//
// (purana code MAT dekhna -- retrieval. copy-pen pe socho:
//  window [left..right] chalao -> repeat char aaye to left ko aage khisko jab tak repeat gone.
//  window ki max length track karo. set/map se dekho char window me hai ya nahi.)
//
// Tests (// expected):
//   "abcabcbb" -> 3
//   "bbbbb"    -> 1
//   "pwwkew"   -> 3
//   ""         -> 0
//   "au"       -> 2
//   "dvdf"     -> 3
//   "abba"     -> 2
// ============================================================

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int lengthOfLongestSubstring(string s)
{
    if (s.empty())
    {
        return 0;
    }

    int ans = INT_MIN;
    unordered_map<char, int> mp; // abcabcbb
    int i = 0, j = 0;
    while (j < s.size())
    {
        mp[s[j]]++;
        while (mp[s[j]] > 1)
        {
            // cout << "->>" << mp[s[j]] << endl;
            mp[s[i]]--;
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}

int main()
{
    cout << lengthOfLongestSubstring("abcabcbb") << " (expected 3)\n";
    cout << lengthOfLongestSubstring("bbbbb") << " (expected 1)\n";
    cout << lengthOfLongestSubstring("pwwkew") << " (expected 3)\n";
    cout << lengthOfLongestSubstring("") << " (expected 0)\n";
    cout << lengthOfLongestSubstring("au") << " (expected 2)\n";
    cout << lengthOfLongestSubstring("dvdf") << " (expected 3)\n";
    cout << lengthOfLongestSubstring("abba") << " (expected 2)\n";
    return 0;
}
