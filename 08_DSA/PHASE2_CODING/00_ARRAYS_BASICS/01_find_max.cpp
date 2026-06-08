// ============================================================
// FIND MAX  —  Arrays basics
// ============================================================
// Array `nums` mein sabse BADA element return karo.
//
// Example:
//   {3, 7, 2, 9, 4, 1}  ->  9
//
// INTUITION (soch — code TU likhega):
//   Champion ki tarah soch — shuru mein pehla element champion.
//   Har agle element se poochho "champion se bada?" — haan to woh naya champion.
//   Poora array ghoom ke jo champion bacha, wahi sabse bada.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int findMax(vector<int> &nums)
{
    int maxi = INT_MIN;
    for (int i = 0; i < nums.size(); i++)
    {
        maxi = max(maxi, nums[i]);
    }
    return maxi;
}

int main()
{
    vector<int> nums = {3, 7, 2, 9, 4, 1};
    cout << findMax(nums) << endl; // expected: 9
    return 0;
}
