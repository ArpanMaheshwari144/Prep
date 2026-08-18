// ============================================================
// MEETING ROOMS — (LeetCode 252, Easy)   [INTERVALS #2 · sort + overlap check]
// ============================================================
// meetings[] = [[start,end],...]. Batao: kya ek insaan SAARE meetings attend kar sakta?
//   (yani koi bhi 2 meeting OVERLAP na karein.) true / false.
//
//   [[0,30],[5,10],[15,20]]  -> false   (0-30 aur 5-10 overlap)
//   [[7,10],[2,4]]           -> true    (koi overlap nahi)
//   [[1,5],[5,10]]           -> true    (touching -- ek 5 pe khatam, doosra 5 se shuru = OK)
//   [[1,5],[4,10]]           -> false
//   [[5,8]]                  -> true
//
//   idea: SORT by start -> adjacent check. curr.start < prev.end -> OVERLAP -> false.
//     ('<' na ki '<=' -- touching [5,10] & [1,5] overlap NAHI, meeting-boundary pe OK.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (Merge-Intervals ka TWIN: sort + overlap-check; merge nahi, bool)
//  same pattern jaisa Merge: SORT by start -> adjacent overlap check.
//  1. sort by start.
//  2. har next: OVERLAP (curr.start < prev.end)? -> koi 2 meeting takra rahe -> return false.
//  3. loop khatam, koi overlap nahi -> return true (saare attend ho sakte).
//  MERGE vs MEETING -- boundary FARAK (yehi crux, subtle):
//     Merge:   touching MERGE karna -> curr.start <= prev.end   (<=)
//     Meeting: touching OK (ek 5-pe-khatam, doosra 5-se-shuru = takrav nahi) -> curr.start < prev.end   (<)
//  (yahan 'ans' vector ki zaroorat NAHI thi -- bas prev.end se compare; par chalta hai.)
// ============================================================
bool canAttendMeetings(vector<vector<int>> &intervals)
{
    sort(intervals.begin(), intervals.end()); // start se sort (default first-element)
    vector<vector<int>> ans;
    ans.push_back(intervals[0]); // pehla baseline

    for (int i = 1; i < intervals.size(); i++)
    {
        // OVERLAP: curr ka start < prev ka END  ('<' -- touching OK, merge se yahi farak)
        if (intervals[i][0] < ans.back()[1])
        {
            return false; // 2 meeting takra rahe -> saare attend nahi kar sakte
        }
        else
        {
            ans.push_back(intervals[i]); // no overlap -> aage
        }
    }
    return true; // koi overlap nahi -> saare attend ho sakte
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<vector<int>> in, bool exp, int t)
{
    bool got = canAttendMeetings(in);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  got=" << (got ? "true" : "false") << "\n";
}

int main()
{
    check({{0, 30}, {5, 10}, {15, 20}}, false, 1);
    check({{7, 10}, {2, 4}}, true, 2);
    check({{1, 5}, {5, 10}}, true, 3);
    check({{1, 5}, {4, 10}}, false, 4);
    check({{5, 8}}, true, 5);
    return 0;
}
