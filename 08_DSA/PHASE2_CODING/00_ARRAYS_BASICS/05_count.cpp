// ============================================================
// COUNT OCCURRENCES  —  Arrays basics
// ============================================================
// Array `nums` mein `target` KITNI baar aata hai — woh count return karo.
//
// Example:
//   {1, 2, 2, 3, 2, 4}, target = 2  ->  3
//   {1, 2, 2, 3, 2, 4}, target = 5  ->  0
//
// INTUITION (soch — code TU likhega):
//   Soch ek COUNTER (0 se shuru).
//   Har element dekho: agar target hai -> counter ek badhao.
//   Poora ghoom ke counter mein jo number bana, utni baar target aaya.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int countOccur(vector<int> &nums, int target)
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
    cout << countOccur(nums, 2) << endl; // expected: 3
    cout << countOccur(nums, 5) << endl; // expected: 0
    return 0;
}
