// ============================================================
// LONGEST SUBSTRING WITH AT MOST K DISTINCT CHARS — Sliding Window  (REDO — fresh dimaag, paper pehle)
// ============================================================
// string s aur int k. sabse LAMBI substring ki LENGTH lauta jisme
// ZYADA-SE-ZYADA k ALAG (distinct) characters hon.
//   s="eceba", k=2  -> 3
//   s="aa",    k=1  -> 2
//
// Tests (s, k -> expected):
//   "eceba", 2   -> 3
//   "aa", 1      -> 2
//   "abcbc", 2   -> 4
//   "aabbcc", 1  -> 2
//   "aabbcc", 2  -> 4
//   "a", 0       -> 0
//   "", 3        -> 0
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int totalFruit(vector<int> &fruits, int k)
{
    int ans = INT_MIN;
    unordered_map<int, int> mp;
    int i = 0, j = 0;
    while (j < fruits.size())
    {
        mp[fruits[j]]++;
        while (mp.size() > k)
        {
            mp[fruits[i]]--;
            if (mp[fruits[i]] == 0)
            {
                mp.erase(fruits[i]);
            }
            i++;
        }
        ans = max(ans, j - i + 1);
        j++;
    }
    return ans == INT_MIN ? 0 : ans;
}

int longestKDistinct(string s, int k)
{
    return totalFruit(s, k)
}

int main()
{
    cout << longestKDistinct("eceba", 2)  << " (expected 3)\n";
    cout << longestKDistinct("aa", 1)     << " (expected 2)\n";
    cout << longestKDistinct("abcbc", 2)  << " (expected 4)\n";
    cout << longestKDistinct("aabbcc", 1) << " (expected 2)\n";
    cout << longestKDistinct("aabbcc", 2) << " (expected 4)\n";
    cout << longestKDistinct("a", 0)      << " (expected 0)\n";
    cout << longestKDistinct("", 3)       << " (expected 0)\n";
    return 0;
}
