// ============================================================
// LONGEST SUBSTRING WITHOUT REPEATING   —   Sliding Window (VARIABLE)
// ============================================================
// String `s`. Sabse LAMBI substring ki LENGTH jisme koi character REPEAT na ho.
//
// Example:
//   "abcabcbb"  ->  3   ("abc")
//   "bbbbb"     ->  1   ("b")
//   "pwwkew"    ->  3   ("wke")
//   ""          ->  0
//
// INTUITION (soch — code TU likhega):
//   Ek khidki jisme saare characters UNIQUE hain.
//   - RIGHT se badhao — naya character andar laao.
//   - Agar naya character khidki mein PEHLE SE hai (repeat):
//        LEFT se khidki SIKUDAO (characters hatao) jab tak woh repeat-char nikal na jaye.
//   - Har baar khidki ki LAMBAI dekho, sabse LAMBI yaad rakho.
//   - "note" rakho ki abhi khidki mein kaunse characters hain (repeat turant pata chale).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int lengthOfLongestSubstring(string s)
{
    int i = 0;
    int j = 0;
    int n = s.size();
    unordered_map<char, int> mp;
    int ans = INT_MIN;
    while (j < n)
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
    cout << lengthOfLongestSubstring("abcabcbb") << endl; // expected: 3
    cout << lengthOfLongestSubstring("pwwkew") << endl;   // expected: 3
    return 0;
}
