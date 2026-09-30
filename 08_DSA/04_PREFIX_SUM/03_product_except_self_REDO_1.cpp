// ============================================================
// PRODUCT OF ARRAY EXCEPT SELF (LC 238) — REDO_1 (khaali page)
// ============================================================
// nums[] diya. output[] lautao jahan output[i] = nums ke SAARE elements ka
// product, SIWAAY nums[i] ke.
// DIVISION use nahi karna. O(n) chahiye.
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   [1,2,3,4]         -> [24,12,8,6]
//   [-1,1,0,-3,3]     -> [0,0,9,0,0]
//   [2,3]             -> [3,2]
//   [0,0]             -> [0,0]
//   [0,4,5]           -> [20,0,0]
//   [-2,-3,4]         -> [-12,-8,6]
//   [1,1,1,1,1]       -> [1,1,1,1,1]
//   [2,0,3,0]         -> [0,0,0,0]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> productExceptSelf(vector<int> &nums)
{
    int n = nums.size();
    vector<int> left(n, 1);
    vector<int> right(n, 1);
    vector<int> ans(n, 1);
    int prod = 1;

    for (int i = 0; i < n; i++)
    {
        left[i] = prod;
        prod *= nums[i];
    }

    prod = 1;
    for (int i = n - 1; i >= 0; i--)
    {
        right[i] = prod;
        prod *= nums[i];
    }

    for (int i = 0; i < n; i++)
    {
        ans[i] = left[i] * right[i];
    }
    return ans;
}

// ============================ TESTS ============================
int passed = 0, total = 0;

void check(vector<int> in, vector<int> exp)
{
    total++;
    vector<int> got = productExceptSelf(in);
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << (ok ? "PASS  " : "FAIL  ") << "test " << total << "  got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    check({1, 2, 3, 4}, {24, 12, 8, 6});
    check({-1, 1, 0, -3, 3}, {0, 0, 9, 0, 0});
    check({2, 3}, {3, 2});
    check({0, 0}, {0, 0});
    check({0, 4, 5}, {20, 0, 0});
    check({-2, -3, 4}, {-12, -8, 6});
    check({1, 1, 1, 1, 1}, {1, 1, 1, 1, 1});
    check({2, 0, 3, 0}, {0, 0, 0, 0});

    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
