// ============================================================
// NEXT GREATER ELEMENT II — (LeetCode 503, Medium)   [STACK #06 · circular]  — REDO_1
// ============================================================
// int array nums (CIRCULAR). har element ke liye uska NEXT GREATER number lauta
// (array ke end ke baad phir shuru se dekh sakte -- circular). na mile -> -1.
//   [1,2,1]      -> [2,-1,2]     (aakhri 1 ka next greater circular me 2)
//   [5,4,3,2,1]  -> [-1,5,5,5,5]
//   [1,2,3,4,3]  -> [2,3,4,-1,4]
// ============================================================
#include <bits/stdc++.h>
using namespace std;

vector<int> nextGreaterElements(vector<int> &nums)
{
    if (nums.size() == 0)
        return {};

    int n = nums.size();
    stack<int> st;
    vector<int> ans(n, -1);
    for (int i = 0; i < 2 * n; i++)
    {
        while (!st.empty() && nums[st.top()] < nums[i % n])
        {
            int tp = st.top();
            st.pop();
            ans[tp] = nums[i % n];
        }
        st.push(i % n);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int passed = 0, total = 0;
void check(vector<int> nums, vector<int> expected, int t)
{
    total++;
    vector<int> got = nextGreaterElements(nums);
    bool ok = (got == expected);
    if (ok)
        passed++;
    cout << "Test " << t << ": " << (ok ? "PASS" : "FAIL") << "  got [";
    for (int i = 0; i < (int)got.size(); i++)
        cout << (i ? "," : "") << got[i];
    cout << "]  expected [";
    for (int i = 0; i < (int)expected.size(); i++)
        cout << (i ? "," : "") << expected[i];
    cout << "]\n";
}

int main()
{
    check({1, 2, 1}, {2, -1, 2}, 1);
    check({5, 4, 3, 2, 1}, {-1, 5, 5, 5, 5}, 2);
    check({1, 2, 3, 4, 3}, {2, 3, 4, -1, 4}, 3);
    check({3, 3}, {-1, -1}, 4);
    check({1}, {-1}, 5);
    check({1, 2, 3}, {2, 3, -1}, 6);
    check({2, 1, 2, 4, 3}, {4, 2, 4, -1, 4}, 7);
    check({7, 7, 7, 7}, {-1, -1, -1, -1}, 8);
    check({}, {}, 9);
    check({-2, -1, -3}, {-1, -1, -2}, 10);
    check({4, 1, 2}, {-1, 2, 4}, 11);
    check({1, 5, 3, 6, 8}, {5, 6, 6, 8, -1}, 12);
    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
