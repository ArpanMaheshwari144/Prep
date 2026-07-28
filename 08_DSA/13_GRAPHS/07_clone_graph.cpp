// ============================================================
// CLONE GRAPH — (LeetCode 133, Medium)   [GRAPHS #7 · deep-copy + hashmap]
// ============================================================
// Ek connected UNDIRECTED graph ka ek node diya (uske through pura graph reachable).
// DEEP COPY (clone) banake return karo -- bilkul naya graph, same structure.
//   deep = naye nodes; neighbours bhi NAYE clone-nodes ki taraf (purane nahi).
//
//   Node: { int val; vector<Node*> neighbors; }
//   val 1..N, har node ki val unique (1-indexed).
//
//   ★ twist: graph me CYCLE hoti (A-B, B-A) -> traverse karte same node baar-baar
//     clone na ho jaaye -> "kaunsa already clone hua + uska clone kya" yaad rakhna.
//
//   ★★★ SAAR: "mere GRAPH ke saare node map me clone bana (A->A', B->B'),
//              phir un clones ko CONNECT kar (A'--B')." bas yehi kaam.
// ============================================================
// ---- APPROACH ----  (BFS + hashmap; DFS-recursion se bhi hota)
//  ★ idea: map me har node ka clone BANAO (A->A', B->B') -> phir un clones ko CONNECT karo (sab map ke andar).
//     2 kaam:  (1) clone map me daalo   (2) neighbours ke clone se JODO (push_back).
//     map DO kaam karta:  "ye node clone ho chuka?" (cycle-rok) + "uska clone ye raha" (jodne ko).
//     ★ map ki VALUE hi clone hai -> mp[node] = us node ka clone khud.
//
//  ★ VISUAL (A-B graph):
//        original:  A --- B
//        (1) clone map me daalo:  mp = { A->A' ,  B->B' }    // naye node, abhi LOOSE (jude nahi)
//        (2) clones ko JODO:      A' --- B'                  // mp[A]->neighbors += mp[B]
//        => naya deep-copy graph  A'-B'  taiyaar
//
//  ★★ TEMPLATE (BFS):
//  1. start node ka clone banao -> mp[node]=clone ; queue me START node (purana) push.
//  2. while queue: curr nikalo -> uske HAR neighbour it pe:
//        (a) agar it clone NAHI hua (mp me nahi) -> clone banao + mp[it]=clone + q.push(it).
//        (b) mp[curr]->neighbors.push_back(mp[it]);   // JODO -- ye HAMESHA (naya ho ya purana)
//  3. return mp[node].
//
//  ★ 2 alag kaam (yehi crux samjha):
//     - clone BANANA (map me daalna) = node EXIST karwa do (loose)
//     - JODNA (push_back) = A'-B' ke beech edge -- bina iske clones jude nahi -> structure adhoora.
//  ★ "clone naya banao SIRF jab map me nahi" -> cycle me duplicate/overwrite rok + queue infinite-rok.
//  ★ = BFS (queue+map) ; DFS version = recursion + same map (return mp[node]).
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

// ─── YAHAN SOLVE KAR ────────────────────────────────────────
Node *cloneGraph(Node *node)
{
    if (!node)
        return NULL;

    unordered_map<Node *, Node *> mp; // purana node -> uska clone
    queue<Node *> q;

    // STEP 1: START node ka clone -> map me daalo + queue me START (purana) push
    Node *clone = new Node();
    clone->val = node->val;

    // mp[node] = clone -> key = node (A, purana),  value = clone (A', naya).  yaani "A ka clone A'"
    mp[node] = clone;
    q.push(node); // A (purana) ko queue me daala

    // STEP 2: BFS -> har node ke neighbours ka clone + JODO
    while (!q.empty())
    {
        Node *curr = q.front(); // curr = A (queue se nikla)
        q.pop();

        for (auto &it : curr->neighbors)
        {
            // (a) neighbour clone NAHI hua? -> abhi banao + map + queue (naya hi push)
            if (mp.count(it) == 0)
            {
                Node *newClone = new Node();
                newClone->val = it->val;

                // B ka clone B' bana -> mp[B] = B'
                mp[it] = newClone;
                q.push(it); // taaki iske neighbours bhi baad me process ho
            }
            // (b) JODO -- curr ke clone ko it ke clone se (ye HAMESHA, naya/purana koi bhi)
            // mp[curr] = A ki map-value = A'   |   mp[it] = B ki map-value = B'
            // A' ke neighbour-list me B' jod diya (clones aapas me jude)
            mp[curr]->neighbors.push_back(mp[it]);
        }
    }
    return mp[node];
}

// ─── TEST HARNESS (isko haath mat lagana) ───────────────────
// graph ko adj-list se banata, aur clone ko verify karta:
//   (1) structure same? (val + neighbour-vals)   (2) DEEP? (pointers alag)
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
    // BFS dono pe saath-saath; val match + pointer ALAG check
    if (!orig && !clone)
        return true;
    if (!orig || !clone)
        return false;
    unordered_map<Node *, Node *> seen; // orig -> clone
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
                return false; // neighbour bhi naya hona chahiye
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
    // T1: 4 nodes square: 1-2, 2-3, 3-4, 4-1, plus 1-3 (LC example)
    vector<vector<int>> a1 = {{}, {2, 4}, {1, 3}, {2, 4}, {1, 3}};
    Node *g1 = buildGraph(4, a1);
    cout << "T1: " << (verify(g1, cloneGraph(g1)) ? "PASS" : "FAIL") << "\n";

    // T2: single node, no neighbours
    vector<vector<int>> a2 = {{}, {}};
    Node *g2 = buildGraph(1, a2);
    cout << "T2: " << (verify(g2, cloneGraph(g2)) ? "PASS" : "FAIL") << "\n";

    // T3: two nodes 1-2
    vector<vector<int>> a3 = {{}, {2}, {1}};
    Node *g3 = buildGraph(2, a3);
    cout << "T3: " << (verify(g3, cloneGraph(g3)) ? "PASS" : "FAIL") << "\n";

    // T4: empty graph
    cout << "T4: " << (verify(nullptr, cloneGraph(nullptr)) ? "PASS" : "FAIL") << "\n";
    return 0;
}
