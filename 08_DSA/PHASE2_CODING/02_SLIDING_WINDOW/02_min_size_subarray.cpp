// ============================================================
// MIN SIZE SUBARRAY SUM   —   Sliding Window (VARIABLE)
// ============================================================
// Array `nums` (positive), int `target`. Sabse CHHOTI lambai ka lagatar
// subarray jiska sum >= target — woh lambai return karo. Na mile to 0.
//
// Example:
//   {2, 3, 1, 2, 4, 3}, target = 7  ->  2   (4 + 3)
//   {1, 1, 1, 1},       target = 7  ->  0   (kabhi 7 nahi)
//
// INTUITION (soch — code TU likhega):
//   Ek khidki jo BAD-TI aur GHAT-TI hai (variable).
//   - RIGHT se EXPAND karo — element add, sum badhta jaye.
//   - Jab sum >= target ho jaye:
//        LEFT se WHILE-SHRINK karo (element hatao, sum ghatta), jab tak sum >= target rahe,
//        har valid pe sabse CHHOTI lambai yaad rakho.
//   - Poora ghoom ke jo sabse chhoti lambai mili = answer. (kabhi >= target na ho -> 0)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int minSubArrayLen(vector<int> &nums, int target)
{
    int i = 0, j = 0, minLen = INT_MAX, sum = 0;
    while (j < nums.size())
    {
        sum += nums[j];
        while (sum >= target)
        {
            minLen = min(minLen, j - i + 1);
            sum -= nums[i];
            i++;
        }
        j++;
    }
    return minLen == INT_MAX ? 0 : minLen;
}

int main()
{
    vector<int> a = {2, 3, 1, 2, 4, 3};
    cout << minSubArrayLen(a, 7) << endl; // expected: 2

    vector<int> b = {1, 1, 1, 1};
    cout << minSubArrayLen(b, 7) << endl; // expected: 0
    return 0;
}
