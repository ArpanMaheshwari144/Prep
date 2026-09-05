// ============================================================
// NON-OVERLAPPING INTERVALS               (LC-435 — GREEDY interval scheduling)
// ============================================================
// intervals[] diya (har interval = [start, end]). MINIMUM kitne intervals
// HATANE padenge taaki baaki sab NON-OVERLAPPING ho jaayein? count return karo.
// (do interval overlap: ek ka start doosre ke end se PEHLE aa jaaye.)
//
//   [[1,2],[2,3],[3,4],[1,3]] -> 1   ([1,3] hatao -> baaki non-overlap)
//
// ---- TEST CASES (intervals -> expected removals) ----
//   [[1,2],[2,3],[3,4],[1,3]]           -> 1
//   [[1,2],[1,2],[1,2]]                 -> 2
//   [[1,2],[2,3]]                       -> 0
//   [[1,100],[11,22],[1,11],[2,12]]     -> 2
//   [[1,2]]                             -> 0
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int eraseOverlapIntervals(vector<vector<int>> &intervals)
{
    sort(begin(intervals), end(intervals));
    int start = 0, end = 1, count = 0;
    while (end < intervals.size())
    {
        // no overlap
        if (intervals[start][1] <= intervals[end][0])
        {
            start = end;
            end++;
        }

        // overlap
        else
        {
            count++;
            if (intervals[start][1] > intervals[end][1])
            {
                start = end;
            }
            end++;
        }
    }
    return count;
}

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    vector<vector<int>> a1 = {{1, 2}, {2, 3}, {3, 4}, {1, 3}};
    check(eraseOverlapIntervals(a1), 1, "case1");

    vector<vector<int>> a2 = {{1, 2}, {1, 2}, {1, 2}};
    check(eraseOverlapIntervals(a2), 2, "case2");

    vector<vector<int>> a3 = {{1, 2}, {2, 3}};
    check(eraseOverlapIntervals(a3), 0, "case3");

    vector<vector<int>> a4 = {{1, 100}, {11, 22}, {1, 11}, {2, 12}};
    check(eraseOverlapIntervals(a4), 2, "case4");

    vector<vector<int>> a5 = {{1, 2}};
    check(eraseOverlapIntervals(a5), 0, "case5");

    vector<vector<int>> a6 = {{1, 100}, {2, 3}, {3, 4}};
    check(eraseOverlapIntervals(a6), 1, "case6");

    return 0;
}
