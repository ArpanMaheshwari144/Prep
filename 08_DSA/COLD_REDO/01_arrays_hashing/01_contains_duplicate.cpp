// ============================================================
// CONTAINS DUPLICATE   —   Arrays & Hashing   [COLD REDO]
// ============================================================
// Array mein koi value EK se zyada baar hai? -> true. Sab unique -> false.
//   [1,2,3,1]       -> true   (1 do baar)
//   [1,2,3,4]       -> false
//   [1,1,1,3,3,4,3,2,4,2] -> true
//
// SIGNAL: "koi value repeat / pehle dekha?" -> Hashing (set/map).
//   (blank se, apni soch se. self-run: Ctrl+Alt+N. no peeking old code.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool containsDuplicate(vector<int> &nums)
{
    unordered_map<int, int> mp;

    for (auto &it : nums)
    {
        mp[it]++;

        if (mp[it] > 1)
        {
            return true;
        } 
    }
    return false;
}

int main()
{
    vector<int> a = {1, 2, 3, 1};
    cout << containsDuplicate(a) << endl; // expected 1

    vector<int> b = {1, 2, 3, 4};
    cout << containsDuplicate(b) << endl; // expected 0

    vector<int> c = {1, 1, 1, 3, 3, 4, 3, 2, 4, 2};
    cout << containsDuplicate(c) << endl; // expected 1

    vector<int> d = {1};
    cout << containsDuplicate(d) << endl; // expected 0

    vector<int> e = {};
    cout << containsDuplicate(e) << endl; // expected 0
    return 0;
}
