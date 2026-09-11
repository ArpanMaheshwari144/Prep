// ============================================================
// FIND ALL NUMBERS DISAPPEARED IN AN ARRAY — (LeetCode 448)  [REDO_1 · blank retrieval]
// ============================================================
// nums[] size n, values 1..n (repeat ho sakte, kuch missing). [1..n] me se jo MISSING wo sab return.
// GOAL: O(n) time, O(1) EXTRA space (output count nahi).
//
//   [4,3,2,7,8,2,3,1]  -> [5,6]
//   [1,1]              -> [2]
//   [1,2,3,4]          -> []
//   [2,2,2,2]          -> [1,3,4]
//   [1]                -> []
// insight (khud yaad kar): array-KHUD hashmap -> value v dikhi to index (v-1) ko NEGATIVE mark ->
//                          end me jo index +ve = wo (index+1) MISSING. (abs() lena.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

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

// ─── TESTS (haath mat lagana) ──────────────────────────
void check(vector<int> in, vector<int> exp, int t)
{
    vector<int> got = findDisappearedNumbers(in);
    sort(got.begin(), got.end());
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
