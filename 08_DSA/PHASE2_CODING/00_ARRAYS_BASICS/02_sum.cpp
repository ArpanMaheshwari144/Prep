// ============================================================
// SUM  —  Arrays basics
// ============================================================
// Array `nums` ke saare elements ka SUM return karo.
//
// Example:
//   {3, 7, 2, 9, 4, 1}  ->  26
//
// INTUITION (soch — code TU likhega):
//   Soch ek BUCKET — khaali bucket se shuru.
//   Har element ko bucket mein daalte jao (jodte jao).
//   Poora array ghoom ke bucket mein jo total bana, wahi sum.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int findSum(vector<int> &nums)
{
    int sum = 0;
    for (int i = 0; i < nums.size(); i++)
    {
        sum += nums[i];
    }
    return sum;
}

int main()
{
    vector<int> nums = {3, 7, 2, 9, 4, 1};
    cout << findSum(nums) << endl; // expected: 26
    return 0;
}
