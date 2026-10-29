// ═══════════════════════════════════════════════════════════════════════
//  PEHLE YE (30 sec):
//  1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
//  2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
//  3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
//  4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
//  5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
//  6. Code ke BAAD, submit se pehle: ★ EDGE line + ek chhota dushman-example chalao
// ═══════════════════════════════════════════════════════════════════════
//  DIJKSTRA: SHORTEST PATH (weighted graph)
//  n nodes (0..n-1). undirected WEIGHTED edges: har edge {u, v, w} = u-v ke beech weight w (w >= 0).
//  source diya. Har node tak ka sabse chhota total weight lautao (vector<int>).
//  Jahan pahunch hi nahi sakte -> INF (1e9).
//
//  INPUT-FORM: 1 <= n <= 1e4, 0 <= edges <= 5e4, 0 <= w <= 1e4.
//  TEST-CASES (input -> expected):
//   1) n=4, 0-1(4) 0-2(1) 1-2(2) 1-3(1) 2-3(5), src 0   -> [0,3,1,4]   (seedha 0-1 mehnga, 0-2-1 sasta)
//   2) n=3, 0-1(1) 1-2(2), src 0                        -> [0,1,3]     (chain)
//   3) n=3, 0-1(5), src 0                               -> [0,5,INF]   (2 tak raasta nahi)
//   4) n=1, koi edge nahi, src 0                        -> [0]
//   5) n=4, T1 wala graph, src 3                        -> [4,1,3,0]   (source beech ka)
//   6) n=3, 0-1(10) 0-1(2) 1-2(3), src 0                -> [0,2,5]     (do edge same jodi, sasti lo)
//   7) n=3, 0-1(0) 1-2(0), src 0                        -> [0,0,0]     (weight 0)
//   8) n=5, 0-1(1) 1-2(1) 2-3(1) 3-4(1) 0-4(10), src 0  -> [0,1,2,3,4] (lamba raasta sasta)
//   9) n=4, 0-1(2) 2-3(3), src 2                        -> [INF,INF,0,3] (do alag tukde)
//  10) n=4, 0-1(1) 0-2(4) 1-2(1) 2-3(1) 1-3(5), src 0  -> [0,1,2,3]
//  11) n=2, 0-1(10000), src 1                           -> [10000,0]
//  12) n=6, 0-1(7) 0-2(9) 0-5(14) 1-2(10) 1-3(15) 2-3(11) 2-5(2) 3-4(6) 4-5(9), src 0 -> [0,7,9,20,20,11]
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

vector<int> dijkstra(int n, vector<vector<int>> &edges, int src)
{
    unordered_map<int, vector<pair<int, int>>> adj;
    for (auto &it : edges)
    {
        int u = it[0];
        int v = it[1];
        int w = it[2];

        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(n, 1e9);
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty())
    {
        int wt = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if (wt > dist[node])
        {
            continue;
        }

        for (auto &it : adj[node])
        {
            if (wt + it.second < dist[it.first])
            {
                dist[it.first] = wt + it.second;
                pq.push({wt + it.second, it.first});
            }
        }
    }
    return dist;
}

// ---- test helper ----
void check(int n, vector<vector<int>> edges, int src, vector<int> exp, string name)
{
    vector<int> got = dijkstra(n, edges, src);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name << " | got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << (got[i] >= INF ? -1 : got[i]) << (i + 1 < (int)got.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    vector<vector<int>> g1 = {{0, 1, 4}, {0, 2, 1}, {1, 2, 2}, {1, 3, 1}, {2, 3, 5}};
    check(4, g1, 0, {0, 3, 1, 4}, "t1");
    check(3, {{0, 1, 1}, {1, 2, 2}}, 0, {0, 1, 3}, "t2 chain");
    check(3, {{0, 1, 5}}, 0, {0, 5, INF}, "t3 unreachable");
    check(1, {}, 0, {0}, "t4 ek node");
    check(4, g1, 3, {4, 1, 3, 0}, "t5 source beech ka");
    check(3, {{0, 1, 10}, {0, 1, 2}, {1, 2, 3}}, 0, {0, 2, 5}, "t6 dohri edge");
    check(3, {{0, 1, 0}, {1, 2, 0}}, 0, {0, 0, 0}, "t7 weight 0");
    check(5, {{0, 1, 1}, {1, 2, 1}, {2, 3, 1}, {3, 4, 1}, {0, 4, 10}}, 0, {0, 1, 2, 3, 4}, "t8 lamba sasta");
    check(4, {{0, 1, 2}, {2, 3, 3}}, 2, {INF, INF, 0, 3}, "t9 do tukde");
    check(4, {{0, 1, 1}, {0, 2, 4}, {1, 2, 1}, {2, 3, 1}, {1, 3, 5}}, 0, {0, 1, 2, 3}, "t10");
    check(2, {{0, 1, 10000}}, 1, {10000, 0}, "t11");
    check(6, {{0, 1, 7}, {0, 2, 9}, {0, 5, 14}, {1, 2, 10}, {1, 3, 15}, {2, 3, 11}, {2, 5, 2}, {3, 4, 6}, {4, 5, 9}}, 0, {0, 7, 9, 20, 20, 11}, "t12");
    return 0;
}
