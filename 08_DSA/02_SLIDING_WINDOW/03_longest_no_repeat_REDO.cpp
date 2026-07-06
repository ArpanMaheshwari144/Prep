// ============================================================
// LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS — BLANK REDO (6-Jul)
// ============================================================
// Ek string s di. SABSE LAMBE substring ki LENGTH nikaalo jisme
// koi character REPEAT na ho.  (substring = contiguous.)
//
// (approach yahan LIKHA NAHI — khud dimaag se. purani file mat kholna.
//  atko -> "atka hoon yahan" bol, direction dunga.)
//
// Tests (// expected):
//   "abcabcbb"   -> 3   ("abc")
//   "bbbbb"      -> 1   ("b")
//   "pwwkew"     -> 3   ("wke")
//   ""           -> 0
//   "dvdf"       -> 3   ("vdf")
//   "au"         -> 2
// ============================================================

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

int lengthOfLongestSubstring(string s)
{

    int i = 0, j = 0;
    unordered_map<char, int> mp; 
    int ans = INT_MIN;
    while (j < s.size()) // "abcabcbb"   -> 3   ("abc")
    {
        mp[s[j]]++; // map mei store
        while (mp[s[j]] > 1) // shrink karo acc to condition
        {
            mp[s[i]]--;
            i++;
        }
        ans = max(ans, j - i + 1); // ans store
        j++; // window expand
    }
    return ans == INT_MIN ? 0 : ans;
}

int main()
{
    cout << lengthOfLongestSubstring("abcabcbb") << " (expected 3)\n";
    cout << lengthOfLongestSubstring("bbbbb") << " (expected 1)\n";
    cout << lengthOfLongestSubstring("pwwkew") << " (expected 3)\n";
    cout << lengthOfLongestSubstring("") << " (expected 0)\n";
    cout << lengthOfLongestSubstring("dvdf") << " (expected 3)\n";
    cout << lengthOfLongestSubstring("au") << " (expected 2)\n";
    return 0;
}
