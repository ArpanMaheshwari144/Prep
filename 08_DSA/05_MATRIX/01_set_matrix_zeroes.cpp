// ============================================================
// SET MATRIX ZEROES — Matrix
// ============================================================
// m x n matrix diya. agar koi element 0 hai -> uski POORI ROW aur POORI COLUMN
// ko 0 kar do. IN-PLACE (matrix hi modify karo). return void.
//
// ---- ARPAN KI APPROACH ----
//  do marker arrays: zeroRow[row], zeroCol[col]. pehle pass me poora matrix ghoomo,
//  jahan matrix[i][j]==0 mile wahan zeroRow[i]=1 aur zeroCol[j]=1 mark kar do.
//  doosre pass me har cell dekho -- agar uski row ya col marked hai to matrix[i][j]=0. in-place.
//
// Tests (// expected):
//   [[1,1,1],[1,0,1],[1,1,1]]        -> [[1,0,1],[0,0,0],[1,0,1]]
//   [[0,1,2,0],[3,4,5,2],[1,3,1,5]]  -> [[0,0,0,0],[0,4,5,0],[0,3,1,0]]
//   [[1,2],[3,4]]                    -> [[1,2],[3,4]]   (koi 0 nahi)
//   [[0]]                            -> [[0]]
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

void setZeroes(vector<vector<int>> &matrix)
{
    int row = matrix.size();
    int col = matrix[0].size();

    vector<int> zeroRow(row, -1);
    vector<int> zeroCol(col, -1);

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (matrix[i][j] == 0)
            {
                zeroRow[i] = 1;
                zeroCol[j] = 1;
            }
        }
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (zeroRow[i] == 1 || zeroCol[j] == 1)
            {
                matrix[i][j] = 0;
            }
        }
    }
}

int main()
{
    vector<vector<int>> m1 = {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
    vector<vector<int>> m2 = {{0, 1, 2, 0}, {3, 4, 5, 2}, {1, 3, 1, 5}};
    vector<vector<int>> m3 = {{1, 2}, {3, 4}};
    vector<vector<int>> m4 = {{0}};

    auto p = [](vector<vector<int>> &mat)
    {
        for (auto &row : mat)
        {
            for (int x : row)
                cout << x << " ";
            cout << "| ";
        }
        cout << "\n";
    };
    setZeroes(m1);
    p(m1); // 1 0 1 | 0 0 0 | 1 0 1 |
    setZeroes(m2);
    p(m2); // 0 0 0 0 | 0 4 5 0 | 0 3 1 0 |
    setZeroes(m3);
    p(m3); // 1 2 | 3 4 |
    setZeroes(m4);
    p(m4); // 0 |
    return 0;
}
