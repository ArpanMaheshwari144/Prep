// ============================================================
// SET MATRIX ZEROES   —   Grid (row/col marking)
// ============================================================
// m x n matrix. Agar koi cell 0 hai -> uski POORI row aur POORI column 0 kar do. In-place.
//
// Example:
//   [1 1 1]        [1 0 1]
//   [1 0 1]   ->   [0 0 0]
//   [1 1 1]        [1 0 1]
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   TWIST: chalte-chalte 0 mat karo -> naye 0 confuse karenge (cascade -> poori matrix 0).
//   To 2-pass:
//     Pass 1 — poori matrix scan; jahan 0 mila uski position (ya us row/col) YAAD rakho.
//     Pass 2 — har yaad-rakhe 0 ke liye: poori ROW + poori COLUMN zero karo (cell nahi, pura row+col).
//   Storage choice: (i,j) positions yaad rakho, YA rows-set + cols-set (kaunsi row/col mein 0 tha).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void setZeroes(vector<vector<int>> &matrix)
{
    int row = matrix.size();
    int col = matrix[0].size();
    vector<bool> zeroRow(row, false), zeroCol(col, false);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (matrix[i][j] == 0)
            {
                zeroRow[i] = true;
                zeroCol[j] = true;
            }
        }
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (zeroRow[i] == true || zeroCol[j] == true)
            {
                matrix[i][j] = 0;
            }
        }
    }
}

int main()
{
    vector<vector<int>> matrix = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}};

    setZeroes(matrix);

    for (auto &row : matrix)
    {
        for (int x : row)
            cout << x << " ";
        cout << endl;
    }
    // expected:
    //   1 0 1
    //   0 0 0
    //   1 0 1
    return 0;
}
