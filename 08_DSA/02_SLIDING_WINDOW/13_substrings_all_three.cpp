// ============================================================
// NUMBER OF SUBSTRINGS CONTAINING ALL THREE CHARACTERS — (LeetCode 1358, Medium)   [REDO — blank, hand-trace]
// ============================================================
// string s diya jisme sirf 'a', 'b', 'c' hain. aise substrings ki GINTI lautao
// jinme a, b, aur c -- TEENO kam-se-kam ek baar aayein.
//
//   "abcabc"  -> 10
//   "aaacb"   -> 3
//   "abc"     -> 1
//   "cabc"    -> 3
//   "aaa"     -> 0   (b, c nahi)
//   "ccccc"   -> 0
//
// ============================================================

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int minWindow(string s, string t)
{
    unordered_map<char, int> mp;
    for (auto &it : t)
    {
        mp[it]++;
    }

    int i = 0, j = 0, count = t.size(), ans = 0;
    while (j < s.size())
    {
        if (mp[s[j]] > 0)
        {
            count--;
        }
        mp[s[j]]--;

        while (count == 0)
        {
            mp[s[i]]++;
            if (mp[s[i]] > 0)
            {
                count++;
            }
            i++;
        }
        ans += i;
        j++;
    }
    return ans;
}

int numberOfSubstrings(string s)
{
    return minWindow(s, "abc");
}

int main()
{
    cout << numberOfSubstrings("abcabc") << " (expected 10)\n";
    cout << numberOfSubstrings("aaacb") << " (expected 3)\n";
    cout << numberOfSubstrings("abc") << " (expected 1)\n";
    cout << numberOfSubstrings("cabc") << " (expected 3)\n";
    cout << numberOfSubstrings("aaa") << " (expected 0)\n";
    cout << numberOfSubstrings("ccccc") << " (expected 0)\n";
    return 0;
}
