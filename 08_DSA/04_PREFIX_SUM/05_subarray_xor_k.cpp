// ============================================================
// COUNT SUBARRAYS WITH XOR = K     (GfG classic, prefix-XOR)
// ============================================================
// nums[] diya + ek number k. Ginno kitne SUBARRAYS (contiguous) hain
// jinke saare elements ka XOR = k ho.
//
//   XOR yaad: a^a = 0 · a^0 = a · order matter nahi.
//   [4,2,2,6,4], k=6  -> 4   ([4,2], [2,2,6], [4,2,2,6,4], [6])
//
// >>> HINT NAHI. ye SUBARRAY-SUM-K (03_HASHING/04) ka XOR-cousin hai:
//     wahan sum tha -> prefix-sum + hashmap ; yahan XOR -> prefix-XOR + hashmap. <<<
//
// ---- TEST CASES (nums, k -> expected count) ----
//   [4,2,2,6,4],   k=6  -> 4
//   [5,6,7,8,9],   k=5  -> 2     ([5], [5,6,7,8,9]? -> XOR check)
//   [1,1,1,1],     k=0  -> 4     (even-length wale)
//   [3,3],         k=0  -> 1     ([3,3])
//   [1,2,3],       k=1  -> 2     ([1] aur [2,3] dono XOR=1)
//   [4],           k=4  -> 1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int subarrayXorK(vector<int> &nums, int k)
{
    unordered_map<int, int> mp;
    int xorr = 0;
    int count = 0;
    mp[0] = 1;
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
    return 0;
}
