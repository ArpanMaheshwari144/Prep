// ============================================================
// SPIRAL MATRIX — Matrix — REDO_1 (khaali page)
// ============================================================
// m x n matrix diya. SAARE elements SPIRAL order me (bahar se andar, clockwise)
// ek vector me return karo.
//   [[1,2,3],[4,5,6],[7,8,9]] -> [1,2,3,6,9,8,7,4,5]
//
// Tests (// expected):
//   [[1,2,3],[4,5,6],[7,8,9]]              -> 1 2 3 6 9 8 7 4 5
//   [[1,2,3,4],[5,6,7,8],[9,10,11,12]]     -> 1 2 3 4 8 12 11 10 9 5 6 7
//   [[1]]                                  -> 1
//   [[1,2],[3,4]]                          -> 1 2 4 3
//   [[1,2,3]]                              -> 1 2 3          (ek hi row)
//   [[1],[2],[3]]                          -> 1 2 3          (ek hi column)
//   [[1,2],[3,4],[5,6]]                    -> 1 2 4 6 5 3
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> spiralOrder(vector<vector<int>> &matrix)
{
    int row = matrix.size();
    int col = matrix[0].size();

    int top = 0, bottom = row - 1, left = 0, right = col - 1;
    vector<int> ans;

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
        }
        bottom--;

        if (left <= right)
        {
            for (int i = bottom; i >= top; i--)
            {
                ans.push_back(matrix[i][left]);
            }
        }
        left++;
    }
    return ans;
}

// ---- test harness (mat chhed) ----
void check(vector<vector<int>> m, vector<int> expected)
{
    vector<int> got = spiralOrder(m);
    cout << (got == expected ? "PASS" : "FAIL") << "  got=";
    for (int x : got)
        cout << x << " ";
    cout << "\n";
}

int main()
{
    check({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {1, 2, 3, 6, 9, 8, 7, 4, 5});
    check({{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}}, {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7});
    check({{1}}, {1});
    check({{1, 2}, {3, 4}}, {1, 2, 4, 3});
    check({{1, 2, 3}}, {1, 2, 3});
    check({{1}, {2}, {3}}, {1, 2, 3});
    check({{1, 2}, {3, 4}, {5, 6}}, {1, 2, 4, 6, 5, 3});
    return 0;
}
