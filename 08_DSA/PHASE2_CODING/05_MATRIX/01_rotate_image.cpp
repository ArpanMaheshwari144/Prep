// ============================================================
// ROTATE IMAGE (90 degree clockwise, IN-PLACE)   —   Grid / Matrix
// ============================================================
// n x n matrix ko 90 degree clockwise ghumao. Nayi matrix NAHI banani (in-place).
//
// Example:
//   [1 2 3]        [7 4 1]
//   [4 5 6]   ->   [8 5 2]
//   [7 8 9]        [9 6 3]
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   Observation: har COLUMN -> ek ROW ban jaati hai, ULTI.
//   Direct in-place karne mein overwrite -> data kho jaata. Isliye 2-step trick:
//     STEP 1 — TRANSPOSE: board[i][j] <-> board[j][i]  (diagonal pe flip, khadi<->padi)
//              (sirf upper triangle pe swap, warna do baar swap ho ke wapas aa jaayega)
//     STEP 2 — har ROW ko REVERSE karo.
//   Dono ke baad -> 90 degree clockwise rotate. (12th wala matrix transpose hi hai.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

void rotate(vector<vector<int>> &matrix)
{
    int row = matrix.size();
    int col = matrix[0].size();

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (j > i)
                swap(matrix[i][j], matrix[j][i]);
        }
    }

    for (int i = 0; i < row; i++)
    {
        reverse(matrix[i].begin(), matrix[i].end());
    }
}

int main()
{
    vector<vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    rotate(matrix);

    for (auto &row : matrix)
    {
        for (int x : row)
            cout << x << " ";
        cout << endl;
    }
    // expected:
    //   7 4 1
    //   8 5 2
    //   9 6 3
    return 0;
}
