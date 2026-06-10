// ============================================================
// SUBARRAY SUM EQUALS K   —   Prefix Sum + HashMap
// ============================================================
// Array `nums` (negatives bhi) + int `k`. Kitne CONTIGUOUS subarrays
// jinka sum = k — woh COUNT return karo.
//
// Example:
//   {1, 1, 1}, k = 2  ->  2
//   {1, 2, 3}, k = 3  ->  2
//   {0, 0, 0}, k = 0  ->  6   (edge)
//
// INTUITION (soch — code TU likhega):
//   Gullak (running sum) chalao — shuru se ab tak ka jod.
//   Har point pe poochho: "(running sum − k) pehle KITNI baar dekha?" (note/map se)
//        utni baar dekha → utne subarrays yahan khatam → count mein jodo.
//   Note (map): har running-sum -> kitni baar aaya.
//   EDGE: shuru pe gullak 0 hota -> map mein 0 ko "1 baar" se shuru karo.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int subarraySum(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    int runningSum = 0;
    int ans = 0;
    mp[0] = 1;
    for (int i = 0; i < nums.size(); i++) // a = {1, 1, 1};
    {
        runningSum += nums[i];
        ans += mp[runningSum - k]; // (hum k find? => runningSum - (runningSum - k) = k)
        mp[runningSum]++;
    }
    return ans;
}

int main()
{
    vector<int> a = {1, 1, 1};
    cout << subarraySum(a, 2) << endl; // expected: 2

    vector<int> b = {0, 0, 0};
    cout << subarraySum(b, 0) << endl; // expected: 6
    return 0;
}
