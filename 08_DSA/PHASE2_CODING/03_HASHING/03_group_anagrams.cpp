// ============================================================
// GROUP ANAGRAMS                       (Pattern: Hashing - HashMap grouping)
// ============================================================
// Strings ka array `strs` diya hai. Anagrams ko SAATH group karo.
// (Anagram = same letters, alag order — "eat" / "tea" / "ate".)
// Return: vector<vector<string>> — har group ek list.
//
// Example:
//   strs = ["eat","tea","tan","ate","nat","bat"]
//   ->  [ ["eat","tea","ate"], ["tan","nat"], ["bat"] ]
//   (group order / andar ka order koi bhi ho sakta — sab anagrams saath)
//
// HINT (concept, code nahi): do anagrams ko SORT karo to dono SAME ban jaate
//   ("eat"->"aet", "tea"->"aet"). Toh sorted-word = group ka KEY.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    vector<vector<string>> groupAnagrams(vector<string> &strs)
    {
        // TODO: tera code (HashMap: key = sorted word, value = list of words) — SOLO
        unordered_map<string, vector<string>> mp;
        for (auto &str : strs)
        {
            string key = str;
            sort(begin(key), end(key));
            mp[key].push_back(str);
        }

        vector<vector<string>> ans;
        for (auto &it : mp)
        {
            ans.push_back(it.second);
        }
        return ans;
    }
};

// ---- test harness (run karke verify — yeh solution nahi, bas runner) ----
int main()
{
    Solution s;
    vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
    vector<vector<string>> ans = s.groupAnagrams(strs);
    for (auto &group : ans)
    {
        cout << "[ ";
        for (auto &w : group)
            cout << w << " ";
        cout << "]" << endl;
    }
    // expected (kisi bhi order mein): [eat tea ate]  [tan nat]  [bat]
    return 0;
}
