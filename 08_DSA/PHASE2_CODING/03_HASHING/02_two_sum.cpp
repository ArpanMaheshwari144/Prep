// ============================================================
// TWO SUM (unsorted)   —   Hashing (HashMap complement)
// ============================================================
// UNSORTED array `nums` + target. Do numbers jinka jod = target — unke INDEX return karo.
//
// Example:
//   {2, 7, 11, 15}, target = 9  ->  {0, 1}   (2 + 7)
//   {3, 2, 4},      target = 6  ->  {1, 2}   (2 + 4)
//
// INTUITION (soch — code TU likhega):
//   Har number pe "JODIDAR" chahiye = (target - current).
//   - "note" (map): number -> uska index.
//   - Har number pe: "kya JODIDAR (target - current) pehle dekha hai?" (map mein?)
//        haan -> dono ke index return
//        nahi -> current ko map mein daal do (apne index ke saath), aage badho.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> twoSum(vector<int> &nums, int target)
{
    unordered_map<int, int> mp;
    for (int i = 0; i < nums.size(); i++)
    {
        if (mp.count(target - nums[i]))
        {
            return {mp[target - nums[i]], i};
        }
        mp[nums[i]] = i;
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
