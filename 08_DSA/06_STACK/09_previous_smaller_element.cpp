// ============================================================
// PREVIOUS SMALLER ELEMENT — Stack   (fresh; Next-Smaller ka MIRROR)
// ============================================================
// ek array nums diya. har element ke liye uske LEFT me jo PEHLA element
// usse CHHOTA (strictly smaller) ho, wo lauta. koi na mile -> -1.
// (Next-Smaller wahi tha bas RIGHT taraf; ye ULTA -- LEFT taraf pehla chhota.)
//
//   [4,5,2,10,8]   -> [-1, 4, -1, 2, 2]
//        (4 -> left me kuch nahi = -1 · 5 -> 4 · 2 -> left me 4,5 dono bade = -1 · 10 -> 2 · 8 -> left me pehla chhota 2)
//   [1,2,3,4]      -> [-1, 1, 2, 3]     (har ek ke left ka turant chhota)
//   [4,3,2,1]      -> [-1,-1,-1,-1]     (ghatta -> left me koi chhota nahi)
//   [2,2,2]        -> [-1,-1,-1]        (strictly smaller -> barabar count nahi)
//   [5]            -> [-1]
//
// ============================================================

// ---- ARPAN KI APPROACH ----
//  ★ NEXT-SMALLER ka SAME-TO-SAME code -- bas ek cheez badli: for-loop ULTA (right se left, i = n-1 -> 0).
//     (samajh: array ko PEECHE se dekhoge to "right ka next-smaller" ban jaata "left ka previous-smaller".)
//  monotonic stack (index): loop i = n-1 down to 0:
//     while(!empty && nums[st.top()] > nums[i]) -> pop + ans[top] = nums[i]  (curr i us popped ke LEFT me hai -> uska prev-smaller).
//     phir push(i).
//  stack me bache index -> left me koi chhota nahi -> -1 (ans init -1).
//  ★ example {4,5,2,10,8}: peeche se -> 8,10 stack me; 2 aaya (chhota) -> 10 aur 8 dono ka prev-smaller = 2; ... -> [-1,4,-1,2,2].
//  ★ strictly smaller (>) -> [2,2,2] -> saare -1.

#include <bits/stdc++.h>
using namespace std;

vector<int> prevSmaller(vector<int> &nums)
{
    vector<int> ans(nums.size(), -1);
    stack<int> st;

    for (int i = nums.size() - 1; i >= 0; i--)
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

    pr(prevSmaller(a)); // [-1,4,-1,2,2]
    pr(prevSmaller(b)); // [-1,1,2,3]
    pr(prevSmaller(c)); // [-1,-1,-1,-1]
    pr(prevSmaller(d)); // [-1,-1,-1]
    pr(prevSmaller(e)); // [-1]
    return 0;
}
