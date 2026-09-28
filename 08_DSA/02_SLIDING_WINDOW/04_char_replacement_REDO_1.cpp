// ============================================================
// LONGEST REPEATING CHARACTER REPLACEMENT (LC 424) — REDO_1 (28-Sep, khaali page)
// ============================================================
// String s (sirf UPPERCASE letters) aur ek number k diya.
// Tu ZYADA SE ZYADA k characters ko kisi bhi doosre uppercase letter me
// BADAL sakta hai. Badalne ke baad, SABSE LAMBI substring ki LENGTH nikaalo
// jisme SAARE characters SAME ho.
//
// Walmart ke asli DSA round me aaya.
// Expected answers brute force se nikaale hain.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int characterReplacement(string s, int k)
{
    unordered_map<char, int> mp;
    int maxLen = INT_MIN, maxFreq = INT_MIN, i = 0, j = 0;
    while (j < s.size())
    {
        mp[s[j]]++;
        maxFreq = max(maxFreq, mp[s[j]]);

        while (j - i + 1 - maxFreq > k)
        {
            mp[s[i]]--;
            if (mp[s[i]] == 0)
            {
                mp.erase(s[i]);
            }
            i++;
        }

        if (j - i + 1 - maxFreq <= k)
        {
            maxLen = max(maxLen, j - i + 1);
        }
        j++;
    }
    return maxLen;
}

// ============================ TESTS ============================
int passed = 0, total = 0;
void check(const string &s, int k, int exp)
{
    total++;
    int got = characterReplacement(s, k);
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << (ok ? "PASS  " : "FAIL  ") << "\"" << (s.size() > 20 ? s.substr(0, 20) + "..." : s)
         << "\" k=" << k << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check("ABAB", 2, 4);
    check("AABABBA", 1, 4);
    check("AAAA", 0, 4);
    check("ABCDE", 1, 2);
    check("AAAB", 0, 3);
    check("A", 0, 1);
    check("ABBB", 2, 4);
    check("BAAAB", 2, 5);
    check("ABCABCABC", 2, 4);
    check("AAABBBCCCAAA", 3, 6);
    check("KRSCDCSONAJNHLBMDQGIFCPEKPOHQIHLTDIQGEKLRLCQNBOHNDQGHJPNDQPERNFSSSRDEQLFPCCCARFMDLHADJADAGNNSBNCJQOF", 4, 7);

    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
