// ============================================================
// TWO SUM (unsorted)                   (Pattern: Hashing - HashMap)
// ============================================================
// Ek UNSORTED array `nums` aur ek `target` diya hai. Do indices
// return karo (0-based) jinke numbers ka sum = target.
// Exactly ek solution; same element 2 baar use nahi.
//
// (Note: array SORTED nahi → Two Pointer seedha nahi. HashMap use.)
//
// Example:
//   nums = [2, 7, 11, 15], target = 9   ->  [0, 1]   (2 + 7 = 9)
//   nums = [3, 2, 4], target = 6        ->  [1, 2]   (2 + 4 = 6)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    vector<int> twoSum(vector<int> &nums, int target)
    {
        unordered_map<int, int> mp; // nums = [2, 7, 11, 15], target = 9   ->  [0, 1]   (2 + 7 = 9)
        for (int i = 0; i < nums.size(); i++)
        {
            if (mp.find(target - nums[i]) != mp.end())
            {
                return vector<int>{mp[target - nums[i]], i};
            }
            mp[nums[i]] = i;
        }
        return vector<int>{};
    }
};

int main()
{
    Solution s;
    vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    vector<int> ans = s.twoSum(nums, target);
    cout << "[";
    for (int i = 0; i < (int)ans.size(); i++)
        cout << ans[i] << (i + 1 < (int)ans.size() ? ", " : "");
    cout << "]" << endl; // expected: [0, 1]
    return 0;
}
