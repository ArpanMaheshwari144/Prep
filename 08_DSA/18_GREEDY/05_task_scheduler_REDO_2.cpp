// ============================================================
// TASK SCHEDULER (LC 621) — GREEDY  |  REDO_2 (21-Sep, 10 din baad)
// REDO_1 11-Sep ko hua tha. Aaj KHAALI PAGE se dobara.
// Notes/sheet/purani file mat kholna. Atko to TAB peek karna.
// ============================================================
//
// PROBLEM:
//   tasks = letters (A-Z), n = cooldown.
//   SAME task dobara chalane se pehle n ka gap chahiye --
//   beech me doosra task chale, ya CPU idle baithe.
//   Minimum kul time batao (idle bhi ginti me).
//
//   [A,A,A,B,B,B], n=2   ->   A B idle A B idle A B   ->   8
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

int leastInterval(vector<char> &tasks, int n)
{
    vector<int> mp(26);
    for (int i = 0; i < tasks.size(); i++)
    {
        mp[tasks[i] - 'A']++;
    }

    sort(begin(mp), end(mp));

    int maxFreq = mp[25];
    int gadde = maxFreq - 1;
    int idleSlotes = n * gadde;
    for (int i = 24; i >= 0; i--)
    {
        idleSlotes -= min(mp[i], gadde);
    }

    if (idleSlotes > 0)
    {
        return tasks.size() + idleSlotes;
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
        string kyun;
    };

    vector<T> tests = {
        {{'A', 'A', 'A', 'B', 'B', 'B'}, 2, 8, "classic — A B idle A B idle A B"},
        {{'A', 'A', 'A', 'B', 'B', 'B'}, 0, 6, "n=0 — koi gap nahi, sab back to back"},
        {{'A', 'A', 'A', 'A', 'A', 'A'}, 2, 16, "ek hi task — poora idle se bhara"},
        {{'A'}, 5, 1, "ek hi task, cooldown bekaar"},
        {{'A', 'B', 'C', 'D', 'E', 'F'}, 2, 6, "sab alag — idle ki zaroorat hi nahi"},
        {{'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C', 'C', 'D', 'D', 'E'}, 2, 12, "★ gadde POORE bhar gaye — jawab tasks.size() hai, formula se KAM"},
        {{'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D'}, 1, 8, "n=1, sab jodi me"},
        {{'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C'}, 2, 8, "★ DO task ki freq BARABAR (A aur B dono 3) — aakhri row me dono aate"},
        {{'A', 'A', 'A', 'A', 'B', 'C', 'D', 'E', 'F', 'G'}, 2, 10, "maxFreq 4 par bahut saare alag task — gadde bhar gaye"},
        {{'A', 'B'}, 100, 2, "bada cooldown par same task hai hi nahi"},
    };

    int pass = 0;
    for (int i = 0; i < (int)tests.size(); i++)
    {
        vector<char> copy = tests[i].tasks;
        int got = leastInterval(copy, tests[i].n);
        bool ok = (got == tests[i].expected);
        pass += ok;
        cout << (ok ? "PASS  " : "FAIL  ")
             << "n=" << tests[i].n
             << "  expected=" << tests[i].expected
             << "  mila=" << got
             << "   [" << tests[i].kyun << "]\n";
    }
    cout << "\n"
         << pass << " / " << tests.size() << "\n";
    return 0;
}
