// ============================================================
// ROTTING ORANGES — (LeetCode 994, Medium)   [GRAPHS #4 · grid MULTI-SOURCE BFS + levels]
// ============================================================
// grid me har cell:  0 = khali · 1 = FRESH orange · 2 = ROTTEN orange.
// har MINUTE me: koi bhi rotten(2) apne 4-direction (up/down/left/right) FRESH(1) padosi ko rotten kar deta.
// Batao kitne MINUTE me SAARE fresh rotten ho jaayenge. Agar koi fresh KABHI rotten na ho sake -> return -1.
//
//        2 1 1            2 1 1
//        1 1 0            0 1 1
//        0 1 1            1 0 1
//     -> 4 minute      -> -1  (bottom-left ka fresh(1) tak koi rotten nahi pahunch sakta)
//
//   TWIST (naya): ye ISLANDS jaisa grid+BFS hai, PAR --
//     - MULTI-SOURCE: ek source se nahi, SAARE rotten(2) EK saath spread karte -> shuru me sab rotten queue me daalo.
//     - MINUTES = LEVELS: har "minute" = BFS ka ek LEVEL (Tree level-order ka sz-snapshot yaad kar). level ginte jao.
//     - end me koi FRESH bacha? -> -1.
//
//   Example:
//     [[2,1,1],[1,1,0],[0,1,1]]  -> 4
//     [[2,1,1],[0,1,1],[1,0,1]]  -> -1
//     [[0,2]]                     -> 0   (koi fresh hai hi nahi)
// ============================================================
// ---- APPROACH ----  (grid MULTI-SOURCE BFS + level-count)
//  idea: saare rotten EK saath phailte (multi-source). har MINUTE = ek BFS LEVEL. end pe fresh bacha -> -1.
//  init : saare rotten(2) ka (i,j) queue<pair> me -> ye level-0 (multi-source start).
//  dirs : {{1,0},{-1,0},{0,1},{0,-1}} -> 4 padosi (grid ka classic neighbour-trick, bahut jagah use).
//  loop : level-order (sz-snapshot) -> pop (row,col) -> har dir se (nr,nc):
//           in-bounds && grid[nr][nc]==1? -> rot (=2) + push.    har level ke baad mins++.
//  end  : grid me koi 1 bacha? -> return -1.  warna return mins-1.
//  crux: MULTI-SOURCE (saare rotten pehle) + minutes = LEVELS; mins-1 (last level pe kuch rot nahi -> ek extra).
//  neighbour-CONDITION (in-bounds && grid==1) HAR grid-Q me lagti -- '==1' = sirf 1-wale ko rot, SAB ko nahi (question-hisaab badalti). [tera flag]
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int orangesRotting(vector<vector<int>> &grid)
{
    int m = grid.size();
    int n = grid[0].size();

    // MULTI-SOURCE: saare rotten(2) ka (i,j) queue me -> ye level-0 (sab ek saath phailenge)
    queue<pair<int, int>> q;
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

    vector<vector<int>> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; // 4 padosi (classic grid-trick)
    int mins = 0;
    while (!q.empty()) // har ghumaav = EK minute (level)
    {
        int sz = q.size(); // SNAPSHOT: is level ke saare rotten (level-order jaisa)
        for (int i = 0; i < sz; i++)
        {
            int row = q.front().first;
            int col = q.front().second;
            q.pop();

            for (auto &it : dirs) // 4 direction padosi
            {
                int newRow = row + it[0];
                int newCol = col + it[1];
                // in-bounds AUR fresh(==1) -> tabhi rot. (sirf 1-wale ko 2 karo, sab ko nahi)
                if (newRow >= 0 && newRow < m && newCol >= 0 && newCol < n && grid[newRow][newCol] == 1)
                {
                    grid[newRow][newCol] = 2; // rot
                    q.push({newRow, newCol});
                }
            }
        }
        mins++; // ek level poora -> ek minute
    }

    // end: koi fresh(1) bacha? -> kabhi rot nahi hua -> -1
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
    return mins - 1; // mins-1: last level pe kuch rot nahi hota -> ek extra gina gaya
}

int main()
{
    vector<vector<int>> g1 = {{2, 1, 1}, {1, 1, 0}, {0, 1, 1}};
    vector<vector<int>> g2 = {{2, 1, 1}, {0, 1, 1}, {1, 0, 1}};
    vector<vector<int>> g3 = {{0, 2}};

    // ── VISUAL (g1; 2=rotten 1=fresh 0=empty; rotten 4-dir me failta har min) ──
    //   2 1 1
    //   1 1 0     -> saare fresh rot hone me 4 min lagte
    //   0 1 1

    cout << orangesRotting(g1) << " (expected 4)\n";
    cout << orangesRotting(g2) << " (expected -1)\n";
    cout << orangesRotting(g3) << " (expected 0)\n";
    return 0;
}
