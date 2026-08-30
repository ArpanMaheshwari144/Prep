// NON-OVERLAPPING INTERVALS (LC-435) — REDO_1  [GREEDY]
// -----------------------------------------------------------------------------
// KYA KARNA: intervals [start,end] di hain. MINIMUM kitne intervals HATANE padenge
//            taaki baaki saare NON-OVERLAPPING ho jaayein? (us count ko return karo)
//
// INPUT: vector<vector<int>> intervals, har ek = {start, end}
//
// GREEDY
//
// TEST-CASES (input -> expected = min removals):
//   [[1,2],[2,3],[3,4],[1,3]]      -> 1     ([1,3] hatao)
//   [[1,2],[1,2],[1,2]]            -> 2     (do duplicate hatao)
//   [[1,2],[2,3]]                  -> 0     (touching = overlap NAHI)
//   [[1,10],[2,3],[4,5],[6,7]]     -> 1     ([1,10] hatao -> 3 bache)
//   [[1,100],[11,22],[1,11],[2,12]]-> 2

#include <bits/stdc++.h>
using namespace std;

int eraseOverlapIntervals(vector<vector<int>> intervals)
{
    sort(begin(intervals), end(intervals));
    int start = 0, end = 1, count = 0;
    while (end < intervals.size())
    {
        if (intervals[start][1] <= intervals[end][0])
        {
            start = end;
            end++;
        }
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

void check(vector<vector<int>> in, int exp)
{
    int got = eraseOverlapIntervals(in);
    cout << (got == exp ? "PASS" : "FAIL")
         << "  got=" << got << "  exp=" << exp << "\n";
}

int main()
{
    check({{1, 2}, {2, 3}, {3, 4}, {1, 3}}, 1);
    check({{1, 2}, {1, 2}, {1, 2}}, 2);
    check({{1, 2}, {2, 3}}, 0);
    check({{1, 10}, {2, 3}, {4, 5}, {6, 7}}, 1);
    check({{1, 100}, {11, 22}, {1, 11}, {2, 12}}, 2);
    return 0;
}
