// ============================================================
// TASK SCHEDULER  (LC 621)  — GREEDY (most-frequent frame)
// ============================================================
//
// tasks = CPU tasks (letters). n = cooldown.
//   - har task 1 unit leta.
//   - SAME task dobara chalane se pehle kam-se-kam n units gap chahiye
//     (beech me doosra task ya IDLE ho sakta).
// Return: minimum total time (units, idle included) me saare tasks complete.
//
// Example: tasks=[A,A,A,B,B,B], n=2  ->  A B idle A B idle A B  ->  8
//
// Signature (LeetCode):
//     int leastInterval(vector<char>& tasks, int n)
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

int leastInterval(vector<char> &tasks, int n)
{
    if (n == 0)
    {
        return tasks.size();
    }

    vector<int> mp(26, 0);
    for (auto &it : tasks)
    {
        mp[it - 'A']++;
    }

    sort(begin(mp), end(mp));

    int maxFreq = mp[25];
    int gadde = maxFreq - 1;
    int idleSlots = n * gadde;

    for (int i = 24; i >= 0; i--)
    {
        idleSlots -= min(mp[i], gadde);
    }

    if (idleSlots > 0)
    {
        return tasks.size() + idleSlots;
    }

    return tasks.size();
}

// ------------------------------------------------------------
// TEST HARNESS  (chala ke sab PASS aane chahiye)
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
        {{'A', 'A', 'A', 'B', 'B', 'B'}, 2, 8},                                     // classic — idle chahiye
        {{'A', 'A', 'A', 'B', 'B', 'B'}, 0, 6},                                     // n=0 -> koi cooldown nahi -> seedha length
        {{'A'}, 2, 1},                                                              // single task
        {{'A', 'B', 'C', 'D', 'E', 'F'}, 2, 6},                                     // sab distinct -> koi idle nahi -> length
        {{'A', 'A', 'A', 'A', 'A', 'A', 'B', 'C', 'D', 'E', 'F', 'G'}, 2, 16},      // ek task bahut dominant
        {{'A', 'A', 'A', 'B', 'B', 'B', 'C', 'C', 'C', 'D', 'D', 'E', 'E'}, 3, 13}, // enough tasks -> length WINS (edge)
    };

    int pass = 0;
    for (int i = 0; i < (int)tests.size(); i++)
    {
        vector<char> t = tests[i].tasks; // copy (fn ref leta hai)
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
