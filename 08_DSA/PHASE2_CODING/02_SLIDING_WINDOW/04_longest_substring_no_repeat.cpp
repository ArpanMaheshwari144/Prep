// ============================================================
// LONGEST SUBSTRING WITHOUT REPEATING CHARACTERS
//                                      (Pattern: Sliding Window - VARIABLE)
// ============================================================
// Ek string `s` diya hai. Sabse LAMBI substring ki LENGTH return karo
// jisme koi character REPEAT na ho (sab unique).
//
// Example:
//   "abcabcbb" -> 3   ("abc")
//   "bbbbb"    -> 1   ("b")
//   "pwwkew"   -> 3   ("wke")
//   ""         -> 0
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    int lengthOfLongestSubstring(string s)
    {
        // TODO: tera code (Sliding Window + set/map to track chars) — SOLO
        int i = 0;
        int j = 0;
        int n = s.size();
        int ans = INT_MIN;
        unordered_map<char, int> mp; // "abcabcbb" -> 3   ("abc")
        while (j < n)
        {
            mp[s[j]]++; // expand
            while (mp[s[j]] >= 2)
            { // shrink
                mp[s[i]]--;
                i++;
            }
            ans = max(ans, j - i + 1); // ans
            j++;
        }
        return ans;
    }
};

// ---- test harness (run karke verify — yeh solution nahi, bas runner) ----
int main()
{
    Solution sol;
    cout << sol.lengthOfLongestSubstring("abcabcbb") << endl; // expected: 3
    return 0;
}
