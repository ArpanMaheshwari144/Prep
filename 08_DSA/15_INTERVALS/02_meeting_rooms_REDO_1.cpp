// ==============================================================
// MEETING ROOMS  (LC-252, Easy)  -- REDO_1 (blank, khud likh)
// --------------------------------------------------------------
// PEHLE YE (30 sec):
//   1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//   2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//   3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//   4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//   5. "X jaisa hai, bas ye badla" laga -> X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//   6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// --------------------------------------------------------------
// KYA KARNA:
//   meetings[] = [[start,end],...]. Ek insaan SAARI meetings attend kar sakta hai?
//   (koi bhi 2 meeting overlap na karein.) true / false lautao.
//   Ek meeting jis time pe khatam, doosri usi time se shuru ho sakti hai.
//
// TEST CASES (input -> expected):
//   [[0,30],[5,10],[15,20]]  -> false
//   [[7,10],[2,4]]           -> true
//   [[1,5],[5,10]]           -> true
//   [[1,5],[4,10]]           -> false
//   [[5,8]]                  -> true
//   []                       -> true
//   [[9,12],[1,3],[3,9]]     -> true
//   [[1,4],[6,8],[2,3]]      -> false
// ==============================================================

#include <bits/stdc++.h>
using namespace std;

bool canAttendMeetings(vector<vector<int>> &intervals)
{
    if (intervals.empty())
    {
        return true;
    }

    sort(begin(intervals), end(intervals));
    vector<vector<int>> ans;
    ans.push_back(intervals[0]);
    for (int i = 1; i < intervals.size(); i++)
    {
        if (intervals[i][0] < ans.back()[1])
        {
            return false;
        }
        else
        {
            ans.push_back(intervals[i]);
        }
    }
    return true;
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
    check({}, true, 6);
    check({{9, 12}, {1, 3}, {3, 9}}, true, 7);
    check({{1, 4}, {6, 8}, {2, 3}}, false, 8);
    return 0;
}
