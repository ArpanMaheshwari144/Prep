// ============================================================
// SUBARRAY SUM EQUALS K   (Hashing — prefix-sum + map)
// ============================================================
// Array nums + ek number k diya. KITNE contiguous subarrays hain
// jinka sum EXACTLY k hai -> woh COUNT return karo.
//
// seed: running SUM + map { prefix-sum -> count }.
//        har num pe: sum += num;
//          agar (sum - k) map me hai -> uski count answer me add karo
//          phir map[sum]++
//        init map[0] = 1  (khaali prefix -> "poora-prefix hi k hai" case count ho).
//
// Tests (// expected):
//   [1,1,1],              k=2  -> 2
//   [1,2,3],              k=3  -> 2   ([1,2] aur [3])
//   [1,-1,0],             k=0  -> 3
//   [3,4,7,2,-3,1,4,2],   k=7  -> 4
// ============================================================

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int subarraySum(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    mp[0] = 1;
    int count = 0, sum = 0;
    for (auto &it : nums)
    { //  [1,1,1],              k=2  -> 2
        sum += it;
        count += mp[sum - k];
        mp[sum]++;
    }
    return count;
}

int main()
{
    vector<int> a1 = {1, 1, 1};
    vector<int> a2 = {1, 2, 3};
    vector<int> a3 = {1, -1, 0};
    vector<int> a4 = {3, 4, 7, 2, -3, 1, 4, 2};

    cout << subarraySum(a1, 2) << " (expected 2)\n";
    cout << subarraySum(a2, 3) << " (expected 2)\n";
    cout << subarraySum(a3, 0) << " (expected 3)\n";
    cout << subarraySum(a4, 7) << " (expected 4)\n";
    return 0;
}
