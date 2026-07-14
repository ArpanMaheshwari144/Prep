// ============================================================
// NEXT GREATER ELEMENT — Stack (pattern #3, MONOTONIC STACK intro)
// ============================================================
// Ek array nums diya. Har element ke liye uske DAAYE (right) me jo PEHLA
// element usse BADA hai wo lauta do. Agar koi bada nahi -> -1.
//   nums = [2,1,2,4,3]  ->  [4,2,4,-1,-1]
//     2 -> daaye pehla bada = 4
//     1 -> 2
//     2 -> 4
//     4 -> koi bada nahi -> -1
//     3 -> -1
//
// ---- ARPAN KI APPROACH ----
//  Monotonic stack (INDEX ka). Left se right chalo. Jab tak stack ke top wala
//  element se current bada hai -> pop karo aur un sab ka answer = current value.
//  Phir current index push. Jo end tak stack me reh gaye unka answer -1 rehta.
//
// Tests (// expected):
//   [2,1,2,4,3]      -> 4 2 4 -1 -1
//   [1,2,3,4]        -> 2 3 4 -1
//   [4,3,2,1]        -> -1 -1 -1 -1
//   [5]              -> -1
//   [2,7,3,5,4,6,8]  -> 7 8 5 6 6 8 -1
// ============================================================

#include <iostream>
#include <vector>
#include <stack>
using namespace std;

vector<int> nextGreater(vector<int> &nums)
{
    vector<int> ans(nums.size(), -1);
    stack<int> st;

    for (int i = 0; i < nums.size(); i++)
    {
        while (!st.empty() && nums[st.top()] < nums[i])
        {
            int topp = st.top();
            st.pop();
            ans[topp] = nums[i];
        }
        st.push(i);
    }
    return ans;
}

int main()
{
    vector<vector<int>> tests = {
        {2, 1, 2, 4, 3},
        {1, 2, 3, 4},
        {4, 3, 2, 1},
        {5},
        {2, 7, 3, 5, 4, 6, 8}};

    auto p = [](vector<int> r)
    { for (int x : r) cout << x << " "; cout << "\n"; };

    for (auto &t : tests)
        p(nextGreater(t));
    // expected:
    // 4 2 4 -1 -1
    // 2 3 4 -1
    // -1 -1 -1 -1
    // -1
    // 7 8 5 6 6 8 -1
    return 0;
}
