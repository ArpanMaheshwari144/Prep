// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// FIND ALL ANAGRAMS IN A STRING — REDO_1 (5-Nov, khaali page, C++)
// ============================================================
// do string s aur p. s me p ke SAARE anagram ki START INDEX dhundo (vector me lauta).
// anagram = same characters, order koi bhi (jaise "abc","bca","cab" sab anagram).
//   s="cbaebabacd", p="abc"  -> [0,6]   (s[0..2]="cba", s[6..8]="bac")
//   s="abab",       p="ab"   -> [0,1,2]
//
// Tests (s, p -> expected indices):
//   "cbaebabacd", "abc"  -> 0 6
//   "abab", "ab"         -> 0 1 2
//   "af", "be"           -> (koi nahi)
//   "aa", "bb"           -> (koi nahi)
//   "baa", "aa"          -> 1
//   "a", "ab"            -> (koi nahi)   p, s se lamba
//   "aaaaa", "aa"        -> 0 1 2 3
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isAnagram(string s, string t)
{
    if (s.size() != t.size())
        return 0;

    unordered_map<char, int> mp;
    for (auto &it : s)
        mp[it]++;
    for (auto &it : t)
        mp[it]--;
    for (auto &it : mp)
        if (it.second != 0)
            return false;
    return true;
}

vector<int> findAnagrams(string s, string p)
{
    vector<int> ans;
    int i = 0, j = 0;
    while (j < s.size())
    {
        while (j - i + 1 > p.size())
        {
            i++;
        }
        if (j - i + 1 == p.size())
        {
            if (isAnagram(s.substr(i, j - i + 1), p))
            {
                ans.push_back(i);
            }
        }
        j++;
    }
    return ans;
}

// ---------- printer (boilerplate, chhoo mat) ----------
void check(string s, string p)
{
    vector<int> r = findAnagrams(s, p);
    cout << "s=\"" << s << "\" p=\"" << p << "\" -> [ ";
    for (int x : r)
        cout << x << " ";
    cout << "]\n";
}

int main()
{
    check("cbaebabacd", "abc"); // 0 6
    check("abab", "ab");        // 0 1 2
    check("af", "be");          // (empty)
    check("aa", "bb");          // (empty)
    check("baa", "aa");         // 1
    check("a", "ab");           // (empty)
    check("aaaaa", "aa");       // 0 1 2 3
    return 0;
}
