// ============================================================
// IS GRAPH BIPARTITE? — (LeetCode 785, Medium)   [GRAPHS #8 · 2-colour BFS]
// ============================================================
// adjacency-list `graph` di: graph[i] = node i ke saare neighbours.
// Batao graph BIPARTITE hai? (true / false)
//
//   bipartite = saare nodes ko 2 group me baant sako, taaki HAR edge do
//               alag group ke beech ho (ek hi group ke andar koi edge nahi).
//   = 2-colour: har node red/blue, koi 2 ADJACENT nodes SAME rang na ho.
//     ho gaya -> true.  kahin adjacent same-rang forced -> false.
//
//   graph DISCONNECTED ho sakta -> outer loop saare nodes pe (jaise Connected Components).
//
//   Example:
//     [[1,3],[0,2],[1,3],[0,2]]        -> true   (square 0-1-2-3, 2-colour ho jaata)
//     [[1,2,3],[0,2],[0,1,3],[0,2]]    -> false  (0-1-2 triangle, odd cycle)
// ============================================================
// ---- APPROACH ----  (2-COLOUR BFS; input KHUD adj-list hai)
//  idea: har node ko 2 rang (0/1) do, koi 2 ADJACENT SAME-rang na ho -> bipartite.
//          conflict (adjacent same rang forced ho jaaye) -> false.
//  TEMPLATE:
//  1. color[] = -1 (uncolored).  graph KHUD adj-list hai (banani NAHI -- yahan ready milta).
//  2. outer loop har node pe (disconnected ke liye, CONNECTED-COMPONENTS jaisa): uncolored -> BFS.
//  3. BFS: START node ko color do -> color[node]=0. phir curr nikalo -> uske har neighbour it pe:
//        uncolored(-1)   -> color[it] = !color[curr]  (OPPOSITE rang) + push.
//        already colored -> color[it]==color[curr]? -> SAME rang = conflict -> return false.
//  4. kahin conflict nahi -> true.
//  opposite trick: !color[curr] (ya 1-color[curr]) -> 0<->1 flip.
//  START node color ZAROORI (color[node]=0): tabhi curr ka color set hota, tabhi !color[curr]
//     sahi opposite deta. (pehle chhoot gaya tha -> !(-1) se by-luck chal raha tha -> FIX kiya.)
//  = BFS (queue) + outer-loop (CC jaisa) + colour-check. ODD-CYCLE hone pe hi false aata.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// BFS: ek component ko 2-colour karta. conflict mila -> false, warna true.
bool BFS(vector<vector<int>> &graph, int node, vector<int> &color)
{
    queue<int> q;
    q.push(node);
    color[node] = 0; // START ko color do -> tabhi !color[curr] sahi opposite dega

    while (!q.empty())
    {
        int curr = q.front();
        q.pop();

        for (auto &it : graph[curr]) // curr ke saare neighbours
        {
            if (color[it] == -1) // uncolored -> OPPOSITE rang do + push
            {
                color[it] = 1 - color[curr]; // OPPOSITE flip: 0->1, 1->0 (sirf 0/1 hai, isliye 1-x kaam karta)
                // (ya: color[it] = !color[curr];  bhi same -> !0=1, !1=0)
                q.push(it);
            }
            else // already colored -> check
            {
                if (color[it] == color[curr]) // SAME rang = conflict
                    return false;             // -> bipartite NAHI
            }
        }
    }
    return true; // is component me koi conflict nahi
}

bool isBipartite(vector<vector<int>> &graph)
{
    int m = graph.size();
    vector<int> color(m, -1); // -1 = uncolored, 0/1 = do rang

    // outer loop: disconnected components ke liye (CC jaisa) -- har uncolored node se BFS
    for (int i = 0; i < m; i++)
    {
        if (color[i] == -1)
        {
            if (BFS(graph, i, color) == false) // koi ek component fail -> pura false
            {
                return false;
            }
        }
    }
    return true; // saare components 2-colour ho gaye
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
int main()
{
    vector<vector<int>> g1 = {{1, 3}, {0, 2}, {1, 3}, {0, 2}};       // square -> true
    vector<vector<int>> g2 = {{1, 2, 3}, {0, 2}, {0, 1, 3}, {0, 2}}; // triangle -> false
    vector<vector<int>> g3 = {{}, {}, {}};                           // no edges -> true
    vector<vector<int>> g4 = {{1}, {0}, {3}, {2}};                   // 2 separate edges -> true
    vector<vector<int>> g5 = {{1, 2}, {0, 2}, {0, 1}};               // triangle 0-1-2 -> false

    // ── VISUAL (har test; index=node, values=neighbours) ──
    //  g1:  0-1     square (4-cycle)     -> 2-colour ho jaata  TRUE
    //       | |
    //       3-2

    cout << "T1: " << (isBipartite(g1) ? "true " : "false") << " (exp true)\n";
    cout << "T2: " << (isBipartite(g2) ? "true " : "false") << " (exp false)\n";
    cout << "T3: " << (isBipartite(g3) ? "true " : "false") << " (exp true)\n";
    cout << "T4: " << (isBipartite(g4) ? "true " : "false") << " (exp true)\n";
    cout << "T5: " << (isBipartite(g5) ? "true " : "false") << " (exp false)\n";
    return 0;
}
