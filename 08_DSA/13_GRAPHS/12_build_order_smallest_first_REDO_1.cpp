// ═══════════════════════════════════════════════════════════════════════
//  BUILD ORDER — SMALLEST READY FIRST   (REDO_1)
//  Ek project me n module hain (0 .. n-1). deps[i] = {pehle, baad}
//     matlab: module 'pehle' build hone ke BAAD hi 'baad' build ho sakta. (edge: pehle -> baad)
//  Build server ek time pe EK module build karta. Jitne module READY hain (saare dependency
//     build ho chuke), unme se SABSE CHHOTA number wala pehle build. phir repeat.
//  RETURN: poora build order. Loop ho (koi ready nahi, par module baaki) -> KHAALI vector.
//
//  TEST-CASES (input -> expected):
//   1) n=3, []                                         -> 0 1 2
//   2) n=3, [2->0]                                     -> 1 2 0
//   3) n=4, [0->3, 1->3, 3->2]                         -> 0 1 3 2
//   4) n=2, [0->1, 1->0]                               -> (khaali)   loop
//   5) n=3, [0->1, 1->2, 2->1]                         -> (khaali)   0 ho gaya, phir 1-2 loop
//   6) n=1, []                                         -> 0
//   7) n=1, [0->0]                                     -> (khaali)   self-loop
//   8) n=4, [0->1]                                     -> 0 1 2 3    (1 baad me ready hua, phir bhi 2 se pehle)
//   9) n=5, [0->2, 1->2, 2->3, 4->3]                   -> 0 1 2 4 3
//  10) n=4, [3->0, 2->0, 1->0]                         -> 1 2 3 0
//  11) n=5, [4->0, 3->1, 2->1]                         -> 2 3 1 4 0
//  12) n=6, [5->2, 5->0, 4->0, 4->1, 2->3, 3->1]       -> 4 5 0 2 3 1
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

vector<int> buildOrder(int n, vector<vector<int>> &deps)
{
    unordered_map<int, vector<int>> adj;
    vector<int> inDegree(n, 0);
    for (auto &it : deps)
    {
        int u = it[0];
        int v = it[1];
        adj[u].push_back(v);
        inDegree[v]++;
    }

    priority_queue<int, vector<int>, greater<int>> pq;
    vector<int> ans;
    for (int i = 0; i < n; i++)
    {
        if (inDegree[i] == 0)
        {
            pq.push(i);
        }
    }

    while (!pq.empty())
    {
        int node = pq.top();
        pq.pop();
        ans.push_back(node);

        for (auto &it : adj[node])
        {
            inDegree[it]--;
            if (inDegree[it] == 0)
            {
                pq.push(it);
            }
        }
    }
    if (ans.size() == n)
    {
        return ans;
    }
    return {};
}

// ---- test helper (ise mat chhed) ----
string show(const vector<int> &v)
{
    if (v.empty())
        return "(khaali)";
    string s;
    for (int x : v)
        s += to_string(x) + " ";
    s.pop_back();
    return s;
}

void check(int n, vector<vector<int>> deps, vector<int> exp, string name)
{
    vector<int> got = buildOrder(n, deps);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << show(got) << " exp=" << show(exp) << "\n";
}

int main()
{
    check(3, {}, {0, 1, 2}, "t1 no-dep");
    check(3, {{2, 0}}, {1, 2, 0}, "t2 one-dep");
    check(4, {{0, 3}, {1, 3}, {3, 2}}, {0, 1, 3, 2}, "t3 two-before");
    check(2, {{0, 1}, {1, 0}}, {}, "t4 2-loop");
    check(3, {{0, 1}, {1, 2}, {2, 1}}, {}, "t5 partial-loop");
    check(1, {}, {0}, "t6 single");
    check(1, {{0, 0}}, {}, "t7 self-loop");
    check(4, {{0, 1}}, {0, 1, 2, 3}, "t8 late-ready-smaller");
    check(5, {{0, 2}, {1, 2}, {2, 3}, {4, 3}}, {0, 1, 2, 4, 3}, "t9 diamond");
    check(4, {{3, 0}, {2, 0}, {1, 0}}, {1, 2, 3, 0}, "t10 zero-last");
    check(5, {{4, 0}, {3, 1}, {2, 1}}, {2, 3, 1, 4, 0}, "t11 mixed");
    check(6, {{5, 2}, {5, 0}, {4, 0}, {4, 1}, {2, 3}, {3, 1}}, {4, 5, 0, 2, 3, 1}, "t12 big");
    return 0;
}
