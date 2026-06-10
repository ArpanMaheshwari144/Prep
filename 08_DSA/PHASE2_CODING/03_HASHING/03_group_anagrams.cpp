// ============================================================
// GROUP ANAGRAMS   —   Hashing (HashMap grouping by sorted-key)
// ============================================================
// Strings ka array `strs`. Anagrams (same letters, alag order) ko SAATH group karo.
// Return: vector<vector<string>> (har group ek list).
//
// Example:
//   ["eat","tea","tan","ate","nat","bat"]
//      ->  [["eat","tea","ate"], ["tan","nat"], ["bat"]]   (kisi bhi order mein)
//
// INTUITION (soch — code TU likhega):
//   Anagrams ki common PEHCHAAN: word ko SORT karo -> anagrams same ban jaate
//   ("eat"->"aet", "tea"->"aet"). Toh sorted-word = group ki key.
//   - "note" (map): sorted-word -> us group ke saare ORIGINAL words ki list.
//   - Har word: sort karke key banao, us key ki list mein original word daalo.
//   - End mein map ki saari lists = answer.
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

int main()
{
    vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
    vector<vector<string>> ans = groupAnagrams(strs);
    for (auto &group : ans)
    {
        cout << "[ ";
        for (auto &w : group)
            cout << w << " ";
        cout << "]" << endl;
    }
    // expected (kisi bhi order): [eat tea ate]  [tan nat]  [bat]
    return 0;
}
