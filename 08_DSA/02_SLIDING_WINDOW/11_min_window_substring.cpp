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
// ---- ARPAN KI APPROACH (HAND-TRACE se DERIVE kiya, 22-Jul) ----
//  ★ HAND-TRACE ne behavior diya (kaagaz pe "ADOBECODEBANC","ABC" trace kiya):
//     expand j jab tak window VALID (t ke saare char) -> phir shrink i jab tak toote
//     -> phir expand -> phir shrink. har valid window pe MIN track. (BANC = 4.)
//  ★ MAP = "code ki AANKHEIN": aankhon se jo dikhta ("ABC hai kya?") wo code map+count se track karta.
//     need-map = t ke char+count.  count = kitne char ABHI baaki (0 = valid).
//  EXPAND (j):  if(mp[s[j]]>0) count--;   phir  mp[s[j]]--;   <-- mp-- IF ke BAAHAR (HAMESHA)
//     ★★ WHY bahar (D-asymmetry BUG): non-t char (D) if me nahi ghusta -> expand me GHATTA nahi ->
//        par shrink me mp++ hota -> mp[D] 0->1 -> guard>0 TRUE -> count++ GALAT. bahar rakho -> D negative
//        -> shrink me wapas <=0 -> guard kabhi trigger nahi -> D "count ke liye invisible" (jaisa chahiye).
//  VALID (count==0) -> SHRINK (i):  answer-save (len<minLen -> minLen,index) ;
//        mp[s[i]]++;  if(mp[s[i]]>0) count++;  i++;   <-- MIRROR: aana=-- , jaana=++ (guard ke saath)
//  return minLen==INT_MAX ? "" : s.substr(index, minLen).
//  ★★ TWIN: ye aur LC-1358 (substrings-all-3) SAME SW skeleton -- bas answer-handle alag
//     (yahaan MIN track; LC-1358 me COUNT: ans += i). t="abc" daalo to dono ek ho jaate. (recognize+adapt.)
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
