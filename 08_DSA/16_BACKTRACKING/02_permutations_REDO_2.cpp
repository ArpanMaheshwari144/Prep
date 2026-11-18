// ==============================================================
// PERMUTATIONS  (LC-46, Medium)  -- REDO_2 (blank, khud likh)
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
//   nums[] (saare number ALAG). SAARE permutations (har possible ORDER) lautao.
//   Kis order me lautao, farak nahi padta.
//
// TEST CASES (input -> expected):
//   [1,2,3]    -> [1,2,3],[1,3,2],[2,1,3],[2,3,1],[3,1,2],[3,2,1]   (6)
//   [0,1]      -> [0,1],[1,0]                                       (2)
//   [1]        -> [1]                                               (1)
//   [-1,5]     -> [-1,5],[5,-1]                                     (2)
//   [3,1,2]    -> wahi 6 jo [1,2,3] ke                              (6)
//   [1,2,3,4]  -> 24 alag order                                     (24)
//
// (check() order nahi dekhta: got aur exp dono sort karke milata, count bhi chhapta.)
// ==============================================================

#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> permute(vector<int> &nums)
{
    // yahan likh
    return {};
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> in, vector<vector<int>> exp, int t)
{
    vector<vector<int>> got = permute(in);
    sort(got.begin(), got.end());
    sort(exp.begin(), exp.end());
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL")
         << "  count=" << got.size() << " (exp " << exp.size() << ")\n";
}

int main()
{
    vector<vector<int>> six = {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}};
    check({1, 2, 3}, six, 1);
    check({0, 1}, {{0, 1}, {1, 0}}, 2);
    check({1}, {{1}}, 3);
    check({-1, 5}, {{-1, 5}, {5, -1}}, 4);
    check({3, 1, 2}, six, 5);
    check({1, 2, 3, 4},
          {{1, 2, 3, 4}, {1, 2, 4, 3}, {1, 3, 2, 4}, {1, 3, 4, 2}, {1, 4, 2, 3}, {1, 4, 3, 2},
           {2, 1, 3, 4}, {2, 1, 4, 3}, {2, 3, 1, 4}, {2, 3, 4, 1}, {2, 4, 1, 3}, {2, 4, 3, 1},
           {3, 1, 2, 4}, {3, 1, 4, 2}, {3, 2, 1, 4}, {3, 2, 4, 1}, {3, 4, 1, 2}, {3, 4, 2, 1},
           {4, 1, 2, 3}, {4, 1, 3, 2}, {4, 2, 1, 3}, {4, 2, 3, 1}, {4, 3, 1, 2}, {4, 3, 2, 1}},
          6);
    return 0;
}
