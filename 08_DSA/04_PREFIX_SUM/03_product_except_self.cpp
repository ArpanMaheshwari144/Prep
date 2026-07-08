// ============================================================
// PRODUCT OF ARRAY EXCEPT SELF — Prefix Sum (prefix-product) block #3
// ============================================================
// nums[] diya. output[] return karo jahan output[i] = nums ke SAARE elements ka
// product SIWAAY nums[i] ke.
// ★ DIVISION use nahi karna. O(n) chahiye.
//
// (hint: har index ke LEFT ka product + RIGHT ka product.
//  output[i] = (i se pehle sabka product) * (i ke baad sabka product). khud socho.)
//
// Tests (// expected):
//   [1,2,3,4]        -> 24 12 8 6
//   [-1,1,0,-3,3]    -> 0 0 9 0 0
//   [2,3]            -> 3 2
//   [5]              -> 1
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> productExceptSelf(vector<int> &nums)
{
    vector<int> left(nums.size(), 1);
    vector<int> right(nums.size(), 1);
    int prod = 1;
    vector<int> ans(nums.size(), 1);

    for (int i = 0; i < nums.size(); i++)
    {
        left[i] = prod;
        prod *= nums[i];
    }

    prod = 1;
    for (int i = nums.size() - 1; i >= 0; i--)
    {
        right[i] = prod;
        prod *= nums[i];
    }

    for (int i = 0; i < nums.size(); i++)
    {
        ans[i] = left[i] * right[i];
    }
    return ans;
}

int main()
{
    vector<int> a1 = {1, 2, 3, 4};
    vector<int> a2 = {-1, 1, 0, -3, 3};
    vector<int> a3 = {2, 3};
    vector<int> a4 = {5};

    auto p = [](vector<int> r)
    { for (int x : r) cout << x << " "; cout << "\n"; };
    p(productExceptSelf(a1)); // 24 12 8 6
    p(productExceptSelf(a2)); // 0 0 9 0 0
    p(productExceptSelf(a3)); // 3 2
    p(productExceptSelf(a4)); // 1
    return 0;
}
