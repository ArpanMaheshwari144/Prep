// ============================================================
// LARGEST RECTANGLE IN HISTOGRAM — (LeetCode 84, Hard)   [STACK #8]  — REDO_1
// ============================================================
// heights[] diya -- har bar ki WIDTH 1, height = heights[i].
// lagatar bars pe banne wala SABSE BADA rectangle ka AREA nikaalo.
// (rectangle ki height = us range ke bars me SABSE CHHOTI.)
//
//   heights = [2,1,5,6,2,3]   -> 10   (bars 5,6 -> min 5 * width 2)
//   heights = [2,4]           -> 4
//   heights = [2,1,2]         -> 3    (poora: min 1 * width 3)
//
// INPUT-format (tests): heights[] ; expected = max area.
// ============================================================
#include <bits/stdc++.h>
using namespace std;

vector<int> nextSmaller(vector<int> &heights)
{
    int n = heights.size();
    stack<int> st;
    vector<int> ans(n, n);
    for (int i = 0; i < n; i++)
    {
        while (!st.empty() && heights[st.top()] > heights[i])
        {
            int tp = st.top();
            st.pop();
            ans[tp] = i;
        }
        st.push(i);
    }
    return ans;
}

vector<int> prevSmaller(vector<int> &heights)
{
    int n = heights.size();
    stack<int> st;
    vector<int> ans(n, -1);
    for (int i = n - 1; i >= 0; i--)
    {
        while (!st.empty() && heights[st.top()] > heights[i])
        {
            int tp = st.top();
            st.pop();
            ans[tp] = i;
        }
        st.push(i);
    }
    return ans;
}

int largestRectangleArea(vector<int> &heights)
{
    int n = heights.size();
    vector<int> ns = nextSmaller(heights);
    vector<int> ps = prevSmaller(heights);

    // for (int i = 0; i < ns.size(); i++)
    // {
    //     cout << ns[i] << " ";
    // }
    // cout << endl;

    // for (int i = 0; i < ps.size(); i++)
    // {
    //     cout << ps[i] << " ";
    // }
    // cout << endl;

    int ans = INT_MIN;
    for (int i = 0; i < n; i++)
    {
        int width = ns[i] - ps[i] - 1;
        ans = max(ans, heights[i] * width);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> heights, int expected, int t)
{
    int got = largestRectangleArea(heights);
    cout << "T" << t << ": " << (got == expected ? "PASS" : "FAIL")
         << "  got=" << got << " exp=" << expected << "\n";
}

int main()
{
    check({2, 1, 5, 6, 2, 3}, 10, 1);
    check({2, 4}, 4, 2);
    check({1, 1}, 2, 3);
    check({6}, 6, 4);
    check({0}, 0, 5);
    check({2, 1, 2}, 3, 6);
    check({4, 2, 0, 3, 2, 5}, 6, 7);
    check({1, 2, 3, 4, 5}, 9, 8);
    check({5, 4, 3, 2, 1}, 9, 9);
    check({3, 3, 3, 3}, 12, 10);
    check({2, 2, 1, 2, 2}, 5, 11);
    check({6, 2, 5, 4, 5, 1, 6}, 12, 12);
    return 0;
}
