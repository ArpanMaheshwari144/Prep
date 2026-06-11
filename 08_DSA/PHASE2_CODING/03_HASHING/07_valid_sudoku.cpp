// ============================================================
// VALID SUDOKU   —   Hashing (set per row / col / box)
// ============================================================
// 9x9 board (chars '1'-'9' ya '.'=khaali). VALID hai? (solve nahi — sirf check)
// VALID = har row, har column, har 3x3 box mein koi digit repeat NA ho.
//
// INTUITION (tu ne derive kiya — code TU likhega):
//   "seen before?" idea (Contains Duplicate jaisa) — par 3 jagah, har ke liye SET:
//     - ROW check: har row → ek set → digit pehle dekha? false : daalo
//     - COL check: har col → ek set → same
//     - BOX check: har 3x3 box → ek set → same
//   '.' skip karo. Koi duplicate kahin bhi → false. Sab pass → true.
//   (TWIST: 3x3 box ko kaise group/identify karna — woh tu figure karega.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool isValidSudoku(vector<vector<char>> &board)
{
    
}

int main()
{
    vector<vector<char>> valid = {
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}};
    cout << boolalpha << isValidSudoku(valid) << endl; // expected: true

    vector<vector<char>> invalid = valid;
    invalid[0][0] = '8';                                 // column 0 mein do 8 (row 0 aur row 3) -> duplicate
    cout << boolalpha << isValidSudoku(invalid) << endl; // expected: false
    return 0;
}
