// ============================================================
// SPIRAL MATRIX — Matrix
// ============================================================
// m x n matrix diya. SAARE elements SPIRAL order me (bahar se andar, clockwise)
// ek vector me return karo.
//   [[1,2,3],[4,5,6],[7,8,9]] -> [1,2,3,6,9,8,7,4,5]
//
// (approach yahan LIKHA NAHI -- poora tera. copy-pen pe grid+arrows pehle.)
//
// Tests (// expected):
//   [[1,2,3],[4,5,6],[7,8,9]]              -> 1 2 3 6 9 8 7 4 5
//   [[1,2,3,4],[5,6,7,8],[9,10,11,12]]     -> 1 2 3 4 8 12 11 10 9 5 6 7
//   [[1]]                                  -> 1
//   [[1,2],[3,4]]                          -> 1 2 4 3
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<int> spiralOrder(vector<vector<int>> &matrix)
{
    vector<int> ans;
    int row = matrix.size();
    int col = matrix[0].size();
    int top = 0, bottom = row - 1, left = 0, right = col - 1;

    // boundary chcek
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
        cout << "top " << top << " bottom " << bottom << " left " << left << " right " << right << endl;
    }
    return ans;
}

int main()
{
    vector<vector<int>> m1 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    vector<vector<int>> m2 = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    vector<vector<int>> m3 = {{1}};
    vector<vector<int>> m4 = {{1, 2}, {3, 4}};

    auto p = [](vector<int> r)
    { for (int x : r) cout << x << " "; cout << "\n"; };
    p(spiralOrder(m1)); // 1 2 3 6 9 8 7 4 5
    p(spiralOrder(m2)); // 1 2 3 4 8 12 11 10 9 5 6 7
    p(spiralOrder(m3)); // 1
    p(spiralOrder(m4)); // 1 2 4 3
    return 0;
}
