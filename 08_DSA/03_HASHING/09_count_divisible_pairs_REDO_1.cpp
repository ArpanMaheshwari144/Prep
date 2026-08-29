// ============================================================
// COUNT DIVISIBLE PAIRS                         (REDO_1 — blank, spaced)
// ============================================================
// array + k. count karo kitne PAIR (i<j) jinka (nums[i] + nums[j]) % k == 0.
//
//
// ---- TEST CASES (nums, k -> expected) ----
//   [1,2,3,4,5], k=3  -> 4     ( (1,2),(1,5),(2,4),(4,5) )
//   [3,3,3],     k=3  -> 3
//   [1,1,1],     k=2  -> 3
//   [2,4],       k=3  -> 1
//   [1],         k=5  -> 0
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int countDivisiblePairs(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    int sum = 0;
    int count = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        sum = ((nums[i]) % k + k) % k;
        count += mp[(k - sum) % k];
        mp[sum]++;
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
    vector<int> a1 = {1, 2, 3, 4, 5};
    check(countDivisiblePairs(a1, 3), 4, "case1");

    vector<int> a2 = {3, 3, 3};
    check(countDivisiblePairs(a2, 3), 3, "case2");

    vector<int> a3 = {1, 1, 1};
    check(countDivisiblePairs(a3, 2), 3, "case3");

    vector<int> a4 = {2, 4};
    check(countDivisiblePairs(a4, 3), 1, "case4");

    vector<int> a5 = {1};
    check(countDivisiblePairs(a5, 5), 0, "case5");
    return 0;
}
