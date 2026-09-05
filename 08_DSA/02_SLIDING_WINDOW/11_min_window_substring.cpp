// ============================================================
// MINIMUM WINDOW SUBSTRING — (LeetCode 76, Hard)   [REDO — blank]
// ============================================================
// do string s aur t diye. s ka SABSE CHHOTA substring (window) lautao jisme
// t ke SAARE characters aa jayein -- COUNT ke saath (t me 'a' 2 baar -> window
// me bhi kam-se-kam 2 'a'). extra chars allowed. koi window na ho -> "".
//
//   s="ADOBECODEBANC", t="ABC"   -> "BANC"
//   s="a",            t="a"       -> "a"
//   s="a",            t="aa"      -> ""
//   s="aa",           t="aa"      -> "aa"
//   s="ab",           t="b"       -> "b"
//   s="a",            t="b"       -> ""
//   s="cabwefgewcwaefgcf", t="cae" -> "cwae"
//
// ============================================================

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

string minWindow(string s, string t)
{
    unordered_map<char, int> mp;
    for (auto &it : t)
    {
        mp[it]++;
    }

    int i = 0, j = 0, count = t.size(), minLen = INT_MAX, index = 0;
    while (j < s.size())
    {
        if (mp[s[j]] > 0)
        {
            count--;
        }
        mp[s[j]]--;

        while (count == 0)
        {
            int len = j - i + 1;
            if (len < minLen)
            {
                minLen = len;
                index = i;
            }

            mp[s[i]]++;
            if (mp[s[i]] > 0)
            {
                count++;
            }
            i++;
        }
        j++;
    }
    return minLen == INT_MAX ? "" : s.substr(index, minLen);
}

int main()
{
    cout << minWindow("ADOBECODEBANC", "ABC") << " (expected BANC)\n";
    cout << minWindow("a", "a") << " (expected a)\n";
    cout << "[" << minWindow("a", "aa") << "] (expected [])\n";
    cout << minWindow("aa", "aa") << " (expected aa)\n";
    cout << minWindow("ab", "b") << " (expected b)\n";
    cout << "[" << minWindow("a", "b") << "] (expected [])\n";
    cout << minWindow("cabwefgewcwaefgcf", "cae") << " (expected cwae)\n";
    return 0;
}
