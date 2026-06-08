// ============================================================
// COUNT OCCURRENCES  —  Arrays basics  (PRODUCTION: memory se likh)
// ============================================================
// Array `nums` mein `target` KITNI baar aata hai — woh count return karo.
//
// Example:
//   {1, 2, 2, 3, 2, 4}, target = 2  ->  3
//   {1, 2, 2, 3, 2, 4}, target = 5  ->  0
//
// SKELETON (yaad): cnt = 0 → loop: x == target? cnt++ → return cnt   (counter pattern)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int count(vector<int> &nums, int target)
{
    int count = 0;
    for (auto &it : nums)
    {
        if (it == target)
        {
            count++;
        }
    }
    return count;
}

int main()
{
    vector<int> nums = {1, 2, 2, 3, 2, 4};
    cout << count(nums, 2) << endl; // expected: 3
    cout << count(nums, 5) << endl; // expected: 0
    return 0;
}
