// ============================================================
// SPIRAL MATRIX   —   Grid (4 shrinking boundaries)
// ============================================================
// m x n matrix ke saare elements SPIRAL (clockwise) order mein return karo.
//
// Example:
//   [1 2 3]
//   [4 5 6]   ->   [1, 2, 3, 6, 9, 8, 7, 4, 5]
//   [7 8 9]
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   4 boundaries: top, bottom, left, right.  (shuru: top=0, bottom=n-1, left=0, right=m-1)
//   Ek round (jab tak top<=bottom AND left<=right):
//     1. top row    : left -> right   (col l..r), phir  top++
//     2. right col   : top -> bottom   (row t..b), phir  right--
//     3. bottom row  : right -> left   (col r..l), phir  bottom--   [guard: top<=bottom?]
//     4. left col    : bottom -> top   (row b..t), phir  left++      [guard: left<=right?]
//   Har edge ke baad boundary ek kadam ANDAR sikudti -> beech tak pahunch jaati.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

vector<int> spiralOrder(vector<vector<int>> &matrix)
{
    vector<int> ans;
    int m = matrix.size();
    int n = matrix[0].size();

    int topRow = 0;
    int bottomRow = m - 1;
    int leftCol = 0;
    int rightCol = n - 1;

    while (topRow <= bottomRow && leftCol <= rightCol)
    {
        for (int i = leftCol; i <= rightCol; i++)
        {
            ans.push_back(matrix[topRow][i]);
        }
        topRow++;

        for (int i = topRow; i <= bottomRow; i++)
        {
            ans.push_back(matrix[i][rightCol]);
        }
        rightCol--;

        if (topRow <= bottomRow)
        {
            for (int i = rightCol; i >= leftCol; i--)
            {
                ans.push_back(matrix[bottomRow][i]);
            }
            bottomRow--;
        }

        if (leftCol <= rightCol)
        {
            for (int i = bottomRow; i >= topRow; i--)
            {
                ans.push_back(matrix[i][leftCol]);
            }
            leftCol++;
        }
    }
    return ans;
}

int main()
{
    vector<vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};

    vector<int> ans = spiralOrder(matrix);

    for (int x : ans)
        cout << x << " ";
    cout << endl;
    // expected: 1 2 3 6 9 8 7 4 5
    return 0;
}
