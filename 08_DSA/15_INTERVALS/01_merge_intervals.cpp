// ============================================================
// MERGE INTERVALS — (LeetCode 56, Medium)   [INTERVALS #1 · sort + merge]
// ============================================================
// intervals[] = [[start,end],...]. saare OVERLAPPING intervals ko MERGE karo. merged list lautao.
//
//   [[1,3],[2,6],[8,10],[15,18]]  -> [[1,6],[8,10],[15,18]]   (1-3 aur 2-6 overlap -> 1-6)
//   [[1,4],[4,5]]                 -> [[1,5]]                    (touching bhi merge)
//   [[1,4],[2,3]]                 -> [[1,4]]                    (2-3 andar hai)
//   [[1,4]]                       -> [[1,4]]
//
//   idea: SORT by start -> result me first daalo -> har next: overlap (curr.start <= last.end)?
//     -> merge (last.end = max(last.end, curr.end)). warna naya push.
//   C++: sort(intervals.begin(), intervals.end()) -> default first-element (start) se sort (pair jaisa!).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (SORT by start -> ek pass, overlap merge)
//  har interval problem ka pehla step = SORT by start.  phir left-to-right:
//  1. sort(intervals) -> default FIRST-element (start) se sort (pair jaisa).
//  2. ans me PEHLA interval daalo (baseline).
//  3. har next interval [i]:
//       OVERLAP? -> intervals[i][0] (curr start) <= ans.back()[1] (last-merged ka END)
//          -> merge: ans.back()[1] = max(last end, curr end).   (start wahi, end bada wala)
//       NO overlap -> ans me naya push.
//  4. return ans.
//  ans.back() = ans ka LAST interval;  [0]=start, [1]=END. isliye ans.back()[1] = last-merged ka end.
// ============================================================
vector<vector<int>> merge(vector<vector<int>> &intervals)
{
    sort(intervals.begin(), intervals.end()); // start se sort (default first-element)
    vector<vector<int>> ans;
    ans.push_back(intervals[0]); // pehla interval baseline (result me daala)

    for (int i = 1; i < intervals.size(); i++)
    {
        // OVERLAP: curr ka start <= last-merged ka END
        if (intervals[i][0] <= ans.back()[1])
        {
            // merge -> end me bada wala (start same rehta)
            ans.back()[1] = max(ans.back()[1], intervals[i][1]);
        }
        else
        {
            ans.push_back(intervals[i]); // no overlap -> alag interval
        }
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<vector<int>> in, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = merge(in);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=[";
    for (auto &v : got)
        cout << "[" << v[0] << "," << v[1] << "]";
    cout << "]\n";
}

int main()
{
    check({{1, 3}, {2, 6}, {8, 10}, {15, 18}}, {{1, 6}, {8, 10}, {15, 18}}, 1);
    check({{1, 4}, {4, 5}}, {{1, 5}}, 2);
    check({{1, 4}, {2, 3}}, {{1, 4}}, 3);
    check({{1, 4}}, {{1, 4}}, 4);
    return 0;
}
