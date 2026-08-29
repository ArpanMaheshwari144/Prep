// ==============================================================
// COUNT SERVERS THAT COMMUNICATE  (LC-1267)  -- JP Morgan asked
// --------------------------------------------------------------
// KYA KARNA: grid me 1 = server, 0 = khaali. Do server "communicate"
//   kar sakte hain agar wo SAME ROW ya SAME COLUMN me hon.
//   Count karo kitne servers AT LEAST EK doosre server se communicate
//   kar sakte hain (yani jo bilkul akela hai apni row+col me, wo count nahi).
//
// INPUT: grid[m][n], har cell 0 ya 1.
// OUTPUT: ek int -- communicating servers ki ginti.
//
// TEST CASES (input -> expected):
//   [[1,0],[0,1]]                       -> 0   (dono akele apni row+col me)
//   [[1,0],[1,1]]                       -> 3   (teeno connected)
//   [[1,1,1]]                           -> 3   (ek row me 3 -> sab)
//   [[1],[1],[1]]                       -> 3   (ek col me 3 -> sab)
//   [[0,0],[0,0]]                       -> 0   (koi server nahi)
//   [[1,1,0,0],[0,0,1,0],[0,0,1,1]]     -> 5
//   [[1]]                               -> 0   (akela server)
//
// APPROACH (= SET MATRIX ZEROES ka cousin -- 2-marker/2-pass, MARK ki jagah COUNT):
//   rowCount[i] = us row me kitne server · colCount[j] = us col me kitne server.
//   pass-1: har server (grid[i][j]==1) pe -> rowCount[i]++, colCount[j]++.
//   pass-2: har server pe -> agar rowCount[i]>1 YA colCount[j]>1 -> communicate (ans++).
//           (dono ==1 = wo apni row+col me AKELA -> skip.)
//   O(m*n) time, O(m+n) space.
// ==============================================================

#include <bits/stdc++.h>
using namespace std;

int countServers(vector<vector<int>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();

    int ans = 0;

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

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1 && (rowCount[i] > 1 || colCount[j] > 1))
            {
                ans++;
            }
        }
    }
    return ans;
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
    return 0;
}
