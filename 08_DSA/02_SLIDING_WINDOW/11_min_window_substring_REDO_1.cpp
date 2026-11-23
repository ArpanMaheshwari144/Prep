// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// MINIMUM WINDOW SUBSTRING   (LC-76 — REDO_1, khaali page)
// ============================================================
// Do string s aur t. s ka sabse CHHOTA substring (lagaatar) lautao jisme t ke
// SAARE akshar hon, duplicate bhi gin ke (t me do 'a' hain to window me bhi kam se kam do 'a').
// Aisa koi substring na ho to "" lautao. Bade / chhote akshar alag maane jaate ('A' != 'a').
//
//   s = "ADOBECODEBANC", t = "ABC" -> "BANC"
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   "ADOBECODEBANC",       "ABC" -> "BANC"
//   "a",                   "a"   -> "a"
//   "a",                   "aa"  -> ""
//   "aa",                  "aa"  -> "aa"
//   "ab",                  "b"   -> "b"
//   "a",                   "b"   -> ""
//   "cabwefgewcwaefgcf",   "cae" -> "cwae"
//   "aaflslflsldkalskaaa", "aaa" -> "aaa"
//   "abc",                 "cba" -> "abc"
//   "bba",                 "ab"  -> "ba"
//   "aAbB",                "ab"  -> "aAb"
//   "xyzzyx",              "zz"  -> "zz"
// ============================================================

#include <bits/stdc++.h>
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

// ---- test helper (ise mat chhed) ----
int passed = 0, total = 0;
void check(const string &s, const string &t, const string &exp)
{
    total++;
    string got = minWindow(s, t);
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << "s=\"" << s << "\" t=\"" << t << "\" -> got \"" << got << "\" | exp \"" << exp << "\""
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check("ADOBECODEBANC", "ABC", "BANC");
    check("a", "a", "a");
    check("a", "aa", "");
    check("aa", "aa", "aa");
    check("ab", "b", "b");
    check("a", "b", "");
    check("cabwefgewcwaefgcf", "cae", "cwae");
    check("aaflslflsldkalskaaa", "aaa", "aaa");
    check("abc", "cba", "abc");
    check("bba", "ab", "ba");
    check("aAbB", "ab", "aAb");
    check("xyzzyx", "zz", "zz");
    cout << "\n"
         << passed << "/" << total << " PASS\n";
    return 0;
}
