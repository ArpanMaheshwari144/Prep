// ============================================================
// FIND ALL ANAGRAMS IN A STRING — Sliding Window  (fresh)
// ============================================================
// do string s aur p. s me p ke SAARE anagram ki START INDEX dhundo (vector me lauta).
// anagram = same characters, order koi bhi (jaise "abc","bca","cab" sab anagram).
//   s="cbaebabacd", p="abc"  -> [0,6]   (s[0..2]="cba"=anagram, s[6..8]="bac"=anagram)
//   s="abab",       p="ab"   -> [0,1,2] (s[0..1]="ab", s[1..2]="ba", s[2..3]="ab")
//
// Tests (s, p -> expected indices):
//   "cbaebabacd", "abc"  -> 0 6
//   "abab", "ab"         -> 0 1 2
//   "af", "be"           -> (koi nahi)
//   "aa", "bb"           -> (koi nahi)
//   "baa", "aa"          -> 1
//
// ---- ARPAN KI APPROACH ----
//  ye ANAGRAM check + WINDOW ka combo hai (valid-anagram wahi helper reuse kiya).
//  helper isAnagram(a, b): same length + har char ka count barabar -> anagram.
//  s pe i/j se FIXED window (p ki length ka) chalao:
//    window size (j-i+1) > p.size() -> i++ (window ko p-size pe rakho).
//    window size == p.size() -> us window ke substr(i, len) pe isAnagram(substr, p)?
//        anagram mila -> ans me START index (i) daalo.
//  j har baar ++.  (fixed window slide + har window pe anagram-check.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isAnagram(string s, string t)
{
    // cout << "-->" << s << " " << t << endl;
    if (s.size() != t.size())
    {
        return 0;
    }

    unordered_map<char, int> mp;
    for (auto &it : s)
    {
        mp[it]++;
    }

    for (auto &it : t)
    {
        mp[it]--;
    }

    for (auto &it : mp)
    {
        if (it.second != 0)
        {
            return false;
        }
    }
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
            // cout << j - i + 1 << endl;
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
    return 0;
}
