// ============================================================
// TWO SUM   —   Arrays & Hashing   [COLD REDO]
// ============================================================
// Unsorted array + target. Do INDEX do jinki values ka jod = target.
// Exactly ek answer hota (assume). Same element 2 baar use nahi.
//   [2,7,11,15], target=9  -> [0,1]   (2+7)
//   [3,2,4],     target=6  -> [1,2]   (2+4)
//   [3,3],       target=6  -> [0,1]
//
// SIGNAL: "do number ka jod = target, unsorted" -> Hashing (complement map).
//   (blank se, apni soch. self-run Ctrl+Alt+N. no peeking.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> twoSum(vector<int> &nums, int target)
{
    unordered_map<int, int> mp;
    for (int i = 0; i < nums.size(); i++)
    {
        if (mp.find(target - nums[i]) != mp.end())
        {
            return vector<int>{mp[target - nums[i]], i};
        }
        mp[nums[i]] = i;
    }
    return vector<int>{-1, -1};
}

int main()
{
    vector<int> a = {2, 7, 11, 15};
    vector<int> r1 = twoSum(a, 9);
    cout << r1[0] << " " << r1[1] << endl; // expected 0 1

    vector<int> b = {3, 2, 4};
    vector<int> r2 = twoSum(b, 6);
    cout << r2[0] << " " << r2[1] << endl; // expected 1 2

    vector<int> c = {3, 3};
    vector<int> r3 = twoSum(c, 6);
    cout << r3[0] << " " << r3[1] << endl; // expected 0 1

    vector<int> d = {-1, -2, -3, -4, -5};
    vector<int> r4 = twoSum(d, -8);
    cout << r4[0] << " " << r4[1] << endl; // expected 2 4
    return 0;
}
