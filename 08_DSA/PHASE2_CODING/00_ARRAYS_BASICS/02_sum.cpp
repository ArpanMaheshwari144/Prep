// ============================================================
// SUM  —  Arrays basics  (PRODUCTION: memory se likh, dikhaya code band karke)
// ============================================================
// Array `nums` ke saare elements ka SUM return karo.
//
// Example:
//   {3, 7, 2, 9, 4, 1}  ->  26
//
// SKELETON (yaad): sum = 0 → loop: sum += current → return sum   (bucket pattern)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int findSum(vector<int> &nums)
{
    int sum = 0;
    for (auto &it : nums)
    {
        sum += it;
    }
    return sum;
}

int main()
{
    vector<int> nums = {3, 7, 2, 9, 4, 1};
    cout << findSum(nums) << endl; // expected: 26
    return 0;
}
