// ============================================================
// WORD LADDER (LC 127) — BFS shortest-path  |  REDO_1 (blank retrieval, 13-Sep)
// Kal solve kiya (BFS + in-place modify + erase-as-visited). Aaj KHAALI PAGE se dobara.
// Sheet/kal-ka-code mat dekh; atko to tabhi peek.
// ============================================================
//
// beginWord -> endWord, har step EK letter badlo, har beech ka word wordList me ho.
// Return: shortest sequence me kitne WORD (begin+end included). Rasta nahi -> 0.
//   "hit" -> "cog", [hot,dot,dog,lot,log,cog]  ->  hit hot dot dog cog = 5
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

int ladderLength(string beginWord, string endWord, vector<string> &wordList)
{
    unordered_set<string> st(begin(wordList), end(wordList));
    queue<pair<string, int>> q;
    q.push({beginWord, 1});
    st.erase(beginWord);
    while (!q.empty())
    {
        string word = q.front().first;
        int steps = q.front().second;
        q.pop();
        if (word == endWord)
        {
            return steps;
        }

        for (int i = 0; i < word.size(); i++)
        {
            char original = word[i];
            for (char ch = 'a'; ch <= 'z'; ch++)
            {
                word[i] = ch;
                if (st.find(word) != st.end())
                {
                    st.erase(word);
                    q.push({word, steps + 1});
                }
            }
            word[i] = original;
        }
    }
    return 0;
}

// ------------------------------------------------------------
int main()
{
    struct T
    {
        string begin, end;
        vector<string> list;
        int expected;
    };
    vector<T> tests = {
        {"hit", "cog", {"hot", "dot", "dog", "lot", "log", "cog"}, 5},
        {"hit", "cog", {"hot", "dot", "dog", "lot", "log"}, 0},
        {"hot", "dog", {"hot", "dog", "dot"}, 3},
        {"a", "c", {"a", "b", "c"}, 2},
    };
    int pass = 0;
    for (int i = 0; i < (int)tests.size(); i++)
    {
        vector<string> lst = tests[i].list;
        int got = ladderLength(tests[i].begin, tests[i].end, lst);
        bool ok = (got == tests[i].expected);
        cout << "Test " << (i + 1) << ": got=" << got
             << " expected=" << tests[i].expected
             << "  -> " << (ok ? "PASS" : "FAIL") << "\n";
        pass += ok;
    }
    cout << "\n"
         << pass << "/" << tests.size() << " passed\n";
    return 0;
}
