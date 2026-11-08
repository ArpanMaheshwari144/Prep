// ============================================================
// PEHLE YE (har problem pe, 30 sec)
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree? intervals? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE -> "baar-baar SAME kya nikal raha?" -> STORE -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" -> X me jo THI HI NAHI, wo line kya? (edge wahin)
//   6. Code ke BAAD: pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// SPIRAL MATRIX   [REDO_2]
// ============================================================
// m x n matrix diya. SAARE elements SPIRAL order me (bahar se andar, clockwise)
// ek vector me lautao.
//
//   [[1,2,3],[4,5,6],[7,8,9]]                          -> 1 2 3 6 9 8 7 4 5
//   [[1,2,3,4],[5,6,7,8],[9,10,11,12]]                 -> 1 2 3 4 8 12 11 10 9 5 6 7
//   [[1]]                                              -> 1
//   [[1,2],[3,4]]                                      -> 1 2 4 3
//   [[1,2,3]]                                          -> 1 2 3          (ek hi row)
//   [[1],[2],[3]]                                      -> 1 2 3          (ek hi column)
//   [[1,2],[3,4],[5,6]]                                -> 1 2 4 6 5 3
//   [[1,2,3],[4,5,6]]                                  -> 1 2 3 6 5 4
//   [[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]]   -> 1 2 3 4 8 12 16 15 14 13 9 5 6 7 11 10
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> spiralOrder(vector<vector<int>> &matrix)
{
    vector<int> ans;
    int row = matrix.size(), col = matrix[0].size();
    int top = 0, left = 0, right = col - 1, bottom = row - 1;
    while (top <= bottom && left <= right)
    {
        for (int i = left; i <= right; i++)
        {
            ans.push_back(matrix[top][i]);
        }
        top++;

        for (int i = top; i <= bottom; i++)
        {
            ans.push_back(matrix[i][right]);
        }
        right--;

        if (top <= bottom)
        {
            for (int i = right; i >= left; i--)
            {
                ans.push_back(matrix[bottom][i]);
            }
            bottom--;
        }

        if (left <= right)
        {
            for (int i = bottom; i >= top; i--)
            {
                ans.push_back(matrix[i][left]);
            }
            left++;
        }
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<vector<int>> m, vector<int> exp, int t)
{
    vector<int> got = spiralOrder(m);
    cout << "Test " << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=";
    for (int x : got)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    check({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {1, 2, 3, 6, 9, 8, 7, 4, 5}, 1);
    check({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}}, {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7}, 2);
    check({{1}}, {1}, 3);
    check({{1, 2}, {3, 4}}, {1, 2, 4, 3}, 4);
    check({{1, 2, 3}}, {1, 2, 3}, 5);
    check({{1}, {2}, {3}}, {1, 2, 3}, 6);
    check({{1, 2}, {3, 4}, {5, 6}}, {1, 2, 4, 6, 5, 3}, 7);
    check({{1, 2, 3}, {4, 5, 6}}, {1, 2, 3, 6, 5, 4}, 8);
    check({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}},
          {1, 2, 3, 4, 8, 12, 16, 15, 14, 13, 9, 5, 6, 7, 11, 10}, 9);
    return 0;
}
