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
// ---- ARPAN KI APPROACH (22-Jul, derive-first + struggle-first) ----
//  ★ KADANE family: har position pe "best-ending-HERE" nikalo + global best track. PAR product me
//     NEGATIVE-FLIP hota (num<0 -> min(bahut chhota) × num = BADA positive) -> MAX aur MIN DONO track.
//  ★ har step 3 CANDIDATE: {num (RESTART), num×oldMax, num×oldMin}. newMax=max(3), newMin=min(3).
//     ★★ old value FREEZE (temp): temp=max(3);  min=min(3, PURANE max se);  max=temp.
//        warna naya-max, min ke calc me ghus jaata -> BUG (2 var + 1 temp).
//     ans = max(ans, max).  ★ ans me PURANA ans rakhna (running best) -- warna reset ho ke best bhool jaata.
//  ★ WHY 3-candidate (na single running prod): single wala RESTART nahi kar sakta + MIN-flip use nahi kar sakta.
//  ★ TWIN: LC-53 Max-Subarray = SUM (ek value, no-flip) · LC-1749 Max-Abs-Sum = SUM par max+min dono.
//     (poori family + why + transfer-seekh = 04_max_abs_sum.cpp me)
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
