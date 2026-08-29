// ============================================================
// MOVE ZEROES — REDO   [LC-283, TWO-POINTER]  (comfort redo, memory se)
// ============================================================
// nums[] me saare 0 END me shift karo, non-zero ka ORDER same rahe. (in-place)
//   [0,1,0,3,12] -> [1,3,12,0,0]  ·  [0] -> [0]  ·  [1,2,3] -> [1,2,3]  ·  [0,0,1] -> [1,0,0]
//
//   slow = agli jagah jahan non-zero rakhna hai; fast SCAN.
//   fast pe non-zero -> swap(nums[slow], nums[fast]) PEHLE -> PHIR slow++.  fast har baar++.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void moveZeroes(vector<int> &nums)
{
    int slow = 0;
    int fast = 0;
    while (fast < nums.size())
    {
        if (nums[fast] != 0)
        {
            swap(nums[slow], nums[fast]);
            slow++;
        }
        fast++;
    }
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, vector<int> exp, int t)
{
    vector<int> got = in;
    moveZeroes(got);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=[";
    for (int x : got)
        cout << x << " ";
    cout << "]\n";
}

int main()
{
    check({0, 1, 0, 3, 12}, {1, 3, 12, 0, 0}, 1);
    check({0}, {0}, 2);
    check({1, 2, 3}, {1, 2, 3}, 3);
    check({0, 0, 1}, {1, 0, 0}, 4);
    check({1, 0, 1}, {1, 1, 0}, 5);
    return 0;
}
