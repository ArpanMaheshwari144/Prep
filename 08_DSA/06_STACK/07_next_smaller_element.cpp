// ============================================================
// NEXT SMALLER ELEMENT — Stack   (fresh; Next-Greater ka sibling)
// ============================================================
// ek array nums diya. har element ke liye uske RIGHT me jo PEHLA element
// usse CHHOTA (strictly smaller) ho, wo lauta. koi na mile -> -1.
// (Next Greater Element wahi hai, bas ULTA -- wahan bada dhoondhte the, yahan chhota.)
//
//   [4,5,2,10,8]   -> [2, 2, -1, 8, -1]
//        (4 ke aage pehla chhota = 2 · 5 -> 2 · 2 -> koi chhota nahi = -1 · 10 -> 8 · 8 -> -1)
//   [1,2,3,4]      -> [-1,-1,-1,-1]   (badhta hua -> aage koi chhota nahi)
//   [4,3,2,1]      -> [3, 2, 1, -1]   (ghatta hua -> turant agla chhota)
//   [2,2,2]        -> [-1,-1,-1]      (strictly smaller chahiye -> barabar count nahi)
//   [5]            -> [-1]
//
// ============================================================

// ---- ARPAN KI APPROACH ----
//  ★ NEXT-GREATER ka bilkul same code -- bas while-condition ULTA. baaki (index-stack, pop pe ans set, push) sab wahi.
//  next-greater me: nums[st.top()] < nums[i]  (curr BADA -> top ka answer mila).
//  next-smaller me: nums[st.top()] > nums[i]  (curr CHHOTA -> top ka answer mila) -- yahi ek line badli.
//  monotonic stack (index rakho): while(!empty && nums[st.top()] > nums[i]) -> pop + ans[top]=nums[i]. phir push(i).
//  stack me bache index -> koi chhota nahi mila -> -1 (ans init -1).
//  ★ strictly smaller (> , >= nahi) -> [2,2,2] me barabar count nahi -> saare -1.

#include <bits/stdc++.h>
using namespace std;

vector<int> nextSmaller(vector<int> &nums)
{
    vector<int> ans(nums.size(), -1);
    stack<int> st;

    for (int i = 0; i < nums.size(); i++)
    {
        while (!st.empty() && nums[st.top()] > nums[i])
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
    auto pr = [](vector<int> r)
    { cout << "["; for (int i = 0; i < r.size(); i++) cout << r[i] << (i + 1 < r.size() ? "," : ""); cout << "]\n"; };

    vector<int> a = {4, 5, 2, 10, 8};
    vector<int> b = {1, 2, 3, 4};
    vector<int> c = {4, 3, 2, 1};
    vector<int> d = {2, 2, 2};
    vector<int> e = {5};

    pr(nextSmaller(a)); // [2,2,-1,8,-1]
    pr(nextSmaller(b)); // [-1,-1,-1,-1]
    pr(nextSmaller(c)); // [3,2,1,-1]
    pr(nextSmaller(d)); // [-1,-1,-1]
    pr(nextSmaller(e)); // [-1]
    return 0;
}
