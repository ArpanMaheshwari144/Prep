// ============================================================
// FIND THE DUPLICATE NUMBER (LC-287) — Linked List family (Floyd's cycle, ARRAY-application)
// ============================================================
// nums[] me n+1 integers hain, har value 1..n range me. Exactly EK number
// repeat hota hai (ek ya kai baar). Us DUPLICATE number ko return karo.
//   CONSTRAINT: O(1) extra space + array MODIFY nahi (read-only).
//
// ---- SOCH (kyun linked-list) ----
//  har index ko POINTER samjho: i se jump -> nums[i].
//    e.g. nums = [3,1,3,4,2]:  0->nums[0]=3 ->nums[3]=4 ->nums[2]=3 ->4 ->3 ->4... (LOOP)
//  values 1..n + n+1 slots -> koi value 2 baar point hoti -> CHAIN me CYCLE banta.
//  cycle ka ENTRY node = duplicate number.  ->  Floyd fast/slow (jaise detect_cycle):
//    PHASE-1: slow=nums[slow] (1 kadam), fast=nums[nums[fast]] (2 kadam) -> milte (slow==fast).
//    PHASE-2: ek pointer 0 pe reset -> dono 1-1 kadam (p=nums[p], slow=nums[slow]) -> jahan mile = duplicate.
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

// ---- ARPAN KI APPROACH (VERIFIED 5/5) ---- Floyd 2-phase, array-as-pointer
//  array = implicit linked-list: i -> nums[i]. duplicate = cycle-ENTRY node.
//  PHASE-1 (meet): slow=nums[slow] (1 kadam), fast=nums[nums[fast]] (2 kadam) -> slow==fast.
//  PHASE-2 (entry): slow=0 reset -> dono 1-1 kadam (slow=nums[slow], fast=nums[fast]) -> mile = dup.
//  KYUN phase-2: meet-point se aur 0 se cycle-entry tak ka distance BARABAR (Floyd) -> isliye reset+1-1.

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
