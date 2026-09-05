// ============================================================
// REORGANIZE STRING — (LeetCode 767)   (fresh)
// ============================================================
// Ek string s di. Uske characters ko is tarah REARRANGE karo ki
// koi bhi DO ADJACENT char SAME na ho.
// Koi bhi ek valid rearrangement lauta do. Agar possible hi NAHI -> "" lauta.
//
//   s="aab"   -> "aba"        (koi bhi valid chalega, e.g. "aba")
//   s="aaab"  -> ""           (a itni zyada ki spread ho hi nahi sakti)
//
// INPUT-format (tests ke liye):
//   s        = input string
//   possible = true agar valid rearrangement banta hai, warna false
//   -> check() tera reorganizeString(s) chalata, phir verify:
//        possible==false -> tera output "" hona chahiye.
//        possible==true  -> output (a) s ka PERMUTATION ho (same char counts),
//                           (b) koi 2 adjacent same NA ho.
//      (exact string match NAHI — koi bhi valid answer PASS.)
//
//   "aab"     possible   -> e.g. aba
//   "aaab"    NOT         -> ""
//   "a"       possible    -> a
//   "aa"      NOT          -> ""
//   "aaabc"   possible    -> e.g. abaca
//   "vvvlo"   possible    -> e.g. vlvov
//   "aaabbb"  possible    -> e.g. ababab
//
// FAMILY (nudge, baaki KHUD): ye HEAP wala hai — "sabse zyada baar wala char"
//   baar-baar nikaalna. andar ek twist hai (turant wapas mat daal). derive kar.
// ============================================================
// ============================================================
#include <bits/stdc++.h>
using namespace std;

string reorganizeString(string s)
{
    string ans = "";
    unordered_map<char, int> mp;
    priority_queue<pair<int, char>> maxHeap;

    for (auto it : s)
    {
        mp[it]++;
    }

    for (auto &it : mp)
    {
        maxHeap.push({it.second, it.first});
    }

    while (maxHeap.size() > 1)
    {
        auto tp1 = maxHeap.top();
        maxHeap.pop();
        auto tp2 = maxHeap.top();
        maxHeap.pop();

        ans += tp1.second;
        ans += tp2.second;

        tp1.first -= 1;
        tp2.first -= 1;

        if (tp1.first >= 1)
        {
            maxHeap.push(tp1);
        }
        if (tp2.first >= 1)
        {
            maxHeap.push(tp2);
        }
    }

    if (!maxHeap.empty())
    {
        if (maxHeap.top().first > 1)
        {
            return "";
        }
        else
        {
            ans += maxHeap.top().second;
        }
    }
    return ans;
}

// ---------- helper (chhoo mat) ----------
void check(string s, bool possible)
{
    string got = reorganizeString(s);
    bool ok = true;
    string why = "";

    if (!possible)
    {
        if (got != "")
        {
            ok = false;
            why = "expected \"\" (impossible) but got \"" + got + "\"";
        }
    }
    else
    {
        // (1) same length + same char counts (permutation)?
        if (got.size() != s.size())
        {
            ok = false;
            why = "length mismatch";
        }
        else
        {
            vector<int> ca(256, 0), cb(256, 0);
            for (char c : s)
                ca[(unsigned char)c]++;
            for (char c : got)
                cb[(unsigned char)c]++;
            if (ca != cb)
            {
                ok = false;
                why = "not a permutation of input";
            }
        }
        // (2) no two adjacent same?
        for (int i = 1; ok && i < (int)got.size(); i++)
        {
            if (got[i] == got[i - 1])
            {
                ok = false;
                why = "adjacent same @" + to_string(i) + " (\"" + got + "\")";
                break;
            }
        }
    }

    cout << (ok ? "PASS" : "FAIL");
    if (!ok)
        cout << "  (" << why << ")";
    cout << "\n";
}

int main()
{
    check("aab", true);
    check("aaab", false);
    check("a", true);
    check("aa", false);
    check("aaabc", true);
    check("vvvlo", true);
    check("aaabbb", true);
    return 0;
}
