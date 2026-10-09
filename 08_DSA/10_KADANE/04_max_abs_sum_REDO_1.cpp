// ═══════════════════════════════════════════════════════════════════════
//  MAXIMUM ABSOLUTE SUM OF ANY SUBARRAY  (LC 1749)  — REDO
//  int array diya. kisi bhi CONTIGUOUS subarray ka sum lo -> uska ABSOLUTE
//  value (|sum|). sabse BADA |sum| return karo.
//
//  INPUT-FORM: nums me positive, negative, zero sab ho sakte. size >= 1.
//  TEST-CASES (input -> expected):
//   1) [1,-3,2,3,-4]              -> 5    ([2,3] = 5)
//   2) [2,-5,1,-4,3,-2]           -> 8    ([-5,1,-4] = -8)
//   3) [1,2,3]                    -> 6
//   4) [-1,-2,-3]                 -> 6    (|-6|)
//   5) [5]                        -> 5
//   6) [-5]                       -> 5    (ek hi element, negative)
//   7) [0,0,0]                    -> 0
//   8) [3,-1,3]                   -> 5
//   9) [-2,1,-3,4,-1,2,1,-5,4]    -> 6
//  10) [10,-20,10]                -> 20
//  11) [1,-1,1,-1]                -> 1
//  12) [-3,5,-3]                  -> 5
//  13) [10,-1]                    -> 10   (sabse bada = pehla element akela)
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int maxAbsoluteSum(vector<int> &nums)
{
    int n = nums.size();
    if (n == 1)
    {
        return abs(nums[0]);
    }

    int mini = 0;
    int maxi = 0;
    int ans = 0;
    for (int i = 0; i < n; i++)
    {
        int temp = max({nums[i], mini + nums[i], maxi + nums[i]});
        mini = min({nums[i], mini + nums[i], maxi + nums[i]});
        maxi = temp;
        ans = max(ans, max(abs(mini), abs(maxi)));
    }
    return ans;
}

// ---- test helper ----
void check(vector<int> nums, int exp, string name)
{
    int got = maxAbsoluteSum(nums);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({1, -3, 2, 3, -4}, 5, "t1");
    check({2, -5, 1, -4, 3, -2}, 8, "t2 negative wala bada");
    check({1, 2, 3}, 6, "t3 sab positive");
    check({-1, -2, -3}, 6, "t4 sab negative");
    check({5}, 5, "t5 single");
    check({-5}, 5, "t6 single negative");
    check({0, 0, 0}, 0, "t7 zeros");
    check({3, -1, 3}, 5, "t8");
    check({-2, 1, -3, 4, -1, 2, 1, -5, 4}, 6, "t9");
    check({10, -20, 10}, 20, "t10");
    check({1, -1, 1, -1}, 1, "t11");
    check({-3, 5, -3}, 5, "t12");
    check({10, -1}, 10, "t13 pehla element akela");
    return 0;
}
