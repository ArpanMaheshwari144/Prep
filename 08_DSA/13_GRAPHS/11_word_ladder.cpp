// ============================================================
// WORD LADDER (LC 127) — BFS shortest-path (unweighted)
// ============================================================
//
// beginWord -> endWord, har step me EK letter badlo, har beech ka word
// wordList me ho. Return: shortest sequence me kitne WORD (begin+end included).
// Rasta nahi -> 0.
//
// Example: begin="hit", end="cog", list=[hot,dot,dog,lot,log,cog]
//          hit -> hot -> dot -> dog -> cog  = 5 words  -> return 5
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

int ladderLength(string beginWord, string endWord, vector<string> &wordList)
{
    queue<pair<string, int>> q;
    unordered_set<string> st(begin(wordList), end(wordList));
    q.push({beginWord, 1});
    st.erase(beginWord);
    while (!q.empty())
    {
        string word = q.front().first;
        int level = q.front().second;
        q.pop();

        if (word == endWord)
        {
            return level;
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
                    q.push({word, level + 1});
                }
            }
            word[i] = original;
        }
    }
    return 0;
}

// ------------------------------------------------------------
// TEST HARNESS
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
        {"hit", "cog", {"hot", "dot", "dog", "lot", "log", "cog"}, 5}, // classic path
        {"hit", "cog", {"hot", "dot", "dog", "lot", "log"}, 0},        // endWord list me nahi -> 0
        {"hot", "dog", {"hot", "dog", "dot"}, 3},                      // hot->dot->dog
        {"a", "c", {"a", "b", "c"}, 2},                                // a->c direct (1 change)
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
