// ============================================================
// CLONE GRAPH — REDO  (LeetCode 133, Medium)   [GRAPHS #7 · deep-copy]
// ============================================================
// Ek connected UNDIRECTED graph ka ek node diya. DEEP COPY (clone) banake return karo
// -- bilkul naya graph, same structure. neighbours bhi NAYE clone-nodes ki taraf.
//   twist: CYCLE hoti -> same node dobara clone na ho.
//
//   (memory se likh. atke to 07_clone_graph.cpp / sheet peek. koi guilt nahi.)
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int val;
    vector<Node *> neighbors;
    Node() { val = 0; }
    Node(int _val) { val = _val; }
};

// ─── YAHAN REDO KAR ─────────────────────────────────────────
Node *cloneGraph(Node *node)
{
    if (!node) // base: khaali graph
    {
        return nullptr;
    }

    unordered_map<Node *, Node *> mp; // purana node (A) -> uska clone (A')
    queue<Node *> q;

    // STEP 1: START node ka clone -> map me daalo + queue me START (purana) push
    Node *clone = new Node();
    clone->val = node->val;
    mp[node] = clone; // key = node (A, purana) -> value = clone (A', naya)
    q.push(node);

    // STEP 2: BFS -> har neighbour ka clone + JODO
    while (!q.empty())
    {
        Node *curr = q.front(); // curr = A
        q.pop();

        for (auto &it : curr->neighbors)
        {
            // (a) neighbour clone NAHI hua (map me nahi)? -> banao + map + queue
            if (mp.find(it) == mp.end())
            {
                Node *newClone = new Node();
                newClone->val = it->val;
                mp[it] = newClone; // B -> B'
                q.push(it);
            }
            // (b) JODO -- A' ke neighbour-list me B' (mp[curr]=A', mp[it]=B')
            mp[curr]->neighbors.push_back(mp[it]);
        }
    }
    return mp[node]; // A ka clone (A') return
}

// ─── TEST HARNESS (isko haath mat lagana) ───────────────────
Node *buildGraph(int n, vector<vector<int>> &adj)
{
    if (n == 0)
        return nullptr;
    vector<Node *> nodes(n + 1);
    for (int i = 1; i <= n; i++)
        nodes[i] = new Node(i);
    for (int i = 1; i <= n; i++)
        for (int nb : adj[i])
            nodes[i]->neighbors.push_back(nodes[nb]);
    return nodes[1];
}

bool verify(Node *orig, Node *clone)
{
    if (!orig && !clone)
        return true;
    if (!orig || !clone)
        return false;
    unordered_map<Node *, Node *> seen;
    queue<Node *> q;
    q.push(orig);
    seen[orig] = clone;
    while (!q.empty())
    {
        Node *o = q.front();
        q.pop();
        Node *c = seen[o];
        if (o->val != c->val)
            return false;
        if (o == c)
            return false; // SAME pointer = shallow, FAIL
        if (o->neighbors.size() != c->neighbors.size())
            return false;
        for (int i = 0; i < (int)o->neighbors.size(); i++)
        {
            Node *on = o->neighbors[i];
            Node *cn = c->neighbors[i];
            if (on->val != cn->val)
                return false;
            if (on == cn)
                return false;
            if (!seen.count(on))
            {
                seen[on] = cn;
                q.push(on);
            }
        }
    }
    return true;
}

int main()
{
    vector<vector<int>> a1 = {{}, {2, 4}, {1, 3}, {2, 4}, {1, 3}}; // 4-node square+diag
    Node *g1 = buildGraph(4, a1);
    cout << "T1: " << (verify(g1, cloneGraph(g1)) ? "PASS" : "FAIL") << "\n";

    vector<vector<int>> a2 = {{}, {}}; // single node
    Node *g2 = buildGraph(1, a2);
    cout << "T2: " << (verify(g2, cloneGraph(g2)) ? "PASS" : "FAIL") << "\n";

    vector<vector<int>> a3 = {{}, {2}, {1}}; // two nodes 1-2
    Node *g3 = buildGraph(2, a3);
    cout << "T3: " << (verify(g3, cloneGraph(g3)) ? "PASS" : "FAIL") << "\n";

    cout << "T4: " << (verify(nullptr, cloneGraph(nullptr)) ? "PASS" : "FAIL") << "\n"; // empty
    return 0;
}
