// ═══════════════════════════════════════════════════════════════════════
//  COUNT DIVISIBLE PAIRS  — REDO_3                              (HASHING)
//  nums[] aur k. kitne PAIR (i < j) jinka (nums[i] + nums[j]) % k == 0 ? -> COUNT.
//
//  INPUT-FORM: 1 <= nums.size(), k >= 1, nums[i] NEGATIVE bhi ho sakta.
//  TEST-CASES (nums, k -> expected):
//   1) [1,2,3,4,5],      k=3  -> 4
//   2) [3,3,3],          k=3  -> 3
//   3) [1,1,1],          k=2  -> 3
//   4) [2,4],            k=3  -> 1
//   5) [1],              k=5  -> 0
//   6) [1,2,3,4],        k=1  -> 6
//   7) [5,10,15,20],     k=5  -> 6
//   8) [1,3,5,7],        k=4  -> 4
//   9) [2,2,2,2],        k=4  -> 6
//  10) [1,2],            k=4  -> 0
//  11) [-1,1,4,-4],      k=5  -> 4
//  12) [6,7,8,9,10,11],  k=6  -> 2
//  13) [2,4,6,8,10],     k=2  -> 10
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int countDivisiblePairs(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    int count = 0;
    for (auto &it : nums)
    {
        int rem = (((it % k) + k) % k);
        count += mp[(k - rem) % k];
        mp[rem]++;
    }
    return count;
}

// ---- test helper ----
void check(vector<int> nums, int k, int exp, string name)
{
    int got = countDivisiblePairs(nums, k);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, 2, 3, 4, 5}, 3, 4, "t1");
    check({3, 3, 3}, 3, 3, "t2");
    check({1, 1, 1}, 2, 3, "t3");
    check({2, 4}, 3, 1, "t4");
    check({1}, 5, 0, "t5 single");
    check({1, 2, 3, 4}, 1, 6, "t6 k=1");
    check({5, 10, 15, 20}, 5, 6, "t7 sab divisible");
    check({1, 3, 5, 7}, 4, 4, "t8");
    check({2, 2, 2, 2}, 4, 6, "t9 sab same");
    check({1, 2}, 4, 0, "t10 koi nahi");
    check({-1, 1, 4, -4}, 5, 4, "t11 negative");
    check({6, 7, 8, 9, 10, 11}, 6, 2, "t12");
    check({2, 4, 6, 8, 10}, 2, 10, "t13");
    return 0;
}
