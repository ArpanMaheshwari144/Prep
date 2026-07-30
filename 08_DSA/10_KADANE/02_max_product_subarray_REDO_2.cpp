// ============================================================
// MAXIMUM PRODUCT SUBARRAY — REDO (fresh, memory se)   [LeetCode 152, Medium]
// ============================================================
// int array. aisa CONTIGUOUS subarray jiska elements ka PRODUCT sabse BADA. wo lautao.
//
//   [2,3,-2,4]        -> 6
//   [-2,0,-1]         -> 0
//   [-2,3,-4]         -> 24
//   [2,-5,-2,-4,3]    -> 24
//   [-3,-1,-1]        -> 3
//   [5]               -> 5
//
//   ★ trick (halka yaad?): neg×neg = BADA -> MIN bhi track (sirf max se nahi banega).
//     max_prod + min_prod dono chalao; freeze temp se (min purane max/min se nikaalo).
//     atke to peek -- koi guilt nahi.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ─── YAHAN REDO KAR ─────────────────────────────────────────
int maxProduct(vector<int> &nums)
{
    int minProd = 1;
    int maxProd = 1;
    int ans = INT_MIN;
    for (int i = 0; i < nums.size(); i++)
    {
        int temp = max({nums[i], nums[i] * maxProd, nums[i] * minProd});
        minProd = min({nums[i], nums[i] * maxProd, nums[i] * minProd});
        maxProd = temp;
        ans = max(ans, maxProd);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int main()
{
    vector<int> a = {2, 3, -2, 4};
    cout << maxProduct(a) << " (expected 6)\n";
    vector<int> b = {-2, 0, -1};
    cout << maxProduct(b) << " (expected 0)\n";
    vector<int> c = {-2, 3, -4};
    cout << maxProduct(c) << " (expected 24)\n";
    vector<int> d = {2, -5, -2, -4, 3};
    cout << maxProduct(d) << " (expected 24)\n";
    vector<int> e = {-3, -1, -1};
    cout << maxProduct(e) << " (expected 3)\n";
    vector<int> f = {5};
    cout << maxProduct(f) << " (expected 5)\n";
    return 0;
}
