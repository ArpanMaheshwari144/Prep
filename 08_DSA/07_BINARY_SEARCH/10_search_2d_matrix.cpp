// ============================================================
// SEARCH A 2D MATRIX — Binary Search  (blank test)
// ============================================================
// m x n matrix: har row sorted, aur har row ka PEHLA element pichli row ke
// AAKHRI se bada (yani poora matrix ek sorted 1D array jaisा). target dhoondo -> true/false.
//   [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target=3 -> true
//
// (approach LIKHA NAHI -- blank. socho: poore matrix ko ek VIRTUAL 1D sorted array maano
//  (size = m*n) -> uspe normal BINARY SEARCH.
//  index i ko (row, col) me kaise badlein? -> row = i / cols, col = i % cols. socho.
//  copy-pen pe trace kar.)
//
// Tests (// expected):
//   [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target=3   -> 1 (true)
//   [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target=13  -> 0 (false)
//   [[1]], target=1                                      -> 1
//   [[1]], target=2                                      -> 0
//   [[1,3]], target=3                                    -> 1
//   [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target=60  -> 1
//   [[1,3,5,7],[10,11,16,20],[23,30,34,60]], target=0   -> 0
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

bool searchMatrix(vector<vector<int>> &matrix, int target)

{
    int row = matrix.size();
    int col = matrix[0].size();
    int low = 0, high = row * col - 1;
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
    vector<vector<int>> m = {{1, 3, 5, 7}, {10, 11, 16, 20}, {23, 30, 34, 60}};
    vector<vector<int>> s = {{1}};
    vector<vector<int>> r = {{1, 3}};

    cout << searchMatrix(m, 3) << " (expected 1)\n";
    cout << searchMatrix(m, 13) << " (expected 0)\n";
    cout << searchMatrix(s, 1) << " (expected 1)\n";
    cout << searchMatrix(s, 2) << " (expected 0)\n";
    cout << searchMatrix(r, 3) << " (expected 1)\n";
    cout << searchMatrix(m, 60) << " (expected 1)\n";
    cout << searchMatrix(m, 0) << " (expected 0)\n";
    return 0;
}
