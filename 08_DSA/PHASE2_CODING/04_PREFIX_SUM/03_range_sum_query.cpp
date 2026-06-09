// ============================================================
// RANGE SUM QUERY (immutable)   —   Prefix Sum (core)   [LeetCode 303]
// ============================================================
// Array `nums` fixed. Query baar-baar: "index left se right tak ka sum?"
// Har query ka jawaab TURANT (O(1)) chahiye — har baar left..right ginna nahi.
//
// Example:
//   nums = {1, 2, 3, 4, 5}
//   sumRange(1, 3) -> 9    (2+3+4)
//   sumRange(0, 4) -> 15   (poora)
//   sumRange(2, 2) -> 3    (sirf ek element)
//
// INTUITION (soch — code TU likhega):
//   Constructor mein EK baar "shuru se har point tak ka total" (cumulative) bana ke rakho.
//   sumRange: range [left..right] = (right tak ka total) - (left se theek PEHLE tak ka total).
//   = do cumulative readings ka ANTAR (wahi gullak wala difference).
//   left == 0 ho to "left se pehle ka total" = 0.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class NumArray
{
public:
    vector<int> prefix;
    NumArray(vector<int> &nums)
    {
        int n = nums.size();
        prefix.resize(n, 0);
        prefix[0] = nums[0];
        for (int i = 1; i < n; i++)
        {
            prefix[i] = prefix[i - 1] + nums[i];
        }
    }

    int sumRange(int left, int right)
    {
        if (left == 0)
            return prefix[right];
        return prefix[right] - prefix[left - 1];
    }
};

int main()
{
    vector<int> nums = {1, 2, 3, 4, 5};
    NumArray na(nums);

    cout << na.sumRange(1, 3) << endl; // expected: 9
    cout << na.sumRange(0, 4) << endl; // expected: 15
    cout << na.sumRange(2, 2) << endl; // expected: 3
    return 0;
}
