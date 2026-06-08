// ============================================================
// LINEAR SEARCH  —  Arrays basics
// ============================================================
// Array `nums` mein `target` ka INDEX return karo; na mile to -1.
//
// Example:
//   {3, 7, 2, 9, 4, 1}, target = 9  ->  3
//   {3, 7, 2, 9, 4, 1}, target = 5  -> -1
//
// INTUITION (soch — code TU likhega):
//   Ek-ek karke har element dekho: "kya yeh target hai?"
//     mil gaya  -> uski JAGAH (index) bata do
//   Poora ghoom liya, nahi mila -> -1 (array mein hai hi nahi).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int search(vector<int> &nums, int target)
{
    for (int i = 0; i < nums.size(); i++)
    {
        if (nums[i] == target)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    vector<int> nums = {3, 7, 2, 9, 4, 1};
    cout << search(nums, 9) << endl; // expected: 3
    cout << search(nums, 5) << endl; // expected: -1
    return 0;
}
