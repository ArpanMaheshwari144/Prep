// ============================================================
// SEARCH A 2D MATRIX   —   Binary Search (2D ko 1D treat karo)
// ============================================================
// m x n matrix: har ROW left->right sorted, AUR har row ka pehla element
// pichli row ke aakhri se bada. = poori matrix ek LAMBI sorted line.
// target hai ya nahi -> true / false. O(log(m*n)).
//   [[1, 3, 5, 7],
//    [10,11,16,20],
//    [23,30,34,60]]
//
// SIGNAL: "fully sorted grid + search O(log)" -> Binary Search on virtual 1D index.
//   KEY (tu code mein nikaalega): mid ek 1D index hai (0..m*n-1), par value
//   matrix[row][col] pe hai. mid ko (row,col) mein todo -> #columns se socho
//   (divide aur remainder ka khel).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool searchMatrix(vector<vector<int>> &matrix, int target)
{
    int row = matrix.size();
    int col = matrix[0].size();

    int low = 0;
    int high = row * col - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (matrix[mid / col][mid % col] == target)
        {
            return true;
        }
        else if (matrix[mid / col][mid % col] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return false;
}

int main()
{
    vector<vector<int>> a = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
    cout << searchMatrix(a, 3) << endl;  // expected 1  (true)
    cout << searchMatrix(a, 13) << endl; // expected 0  (false)
    cout << searchMatrix(a, 1) << endl;  // expected 1  (corner: pehla)
    cout << searchMatrix(a, 60) << endl; // expected 1  (corner: aakhri)
    cout << searchMatrix(a, 16) << endl; // expected 1
    cout << searchMatrix(a, 22) << endl; // expected 0

    vector<vector<int>> b = {{5}};
    cout << searchMatrix(b, 5) << endl; // expected 1
    cout << searchMatrix(b, 2) << endl; // expected 0
    return 0;
}
