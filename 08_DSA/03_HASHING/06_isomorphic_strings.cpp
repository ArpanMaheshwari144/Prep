// ============================================================
// ISOMORPHIC STRINGS — Hashing  (fresh)
// ============================================================
// do string s aur t (same length). TRUE agar s ke characters ko REPLACE karke
// t banaya ja sakta -- har char ek hi char me map ho (consistent), aur do alag
// char SAME char me map na hon (one-to-one).
//   "egg", "add"   -> true   (e->a, g->d  consistent)
//   "foo", "bar"   -> false  (o ko b AUR r dono me nahi map kar sakte)
//   "paper","title"-> true   (p->t, a->i, e->l, r->e)
//   "badc","baba"  -> false  (d aur c dono 'a'/'b' me clash)


//
// Tests (s, t -> expected):
//   "egg", "add"      -> 1
//   "foo", "bar"      -> 0
//   "paper", "title"  -> 1
//   "badc", "baba"    -> 0
//   "a", "a"          -> 1
//   "ab", "aa"        -> 0     (a->a, b->a : do char same me map = galat)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isIsomorphic(string s, string t)
{
    unordered_map<char, char> mp1;
    unordered_map<char, char> mp2;
    for (int i = 0; i < s.size(); i++)
    {
        // || KYUN (&& nahi): do taraf ka clash-check hai -- mp1 (s->t) side AUR mp2 (t->s) side.
        //   in dono me se KOI BHI EK clash mila -> galat -> return false. isliye OR (||).
        //   (&& hota to DONO taraf ek saath clash chahiye hota -> galat; ek hi taraf ka clash bhi false hona chahiye.)
        //   example: pehle g->d map kiya; ab wahi g phir 'e' char pe aa raha (g->e) -> mp1 side clash -> || se turant false.
        // NOTE: !mp1.empty() && !mp2.empty() guard HATA diya -- redundant. count() khud absent-key
        //   handle karta (0 return -> condition false -> koi clash nahi -> else me jodo).
        if ((mp1.count(s[i]) == 1 && mp1[s[i]] != t[i]) ||
            (mp2.count(t[i]) == 1 && mp2[t[i]] != s[i]))
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
    return 0;
}
