// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// FIND ALL ANAGRAMS IN A STRING (LC-438) — REDO_1 (khaali page, C++, bina dekhe: main file help se bana tha)
// ============================================================
// do string s aur p (sirf chhote akshar a-z). s me p ke SAARE anagram ki START INDEX dhundo (vector me lauta).
// anagram = same characters, order koi bhi (jaise "abc","bca","cab" sab anagram).
//   s="cbaebabacd", p="abc"  -> [0,6]   (s[0..2]="cba", s[6..8]="bac")
//   s="abab",       p="ab"   -> [0,1,2]
// LeetCode limit: s aur p dono 30,000 tak. purana substr wala code yahan TLE (34/66) -> ye wala BADE test pe bhi tez chale.
//
// Tests (s, p -> expected indices):
//   "cbaebabacd", "abc"  -> 0 6
//   "abab", "ab"         -> 0 1 2
//   "af", "be"           -> (koi nahi)
//   "aa", "bb"           -> (koi nahi)
//   "baa", "aa"          -> 1
//   "a", "ab"            -> (koi nahi)   p, s se lamba
//   "aaaaa", "aa"        -> 0 1 2 3
//   BADA: s = 30000 'a', p = 15000 'a' -> 15001 index, 1 second se kaafi kam me
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> findAnagrams(string s, string p)
{
    vector<int> ans;

    unordered_map<char, int> mp1;
    unordered_map<char, int> mp2;

    for (int i = 0; i < p.size(); i++)
    {
        mp1[p[i] - 'a']++;
    }

    int i = 0, j = 0;
    while (j < s.size())
    {
        mp2[s[j] - 'a']++;
        if (j - i + 1 == p.size())
        {
            if (mp1 == mp2)
            {
                ans.push_back(i);
            }
            mp2[s[i] - 'a']--;
            if (mp2[s[i] - 'a'] == 0)
            {
                mp2.erase(s[i] - 'a');
            }
            i++;
        }
        j++;
    }
    return ans;
}

// ---------- tests (boilerplate, chhoo mat) ----------
void check(string s, string p, vector<int> exp, string name)
{
    vector<int> got = findAnagrams(s, p);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name << " | got=[ ";
    for (int x : got)
        cout << x << " ";
    cout << "] exp=[ ";
    for (int x : exp)
        cout << x << " ";
    cout << "]\n";
}

int main()
{
    check("cbaebabacd", "abc", {0, 6}, "t1");
    check("abab", "ab", {0, 1, 2}, "t2");
    check("af", "be", {}, "t3");
    check("aa", "bb", {}, "t4");
    check("baa", "aa", {1}, "t5");
    check("a", "ab", {}, "t6 p lamba");
    check("aaaaa", "aa", {0, 1, 2, 3}, "t7");

    string S(30000, 'a'), P(15000, 'a');
    auto t0 = chrono::steady_clock::now();
    vector<int> r = findAnagrams(S, P);
    double sec = chrono::duration<double>(chrono::steady_clock::now() - t0).count();
    cout << (r.size() == 15001 ? "PASS" : "FAIL") << " | t8 BADA | got size=" << r.size()
         << " exp=15001 | time=" << sec << "s (REDO_1 wala ~1.1s tha)\n";
    return 0;
}
