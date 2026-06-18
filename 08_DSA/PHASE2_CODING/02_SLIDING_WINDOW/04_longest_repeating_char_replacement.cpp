// ============================================================
// LONGEST REPEATING CHARACTER REPLACEMENT   —   Sliding Window (variable)
// ============================================================
// String s + int k. At most k characters badal sakte ho (koi bhi -> koi bhi).
// Sabse lamba substring jisme (k changes ke baad) saare SAME letter ho jaayein -> uski length.
//
// Example:
//   s = "AABABBA", k = 1  ->  4   ("AABA" -> 1 'B' ko 'A' -> "AAAA")
//   s = "ABAB",    k = 2  ->  4
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   Variable sliding window (left/right) + count map.
//   Window VALID jab:  (windowLen - maxFreq) <= k   [baaki chars ko majority mein badal sakte]
//     - right badhao: count[s[right]]++ ; maxFreq = max(maxFreq, count[s[right]])
//     - agar (windowLen - maxFreq) > k  -> window invalid -> left badhao (count[s[left]]--)
//     - answer = max(answer, windowLen)   // valid window length
//   maxFreq: poore map pe loop nahi — bas naye char ke count se max nikaalo.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int characterReplacement(string s, int k)
{
    int i = 0, j = 0, maxFreq = INT_MIN, ans = -1;
    unordered_map<char, int> mp;

    while (j < s.size())
    {
        mp[s[j]]++;
        maxFreq = max(maxFreq, mp[s[j]]);
        if ((j - i + 1) - maxFreq <= k)
        {
            ans = max(ans, j - i + 1);
        }
        if ((j - i + 1) - maxFreq > k)
        {
            mp[s[i]]--;
            i++;
        }
        j++;
    }
    return ans;
}

int main()
{
    cout << characterReplacement("AABABBA", 1) << endl; // expected: 4
    cout << characterReplacement("ABAB", 2) << endl;    // expected: 4
    cout << characterReplacement("AAAA", 0) << endl;    // expected: 4
    return 0;
}
