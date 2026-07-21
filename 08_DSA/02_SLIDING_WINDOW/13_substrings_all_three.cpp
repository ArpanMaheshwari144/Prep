// ============================================================
// NUMBER OF SUBSTRINGS CONTAINING ALL THREE CHARACTERS — (LeetCode 1358, Medium)   [REDO — blank]
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
// ---- HAND-TRACE (copy-pen: "abcabc" -> 10, dekh 10 kaise aata) ----
//   har RIGHT-end j pe gino: yahaan END hone wale kitne substrings me a,b,c TEENO hain.
//     j=0 'a': [a]                            -> 0
//     j=1 'b': [ab][b]                        -> 0
//     j=2 'c': [abc]                          -> 1   (valid start: 0)
//     j=3 'a': [abca] [bca]                   -> 2   (valid start: 0,1)
//     j=4 'b': [abcab] [bcab] [cab]           -> 3   (valid start: 0,1,2)
//     j=5 'c': [abcabc] [bcabc] [cabc] [abc]  -> 4   (valid start: 0,1,2,3)
//   total = 0+0+1+2+3+4 = 10
//   ★ SW skeleton: expand j -> window VALID (abc teeno)? -> shrink (i++) + har right-end pe
//     "kitne valid left-starts" jodo (ans += valid-lefts). need={a:1,b:1,c:1} HARDCODE, count=3.
// ============================================================
// ---- ARPAN KI APPROACH (intuition + mechanic) ----
//  ★★ INTUITION (asli baat): valid window (abc teeno) ko LEFT se aur bada karo -> wo VALID hi REHTA
//     (extra chars se abc gaayab nahi hote). isliye har right-end pe valid left-starts ek CONTINUOUS
//     block hain (0 se ek point tak) -> unhe EK SAATH gin lo (ans += i), ek-ek check nahi.
//  ★ MECHANIC: need={a:1,b:1,c:1} HARDCODE, count=3.
//     expand: if(mp[s[j]]>0) count--;  phir mp[s[j]]-- (if ke BAHAR -- extras negative track hon).
//     valid (count==0) -> shrink: mp[s[i]]++; if(mp[s[i]]>0) count++; i++  (i ko tightest tak).
//     ans += i  (while ke BAAD) -> is right-end pe valid left-starts ki ginti.
//  ★ mp<0 ka matlab = "window me is char ke kitne EXTRA" -> shrink me: extra hatao (mp<=0, valid rehta),
//     zaroori hatao (mp>0 ho gaya, window toota). isliye mp-- if ke BAHAR (warna extras 0 pe atak -> galat).
// ============================================================

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

int numberOfSubstrings(string s)
{
    unordered_map<char, int> mp;
    mp['a']++;
    mp['b']++;
    mp['c']++;
    int count = 3;

    int i = 0, j = 0, ans = 0;
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
