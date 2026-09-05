// ============================================================
// WORD SEARCH   (REDO_1 — blank se, kal seekha tha)
// ============================================================
// 2D grid (chars) + string `word`. Batao: kya `word` grid me banta hai —
// adjacent cells (up/down/left/right) se, har cell EK BAAR use. (diagonal NAHI.)
// return: true / false.
//
// ---- TEST CASES ----
//   board = A B C E
//           S F C S
//           A D E E
//     "ABCCED" -> true
//     "SEE"    -> true
//     "ABCB"   -> false   (B dobara use nahi)
//   board = [[A]]   "A"  -> true
//   board = [[A]]   "AB" -> false
// ============================================================

#include <bits/stdc++.h>
using namespace std;


bool DFS(int m, int n, int i, int j, vector<vector<char>> &board, string word, int index,
         vector<vector<bool>> vis)
{
    if (i >= m || i < 0 || j >= n || j < 0 || vis[i][j] == true || board[i][j] != word[index])
        return false;

    vis[i][j] = true;
    index++;

    if (DFS(m, n, i + 1, j, board, word, index, vis) == true ||
        DFS(m, n, i - 1, j, board, word, index, vis) == true ||
        DFS(m, n, i, j - 1, board, word, index, vis) == true ||
        DFS(m, n, i, j + 1, board, word, index, vis) == true)
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
                if (DFS(m, n, i, j, board, word, 0, vis) == true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

// ---- test helper (ise mat chhed) ----
void check(bool got, bool exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<vector<char>> b1 = {{'A', 'B', 'C', 'E'},
                               {'S', 'F', 'C', 'S'},
                               {'A', 'D', 'E', 'E'}};
    check(exist(b1, "ABCCED"), true, "case1");

    vector<vector<char>> b2 = {{'A', 'B', 'C', 'E'},
                               {'S', 'F', 'C', 'S'},
                               {'A', 'D', 'E', 'E'}};
    check(exist(b2, "SEE"), true, "case2");

    vector<vector<char>> b3 = {{'A', 'B', 'C', 'E'},
                               {'S', 'F', 'C', 'S'},
                               {'A', 'D', 'E', 'E'}};
    check(exist(b3, "ABCB"), false, "case3");

    vector<vector<char>> b4 = {{'A'}};
    check(exist(b4, "A"), true, "case4");

    vector<vector<char>> b5 = {{'A'}};
    check(exist(b5, "AB"), false, "case5");
    return 0;
}
