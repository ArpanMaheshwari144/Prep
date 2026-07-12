// ============================================================
// SPIRAL MATRIX — Matrix
// ============================================================
// m x n matrix diya. SAARE elements SPIRAL order me (bahar se andar, clockwise)
// ek vector me return karo.
//   [[1,2,3],[4,5,6],[7,8,9]] -> [1,2,3,6,9,8,7,4,5]
//
// ---- ARPAN KI APPROACH ----
//  4 boundaries rakho: top, bottom, left, right. while(top<=bottom && left<=right):
//  top row L->R chalo (phir top++), right col T->B (phir right--),
//  bottom row R->L agar top<=bottom (phir bottom--), left col B->T agar left<=right (phir left++).
//  boundaries andar simat-te jaate, saara spiral order ans me push hota jaata.
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

    // boundary check
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

        // yahan BOTTOM row daal rahe hai -> isliye pehle check: top <= bottom hai ya nahi?
        // (agar top bottom se aage nikal gaya to row already ho chuki -> guard na ho to DUPLICATE element daal denge)
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
