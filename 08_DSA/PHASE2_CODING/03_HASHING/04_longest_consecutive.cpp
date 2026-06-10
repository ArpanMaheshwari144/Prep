// ============================================================
// LONGEST CONSECUTIVE SEQUENCE   —   Hashing (HashSet)
// ============================================================
// UNSORTED array. Sabse lambi CONSECUTIVE (lagataar: 1,2,3,4..) sequence ki LENGTH.
//
// Example:
//   {100, 4, 200, 1, 3, 2}  ->  4   (1,2,3,4)
//
// INTUITION (TU ne derive kiya — code TU likhega):
//   Saare numbers ek SET mein daalo (O(1) lookup, bina sort).
//   Har number pe: agar (num - 1) set mein NAHI hai -> yeh sequence ka START hai.
//     start se aage gino: jab tak (start + 1) set mein hai -> count++, start++.
//   Sirf STARTS se ginne se har number max 2 baar dekha jaata -> O(n).
//   Sabse lambi count yaad rakho.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int longestConsecutive(vector<int> &nums)
{
    unordered_set<int> st;
    for (auto &it : nums)
    {
        st.insert(it);
    }

    int count = 0;
    int ans = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        int num = nums[i];
        if (!st.count(num - 1))
        {
            int start = nums[i];
            while (st.count(start))
            {
                count++;
                ans = max(ans, count);
                start++;
            }
            count = 0;
            
        }
    }
    return ans;
}

int main()
{
    vector<int> a = {100, 4, 200, 1, 3, 2};
    cout << longestConsecutive(a) << endl; // expected: 4
    return 0;
}
