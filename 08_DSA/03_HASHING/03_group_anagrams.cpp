// ============================================================
// GROUP ANAGRAMS   (Hashing — canonical KEY se grouping)
// ============================================================
// Strings ka array diya. Anagrams ko SAATH group karo.
// (anagram = same letters, order alag. "eat","tea","ate" ek group.)
//
// ---- ARPAN KI APPROACH ----
//  har word ko SORT karke canonical key banao ("eat"/"tea" -> "aet" = same key).
//  map { sorted-key -> list-of-words }. same key wale ek group. map ki values = answer.
//
// Tests (// expected — groups; order alag ho sakta):
//   ["eat","tea","tan","ate","nat","bat"]
//     -> [eat,tea,ate] [tan,nat] [bat]
//   ["a"]              -> [a]
//   ["",""]           -> [ , ]   (dono empty -> ek group)
// ============================================================

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string> &strs)
{
    unordered_map<string, vector<string>> mp;
    vector<vector<string>> ans;

    // ["eat","tea","tan","ate","nat","bat"]
    for (auto &it : strs)
    {
        string key = it;
        sort(begin(key), end(key));
        mp[key].push_back(it);
    }

    for (auto &it : mp)
    {
        ans.push_back(it.second);
    }
    return ans;
}

void print(vector<vector<string>> &res)
{
    for (auto &g : res)
    {
        cout << "[";
        for (int i = 0; i < g.size(); i++)
            cout << g[i] << (i + 1 < g.size() ? "," : "");
        cout << "] ";
    }
    cout << "\n";
}

int main()
{
    vector<string> a1 = {"eat", "tea", "tan", "ate", "nat", "bat"};
    vector<string> a2 = {"a"};
    vector<string> a3 = {"", ""};

    auto r1 = groupAnagrams(a1);
    print(r1); // [eat,tea,ate] [tan,nat] [bat]
    auto r2 = groupAnagrams(a2);
    print(r2); // [a]
    auto r3 = groupAnagrams(a3);
    print(r3); // [,]
    return 0;
}
