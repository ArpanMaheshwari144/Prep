// ─────────────────────────────────────────────────────────────
//  SUBARRAY SUMS DIVISIBLE BY K  (LC 974)   — REDO_3
//
//  KYA KARNA: array `nums` + int `k` diya. Aise CONTIGUOUS subarrays
//             GINO jinka sum k se DIVISIBLE ho (sum % k == 0).
//             count return karo.
//
//  INPUT: nums = [4,5,0,-2,-3,1], k = 5   -> answer = 7
//         (7 subarrays hain jinka sum %5 == 0)
//
//  TEST-CASES (input -> expected):
//    [4,5,0,-2,-3,1], k=5   -> 7
//    [5],           k=9     -> 0
//    [5],           k=5     -> 1
//    [0,0,0],       k=1     -> 6   (har subarray, 0%1==0)
//    [-1,2,9],      k=2     -> 2
// ─────────────────────────────────────────────────────────────
#include <bits/stdc++.h>
using namespace std;

int subarraysDivByK(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    int sum = 0, count = 0;
    mp[0] = 1;
    for (auto &it : nums)
    {
        sum += it;
        int rem = ((sum) % k + k) % k;
        count += mp[rem];
        mp[rem]++;
    }
    return count;
}

void check(vector<int> nums, int k, int exp)
{
    int got = subarraysDivByK(nums, k);
    cout << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({4, 5, 0, -2, -3, 1}, 5, 7);
    check({5}, 9, 0);
    check({5}, 5, 1);
    check({0, 0, 0}, 1, 6);
    check({-1, 2, 9}, 2, 2);
    return 0;
}
