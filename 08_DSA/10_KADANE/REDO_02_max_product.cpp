// ============================================================
// [COLD REDO] MAXIMUM PRODUCT SUBARRAY — (LeetCode 152, Medium)
// ============================================================
// sheet/original file MAT dekh. fresh dimaag se, khud se solve kar.
// int array diya. aisा CONTIGUOUS subarray dhoondho jiska elements ka
// PRODUCT sabse BADA ho. wo max product lautao.
//
//   [2,3,-2,4]        -> 6
//   [-2,0,-1]         -> 0
//   [-2,3,-4]         -> 24
//   [2,-5,-2,-4,3]    -> 24
//   [-3,-1,-1]        -> 3
//   [5]               -> 5
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxProduct(vector<int> &nums)
{
    if (nums.size() == 1)
    {
        return nums[0];
    }

    int maxProd = nums[0];
    int minProd = nums[0];
    int ans = 0; //  [-2,3,-4]         -> 24
    for (int i = 1; i < nums.size(); i++)
    {
        // TRICK: har step 3 candidate -> { nums[i], nums[i]*maxProd, nums[i]*minProd }.
        // inka max = naya maxProd, min = naya minProd. (neg*neg flip -> min bhi carry, freeze temp se)
        int temp = max({nums[i], maxProd * nums[i], minProd * nums[i]});
        minProd = min({nums[i], maxProd * nums[i], minProd * nums[i]});
        maxProd = temp;
        ans = max(ans, maxProd);
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
