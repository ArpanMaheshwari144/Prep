// ============================================================
// PEHLE YE (har problem pe, 30 sec)
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree? intervals? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE -> "baar-baar SAME kya nikal raha?" -> STORE -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" -> X me jo THI HI NAHI, wo line kya? (edge wahin)
//   6. Code ke BAAD: pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// LARGEST RECTANGLE IN HISTOGRAM (LeetCode 84)   [REDO_2]
// ============================================================
// heights[] diya, har bar ki WIDTH 1, height = heights[i].
// lagatar bars pe banne wala SABSE BADA rectangle ka AREA lautao.
// (rectangle ki height = us range ke bars me SABSE CHHOTI.)
//
//   [2,1,5,6,2,3]      -> 10   (bars 5,6 -> min 5 * width 2)
//   [2,4]              -> 4
//   [1,1]              -> 2
//   [6]                -> 6
//   [0]                -> 0
//   [2,1,2]            -> 3    (poora: min 1 * width 3)
//   [4,2,0,3,2,5]      -> 6
//   [1,2,3,4,5]        -> 9
//   [5,4,3,2,1]        -> 9
//   [3,3,3,3]          -> 12
//   [2,2,1,2,2]        -> 5
//   [6,2,5,4,5,1,6]    -> 12
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> nextSmaller(vector<int> &heights)
{
    int n = heights.size();
    vector<int> ans(n, n);
    stack<int> st;
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
    vector<int> ans(n, -1);
    stack<int> st;
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
    int area = INT_MIN;
    for (int i = 0; i < n; i++)
    {
        int width = ns[i] - ps[i] - 1;
        area = max(area, heights[i] * width);
    }
    return area;
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
