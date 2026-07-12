// ============================================================
// FRUIT INTO BASKETS — Sliding Window  (fresh)
// ============================================================
// fruits[] = tree row, har number ek fruit-TYPE. tere paas 2 basket, har basket
// me SIRF EK type. tu ek continuous window (row me consecutive trees) se fruit uthata.
// SABSE LAMBI window ki LENGTH lauta jisme AT MOST 2 alag types hon.
//   (= "longest subarray with AT MOST 2 distinct elements")
//   [1,2,1]       -> 3   (1,2,1 -> 2 types)
//   [0,1,2,2]     -> 3   (1,2,2)
//   [1,2,3,2,2]   -> 4   (2,3,2,2)
//
// (approach yahan LIKHA NAHI -- code TU likhega. Arpan ki approach BAAD me,
//  jab tu code kare. copy-pen pe trace pehle.)
//
// Tests (// expected):
//   [1,2,1]         -> 3
//   [0,1,2,2]       -> 3
//   [1,2,3,2,2]     -> 4
//   [3,3,3,1,2,1,1,2,3,3,4] -> 5
//   [1]             -> 1
//   [1,1,1,1]       -> 4
// ============================================================

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int totalFruit(vector<int> &fruits)
{
    int ans = INT_MIN;
    unordered_map<int, int> mp; // {0, 1, 2, 2};
    int i = 0, j = 0;
    while (j < fruits.size())
    {
        mp[fruits[j]]++;
        while (mp.size() > 2)
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

int main()
{
    vector<int> a = {1, 2, 1};
    vector<int> b = {0, 1, 2, 2};
    vector<int> c = {1, 2, 3, 2, 2};
    vector<int> d = {3, 3, 3, 1, 2, 1, 1, 2, 3, 3, 4};
    vector<int> e = {1};
    vector<int> f = {1, 1, 1, 1};

    cout << totalFruit(a) << " (expected 3)\n";
    cout << totalFruit(b) << " (expected 3)\n";
    cout << totalFruit(c) << " (expected 4)\n";
    cout << totalFruit(d) << " (expected 5)\n";
    cout << totalFruit(e) << " (expected 1)\n";
    cout << totalFruit(f) << " (expected 4)\n";
    return 0;
}
