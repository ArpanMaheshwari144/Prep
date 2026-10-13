// ═══════════════════════════════════════════════════════════════════════
//  MERGE INTERVALS — (LeetCode 56)   (REDO_1)
//  intervals[] = [[start,end], ...]  (kisi bhi order me aa sakte)
//  Saare OVERLAPPING intervals ko MERGE karo, merged list lautao (start ke order me).
//  Chhoote hue bhi (ek ka end == doosre ka start) merge hote hain.
//
//  TEST-CASES (input -> expected):
//   1) [[1,3],[2,6],[8,10],[15,18]]      -> [[1,6],[8,10],[15,18]]
//   2) [[1,4],[4,5]]                     -> [[1,5]]            touching
//   3) [[1,4],[2,3]]                     -> [[1,4]]            andar wala
//   4) [[1,4]]                           -> [[1,4]]            akela
//   5) [[8,10],[1,3],[2,6]]              -> [[1,6],[8,10]]     unsorted input
//   6) [[1,4],[0,4]]                     -> [[0,4]]
//   7) [[1,4],[5,6]]                     -> [[1,4],[5,6]]      paas hai par chhoote nahi
//   8) [[1,10],[2,3],[4,5],[6,7]]        -> [[1,10]]           sab ek ke andar
//   9) [[2,3],[4,5],[6,7],[8,9],[1,10]]  -> [[1,10]]           bada wala aakhir me
//  10) [[1,4],[2,5],[3,6]]               -> [[1,6]]            chain
//  11) [[5,5],[5,5]]                     -> [[5,5]]            same point
//  12) [[0,0],[1,1]]                     -> [[0,0],[1,1]]
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> merge(vector<vector<int>> &intervals)
{
    sort(begin(intervals), end(intervals));
    vector<vector<int>> ans;
    ans.push_back(intervals[0]);
    for (int i = 1; i < intervals.size(); i++)
    {
        if (ans.back()[1] >= intervals[i][0])
        {
            ans.back()[1] = max(ans.back()[1], intervals[i][1]);
        }
        else
        {
            ans.push_back(intervals[i]);
        }
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
string show(const vector<vector<int>> &v)
{
    string s = "[";
    for (auto &x : v)
        s += "[" + to_string(x[0]) + "," + to_string(x[1]) + "]";
    return s + "]";
}

void check(vector<vector<int>> in, vector<vector<int>> exp, string name)
{
    vector<vector<int>> got = merge(in);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << show(got) << " exp=" << show(exp) << "\n";
}

int main()
{
    check({{1, 3}, {2, 6}, {8, 10}, {15, 18}}, {{1, 6}, {8, 10}, {15, 18}}, "t1 basic");
    check({{1, 4}, {4, 5}}, {{1, 5}}, "t2 touching");
    check({{1, 4}, {2, 3}}, {{1, 4}}, "t3 inside");
    check({{1, 4}}, {{1, 4}}, "t4 single");
    check({{8, 10}, {1, 3}, {2, 6}}, {{1, 6}, {8, 10}}, "t5 unsorted");
    check({{1, 4}, {0, 4}}, {{0, 4}}, "t6 earlier-start");
    check({{1, 4}, {5, 6}}, {{1, 4}, {5, 6}}, "t7 gap");
    check({{1, 10}, {2, 3}, {4, 5}, {6, 7}}, {{1, 10}}, "t8 all-inside");
    check({{2, 3}, {4, 5}, {6, 7}, {8, 9}, {1, 10}}, {{1, 10}}, "t9 big-last");
    check({{1, 4}, {2, 5}, {3, 6}}, {{1, 6}}, "t10 chain");
    check({{5, 5}, {5, 5}}, {{5, 5}}, "t11 same-point");
    check({{0, 0}, {1, 1}}, {{0, 0}, {1, 1}}, "t12 points-gap");
    return 0;
}
