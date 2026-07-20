// ============================================================
// MAXIMAL RECTANGLE — (LeetCode 85)   (fresh; Largest-Rectangle-Histogram ka 2D bhai)
// ============================================================
// ek 2D binary matrix diya (sirf 0 aur 1). usme SIRF 1 se bhare hue sabse
// BADA rectangle ka AREA nikaalo.
//
//   matrix =
//     1 0 1 0 0
//     1 0 1 1 1
//     1 1 1 1 1
//     1 0 0 1 0        -> 6   (row1..row2 me col2,3,4 wala 1-block: 2 row x 3 col = 6)
//
//   [[0]]              -> 0
//   [[1]]              -> 1
//   [[1,1],[1,1]]      -> 4
//   [[0,1],[1,0]]      -> 1
//   [[1,1,1],[1,1,1]]  -> 6
//
//   (hint jo kal socha tha: HAR ROW ko ek HISTOGRAM maano -- us row tak har column me
//    kitne LAGATAR 1 (upar tak) = us column ki height. phir har row ke histogram pe
//    kal wala LARGEST-RECTANGLE-IN-HISTOGRAM laga do -> sab ka max.)
// ============================================================

// ---- ARPAN KI APPROACH ----
//  ★ ye question = largestRectangleArea (histogram) ka SAME-TO-SAME, bas 1D ki jagah 2D.
//  ★ IDEA: matrix ke HAR ROW ko ek HISTOGRAM me badlo, phir us row ke histogram pe kal wala
//     largest-rectangle laga do -> saare rows ka MAX = answer.
//  ★ HISTOGRAM kaise banta (heights array): row-by-row upar se neeche chalte hue --
//        cell == 1 -> heights[j] += 1  (upar wale 1s ke saath height build up)
//        cell == 0 -> heights[j] = 0   (column toot gaya -> reset)
//     example row2 tak col 2,3,4 me do lagataar 1 -> height 2, width 3 -> 2*3 = 6.
//  ★ heights array COL ke size ka (na ki row ka) -- kyunki har COLUMN apni height accumulate karta neeche jaate.
//     (row=4, col=5 -> heights = size 5.)  ★ inner loop `j` (col), outer `i` (row).
//  ★ har row ke baad -> maxi = max(maxi, largestRectangleArea(heights)). histogram ka poora code COPY-PASTE (prev/next-smaller + area).

#include <bits/stdc++.h>
using namespace std;

vector<int> prevSmaller(vector<int> &nums)
{
    vector<int> ans(nums.size(), -1);
    stack<int> st;

    for (int i = nums.size() - 1; i >= 0; i--)
    {
        while (!st.empty() && nums[st.top()] > nums[i])
        {
            int topp = st.top();
            st.pop();
            ans[topp] = i;
        }
        st.push(i);
    }
    return ans;
}

vector<int> nextSmaller(vector<int> &nums)
{
    int n = nums.size();
    vector<int> ans(nums.size(), n);
    stack<int> st;

    for (int i = 0; i < nums.size(); i++)
    {
        while (!st.empty() && nums[st.top()] > nums[i])
        {
            int topp = st.top();
            st.pop();
            ans[topp] = i;
        }
        st.push(i);
    }
    return ans;
}

int largestRectangleArea(vector<int> &heights)
{
    vector<int> pS = prevSmaller(heights);
    vector<int> nS = nextSmaller(heights);

    // for (int i = 0; i < pS.size(); i++)
    // {
    //     cout << pS[i] << " ";
    // }
    // cout << endl;

    // for (int i = 0; i < nS.size(); i++)
    // {
    //     cout << nS[i] << " ";
    // }
    // cout << endl;

    int ans = INT_MIN;
    for (int i = 0; i < heights.size(); i++)
    {

        int width = (nS[i] - pS[i] - 1);
        // cout << heights[i] << " " << width << endl;
        ans = max(ans, (heights[i] * width));
    }
    return ans;
}

int maximalRectangle(vector<vector<int>> &matrix)
{
    int row = matrix.size();
    int col = matrix[0].size();
    vector<int> heights(col, 0);
    int maxi = INT_MIN;
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (matrix[i][j] == 1)
            {
                heights[j] += 1;
            }
            else
            {
                heights[j] = 0;
            }
        }
        maxi = max(maxi, largestRectangleArea(heights));
    }
    return maxi;
}

int main()
{
    vector<vector<int>> a = {{1, 0, 1, 0, 0}, {1, 0, 1, 1, 1}, {1, 1, 1, 1, 1}, {1, 0, 0, 1, 0}};
    vector<vector<int>> b = {{0}};
    vector<vector<int>> c = {{1}};
    vector<vector<int>> d = {{1, 1}, {1, 1}};
    vector<vector<int>> e = {{0, 1}, {1, 0}};
    vector<vector<int>> f = {{1, 1, 1}, {1, 1, 1}};

    cout << maximalRectangle(a) << " (expected 6)\n";
    cout << maximalRectangle(b) << " (expected 0)\n";
    cout << maximalRectangle(c) << " (expected 1)\n";
    cout << maximalRectangle(d) << " (expected 4)\n";
    cout << maximalRectangle(e) << " (expected 1)\n";
    cout << maximalRectangle(f) << " (expected 6)\n";
    return 0;
}
