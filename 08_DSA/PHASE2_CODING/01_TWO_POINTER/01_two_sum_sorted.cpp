// ============================================================
// TWO SUM II (sorted)   —   Two Pointer (opposite ends)
// ============================================================
// SORTED array `nums`. Do numbers jinka sum = target — unke INDEX return karo {i, j}.
//
// Example:
//   {2, 7, 11, 15}, target = 9   ->  {0, 1}   (2 + 7)
//   {1, 3, 4, 5, 7}, target = 9  ->  {2, 4}   (4 + 5)
//
// INTUITION (plain words — isse CODE TU likh):
//   - left = 0, right = last
//   - sum = nums[left] + nums[right]
//        sum == target  -> return {left, right}
//        sum < target   -> left++   (bada chahiye)
//        sum > target   -> right--  (chhota chahiye)
//   - loop jab tak left < right
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> twoSum(vector<int> &nums, int target)
{
    int left = 0;
    int right = nums.size() - 1;
    while (left < right)
    {
        if (nums[left] + nums[right] == target)
        {
            return {left, right};
        }
        else if (nums[left] + nums[right] < target)
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    return {-1, -1};
}

int main()
{
    vector<int> nums = {2, 7, 11, 15};
    vector<int> ans = twoSum(nums, 9);
    cout << "{" << ans[0] << ", " << ans[1] << "}" << endl; // expected: {0, 1}
    return 0;
}
