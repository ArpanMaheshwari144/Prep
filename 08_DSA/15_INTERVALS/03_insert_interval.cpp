// ============================================================
// INSERT INTERVAL — (LeetCode 57, Medium)   [INTERVALS #3 · 3-phase merge]
// ============================================================
// intervals[] pehle se SORTED + NON-overlapping. ek newInterval diya -> usko insert karo
// (overlap ho to merge). result (sorted, non-overlapping) lautao.
//
//   [[1,3],[6,9]],  new=[2,5]                      -> [[1,5],[6,9]]
//   [[1,2],[3,5],[6,7],[8,10],[12,16]], new=[4,8]  -> [[1,2],[3,10],[12,16]]
//   [],  new=[5,7]                                 -> [[5,7]]
//   [[1,5]], new=[2,3]                             -> [[1,5]]
//   [[1,5]], new=[6,8]                             -> [[1,5],[6,8]]
//
//   ★ idea (3 PHASE -- already sorted, isliye sort nahi):
//     1. jo intervals new se PEHLE khatam (interval.end < new.start) -> seedha result me.
//     2. jo OVERLAP (interval.start <= new.end) -> merge: new = [min(starts), max(ends)].
//     3. merged new push -> phir baaki (jo new ke baad) result me.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- APPROACH ----  (already SORTED -> 3 phase, no sort needed)
//  ★ intervals pehle se sorted+non-overlapping. isliye ek left-to-right pass:
//  1. jo new se PEHLE khatam (interval.end < new.start) -> seedha ans me (before-wale).
//  2. jo OVERLAP -> new ko GROW karo (nigal lo): new[0]=min, new[1]=max. push abhi nahi.
//  3. jo new ke AAGE shuru (interval.start > new.end) -> BREAK (sorted hai, aage sab non-overlap).
//  4. loop-ke-baad: bada-hua new push -> phir 'count' se aage baaki intervals push.
//  ★★ crux -- new = "growing box": har overlap use stretch karta; sab overlap khatam pe ek baar push.
//  ★ break-optimization: teesri phase me interval.start > new.end -> aage sab aur door -> ruk jao.
//     'count' = kahan ruke -> doosra loop wahin se baaki push karta.  (O(n))
// ============================================================
// ─── YAHAN SOLVE KAR ────────────────────────────────────────
vector<vector<int>> insert(vector<vector<int>> &intervals, vector<int> &newInterval)
{
    vector<vector<int>> ans;
    int count = 0;
    for (int i = 0; i < intervals.size(); i++)
    {
        if (intervals[i][1] < newInterval[0])
        {
            ans.push_back(intervals[i]);
        }
        // iska matba ukos aage se ab koi hmara candidate nahi loop ko rok kpoi mtba nahi aage jaane ka
        else if (intervals[i][0] > newInterval[1])
        {
            break;
        }
        else
        {
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
        }
        count++;
    }
    ans.push_back(newInterval);

    for (int i = count; i < intervals.size(); i++)
    {
        ans.push_back(intervals[i]);
    }
    return ans;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<vector<int>> in, vector<int> ni, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = insert(in, ni);
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=[";
    for (auto &v : got)
        cout << "[" << v[0] << "," << v[1] << "]";
    cout << "]\n";
}

int main()
{
    check({{1, 3}, {6, 9}}, {2, 5}, {{1, 5}, {6, 9}}, 1);
    check({{1, 2}, {3, 5}, {6, 7}, {8, 10}, {12, 16}}, {4, 8}, {{1, 2}, {3, 10}, {12, 16}}, 2);
    check({}, {5, 7}, {{5, 7}}, 3);
    check({{1, 5}}, {2, 3}, {{1, 5}}, 4);
    check({{1, 5}}, {6, 8}, {{1, 5}, {6, 8}}, 5);
    return 0;
}
