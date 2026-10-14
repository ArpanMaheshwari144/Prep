// ============================================================
// WORD SEARCH — REDO_1   [LC-79, BACKTRACKING #5]
// ============================================================
// 2D grid (chars) + string `word`. Batao: kya `word` grid me banta hai —
// adjacent cells (up/down/left/right) se, har cell EK BAAR use. (diagonal NAHI.)
// return: true / false.
//
//   board = A B C E
//           S F C S
//           A D E E
//     "ABCCED" -> true   ·   "SEE" -> true   ·   "ABCB" -> false (B dobara nahi)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool solve(vector<vector<char>> &board, string word, int index, int i, int j, int m, int n, vector<vector<bool>> &vis)
{
    if (i < 0 || i >= m || j < 0 || j >= n || vis[i][j] || board[i][j] != word[index])
    {
        return false;
    }

    vis[i][j] = true;
    index++;
    if (solve(board, word, index, i + 1, j, m, n, vis) ||
        solve(board, word, index, i, j + 1, m, n, vis) ||
        solve(board, word, index, i - 1, j, m, n, vis) ||
        solve(board, word, index, i, j - 1, m, n, vis))
    {
        return true;
    }

    vis[i][j] = false;
    return index == word.size() ? true : false;
}

bool exist(vector<vector<char>> &board, string word)
{
    int m = board.size();
    int n = board[0].size();
    vector<vector<bool>> vis(m + 1, vector<bool>(n + 1, false));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (board[i][j] == word[0])
            {
                if (solve(board, word, 0, i, j, m, n, vis) == true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<vector<char>> board, string word, bool exp, int t)
{
    bool got = exist(board, word);
    cout << "test " << t << " (" << word << ") -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<vector<char>> b = {{'A', 'B', 'C', 'E'},
                              {'S', 'F', 'C', 'S'},
                              {'A', 'D', 'E', 'E'}};
    check(b, "ABCCED", true, 1);
    check(b, "SEE", true, 2);
    check(b, "ABCB", false, 3); // B dobara use nahi

    check({{'A'}}, "A", true, 4);
    check({{'A'}}, "AB", false, 5);

    check({{'A', 'B', 'C', 'E'},
           {'S', 'F', 'E', 'S'},
           {'A', 'D', 'E', 'E'}},
          "ABCESEEEFS", true, 6); // ghoom ke wapas aata raasta

    check({{'a', 'a'}}, "aaa", false, 7); // 2 cell, 3 letter
    check({{'a', 'b'}, {'c', 'd'}}, "abdc", true, 8);
    check({{'a', 'b'}, {'c', 'd'}}, "abcd", false, 9); // b -> c diagonal
    check({{'A', 'B', 'C', 'D'}}, "DCBA", true, 10);   // ulta
    check({{'C', 'A', 'A'},
           {'A', 'A', 'A'},
           {'B', 'C', 'D'}},
          "AAB", true, 11);
    check({{'A', 'B'}}, "ABA", false, 12); // A dobara nahi
    return 0;
}
