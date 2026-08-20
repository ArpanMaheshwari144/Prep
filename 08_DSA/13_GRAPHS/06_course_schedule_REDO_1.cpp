// ═══════════════════════════════════════════════════════════════════════
//  COURSE SCHEDULE  (LC 207)  — REDO
//  numCourses courses (0 .. numCourses-1). prerequisites[i] = {course, pre}
//     matlab: course lene se PEHLE 'pre' lena zaroori. (edge: pre -> course)
//  RETURN: kya SAARE courses complete kar sakte? (true/false)
//
//  INPUT-FORM: directed graph. {course, pre} = pre se course pe jaana padta.
//  SOCH: directed graph me "sab complete ho paayega ya nahi" ka matlab kya? (tu pehchan)
//
//  TEST-CASES (input -> expected):
//   1) n=2, [[1,0]]                          -> true   (0->1, seedha)
//   2) n=2, [[1,0],[0,1]]                    -> false  (0<->1 cycle)
//   3) n=1, []                               -> true   (ek course, koi pre nahi)
//   4) n=3, [[1,0],[2,1]]                    -> true   (chain 0->1->2)
//   5) n=3, [[0,1],[1,2],[2,0]]              -> false  (3-cycle)
//   6) n=4, [[1,0],[2,0],[3,1],[3,2]]        -> true   (DAG)
//   7) n=2, []                               -> true   (koi edge nahi)
//   8) n=5, [[1,0],[2,1],[3,2],[4,3]]        -> true   (lambi chain)
//   9) n=3, [[1,0],[1,2],[0,2]]              -> true   (DAG: 2->0, 2->1, 0->1)
//  10) n=4, [[0,1],[1,2],[2,3],[3,1]]        -> false  (cycle 1->2->3->1)
//  11) n=1, [[0,0]]                          -> false  (self-loop = cycle)
// ═══════════════════════════════════════════════════════════════════════

#include <bits/stdc++.h>
using namespace std;

bool canFinish(int numCourses, vector<vector<int>> &prerequisites)
{
    unordered_map<int, vector<int>> adj;
    vector<int> indegree(numCourses, 0);
    for (auto &it : prerequisites)
    {
        int u = it[1];
        int v = it[0];

        adj[u].push_back(v);
        indegree[v]++;
    }

    queue<int> q;
    vector<int> ans;
    for (int i = 0; i < numCourses; i++)
    {
        if (indegree[i] == 0)
        {
            q.push(i);
        }
    }

    while (!q.empty())
    {
        int node = q.front();
        q.pop();
        ans.push_back(node);
        for (auto &it : adj[node])
        {
            indegree[it]--;
            if (indegree[it] == 0)
            {
                q.push(it);
            }
        }
    }
    return ans.size() == numCourses;
}

// ---- test helper ----
void check(int n, vector<vector<int>> pre, bool exp, string name)
{
    bool got = canFinish(n, pre);
    cout << (got == exp ? "PASS" : "FAIL") << " | " << name
         << " | got=" << (got ? "true" : "false")
         << " exp=" << (exp ? "true" : "false") << "\n";
}

int main()
{
    check(2, {{1, 0}}, true, "t1 simple");
    check(2, {{1, 0}, {0, 1}}, false, "t2 2-cycle");
    check(1, {}, true, "t3 single");
    check(3, {{1, 0}, {2, 1}}, true, "t4 chain3");
    check(3, {{0, 1}, {1, 2}, {2, 0}}, false, "t5 3-cycle");
    check(4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true, "t6 DAG");
    check(2, {}, true, "t7 no-edge");
    check(5, {{1, 0}, {2, 1}, {3, 2}, {4, 3}}, true, "t8 long-chain");
    check(3, {{1, 0}, {1, 2}, {0, 2}}, true, "t9 DAG2");
    check(4, {{0, 1}, {1, 2}, {2, 3}, {3, 1}}, false, "t10 back-edge-cycle");
    check(1, {{0, 0}}, false, "t11 self-loop");
    return 0;
}
