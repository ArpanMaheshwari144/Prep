// ============================================================
// FIND ALL NUMBERS DISAPPEARED IN AN ARRAY — (LeetCode 448, Easy)
// [array-as-hashmap · index-marking, O(1) extra space]
// ============================================================
// nums[] ka size n, har value 1..n ke beech (kuch repeat ho sakte, kuch missing).
// [1..n] me se jo MISSING hain, sab return kar.
// GOAL: O(n) time, O(1) EXTRA space (output vector count nahi hota).
//
//   [4,3,2,7,8,2,3,1]  -> [5,6]
//   [1,1]              -> [2]
//   [1,2,3,4]          -> []      (kuch missing nahi)
//   [2,2,2,2]          -> [1,3,4]
//   [1]                -> []
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ============================================================
vector<int> findDisappearedNumbers(vector<int> &nums)
{
    vector<int> ans;
    for (int i = 0; i < nums.size(); i++)
    {
        int index = abs(nums[i]) - 1;
        if (nums[index] > 0)
        {
            nums[index] = -nums[index];
        }
    }

    for (int i = 0; i < nums.size(); i++)
    {
        if (nums[i] > 0)
        {
            ans.push_back(i + 1);
        }
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, vector<int> exp, int t)
{
    vector<int> got = findDisappearedNumbers(in);
    sort(got.begin(), got.end()); // order-independent compare
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "] exp=[";
    for (int i = 0; i < (int)exp.size(); i++)
        cout << exp[i] << (i + 1 < (int)exp.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    check({4, 3, 2, 7, 8, 2, 3, 1}, {5, 6}, 1);
    check({1, 1}, {2}, 2);
    check({1, 2, 3, 4}, {}, 3);
    check({2, 2, 2, 2}, {1, 3, 4}, 4);
    check({1}, {}, 5);
    return 0;
}
