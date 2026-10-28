// ═══════════════════════════════════════════════════════════════════════
//  PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: ★ EDGE line + ek chhota dushman-example chalao
// ═══════════════════════════════════════════════════════════════════════
//  CAESAR: K PEECHE  (Wise OA, 27-Oct jaisa)
//  s me bade akshar A-Z hain. Har akshar ko alphabet me k jagah PEECHE khiskao.
//  A se peeche jaao to Z pe wapas aao (gol ghoomta hai).
//  A-Z ke alawa jo bhi hai (space wagairah) waisa hi rehne do. Naya string lautao.
//
//  INPUT-FORM: 0 <= s.size() <= 1e5, 0 <= k <= 1e9.
//  TEST-CASES (input -> expected):
//   1) "DEF", 3          -> "ABC"
//   2) "ABC", 3          -> "XYZ"          (A se peeche -> Z)
//   3) "A", 1            -> "Z"
//   4) "HELLO", 0        -> "HELLO"
//   5) "ABC", 26         -> "ABC"          (poora chakkar)
//   6) "ABC", 29         -> "XYZ"          (k > 26)
//   7) "", 5             -> ""
//   8) "HELLO WORLD", 1  -> "GDKKN VNQKC"  (space waisa hi)
//   9) "Z", 25           -> "A"
//  10) "A", 1000000000   -> "O"            (k bahut bada)
//  11) "XYZ", 52         -> "XYZ"
//  12) "WISE", 4         -> "SEOA"
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

string shiftBack(string s, int k)
{
    if (s.size() == 0)
    {
        return "";
    }

    while (k > 26)
    {
        k = k % 26;
    }

    string ans = "";
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == ' ')
        {
            ans += ' ';
            continue;
        }
        ans += ((((s[i] - 'A') - k) + 26) % 26) + 'A';
    }
    return ans;
}

// ---- test helper ----
void check(string s, int k, string exp, string name)
{
    string got = shiftBack(s, k);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=\"" << got << "\" exp=\"" << exp << "\"\n";
}

int main()
{
    check("DEF", 3, "ABC", "t1");
    check("ABC", 3, "XYZ", "t2 A se peeche");
    check("A", 1, "Z", "t3");
    check("HELLO", 0, "HELLO", "t4 k=0");
    check("ABC", 26, "ABC", "t5 poora chakkar");
    check("ABC", 29, "XYZ", "t6 k>26");
    check("", 5, "", "t7 khaali");
    check("HELLO WORLD", 1, "GDKKN VNQKC", "t8 space");
    check("Z", 25, "A", "t9");
    check("A", 1000000000, "O", "t10 bada k");
    check("XYZ", 52, "XYZ", "t11");
    check("WISE", 4, "SEOA", "t12");
    return 0;
}
