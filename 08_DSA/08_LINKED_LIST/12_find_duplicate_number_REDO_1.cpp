// ═══════════════════════════════════════════════════════════════════════
//  FIND THE DUPLICATE NUMBER — (LeetCode 287)   (REDO_1)
//  nums[] me n+1 numbers hain, har value 1..n ke beech. Exactly EK value repeat hoti hai
//  (do baar ya zyada baar). Wo DUPLICATE value lautao.
//  SHART: extra space O(1), aur array ko BADALNA nahi (read-only).
//         (sort / hashset / value ko negative karna = shart toot gayi)
//
//  TEST-CASES (input -> expected):
//   1) [1,3,4,2,2]               -> 2
//   2) [3,1,3,4,2]               -> 3
//   3) [1,1]                     -> 1      sabse chhota
//   4) [2,2,2,2,2]               -> 2      kai baar repeat
//   5) [2,5,9,6,9,3,8,9,7,1]     -> 9
//   6) [1,1,2]                   -> 1
//   7) [2,1,2]                   -> 2
//   8) [1,2,3,4,5,6,7,8,9,9]     -> 9      aakhir me
//   9) [4,3,1,4,2]               -> 4
//  10) [1,4,4,2,4]               -> 4
//  (har test ke baad check: nums badla to nahi)
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int findDuplicate(vector<int> &nums)
{
    int slow = 0, fast = 0;
    slow = nums[slow], fast = nums[nums[fast]];
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

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> nums, int exp, string name)
{
    vector<int> before = nums;
    int got = findDuplicate(nums);
    bool ok = (got == exp);
    bool same = (nums == before);
    cout << (ok && same ? "PASS" : "FAIL") << " | " << name
         << " | got=" << got << " exp=" << exp
         << (same ? "" : "  (nums BADAL gaya -> shart toot gayi)") << "\n";
}

int main()
{
    check({1, 3, 4, 2, 2}, 2, "t1 basic");
    check({3, 1, 3, 4, 2}, 3, "t2 basic-2");
    check({1, 1}, 1, "t3 smallest");
    check({2, 2, 2, 2, 2}, 2, "t4 many-times");
    check({2, 5, 9, 6, 9, 3, 8, 9, 7, 1}, 9, "t5 bigger");
    check({1, 1, 2}, 1, "t6 n=2 a");
    check({2, 1, 2}, 2, "t7 n=2 b");
    check({1, 2, 3, 4, 5, 6, 7, 8, 9, 9}, 9, "t8 dup-at-end");
    check({4, 3, 1, 4, 2}, 4, "t9 mixed");
    check({1, 4, 4, 2, 4}, 4, "t10 triple");
    return 0;
}
