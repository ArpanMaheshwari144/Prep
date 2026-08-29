// ============================================================
// SUBARRAY SUMS DIVISIBLE BY K  (LC-974)      (REDO_2 — blank, spaced)
// ============================================================
// integer array + k. count karo kitne CONTIGUOUS subarray jinka sum % k == 0.
// (negatives bhi ho sakte -> remainder ka dhyan.)
//
// ---- TEST CASES (nums, k -> expected) ----
//   [4,5,0,-2,-3,1], k=5  -> 7
//   [5],             k=9  -> 0
//   [1,2,3],         k=3  -> 3
//   [-1,2,9],        k=2  -> 2
//   [0],             k=1  -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int subarraysDivByK(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    int sum = 0;
    int count = 0;
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

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<int> a1 = {4, 5, 0, -2, -3, 1};
    check(subarraysDivByK(a1, 5), 7, "case1");

    vector<int> a2 = {5};
    check(subarraysDivByK(a2, 9), 0, "case2");

    vector<int> a3 = {1, 2, 3};
    check(subarraysDivByK(a3, 3), 3, "case3");

    vector<int> a4 = {-1, 2, 9};
    check(subarraysDivByK(a4, 2), 2, "case4");

    vector<int> a5 = {0};
    check(subarraysDivByK(a5, 1), 1, "case5");
    return 0;
}
