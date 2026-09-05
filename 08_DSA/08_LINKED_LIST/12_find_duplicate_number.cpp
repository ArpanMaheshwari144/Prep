// ============================================================
// FIND THE DUPLICATE NUMBER (LC-287) — Linked List family (Floyd's cycle, ARRAY-application)
// ============================================================
// nums[] me n+1 integers hain, har value 1..n range me. Exactly EK number
// repeat hota hai (ek ya kai baar). Us DUPLICATE number ko return karo.
//   CONSTRAINT: O(1) extra space + array MODIFY nahi (read-only).
//
// INPUT format: vector<int> nums (size n+1, values 1..n, exactly 1 duplicate).
//
// TEST CASES (input -> expected):
//   [1,3,4,2,2]                  -> 2
//   [3,1,3,4,2]                  -> 3
//   [1,1]                        -> 1
//   [2,2,2,2,2]                  -> 2
//   [2,5,9,6,9,3,8,9,7,1]        -> 9
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int findDuplicate(vector<int> &nums)
{
    int slow = 0;
    int fast = 0;

    slow = nums[slow];
    fast = nums[nums[fast]];
    while (slow != fast)
    {
        slow = nums[slow];
        fast = nums[nums[fast]];
    }

    slow = 0;
    while (slow != fast)
    {
        slow = nums[slow];
        fast = nums[fast];
    }
    return slow;
}

// ---- test harness ----
void check(vector<int> nums, int expected)
{
    int got = findDuplicate(nums);
    cout << (got == expected ? "PASS" : "FAIL")
         << "  got=" << got << "  exp=" << expected << "\n";
}

int main()
{
    check({1, 3, 4, 2, 2}, 2);
    check({3, 1, 3, 4, 2}, 3);
    check({1, 1}, 1);
    check({2, 2, 2, 2, 2}, 2);
    check({2, 5, 9, 6, 9, 3, 8, 9, 7, 1}, 9);
    return 0;
}
