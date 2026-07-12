// ============================================================
// MERGE TWO SORTED LISTS — Linked List (pattern #5, DUMMY node)
// ============================================================
// do SORTED linked lists (a, b) diye. inhe MERGE karke ek SORTED list banao,
// naya head lautao.
//   a: 1->2->4   b: 1->3->4   =>   1->1->2->3->4->4
//
// ---- ARPAN KI APPROACH ----
//  DUMMY node trick: fake dummy banao, tail usi pe. jab tak dono list zinda ->
//  a aur b ka val compare, CHHOTA wala tail->next me jodo, us list ko + tail ko aage.
//  ek list khatam -> jo doosri bachi use seedha tail->next se latka do.
//  answer = dummy->next (fake head skip). node naye nahi bante, bas link jodte.
//
// Tests (list-a, list-b -> expected):
//   [1,2,4] , [1,3,4]  -> 1 1 2 3 4 4
//   [1,3,5] , [2,4,6]  -> 1 2 3 4 5 6
//   []      , [0]      -> 0
//   [1,2,3] , []       -> 1 2 3
//   []      , []       -> (khali)
// ============================================================

#include <iostream>
#include <vector>
using namespace std;

struct Node
{
    int val;
    Node *next;
    Node(int v) : val(v), next(nullptr) {}
};

// ---- tera kaam: yeh function ----
Node *mergeTwoLists(Node *a, Node *b)
{
    // [1,2,4] , [1,3,4]  -> 1 1 2 3 4 4
    Node *dummy = new Node(0);
    Node *tail = dummy;

    while (a != NULL && b != NULL)
    {
        if (a->val <= b->val)
        {
            tail->next = a;
            a = a->next;
            tail = tail->next;
        }
        else if (b->val < a->val)
        {
            tail->next = b;
            b = b->next;
            tail = tail->next;
        }
    }

    while (a != NULL)
    {
        tail->next = a;
        a = a->next;
        tail = tail->next;
    }

    while (b != NULL)
    {
        tail->next = b;
        b = b->next;
        tail = tail->next;
    }

    return dummy->next;
}

// ---------- helpers (boilerplate, chhoo mat) ----------
Node *buildList(vector<int> v)
{
    Node *dummy = new Node(0), *tail = dummy;
    for (int x : v)
    {
        tail->next = new Node(x);
        tail = tail->next;
    }
    return dummy->next;
}

void printList(Node *head)
{
    for (Node *c = head; c; c = c->next)
        cout << c->val << " ";
    cout << "\n";
}

int main()
{
    vector<pair<vector<int>, vector<int>>> tests = {
        {{1, 2, 4}, {1, 3, 4}},
        {{1, 3, 5}, {2, 4, 6}},
        {{}, {0}},
        {{1, 2, 3}, {}},
        {{}, {}}};

    for (auto &t : tests)
        printList(mergeTwoLists(buildList(t.first), buildList(t.second)));
    // expected:
    // 1 1 2 3 4 4
    // 1 2 3 4 5 6
    // 0
    // 1 2 3
    // (khali line)
    return 0;
}
