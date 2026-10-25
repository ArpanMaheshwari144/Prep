// ============================================================
// MAXIMUM PRODUCT SUBARRAY — (LeetCode 152, Medium)   [REDO_4 — khud bharo]
// ============================================================
// ek int array diya. aisa CONTIGUOUS subarray dhoondho jiska elements ka
// PRODUCT (guna) sabse BADA ho. wo max product lautao.
//
// Tests (expected):
//   1. [2,3,-2,4]          -> 6    (2*3)
//   2. [-2,0,-1]           -> 0
//   3. [-2,3,-4]           -> 24   (-2*3*-4)
//   4. [2,-5,-2,-4,3]      -> 24   (-2*-4*3)
//   5. [-3,-1,-1]          -> 3    (-3*-1)
//   6. [5]                 -> 5
//   7. [-2]                -> -2
//   8. [0,2]               -> 2
//   9. [-1,-2,-3]          -> 6    (-2*-3)
//  10. [-2,-3,0,-4,-5]     -> 20   (-4*-5)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxProduct(vector<int> &nums)
{
}

// ---- tests (ise mat chhedo) ----
int pass = 0, total = 0;
void check(vector<int> nums, int expected, string name)
{
    total++;
    int got = maxProduct(nums);
    bool ok = (got == expected);
    if (ok)
        pass++;
    cout << (ok ? "PASS " : "FAIL ") << name << "  got " << got << " expected " << expected << "\n";
}

int main()
{
    check({2, 3, -2, 4}, 6, "1");
    check({-2, 0, -1}, 0, "2");
    check({-2, 3, -4}, 24, "3");
    check({2, -5, -2, -4, 3}, 24, "4");
    check({-3, -1, -1}, 3, "5");
    check({5}, 5, "6");
    check({-2}, -2, "7");
    check({0, 2}, 2, "8");
    check({-1, -2, -3}, 6, "9");
    check({-2, -3, 0, -4, -5}, 20, "10");

    cout << "\n"
         << pass << "/" << total << " PASS\n";
    return 0;
}
