// ============================================================
// FIRST & LAST POSITION of target — (LeetCode 34, Medium)   [BINARY SEARCH #3]  — REDO_1
// ============================================================
// SORTED array (DUPLICATES ho sakte) + target diya.
//   target ka PEHLA index aur AAKHRI index return karo -> {first, last}.
//   nahi mile -> {-1, -1}.  O(log n) chahiye (linear nahi).
//
//   [5,7,7,8,8,10], target=8  -> 3 4
//   [5,7,7,8,8,10], target=6  -> -1 -1
//   [2,2],          target=2  -> 0 1
//
// INPUT-format (tests): nums[] , target ; expected = {first, last}.
// ============================================================
#include <bits/stdc++.h>
using namespace std;

vector<int> searchRange(vector<int> &nums, int target)
{
    vector<int> ans(2, -1);
    int low = 0, high = nums.size() - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target)
        {
            ans[0] = mid;
            high = mid - 1;
        }
        else if (nums[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    low = 0, high = nums.size() - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (nums[mid] == target)
        {
            ans[1] = mid;
            low = mid + 1;
        }
        else if (nums[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> nums, int target, vector<int> expected, int t)
{
    vector<int> got = searchRange(nums, target);
    cout << "T" << t << ": " << (got == expected ? "PASS" : "FAIL")
         << "  got=" << got[0] << " " << got[1]
         << " exp=" << expected[0] << " " << expected[1] << "\n";
}

int main()
{
    check({5, 7, 7, 8, 8, 10}, 8, {3, 4}, 1);
    check({5, 7, 7, 8, 8, 10}, 6, {-1, -1}, 2);
    check({}, 0, {-1, -1}, 3);
    check({1}, 1, {0, 0}, 4);
    check({2, 2}, 2, {0, 1}, 5);
    check({1}, 0, {-1, -1}, 6);
    check({1, 1, 1, 1, 1}, 1, {0, 4}, 7);
    check({1, 2, 3, 4, 5}, 1, {0, 0}, 8);
    check({1, 2, 3, 4, 5}, 5, {4, 4}, 9);
    check({1, 2, 2, 2, 3}, 2, {1, 3}, 10);
    check({1, 3, 5}, 4, {-1, -1}, 11);
    check({1, 3, 5}, 6, {-1, -1}, 12);
    check({1, 3, 5}, 0, {-1, -1}, 13);
    return 0;
}
