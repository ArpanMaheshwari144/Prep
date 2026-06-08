// ============================================================
// FIND MAX  —  Arrays basics  (PRODUCTION: memory se likh, dikhaya code band karke)
// ============================================================
// Array `nums` mein sabse BADA element return karo.
//
// Example:
//   {3, 7, 2, 9, 4, 1}  ->  9
//
// SKELETON (yaad): best = pehla → loop(1 se): current>best to best=current → return best
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
