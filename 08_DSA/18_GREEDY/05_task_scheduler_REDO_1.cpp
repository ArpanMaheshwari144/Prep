// ============================================================
// TASK SCHEDULER (LC 621) — GREEDY  |  REDO_1 (blank retrieval, 12-Sep)
// Kal solve kiya (idle-slots). Aaj KHAALI PAGE se dobara — memory se derive.
// Notes/sheet mat dekh; atko to tabhi peek.
// ============================================================
//
// tasks = letters, n = cooldown. SAME task dobara se pehle n gap chahiye
// (beech me doosra task ya IDLE). Minimum total time (idle included)?
//   [A,A,A,B,B,B], n=2  ->  A B idle A B idle A B  ->  8
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

int leastInterval(vector<char> &tasks, int n)
{
    vector<int> mp(26, 0);
    for (int i = 0; i < tasks.size(); i++)
    {
        mp[tasks[i] - 'A']++;
    }

    sort(begin(mp), end(mp));

    int maxFreq = mp[25];
    int gadde = maxFreq - 1;
    int totalGadde = n * gadde;
    for (int i = 24; i >= 0; i--)
    {
        totalGadde -= min(mp[i], gadde);
    }

    if (totalGadde > 0)
    {
        return tasks.size() + totalGadde;
    }
    return tasks.size();
}

// ------------------------------------------------------------
int main()
{
    struct T
    {
        vector<char> tasks;
        int n;
        int expected;
    };
    vector<T> tests = {
        {{'A', 'A', 'A', 'B', 'B', 'B'}, 2, 8},
        {{'A', 'A', 'A', 'B', 'B', 'B'}, 0, 6},
        {{'A'}, 2, 1},
        {{'A', 'B', 'C', 'D', 'E', 'F'}, 2, 6},
        {{'A', 'A', 'A', 'A', 'A', 'A', 'B', 'C', 'D', 'E', 'F', 'G'}, 2, 16},
        {{'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C', 'C', 'D', 'D', 'E', 'E'}, 3, 13},
    };
    int pass = 0;
    for (int i = 0; i < (int)tests.size(); i++)
    {
        vector<char> t = tests[i].tasks;
        int got = leastInterval(t, tests[i].n);
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
