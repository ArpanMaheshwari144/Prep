// ==============================================================
// COUNT SERVERS THAT COMMUNICATE  (LC-1267)  -- REDO_2 (khaali page)
// --------------------------------------------------------------
// KYA KARNA: grid me 1=server, 0=khaali. Server "communicate" karta hai
//   agar uski SAME ROW ya SAME COLUMN me koi DOOSRA server ho.
//   Count karo kitne servers communicate karte hain (jo apni row+col
//   dono me akela -> count nahi).
//
// TEST CASES (input -> expected):
//   [[1,0],[0,1]]                       -> 0
//   [[1,0],[1,1]]                       -> 3
//   [[1,1,1]]                           -> 3
//   [[1],[1],[1]]                       -> 3
//   [[0,0],[0,0]]                       -> 0
//   [[1,1,0,0],[0,0,1,0],[0,0,1,1]]     -> 5
//   [[1]]                               -> 0
//   [[0]]                               -> 0
//   [[1,0,0],[0,0,1],[0,1,0]]           -> 0
//   [[1,0,1],[0,1,0],[1,0,1]]           -> 4
//   [[0,0,0,1],[1,0,0,0],[0,0,0,1]]     -> 2
//   [[1,1],[1,1]]                       -> 4
// ==============================================================

#include <bits/stdc++.h>
using namespace std;

int countServers(vector<vector<int>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();
    vector<int> rowCount(m, 0);
    vector<int> colCount(n, 0);

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1)
            {
                rowCount[i]++;
                colCount[j]++;
            }
        }
    }

    int count = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1 && (rowCount[i] > 1 || colCount[j] > 1))
            {
                count++;
            }
        }
    }
    return count;
}

// -------------------- test harness --------------------
void check(vector<vector<int>> grid, int exp)
{
    int got = countServers(grid);
    cout << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    check({{1, 0}, {0, 1}}, 0);
    check({{1, 0}, {1, 1}}, 3);
    check({{1, 1, 1}}, 3);
    check({{1}, {1}, {1}}, 3);
    check({{0, 0}, {0, 0}}, 0);
    check({{1, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}}, 5);
    check({{1}}, 0);
    check({{0}}, 0);
    check({{1, 0, 0}, {0, 0, 1}, {0, 1, 0}}, 0);
    check({{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}, 4);
    check({{0, 0, 0, 1}, {1, 0, 0, 0}, {0, 0, 0, 1}}, 2);
    check({{1, 1}, {1, 1}}, 4);
    return 0;
}
