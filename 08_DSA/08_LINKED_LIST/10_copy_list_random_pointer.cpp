// ============================================================
// COPY LIST WITH RANDOM POINTER — (LeetCode 138)   (fresh)
// ============================================================
// Ek linked list di, har node me val + next + EK EXTRA "random" pointer
// (jo list me KAHIN BHI point kar sakta, ya NULL).
// Iski ek DEEP COPY banao (bilkul naye nodes) — copy ke random pointers
// COPY-list ke andar hi point karein (original ke nodes pe NAHI).
//
// INPUT-format (tests ke liye):
//   vals[] = har node ka value (order me)
//   rnd[]  = har node ke random ka INDEX (0-based) list me; -1 = NULL random
//   -> check() input se original banata, tera copyRandomList() chalata,
//      phir verify: (a) value-order same, (b) random-index same,
//      (c) DEEP hai (copy ke nodes NAYE hain, original wale nahi).
//
//   vals=[7,13,11,10,1]  rnd=[-1,0,4,2,0]   -> wahi structure copy
//   vals=[1,2]           rnd=[1,1]          -> dono ka random node-index 1 pe
//   vals=[3,3,3]         rnd=[-1,0,-1]
//   vals=[]              rnd=[]             -> empty (NULL)
//   vals=[5]             rnd=[-1]           -> single, random NULL
//   vals=[1]             rnd=[0]            -> single, random KHUD pe (self)
//
// FAMILY (nudge, baaki khud): interleave-clone (A->A'->B->B'..) ya
//   hashmap(old->new). random set karne se pehle saare new nodes ban jaayein.
// ============================================================
// ============================================================
#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int val;
    Node *next;
    Node *random;
    Node(int v) : val(v), next(nullptr), random(nullptr) {}
};

Node *copyRandomList(Node *head)
{
    if (head == NULL)
    {
        return NULL;
    }

    unordered_map<Node *, Node *> mp;
    Node *curr = head;
    Node *prev = NULL;
    Node *newHead = NULL;

    // yaha bus humne ek list bana li copy means deep copy banali without Random pointer
    while (curr)
    {
        Node *temp = new Node(curr->val);
        mp[curr] = temp; // random pointre ki mapping
        if (newHead == NULL) // head null hai pehli node
        {
            newHead = temp;
            prev = newHead;
        }
        else
        {
            prev->next = temp;
            prev = temp;
        }
        curr = curr->next;
    }

    // yaha hum random set kar rahe hai theek
    curr = head;
    Node *currRand = newHead;
    while (curr)
    {
        if (curr->random == NULL)
        {
            currRand->random = NULL;
        }
        else
        {
            currRand->random = mp[curr->random];
        }
        curr = curr->next;
        currRand = currRand->next;
    }
    return newHead;
}

// ---------- helpers (chhoo mat) ----------
Node *build(vector<int> vals, vector<int> rnd)
{
    int n = vals.size();
    if (n == 0)
        return NULL;
    vector<Node *> nodes;
    for (int x : vals)
        nodes.push_back(new Node(x));
    for (int i = 0; i < n; i++)
    {
        nodes[i]->next = (i + 1 < n) ? nodes[i + 1] : NULL;
        nodes[i]->random = (rnd[i] == -1) ? NULL : nodes[rnd[i]];
    }
    return nodes[0];
}

void check(vector<int> vals, vector<int> rnd)
{
    Node *orig = build(vals, rnd);
    Node *cp = copyRandomList(orig);

    // original node addresses (deep-copy check ke liye)
    set<Node *> origSet;
    for (Node *t = orig; t; t = t->next)
        origSet.insert(t);

    // copy ke nodes gather + index-map
    vector<Node *> cpNodes;
    for (Node *t = cp; t; t = t->next)
        cpNodes.push_back(t);
    map<Node *, int> cpIdx;
    for (int i = 0; i < (int)cpNodes.size(); i++)
        cpIdx[cpNodes[i]] = i;

    bool ok = true;
    string why = "";

    // (1) length same?
    if ((int)cpNodes.size() != (int)vals.size())
    {
        ok = false;
        why = "length mismatch";
    }
    for (int i = 0; ok && i < (int)cpNodes.size(); i++)
    {
        // (2) value same?
        if (cpNodes[i]->val != vals[i])
        {
            ok = false;
            why = "val mismatch @" + to_string(i);
            break;
        }
        // (3) DEEP? copy node original wala nahi hona chahiye
        if (origSet.count(cpNodes[i]))
        {
            ok = false;
            why = "shallow (original node reused) @" + to_string(i);
            break;
        }
        // (4) random sahi index pe (copy-list ke andar)?
        int expIdx = rnd[i];
        if (cpNodes[i]->random == NULL)
        {
            if (expIdx != -1)
            {
                ok = false;
                why = "random NULL but expected idx @" + to_string(i);
                break;
            }
        }
        else
        {
            if (!cpIdx.count(cpNodes[i]->random))
            {
                ok = false;
                why = "random points OUTSIDE copy (shallow random) @" + to_string(i);
                break;
            }
            if (cpIdx[cpNodes[i]->random] != expIdx)
            {
                ok = false;
                why = "random idx mismatch @" + to_string(i);
                break;
            }
        }
    }

    cout << (ok ? "PASS" : "FAIL");
    if (!ok)
        cout << "  (" << why << ")";
    cout << "\n";
}

int main()
{
    check({7, 13, 11, 10, 1}, {-1, 0, 4, 2, 0});
    check({1, 2}, {1, 1});
    check({3, 3, 3}, {-1, 0, -1});
    check({}, {});
    check({5}, {-1});
    check({1}, {0});
    return 0;
}
