// ============================================================
// RANGE SUM QUERY - IMMUTABLE             (Pattern: Prefix Sum)
// ============================================================
// Ek array `nums` diya hai. Bahut saari queries aayengi: sumRange(i, j)
// = nums[i] + nums[i+1] + ... + nums[j] (dono inclusive).
// Har query O(1) mein answer karni hai (precompute karke).
//
// Example:
//   nums = [-2, 0, 3, -5, 2, -1]
//   sumRange(0, 2)  ->  1    (-2 + 0 + 3)
//   sumRange(2, 5)  -> -1    (3 + -5 + 2 + -1)
//   sumRange(0, 5)  -> -3
//
// Idea: constructor mein prefix precompute karo (ek baar), phir har
//       sumRange O(1): sum(i..j) = prefix[j] - prefix[i-1]  (i=0 edge dhyaan).
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
        prefix = vector<int>(n, 0);
        prefix[0] = nums[0];

        for (int i = 1; i < n; i++)
        {
            prefix[i] = prefix[i - 1] + nums[i];
        }
    }

    int sumRange(int left, int right)
    {
        if (left == 0)
        {
            return prefix[right];
        }
        return prefix[right] - prefix[left - 1];
    }
};

int main()
{
    vector<int> nums = {-2, 0, 3, -5, 2, -1};
    NumArray obj(nums);
    cout << obj.sumRange(0, 2) << endl; // expected: 1
    cout << obj.sumRange(2, 5) << endl; // expected: -1
    cout << obj.sumRange(0, 5) << endl; // expected: -3
    return 0;
}
