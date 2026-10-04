// ============================================================
// SET MATRIX ZEROES (LC 73) — REDO_2 (khaali page)
// ============================================================
// m x n matrix diya. agar koi element 0 hai -> uski POORI ROW aur POORI COLUMN
// ko 0 kar do. IN-PLACE (matrix hi modify karo). return void.
//
// Tests (neeche main() me, khud PASS/FAIL batayega):
//   [[1,1,1],[1,0,1],[1,1,1]]        -> [[1,0,1],[0,0,0],[1,0,1]]
//   [[0,1,2,0],[3,4,5,2],[1,3,1,5]]  -> [[0,0,0,0],[0,4,5,0],[0,3,1,0]]
//   [[1,2],[3,4]]                    -> [[1,2],[3,4]]        (koi 0 nahi)
//   [[0]]                            -> [[0]]
//   [[1,0,3]]                        -> [[0,0,0]]            (ek hi row)
//   [[1],[0],[2]]                    -> [[0],[0],[0]]        (ek hi column)
//   [[1,2,3],[0,5,0],[7,8,9]]        -> [[0,2,0],[0,0,0],[0,8,0]]  (ek row me do 0)
//   [[0,0],[1,1]]                    -> [[0,0],[0,0]]
//   [[1,2,3,4],[5,0,7,8],[9,10,11,0],[13,14,15,16]] -> [[1,0,3,0],[0,0,0,0],[0,0,0,0],[13,0,15,0]]
//   [[-1,0],[2,3]]                   -> [[0,0],[2,0]]        (negative bhi)
// ============================================================

#include <bits/stdc++.h>
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
                zeroRow[i] = 0;
                zeroCol[j] = 0;
            }
        }
    }

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (zeroRow[i] == 0 || zeroCol[j] == 0)
            {
                matrix[i][j] = 0;
            }
        }
    }
}

// ============================ TESTS ============================
int passed = 0, total = 0;

void check(vector<vector<int>> in, vector<vector<int>> exp)
{
    total++;
    setZeroes(in);
    bool ok = (in == exp);
    if (ok)
        passed++;
    cout << (ok ? "PASS  " : "FAIL  ") << "test " << total << "  got=";
    for (auto &r : in)
    {
        cout << "[";
        for (int i = 0; i < (int)r.size(); i++)
            cout << r[i] << (i + 1 < (int)r.size() ? "," : "");
        cout << "]";
    }
    cout << "\n";
}

int main()
{
    check({{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}, {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}});
    check({{0, 1, 2, 0}, {3, 4, 5, 2}, {1, 3, 1, 5}}, {{0, 0, 0, 0}, {0, 4, 5, 0}, {0, 3, 1, 0}});
    check({{1, 2}, {3, 4}}, {{1, 2}, {3, 4}});
    check({{0}}, {{0}});
    check({{1, 0, 3}}, {{0, 0, 0}});
    check({{1}, {0}, {2}}, {{0}, {0}, {0}});
    check({{1, 2, 3}, {0, 5, 0}, {7, 8, 9}}, {{0, 2, 0}, {0, 0, 0}, {0, 8, 0}});
    check({{0, 0}, {1, 1}}, {{0, 0}, {0, 0}});
    check({{1, 2, 3, 4}, {5, 0, 7, 8}, {9, 10, 11, 0}, {13, 14, 15, 16}}, {{1, 0, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {13, 0, 15, 0}});
    check({{-1, 0}, {2, 3}}, {{0, 0}, {2, 0}});

    cout << "\n"
         << passed << "/" << total << " passed\n";
    return 0;
}
