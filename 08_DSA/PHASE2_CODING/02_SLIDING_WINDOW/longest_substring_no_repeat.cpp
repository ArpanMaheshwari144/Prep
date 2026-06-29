// ============================================================
// LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS   (Medium)
// ============================================================
// ek string di. SABSE LAMBI substring ki LENGTH nikaalo jisme koi character REPEAT na ho.
//
//   "abcabcbb"  -> 3   ("abc")
//   "bbbbb"     -> 1   ("b")
//   "pwwkew"    -> 3   ("wke")
//   ""          -> 0
//   "abba"      -> 2   ("ab" ya "ba")
//
// (no signal, no hint — tu khud soch ke solve kar. fasa to "madad" bol.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- TERA kaam: yeh function bhar ----
int lengthOfLongestSubstring(string s)
{
    if (s.size() == 0)
    {
        return 0;
    }
    int i = 0, j = 0;
    unordered_map<char, int> mp;
    int ans = INT_MIN;
    while (j < s.size())
    {
        mp[s[j]]++;

        while (mp[s[j]] > 1)
        {
            mp[s[i]]--;
            i++;
        }

        ans = max(ans, j - i + 1);
        j++;
    }
    return ans;
}

int main()
{
    cout << lengthOfLongestSubstring("abcabcbb") << endl; // expected 3
    cout << lengthOfLongestSubstring("bbbbb") << endl;    // expected 1
    cout << lengthOfLongestSubstring("pwwkew") << endl;   // expected 3
    cout << lengthOfLongestSubstring("") << endl;         // expected 0
    cout << lengthOfLongestSubstring("abba") << endl;     // expected 2
    cout << lengthOfLongestSubstring("dvdf") << endl;     // expected 3 ("vdf")
    return 0;
}
