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

// ---- ARPAN KI APPROACH ----
//  ★ 2 MAP wala trick (dono taraf ki mapping): mp1 = s->t, mp2 = t->s.
//  (sirf ek map kaafi nahi -- "ab"->"aa" me a->a, b->a: do alag s-char same t-char me clash;
//   ye clash pakadne ke liye ULTI taraf (t->s) ki mapping bhi chahiye.)
//
//  ★ MAP me CHECK karne ki technique (yahi seekhi -- har map-problem me kaam aati):
//     "key pehle se hai kya, aur agar hai to uski VALUE wahi hai jo abhi chahiye?"
//        mp1.count(s[i]) == 1   -> s[i] pehle map ho chuka?
//        mp1[s[i]] != t[i]      -> par ab alag t-char pe aa raha -> CLASH -> false.
//     dono taraf yahi: mp2 ke liye bhi (t[i] pehle kisi aur s-char se to nahi bandha).
//
//  loop s/t saath: koi bhi taraf clash mila -> return false. warna dono map me jodo (mp1[s[i]]=t[i], mp2[t[i]]=s[i]).
//  poora chal gaya bina clash -> perfect one-to-one mapping -> true.

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
        // ★ || KYUN (&& nahi): do taraf ka clash-check hai -- mp1 (s->t) side AUR mp2 (t->s) side.
        //   in dono me se KOI BHI EK clash mila -> galat -> return false. isliye OR (||).
        //   (&& hota to DONO taraf ek saath clash chahiye hota -> galat; ek hi taraf ka clash bhi false hona chahiye.)
        //   example: pehle g->d map kiya; ab wahi g phir 'e' char pe aa raha (g->e) -> mp1 side clash -> || se turant false.
        if (!mp1.empty() && !mp2.empty() &&
                (mp1.count(s[i]) == 1 && mp1[s[i]] != t[i]) ||
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
