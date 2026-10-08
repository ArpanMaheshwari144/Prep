// ═══════════════════════════════════════════════════════════════════════
//  IS GRAPH BIPARTITE?  (LC 785)  — REDO
//  adjacency-list `graph` di: graph[i] = node i ke saare neighbours (undirected).
//  RETURN: kya saare nodes ko 2 group me baant sakte, taaki HAR edge do ALAG
//          group ke beech ho (ek hi group ke andar koi edge nahi)? (true/false)
//
//  INPUT-FORM: graph DISCONNECTED bhi ho sakta. node 0 .. n-1.
//  TEST-CASES (input -> expected):
//   1) [[1,3],[0,2],[1,3],[0,2]]              -> true   (square 0-1-2-3)
//   2) [[1,2,3],[0,2],[0,1,3],[0,2]]          -> false  (0-1-2 triangle)
//   3) [[],[],[]]                             -> true   (koi edge nahi)
//   4) [[1],[0],[3],[2]]                      -> true   (do alag edge)
//   5) [[1,2],[0,2],[0,1]]                    -> false  (triangle)
//   6) [[]]                                   -> true   (ek node)
//   7) [[1],[0,2],[1,3],[2,4],[3]]            -> true   (seedhi line 0-1-2-3-4)
//   8) [[1,4],[0,2],[1,3],[2,4],[3,0]]        -> false  (5-cycle)
//   9) [[1,5],[0,2],[1,3],[2,4],[3,5],[4,0]]  -> true   (6-cycle)
//  10) [[1],[0],[3,4],[2,4],[2,3]]            -> false  (0-1 theek, 2-3-4 triangle)
//  11) [[1,2,3,4],[0],[0],[0],[0]]            -> true   (star, beech me 0)
//  12) [[2,3,4],[2,3,4],[0,1],[0,1],[0,1]]    -> true   ({0,1} vs {2,3,4})
//  13) [[],[],[3,4],[2,4],[2,3]]              -> false  (akele nodes, phir triangle)
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

bool isBipartite(vector<vector<int>> &graph)
{
    // TODO: tu likh
    return false;
}

// ---- test helper ----
void check(vector<vector<int>> g, bool exp, string name)
{
    bool got = isBipartite(g);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << (got ? "true" : "false")
         << " exp=" << (exp ? "true" : "false") << "\n";
}

int main()
{
    check({{1, 3}, {0, 2}, {1, 3}, {0, 2}}, true, "t1 square");
    check({{1, 2, 3}, {0, 2}, {0, 1, 3}, {0, 2}}, false, "t2 triangle-inside");
    check({{}, {}, {}}, true, "t3 no-edge");
    check({{1}, {0}, {3}, {2}}, true, "t4 two-edges");
    check({{1, 2}, {0, 2}, {0, 1}}, false, "t5 triangle");
    check({{}}, true, "t6 single");
    check({{1}, {0, 2}, {1, 3}, {2, 4}, {3}}, true, "t7 line");
    check({{1, 4}, {0, 2}, {1, 3}, {2, 4}, {3, 0}}, false, "t8 5-cycle");
    check({{1, 5}, {0, 2}, {1, 3}, {2, 4}, {3, 5}, {4, 0}}, true, "t9 6-cycle");
    check({{1}, {0}, {3, 4}, {2, 4}, {2, 3}}, false, "t10 2nd-component-triangle");
    check({{1, 2, 3, 4}, {0}, {0}, {0}, {0}}, true, "t11 star");
    check({{2, 3, 4}, {2, 3, 4}, {0, 1}, {0, 1}, {0, 1}}, true, "t12 K2,3");
    check({{}, {}, {3, 4}, {2, 4}, {2, 3}}, false, "t13 isolated+triangle");
    return 0;
}
