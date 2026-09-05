// ============================================================
// MAJORITY ELEMENT — (LeetCode 169)   (fresh)
// ============================================================
// ek array nums diya jisme ek element "majority" hai -- yaani wo n/2 se
// ZYADA baar aata hai (n = array size). us majority element ko return karo.
// (maan lo majority hamesha exist karta hai.)
//
//   [3,2,3]           -> 3
//   [2,2,1,1,1,2,2]   -> 2
//   [1]               -> 1
//   [5,5,5,2,2]       -> 5
//   [4,4,4,4,3,3,2]   -> 4
//
// ============================================================
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int majorityElement(vector<int> &nums)
{
    // int n = nums.size();
    // unordered_map<int, int> mp;
    // for (auto &it : nums)
    // {
    //     mp[it]++;
    // }

    // for (auto &it : mp)
    // {
    //     if (it.second > n / 2)
    //     {
    //         return it.first;
    //     }
    // }
    // return 0;

    int count = 1;
    int candidate = nums[0];
    for (int i = 1; i < nums.size(); i++)
    {
        if (candidate == nums[i])
        {
            count++;
        }
        else if (candidate != nums[i])
        {
            count--;

            if (count == 0)
            {
                candidate = nums[i];
                count = 1;
            }
        }
    }
    return candidate;
}

int main()
{
    vector<int> a = {3, 2, 3};
    vector<int> b = {2, 2, 1, 1, 1, 2, 2};
    vector<int> c = {1};
    vector<int> d = {5, 5, 5, 2, 2};
    vector<int> e = {4, 4, 4, 4, 3, 3, 2};

    cout << majorityElement(a) << " (expected 3)\n";
    cout << majorityElement(b) << " (expected 2)\n";
    cout << majorityElement(c) << " (expected 1)\n";
    cout << majorityElement(d) << " (expected 5)\n";
    cout << majorityElement(e) << " (expected 4)\n";
    return 0;
}
