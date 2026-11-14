// ============================================================
// PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// CONNECTED COMPONENTS — UNION-FIND (DSU) se   [GRAPHS #10]  -- REDO 1
// ============================================================
// n node (0 se n-1), undirected edges. Kitne ALAG group (components) hain?
// Is baar BFS / DFS nahi: DSU class khud banao, edge aate-aate group jodo, end me gino.
//
//   n = 5, edges = {0-1, 1-2, 3-4}   ->  2      ( {0,1,2} aur {3,4} )
//
// TEST-CASES (n, edges -> expected):
//   5, {0-1, 1-2, 3-4}                 -> 2
//   5, {0-1, 1-2, 2-3, 3-4}            -> 1
//   4, {}                              -> 4
//   1, {}                              -> 1
//   4, {0-1, 1-2, 2-0}                 -> 2   (cycle wala edge, group pehle se same)
//   3, {0-1, 1-0, 0-1}                 -> 2   (wahi edge baar-baar)
//   6, {0-1, 2-3, 4-5, 1-3}            -> 2   (do group baad me jude)
//   7, {5-6, 4-5, 3-4, 2-3, 1-2, 0-1}  -> 1   (lambi chain, ulte kram me)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

int countComponents(int n, vector<vector<int>> &edges)
{
    // yahan likh
    return 0;
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int main()
{
    int pass = 0, total = 0;
    auto check = [&](int n, vector<vector<int>> e, int exp)
    {
        int got = countComponents(n, e);
        total++;
        if (got == exp) pass++;
        cout << "T" << total << ": " << got << " (exp " << exp << ")" << (got == exp ? "  ok" : "  GALAT") << "\n";
    };
    check(5, {{0, 1}, {1, 2}, {3, 4}}, 2);
    check(5, {{0, 1}, {1, 2}, {2, 3}, {3, 4}}, 1);
    check(4, {}, 4);
    check(1, {}, 1);
    check(4, {{0, 1}, {1, 2}, {2, 0}}, 2);
    check(3, {{0, 1}, {1, 0}, {0, 1}}, 2);
    check(6, {{0, 1}, {2, 3}, {4, 5}, {1, 3}}, 2);
    check(7, {{5, 6}, {4, 5}, {3, 4}, {2, 3}, {1, 2}, {0, 1}}, 1);
    cout << pass << "/" << total << "\n";
    return 0;
}
