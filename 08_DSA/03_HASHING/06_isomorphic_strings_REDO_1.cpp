// ============================================================
// ISOMORPHIC STRINGS — Hashing  (REDO_1, 6-Nov)
// ============================================================
// PEHLE YE (30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// Do string s aur t (same length). TRUE agar s ke characters ko REPLACE karke
// t banaya ja sakta -- har char ek hi char me map ho (consistent), aur do alag
// char SAME char me map na hon (one-to-one).
//
// Tests (s, t -> expected):
//   "egg", "add"      -> 1
//   "foo", "bar"      -> 0
//   "paper", "title"  -> 1
//   "badc", "baba"    -> 0
//   "a", "a"          -> 1
//   "ab", "aa"        -> 0
//   "aa", "ab"        -> 0
//   "abc", "xyz"      -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isIsomorphic(string s, string t)
{
    int n = s.size();
    unordered_map<char, char> mp1, mp2;
    for (int i = 0; i < n; i++)
    {
        if ((mp1.count(s[i]) == 1 && mp1[s[i]] != t[i]) || (mp2.count(t[i]) == 1 && mp2[t[i]] != s[i]))
        {
            return false;
        }
        else
        {
            mp1[s[i]] = t[i];
            mp2[t[i]] = s[i];
        }
    }
    return true;
}

int main()
{
    cout << isIsomorphic("egg", "add") << " (expected 1)\n";
    cout << isIsomorphic("foo", "bar") << " (expected 0)\n";
    cout << isIsomorphic("paper", "title") << " (expected 1)\n";
    cout << isIsomorphic("badc", "baba") << " (expected 0)\n";
    cout << isIsomorphic("a", "a") << " (expected 1)\n";
    cout << isIsomorphic("ab", "aa") << " (expected 0)\n";
    cout << isIsomorphic("aa", "ab") << " (expected 0)\n";
    cout << isIsomorphic("abc", "xyz") << " (expected 1)\n";
    return 0;
}
