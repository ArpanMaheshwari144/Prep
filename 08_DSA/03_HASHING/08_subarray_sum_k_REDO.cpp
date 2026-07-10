// ============================================================
// SUBARRAY SUM EQUALS K — Hashing + Prefix Sum (SPACED-REDO, blank)
// ============================================================
// array nums aur int k diya. kitne CONTINUOUS subarrays hain jinka sum == k?
// (COUNT lautao). negatives bhi ho sakte.
//   nums=[1,1,1], k=2  -> 2   ([1,1] index0-1, [1,1] index1-2)
//   nums=[1,2,3], k=3  -> 2   ([1,2], [3])
//
// (purana code MAT dekhna. copy-pen pe socho:
//  running prefixSum rakho -> map<prefixSum, count>. har step:
//  agar (prefixSum - k) map me hai -> uska count ans me add. phir current prefixSum map me ++.
//  ★ map me {0:1} se start karo (khud se sum=k wale subarray ke liye).)
//
// Tests (// expected):
//   [1,1,1], k=2               -> 2
//   [1,2,3], k=3               -> 2
//   [1,-1,0], k=0              -> 3
//   [3,4,7,2,-3,1,4,2], k=7    -> 4
//   [1], k=0                   -> 0
//   [-1,-1,1], k=0             -> 1
// ============================================================

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int subarraySum(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    int sum = 0, count = 0;
    mp[0] = 1;
    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i];
        count += mp[sum - k];
        mp[sum]++;
    }
    return count;
}

int main()
{
    vector<int> a = {1, 1, 1};
    vector<int> b = {1, 2, 3};
    vector<int> c = {1, -1, 0};
    vector<int> d = {3, 4, 7, 2, -3, 1, 4, 2};
    vector<int> e = {1};
    vector<int> f = {-1, -1, 1};

    cout << subarraySum(a, 2) << " (expected 2)\n";
    cout << subarraySum(b, 3) << " (expected 2)\n";
    cout << subarraySum(c, 0) << " (expected 3)\n";
    cout << subarraySum(d, 7) << " (expected 4)\n";
    cout << subarraySum(e, 0) << " (expected 0)\n";
    cout << subarraySum(f, 0) << " (expected 1)\n";
    return 0;
}
