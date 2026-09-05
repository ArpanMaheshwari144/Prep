// ============================================================
// COURSE SCHEDULE — (LeetCode 207, Medium)   [GRAPHS #6 · TOPO SORT / cycle-detect]
// ============================================================
// numCourses courses hain (0 se numCourses-1). prerequisites list di:
//   [a, b]  matlab -> course 'a' lene ke liye 'b' PEHLE karna padega. (b -> a dependency)
// Batao: kya SAARE courses finish kar sakte ho? (true / false)
//
//   = TOPO SORT: valid order exist karta? = koi CYCLE (circular prereq) to nahi?
//     cycle ho (a->b->a) -> dono ek-dusre ka wait -> IMPOSSIBLE -> false.
//     no cycle (DAG) -> valid order milta -> true.
//
//   KAHN'S ALGO (BFS + indegree) -- jo tune recall kiya:
//     indegree[x] = x ke kitne prerequisites. indegree-0 = koi prereq nahi (abhi kar sakte).
//     queue me saare indegree-0 -> pop -> uske neighbours ka indegree-- -> naya 0 hua? push.
//     count processed nodes. count == numCourses? -> no cycle -> true. warna cycle -> false.
//
//   Example:
//     numCourses=2, prereq=[[1,0]]           -> true   (0 -> 1, valid)
//     numCourses=2, prereq=[[1,0],[0,1]]     -> false  (0->1 aur 1->0 = CYCLE)
//     numCourses=4, prereq=[[1,0],[2,1],[3,2]] -> true (chain 0->1->2->3)
//     numCourses=3, prereq=[[0,1],[1,2],[2,0]] -> false (cycle)
// ============================================================
// ============================================================

#include <bits/stdc++.h>
using namespace std;

bool canFinish(int numCourses, vector<vector<int>> &prerequisites)
{
    // STEP 1: adj-list + indegree (indegree = kitne prereq BACHE)
    unordered_map<int, vector<int>> adj;
    vector<int> inDegree(numCourses, 0);
    for (auto &it : prerequisites)
    {
        int u = it[0];
        int v = it[1];

        adj[u].push_back(v); // u -> v edge
        inDegree[v]++;       // v ka ek prereq badha
    }

    // STEP 2: indegree-0 wale queue me (koi prereq nahi -> abhi kar sakte)
    queue<int> q;
    for (int i = 0; i < numCourses; i++)
    {
        if (inDegree[i] == 0)
        {
            q.push(i);
        }
    }

    // STEP 3: BFS -> process karo, neighbours ka indegree ghatao
    vector<int> topo;
    while (!q.empty())
    {
        int node = q.front();
        q.pop();

        topo.push_back(node); // process ho gaya (count)

        for (auto &it : adj[node])
        {
            inDegree[it]--;         // ek prereq pura hua
            if (inDegree[it] == 0)  // ab is node ke saare prereq done -> ready
            {
                q.push(it);
            }
        }
    }

    // STEP 4: sab process hue? -> no cycle. warna cycle (kuch nodes ka indegree kabhi 0 nahi hua)
    if (topo.size() == numCourses)
    {
        return true;
    }
    return false; // cycle -> saare finish nahi ho sakte
}

int main()
{
    vector<vector<int>> p1 = {{1, 0}};
    vector<vector<int>> p2 = {{1, 0}, {0, 1}};
    vector<vector<int>> p3 = {{1, 0}, {2, 1}, {3, 2}};
    vector<vector<int>> p4 = {{0, 1}, {1, 2}, {2, 0}};

    // ── VISUAL ([a,b] = directed edge a->b; CYCLE ho to false) ──
    //  p4(n=3):  0->1->2->0  (loop = cycle)   -> FALSE   (p3 = chain 3->2->1->0 -> TRUE)

    cout << canFinish(2, p1) << " (expected 1)\n";
    cout << canFinish(2, p2) << " (expected 0)\n";
    cout << canFinish(4, p3) << " (expected 1)\n";
    cout << canFinish(3, p4) << " (expected 0)\n";
    return 0;
}
