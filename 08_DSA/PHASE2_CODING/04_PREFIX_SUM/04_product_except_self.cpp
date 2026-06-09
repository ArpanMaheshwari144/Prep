// ============================================================
// PRODUCT OF ARRAY EXCEPT SELF   —   Prefix Sum (prefix x suffix)   [LeetCode 238]
// ============================================================
// Har position pe answer = baaki SAARE elements ka guna (apne aap ko chhod ke).
// Division use NAHI karna.
//
// Example:
//   nums = {1, 2, 3, 4}  ->  {24, 12, 8, 6}
//     answer[0] = 2*3*4 = 24
//     answer[1] = 1*3*4 = 12
//     answer[2] = 1*2*4 = 8
//     answer[3] = 1*2*3 = 6
//
// INTUITION (soch — code TU likhega):
//   Har point = (LEFT waalon ka guna) x (RIGHT waalon ka guna), khud ko skip.
//   Pass 1 (left se): answer mein "apne se pehle waalon ka guna" bhar do.
//   Pass 2 (right se): ek running-product variable rakho, usi answer ko
//                      "apne se baad waalon ka guna" se multiply karte jao.
//   = do pass, no division.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> productExceptSelf(vector<int> &nums)
{
    int n = nums.size();
    vector<int> left(n, 1);
    int prod = 1;
    for (int i = 0; i < n; i++)
    {
        left[i] = prod;
        prod *= nums[i];
    }

    vector<int> right(n, 1);
    prod = 1;
    for (int i = n - 1; i >= 0; i--)
    {
        right[i] = prod;
        prod *= nums[i];
    }

    vector<int> ans(n, 1);
    for (int i = 0; i < n; i++)
    {
        ans[i] = left[i] * right[i];
    }
    return ans;
}

int main()
{
    vector<int> nums = {1, 2, 3, 4};
    vector<int> ans = productExceptSelf(nums);

    for (int x : ans)
        cout << x << " "; // expected: 24 12 8 6
    cout << endl;
    return 0;
}
