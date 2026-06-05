// ============================================================
// CONTAINS DUPLICATE                   (Pattern: Hashing - HashSet)
// ============================================================
// Ek array `nums` diya hai. `true` return karo agar koi bhi value
// kam se kam DO baar aati hai; `false` agar SAARE elements distinct hain.
//
// Example:
//   nums = [1, 2, 3, 1]   ->  true    (1 do baar)
//   nums = [1, 2, 3, 4]   ->  false   (sab alag)
//   nums = [1, 1, 1, 3, 3, 4, 3, 2, 4, 2]  ->  true
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Solution
{
public:
    bool containsDuplicate(vector<int> &nums)
    {
        unordered_set<int> st;
        for (int i = 0; i < nums.size(); i++)
        {
            if (st.count(nums[i]))
            {
                return true;
            }
            st.insert(nums[i]);
        }
        return false;
    }
};

int main()
{
    Solution s;
    vector<int> nums = {1, 2, 3, 1};
    cout << boolalpha << s.containsDuplicate(nums) << endl; // expected: true
    return 0;
}
