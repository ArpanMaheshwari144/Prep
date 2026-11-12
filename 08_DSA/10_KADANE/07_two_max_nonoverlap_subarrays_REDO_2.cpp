// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// TWO MAX-SUM NON-OVERLAPPING SUBARRAYS   (Google-tag)   (REDO_2 — blank, 13-Nov)
// ============================================================
// 2 subarray: CONTIGUOUS, NON-EMPTY, NON-OVERLAPPING. dono ke sum ka JOD max. wo max lautao.
//
// Tests (neeche main me, sab PASS aane chahiye):
//   [2,1,-99,3,3]     -> 9
//   [1,2,-7,8,6,-4]   -> 17
//   [3,3,-100,6,6]    -> 18
//   [1,2,3,4]         -> 10
//   [5,-2,5]          -> 10
//   [-5,10,10]        -> 20
//   [-1,-2]           -> -3     (sab negative, phir bhi dono non-empty)
//   [-3,-1,-2]        -> -3
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int maxTwoNonOverlap(vector<int> &nums)
{
}

int main()
{
    vector<pair<vector<int>, int>> tests = {
        {{2, 1, -99, 3, 3}, 9},
        {{1, 2, -7, 8, 6, -4}, 17},
        {{3, 3, -100, 6, 6}, 18},
        {{1, 2, 3, 4}, 10},
        {{5, -2, 5}, 10},
        {{-5, 10, 10}, 20},
        {{-1, -2}, -3},
        {{-3, -1, -2}, -3},
    };
    int pass = 0;
    for (auto &t : tests)
    {
        int got = maxTwoNonOverlap(t.first);
        bool ok = (got == t.second);
        if (ok) pass++;
        cout << "got " << got << " | exp " << t.second << (ok ? "   PASS" : "   *** FAIL ***") << "\n";
    }
    cout << "\n" << pass << "/" << tests.size() << " PASS\n";
    return 0;
}
