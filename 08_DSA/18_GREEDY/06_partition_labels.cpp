// ============================================================
// PEHLE YE (har problem pe, 30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// PARTITION LABELS   (LC-763 — NAYA)
// ============================================================
// String s (sirf chhote akshar a-z). s ko jitne ZYADA se zyada tukdon me kaat sakte ho kaato,
// shart: har akshar sirf EK hi tukde me aaye (koi akshar do tukdon me bata nahi hona chahiye).
// Tukde kram me hi rahenge (jodne pe wapas s ban jaaye).
// Har tukde ki LAMBAI lautao, kram me.
//
//   "ababcbacadefegdehijhklij" -> [9,7,8]
//     kyun: "ababcbaca" | "defegde" | "hijhklij"  (a, b, c sirf pehle me; d, e, f, g doosre me; baaki teesre me)
//   "eccbbbbdec" -> [10]   (e pehle bhi hai, aakhir me bhi -> poori string ek hi tukda)
//
// Tests:
//   "ababcbacadefegdehijhklij" -> [9,7,8] · "eccbbbbdec" -> [10] · "a" -> [1]
//   "abc" -> [1,1,1] · "aaaa" -> [4] · "abca" -> [4] · "abac" -> [3,1]
//   "caedbdedda" -> [1,9] · "abcdab" -> [6] · "qiejxqfnqceocmy" -> [13,1,1]
// ============================================================
#include <bits/stdc++.h>
using namespace std;

vector<int> partitionLabels(string s)
{
    int n = s.size();

    // sabki last occurenec store karlo bus kaam kahtma dekh har chceh ka lst update karo size nikal lo
    vector<int> mp(26, -1);
    for (int i = 0; i < n; i++)
    {
        int index = s[i] - 'a';
        mp[index] = i;
    }

    vector<int> ans;
    int i = 0;
    while (i < n)
    {
        int end = mp[s[i] - 'a'];

        int j = i;
        while (j < end)
        {
            end = max(end, mp[s[j] - 'a']);
            j++;
        }
        ans.push_back(j - i + 1);
        i = j + 1;
    }
    return ans;
}

// ---------- checker (ise mat chhed) ----------
int passed = 0, total = 0;
string show(const vector<int> &v)
{
    string r = "[";
    for (int i = 0; i < (int)v.size(); i++)
        r += (i ? "," : "") + to_string(v[i]);
    return r + "]";
}
void check(string s, vector<int> expected)
{
    total++;
    vector<int> got = partitionLabels(s);
    bool ok = (got == expected);
    if (ok)
        passed++;
    cout << "test " << total << "  got=" << show(got) << "  expected=" << show(expected)
         << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    check("ababcbacadefegdehijhklij", {9, 7, 8});
    check("eccbbbbdec", {10});
    check("a", {1});
    check("abc", {1, 1, 1});
    check("aaaa", {4});
    check("abca", {4});
    check("abac", {3, 1});
    check("caedbdedda", {1, 9});
    check("abcdab", {6});
    check("qiejxqfnqceocmy", {13, 1, 1});
    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
