// ============================================================
// MAXIMUM PRODUCT SUBARRAY — (LeetCode 152, Medium)   [derive-first, pen-paper]
// ============================================================
// ek int array diya. aisा CONTIGUOUS subarray dhoondho jiska elements ka
// PRODUCT (guna) sabse BADA ho. wo max product lautao.
//
//   [2,3,-2,4]        -> 6    (2*3)
//   [-2,0,-1]         -> 0
//   [-2,3,-4]         -> 24   (-2*3*-4)
//   [2,-5,-2,-4,3]    -> 24   (-2*-4*3)
//   [-3,-1,-1]        -> 3    (-3*-1)
//   [5]               -> 5
//
// (PEHLE haath se trace kar -- chhota example, jaise [2,3,-2,4] ya [-2,3,-4].
//  dekh negative aane pe kya hota. behavior samajh, phir code. koi hint nahi.)
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxProduct(vector<int> &nums)
{
    if (nums.size() == 1)
    {
        return nums[0];
    }
    int max_prod = nums[0];
    int min_prod = nums[0];
    int ans = 0;
    for (int i = 1; i < nums.size(); i++)
    {
        int temp = max({nums[i], nums[i] * max_prod, nums[i] * min_prod});
        min_prod = min({nums[i], nums[i] * max_prod, nums[i] * min_prod});
        max_prod = temp;
        ans = max(ans, max_prod);
    }
    return ans;
}

int main()
{
    vector<int> a = {2, 3, -2, 4};
    vector<int> b = {-2, 0, -1};
    vector<int> c = {-2, 3, -4};
    vector<int> d = {2, -5, -2, -4, 3};
    vector<int> e = {-3, -1, -1};
    vector<int> f = {5};

    cout << maxProduct(a) << " (expected 6)\n";
    cout << maxProduct(b) << " (expected 0)\n";
    cout << maxProduct(c) << " (expected 24)\n";
    cout << maxProduct(d) << " (expected 24)\n";
    cout << maxProduct(e) << " (expected 3)\n";
    cout << maxProduct(f) << " (expected 5)\n";
    return 0;
}
