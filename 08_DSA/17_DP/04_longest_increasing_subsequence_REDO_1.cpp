// ═══════════════════════════════════════════════════════════════════════
//  PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: ★ EDGE line + ek chhota dushman-example chalao
// ═══════════════════════════════════════════════════════════════════════
//  LONGEST INCREASING SUBSEQUENCE (LC-300)  -- REDO_1 (khaali page, khud likh)
//  nums[] diya. Sabse lambi STRICTLY INCREASING subsequence ki LENGTH lautao.
//  (subsequence = kram same rahe, beech ke skip kar sakte. increasing = har agla > pichla.)
//
//  TEST-CASES (nums -> expected):
//   1) [10,9,2,5,3,7,101,18]  -> 4   ([2,3,7,101])
//   2) [0,1,0,3,2,3]          -> 4   ([0,1,2,3])
//   3) [7,7,7,7]              -> 1   (barabar = increasing nahi)
//   4) [1,2,3,4,5]            -> 5
//   5) [5,4,3,2,1]            -> 1
//   6) [1]                    -> 1   (sabse chhota)
//   7) [4,10,4,3,8,9]         -> 3   ([4,8,9])
//   8) [3,1,2]                -> 2   ([1,2])
//   9) [1,3,6,7,9,4,10,5,6]   -> 6   ([1,3,6,7,9,10])
//  10) [2,2,3,3,4,4]          -> 3   ([2,3,4])
//  11) [-2,-1,0,-5,1]         -> 4   (negative bhi)
//  12) [1,5,2,6,3,7]          -> 4   ([1,2,3,7])
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

int lengthOfLIS(vector<int> &nums)
{
    // yahan likh
    return 0;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int passed = 0, total = 0;
void check(vector<int> in, int exp, int t)
{
    total++;
    int got = lengthOfLIS(in);
    bool ok = (got == exp);
    if (ok)
        passed++;
    cout << "T" << t << ": " << (ok ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    check({10, 9, 2, 5, 3, 7, 101, 18}, 4, 1);
    check({0, 1, 0, 3, 2, 3}, 4, 2);
    check({7, 7, 7, 7}, 1, 3);
    check({1, 2, 3, 4, 5}, 5, 4);
    check({5, 4, 3, 2, 1}, 1, 5);
    check({1}, 1, 6);
    check({4, 10, 4, 3, 8, 9}, 3, 7);
    check({3, 1, 2}, 2, 8);
    check({1, 3, 6, 7, 9, 4, 10, 5, 6}, 6, 9);
    check({2, 2, 3, 3, 4, 4}, 3, 10);
    check({-2, -1, 0, -5, 1}, 4, 11);
    check({1, 5, 2, 6, 3, 7}, 4, 12);
    cout << "\n"
         << passed << "/" << total << " PASS\n";
    return 0;
}
