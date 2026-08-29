// ══════════════════════════════════════════════════════════════════════
// ROTTING ORANGES  (multi-source BFS)  — REDO
//
// KYA KARNA:
//   grid me har cell = 0 (khaali) | 1 (fresh orange) | 2 (rotten orange).
//   Har MINUTE me: har rotten (2) apne 4-direction (up/down/left/right) ke
//   FRESH (1) padosi ko rotten kar deta.
//   Return: kitne MINUTE me saare fresh rotten ho jaate.
//           Agar koi fresh kabhi rotten na ho sake -> return -1.
//           Koi fresh hai hi nahi -> return 0.
//
// INPUT: grid = vector<vector<int>>  (R x C)
//
// TEST CASES (input -> expected):
//   1) [[2,1,1],[1,1,0],[0,1,1]]          -> 4
//   2) [[2,1,1],[0,1,1],[1,0,1]]          -> -1   (bottom-left fresh phansi hui)
//   3) [[0,2]]                            -> 0    (koi fresh nahi)
//   4) [[1]]                              -> -1   (fresh hai, koi rotten nahi)
//   5) [[0]]                              -> 0    (sirf khaali)
//   6) [[2,2],[1,1]]                      -> 1
// ══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int orangesRotting(vector<vector<int>> &grid)
{
    queue<pair<int, int>> q;
    int m = grid.size();
    int n = grid[0].size();
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 2)
            {
                q.push({i, j});
            }
        }
    }

    vector<vector<int>> dirs = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};
    int mins = 0;
    while (!q.empty())
    {
        int sz = q.size();
        while (sz--)
        {
            auto &it = q.front();
            q.pop();

            int row = it.first;
            int col = it.second;

            for (auto &it : dirs)
            {
                int nR = it[0] + row;
                int nC = it[1] + col;
                if (nR >= 0 && nR < m && nC >= 0 && nC < n && grid[nR][nC] == 1)
                {
                    grid[nR][nC] = 2;
                    q.push({nR, nC});
                }
            }
        }
        mins++;
    }

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j] == 1)
            {
                return -1;
            }
        }
    }
    return mins == 0 ? mins : mins - 1;
}

// ---- test harness (isko haath mat lagana) ----
void check(vector<vector<int>> grid, int expected, int tc)
{
    int got = orangesRotting(grid);
    cout << "TC" << tc << ": got=" << got << " expected=" << expected
         << "  " << (got == expected ? "PASS" : "FAIL") << "\n";
}

int main()
{
    check({{2, 1, 1}, {1, 1, 0}, {0, 1, 1}}, 4, 1);
    check({{2, 1, 1}, {0, 1, 1}, {1, 0, 1}}, -1, 2);
    check({{0, 2}}, 0, 3);
    check({{1}}, -1, 4);
    check({{0}}, 0, 5);
    check({{2, 2}, {1, 1}}, 1, 6);
    return 0;
}
