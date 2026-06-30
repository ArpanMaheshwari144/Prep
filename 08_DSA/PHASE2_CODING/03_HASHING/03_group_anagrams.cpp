// ============================================================
// GROUP ANAGRAMS   (Hashing — sorted-word as key)
// ============================================================
// String array. Anagrams (same letters, alag order) ko group karo.
//
// SOCH (seed):
//   2 words anagram -> letters SORT karne pe SAME ("eat"->"aet", "tea"->"aet").
//   -> sorted-word ko KEY banao (unordered_map<string, vector<string>>),
//      same key wale ek group me push.
//
// Tests (// expected — order alag ho sakta, groups same hone chahiye):
//   ["eat","tea","tan","ate","nat","bat"] -> [eat,tea,ate] [tan,nat] [bat]
//   ["",""]                                -> ["",""]
//   ["a"]                                  -> [a]
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

void print(vector<vector<string>> &res)
{
    for (auto &grp : res)
    {
        cout << "[ ";
        for (auto &w : grp)
            cout << "\"" << w << "\" ";
        cout << "] ";
    }
    cout << "\n";
}

int main()
{
    vector<string> a1 = {"eat", "tea", "tan", "ate", "nat", "bat"};
    vector<string> a2 = {"", ""};
    vector<string> a3 = {"a"};

    auto r1 = groupAnagrams(a1);
    print(r1); // expected: [eat,tea,ate] [tan,nat] [bat]
    auto r2 = groupAnagrams(a2);
    print(r2); // expected: ["",""]
    auto r3 = groupAnagrams(a3);
    print(r3); // expected: [a]
    return 0;
}
