// ============================================================
// TOP K FREQUENT ELEMENTS   —   Hashing (frequency map) + top-k
// ============================================================
// Array + int k. Sabse ZYADA baar aane wale K elements return karo.
//
// Example:
//   nums = {1,1,1,2,2,3}, k = 2  ->  {1, 2}   (1 -> 3 baar, 2 -> 2 baar)
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   map mein frequency nikaalo (element -> count).
//   phir top-k frequency waale elements return karo.
//   top-k nikaalne ka tareeka: frequency se SORT karke top k (O(n log n)), YA
//   bucket-sort (index = frequency, peeche se high-freq se k uthao -> O(n)).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> topKFrequent(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    vector<int> ans;
    for (auto &it : nums)
    {
        mp[it]++;
    }
    /*{1, 3}, {2,2}, {3, 1}*/

    vector<vector<int>> bucket(nums.size() + 1);
    for (auto &it : mp)
    {
        bucket[it.second].push_back(it.first);
    }
    /* {{bucket[1] = 3, bucket[2] = 2, bucket[3] = 1}} */

    for (int i = bucket.size() - 1; i >= 0; i--)
    {
        for (auto &it : bucket[i])
        {
            ans.push_back(it);
            if (ans.size() == k)
                return ans;
        }
    }
    return {-1, -1};
}

int main()
{
    vector<int> nums = {1, 1, 1, 2, 2, 3};
    vector<int> ans = topKFrequent(nums, 2);
    for (int x : ans)
        cout << x << " "; // expected: 1 2  (kisi bhi order)
    cout << endl;
    return 0;
}
