// ============================================================
// COUNT SUBARRAYS WITH XOR = K   (GfG, prefix-XOR)  -- REDO_2 (blank)
// ============================================================
// nums[] + k. Ginno kitne SUBARRAYS ka XOR = k ho.
//   [4,2,2,6,4], k=6  -> 4
//
// ---- TEST (nums, k -> expected) ----
//   [4,2,2,6,4],      k=6   -> 4
//   [5,6,7,8,9],      k=5   -> 2
//   [1,1,1,1],        k=0   -> 4
//   [3,3],            k=0   -> 1
//   [1,2,3],          k=1   -> 2
//   [4],              k=4   -> 1
//   [4],              k=5   -> 0
//   [0,0,0],          k=0   -> 6
//   [1,2,3,4],        k=4   -> 2
//   [7,7,7,7,7],      k=7   -> 9
//   [2,4,6,8,10,12],  k=14  -> 3
//   [],               k=3   -> 0
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int subarrayXorK(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    mp[0] = 1;
    int xorr = 0, count = 0;
    for (auto &it : nums)
    {
        xorr ^= it;
        count += mp[xorr ^ k];
        mp[xorr]++;
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
    vector<int> a1 = {4, 2, 2, 6, 4};
    check(subarrayXorK(a1, 6), 4, "case1");
    vector<int> a2 = {5, 6, 7, 8, 9};
    check(subarrayXorK(a2, 5), 2, "case2");
    vector<int> a3 = {1, 1, 1, 1};
    check(subarrayXorK(a3, 0), 4, "case3");
    vector<int> a4 = {3, 3};
    check(subarrayXorK(a4, 0), 1, "case4");
    vector<int> a5 = {1, 2, 3};
    check(subarrayXorK(a5, 1), 2, "case5");
    vector<int> a6 = {4};
    check(subarrayXorK(a6, 4), 1, "case6");
    vector<int> a7 = {4};
    check(subarrayXorK(a7, 5), 0, "case7");
    vector<int> a8 = {0, 0, 0};
    check(subarrayXorK(a8, 0), 6, "case8");
    vector<int> a9 = {1, 2, 3, 4};
    check(subarrayXorK(a9, 4), 2, "case9");
    vector<int> a10 = {7, 7, 7, 7, 7};
    check(subarrayXorK(a10, 7), 9, "case10");
    vector<int> a11 = {2, 4, 6, 8, 10, 12};
    check(subarrayXorK(a11, 14), 3, "case11");
    vector<int> a12 = {};
    check(subarrayXorK(a12, 3), 0, "case12");
    return 0;
}
