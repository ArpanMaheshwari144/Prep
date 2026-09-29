// ============================================================
// GROUP ANAGRAMS (LC 49) — REDO_1 (khaali page)
// ============================================================
// Strings ka array diya. Jo strings ek doosre ke ANAGRAM hain
// (same letters, order alag — "eat","tea","ate"), unhe ek group me rakho.
// Saare groups lautao. Groups ka order aur group ke andar ka order koi bhi chalega.
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   ["eat","tea","tan","ate","nat","bat"] -> [eat,tea,ate] [tan,nat] [bat]
//   ["a"]                                 -> [a]
//   ["",""]                               -> [,]
//   ["abc","def"]                         -> [abc] [def]
//   ["ab","ba","ab"]                      -> [ab,ba,ab]
//   ["aab","aba","baa","abb"]             -> [aab,aba,baa] [abb]
//   ["ddddddddddg","dgggggggggg"]         -> [ddddddddddg] [dgggggggggg]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string> &strs)
{
    unordered_map<string, vector<string>> mp;
    for (auto &it : strs)
    {
        string key = it;
        sort(begin(key), end(key));
        mp[key].push_back(it);
    }

    vector<vector<string>> ans;
    for (auto &it : mp)
    {
        ans.push_back(it.second);
    }
    return ans;
}

// ============================ TESTS ============================
int passed = 0, total = 0;

vector<vector<string>> normalize(vector<vector<string>> g)
{
    for (auto &x : g)
        sort(x.begin(), x.end());
    sort(g.begin(), g.end());
    return g;
}

void check(vector<string> in, vector<vector<string>> exp)
{
    total++;
    auto got = groupAnagrams(in);
    bool ok = normalize(got) == normalize(exp);
    if (ok)
        passed++;
    cout << (ok ? "PASS  " : "FAIL  ") << "test " << total << "  groups got=" << got.size()
         << " exp=" << exp.size() << "\n";
}

int main()
{
    check({"eat", "tea", "tan", "ate", "nat", "bat"}, {{"eat", "tea", "ate"}, {"tan", "nat"}, {"bat"}});
    check({"a"}, {{"a"}});
    check({"", ""}, {{"", ""}});
    check({"abc", "def"}, {{"abc"}, {"def"}});
    check({"ab", "ba", "ab"}, {{"ab", "ba", "ab"}});
    check({"aab", "aba", "baa", "abb"}, {{"aab", "aba", "baa"}, {"abb"}});
    check({"ddddddddddg", "dgggggggggg"}, {{"ddddddddddg"}, {"dgggggggggg"}});

    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
