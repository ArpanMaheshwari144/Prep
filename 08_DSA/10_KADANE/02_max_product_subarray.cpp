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
// ---- ARPAN KI APPROACH (22-Jul, derive-first) ----
//  ★ AHA (ye pura khel isi ek baat pe hai): SUM me sirf "sabse bada" yaad rakhte the (Kadane-53).
//     par PRODUCT me neg × neg = BADA POSITIVE. matlab AAJ ka sabse CHHOTA (most-negative) product
//     kal ek aur negative aane pe achanak SABSE BADA ban sakta hai!
//     -> isliye "sabse chhota" ko PHENKNA nahi -> MAX aur MIN, dono ko saath le chalo.
//        (dekh [-2,3,-4]: -4 pe, jo min tha -6, wo -6×-4 = +24 me FLIP ho gaya -> answer.)
//
//  ★ har element pe 3 RAASTE (kaunsa best?):  { num akela (fresh shuru) , num×oldMax , num×oldMin }
//        newMax = in 3 ka max        newMin = in 3 ka min
//     ★★ TRAP -- FREEZE karo: newMin nikalte waqt PURANA max chahiye. agar max pehle badal diya,
//        to naya max, min ke hisaab me ghus jaata -> galat. isliye  temp = max(3);  min = min(3);  max = temp;
//     ★ ans = max(ans, max)  -- ans me PURANA ans bhi rakho (0 ya dip aaye to best bhool na jaaye).
//
//  ★ TWIN: LC-53 = SUM (flip nahi -> 1 value) · LC-152 = PRODUCT (flip -> max+min, ye) · LC-1749 = ABS-SUM (max+min).
//     poora family = 04_max_abs_sum.cpp
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
