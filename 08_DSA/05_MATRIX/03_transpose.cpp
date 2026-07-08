// ============================================================
// TRANSPOSE MATRIX — Matrix
// ============================================================
// m x n matrix diya. uska TRANSPOSE return karo (rows <-> cols swap).
// matrix[i][j] -> result[j][i].  (non-square bhi ho sakta -> naya n x m matrix.)
//   [[1,2,3],[4,5,6]] -> [[1,4],[2,5],[3,6]]
//
// (approach yahan LIKHA NAHI -- poora tera. copy-pen pe grid pehle.)
//
// Tests (// expected):
//   [[1,2,3],[4,5,6]]              -> 1 4 | 2 5 | 3 6
//   [[1,2,3],[4,5,6],[7,8,9]]      -> 1 4 7 | 2 5 8 | 3 6 9
//   [[1]]                          -> 1
//   [[1,2],[3,4]]                  -> 1 3 | 2 4
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

vector<vector<int>> transpose(vector<vector<int>> &matrix)
{
    int row = matrix.size();
    int col = matrix[0].size();
    vector<vector<int>> ans(col, vector<int>(row, -1));

    for (int i = 0; i < col; i++)
    {
        for (int j = 0; j < row; j++)
        {
            ans[i][j] = matrix[j][i];
        }
    }
    return ans;
}

int main()
{
    vector<vector<int>> m1 = {{1, 2, 3}, {4, 5, 6}};
    vector<vector<int>> m2 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    vector<vector<int>> m3 = {{1}};
    vector<vector<int>> m4 = {{1, 2}, {3, 4}};

    auto p = [](vector<vector<int>> mat)
    {
        for (auto &row : mat)
        {
            for (int x : row)
                cout << x << " ";
            cout << "| ";
        }
        cout << "\n";
    };
    p(transpose(m1)); // 1 4 | 2 5 | 3 6
    p(transpose(m2)); // 1 4 7 | 2 5 8 | 3 6 9
    p(transpose(m3)); // 1
    p(transpose(m4)); // 1 3 | 2 4
    return 0;
}
