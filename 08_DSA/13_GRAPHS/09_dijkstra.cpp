// ============================================================
// DIJKSTRA — Shortest Path (weighted graph)   [GRAPHS #9 · min-heap + dist[] + relax]
// ============================================================
// n nodes (0..n-1). undirected WEIGHTED edges: har edge {u, v, w} = u-v ke beech weight w.
// source diya. Return: har node tak ka SHORTEST total-weight distance (vector<int>).
//   unreachable node -> INF.
//
//   ★ SAAR: BFS + min-heap + dist[]. hamesha sabse-chhoti-dist node uthao ->
//           uske neighbours RELAX karo (dist[node]+w < dist[nbr] -> update+push).
//
//   ---- APPROACH ----  (= BFS TEMPLATE, bas 2 cheez swap + relaxation)
//   ★★ DIJKSTRA = BFS ka WEIGHTED bhai. wahi template, do swap:
//        queue      -> MIN-HEAP (priority_queue, chhoti dist pehle)
//        visited[]  -> dist[]   (har node tak abhi-tak ka shortest; init INF, src=0)
//      + naya = RELAXATION.
//   TEMPLATE:
//     1. edges -> weighted adj-list: adj[u]+={v,w}, adj[v]+={u,w}  (undirected -> DONO taraf).
//     2. dist[]=INF, dist[src]=0. min-heap me {0, src}.
//     3. while heap: {d, node} pop.    // d = node tak ki ABHI-TAK ki dist
//          d > dist[node]?  -> STALE, skip (behtar rasta pehle mil chuka).
//          har neighbour {nbr, w}   // nbr = padosi node,  w = un dono ke beech EDGE ka weight
//            RELAX:  d + w  = nbr tak naya rasta (node-tak-dist + edge-weight).
//                    (d + w) < dist[nbr]?  ->  dist[nbr] = d + w;  push {d+w, nbr}.
//     4. dist[] = saare shortest distances.
//   ★ RELAX = "dheela karo": naya rasta (d+w) purane dist[nbr] se kam? -> update.
//     ★★ w = it.second (EDGE ka weight), na ki dist[node]. -- ye bug HAND-TRACE me pakda tha!
//   ★ min-heap KYUN (queue nahi): weighted me "kam edges" != "kam weight" -> greedily hamesha
//     sabse-sasti-abhi-tak node uthani padti -> min-heap deta.
//
//   ★ adj-list weighted: adj[u] = list of {neighbour, weight}   -> pair<int,int>
//   ★ MIN-HEAP syntax (C++): sabse chhoti dist upar chahiye ->
//       priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
//       (pair = {dist, node}. greater<> = min-heap. default priority_queue MAX-heap hota.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

// ─── YAHAN SOLVE KAR ────────────────────────────────────────
// hint (dhyan, code nahi): dist[] = INF, dist[src]=0. min-heap me {0,src}.
//   pop {d,node} -> agar d > dist[node] STALE skip. warna har {nbr,w} relax.
vector<int> dijkstra(int n, vector<vector<int>> &edges, int src)
{
    // STEP 1: edges -> weighted adj-list (undirected -> DONO taraf jodo)
    unordered_map<int, vector<pair<int, int>>> adj;
    for (auto &it : edges) {
        int u = it[0];
        int v = it[1];
        int w = it[2];

        adj[u].push_back({v, w}); // u ka neighbour v, weight w
        adj[v].push_back({u, w}); // undirected -> v ka neighbour u bhi
    }

    // STEP 2: min-heap {dist, node} (chhoti dist upar) + dist[]=INF, src=0
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(n, 1e9);
    dist[src] = 0;
    pq.push({0, src}); // {distance, node}

    // STEP 3: BFS-jaisa loop, par min-heap se sabse-chhoti-dist node pehle
    while (!pq.empty())
    {
        int wt = pq.top().first;   // is node tak ki abhi tak ki dist
        int node = pq.top().second;
        pq.pop();

        if (wt > dist[node]) // STALE: behtar rasta pehle mil chuka -> chhod do
            continue;

        for (auto &it : adj[node]) // RELAX: har neighbour {nbr=it.first, w=it.second}
        {
            // ★ w = it.second (EDGE ka weight) -- ye HAND-TRACE me pakda (dist[node] nahi)
            if (wt + it.second < dist[it.first]) // naya rasta chhota? -> dheela karo
            {
                dist[it.first] = wt + it.second;
                pq.push({wt + it.second, it.first});
            }
        }
    }
    return dist; // sab shortest distances
}

// ─── TESTS (isko haath mat lagana) ──────────────────────────
void check(vector<int> got, vector<int> exp, int t)
{
    bool ok = (got == exp);
    cout << "T" << t << ": " << (ok ? "PASS" : "FAIL") << "  got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << (got[i] >= INF ? -1 : got[i]) << (i + 1 < (int)got.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    // T1 graph (e1):
    //          4
    //      0 ------- 1
    //      |       / |
    //    1 |    2 /   | 1
    //      |   /      |
    //      2 ------- 3
    //          5
    //   edges: 0-1(4), 0-2(1), 1-2(2), 1-3(1), 2-3(5)   src=0 -> dist=[0,3,1,4]
    vector<vector<int>> e1 = {{0, 1, 4}, {0, 2, 1}, {1, 2, 2}, {1, 3, 1}, {2, 3, 5}};
    check(dijkstra(4, e1, 0), {0, 3, 1, 4}, 1);

    // T2: chain 0-1(1)-2(2) -> [0,1,3]
    vector<vector<int>> e2 = {{0, 1, 1}, {1, 2, 2}};
    check(dijkstra(3, e2, 0), {0, 1, 3}, 2);

    // T3: node 2 UNREACHABLE -> [0,5,INF]
    vector<vector<int>> e3 = {{0, 1, 5}};
    check(dijkstra(3, e3, 0), {0, 5, INF}, 3);

    // T4: single node -> [0]
    vector<vector<int>> e4 = {};
    check(dijkstra(1, e4, 0), {0}, 4);
    return 0;
}
