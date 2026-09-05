// ============================================================
// NEXT GREATER ELEMENT II — Stack (circular)  (fresh)
// ============================================================
// int array nums (CIRCULAR). har element ke liye uska NEXT GREATER number lauta
// (array ke end ke baad phir shuru se dekh sakte -- circular). na mile -> -1.
//   [1,2,1]      -> [2,-1,2]     (aakhri 1 ka next greater circular me 2)
//   [5,4,3,2,1]  -> [-1,5,5,5,5]
//   [1,2,3,4,3]  -> [2,3,4,-1,4]


//
// Tests (nums -> expected):
//   [1,2,1]        -> 2 -1 2
//   [5,4,3,2,1]    -> -1 5 5 5 5
//   [1,2,3,4,3]    -> 2 3 4 -1 4
//   [3,3]          -> -1 -1
//   [1]            -> -1
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> nextGreaterElements(vector<int> &nums)
{
    vector<int> ans(nums.size(), -1);
    stack<int> st;
    int mod = nums.size();

    for (int i = 0; i < nums.size() * 2; i++) //[1,2,1]  -> [2,-1,2]
    {
        // cout << "-->" << (i % mod) << endl;
        while (!st.empty() && nums[st.top()] < nums[(i % mod)])
        {
            int topp = st.top();
            st.pop();
            ans[topp] = nums[(i % mod)];
        }
        st.push(i % mod);
    }
    return ans;
}

// ---------- printer (boilerplate, chhoo mat) ----------
void check(vector<int> nums)
{
    vector<int> r = nextGreaterElements(nums);
    for (int x : r)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    check({1, 2, 1}); // 2 -1 2
    check({5, 4, 3, 2, 1});  // -1 5 5 5 5
    check({1, 2, 3, 4, 3});  // 2 3 4 -1 4
    check({3, 3});           // -1 -1
    check({1});              // -1
    return 0;
}
