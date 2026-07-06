// ============================================================
// SUBARRAY SUM EQUALS K   — BLANK REDO (6-Jul, +1 din retrieval)
// ============================================================
// Array nums + ek number k diya. KITNE contiguous subarrays hain
// jinka sum EXACTLY k hai -> woh COUNT return karo.
//
// (approach yahan LIKHA NAHI — tujhe KHUD dimaag se nikaalna hai.
//  purani solution-file mat kholna. atko -> mujhe bolo, direction dunga.)
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
    // [1,1,1],              k=2  -> 2
    unordered_map<int, int> mp;
    mp[0] = 1;
    int sum = 0, count = 0;
    for (auto &it : nums)
    {
        sum += it;
        count += mp[sum - k]; // this is nothing but prefix[j] - prefix[i-1] = k
        mp[sum]++;
    }

    // for (auto &it : mp)
    // {
    //     cout << it.first << " " << it.second << endl;
    // }

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
