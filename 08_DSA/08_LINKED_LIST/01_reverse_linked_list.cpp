// ============================================================
// REVERSE LINKED LIST — Linked List (pattern #1, foundational)
// ============================================================
// singly linked list ka head diya. use ULTA (reverse) karke naya head lautao.
//   1 -> 2 -> 3 -> 4 -> 5   =>   5 -> 4 -> 3 -> 2 -> 1
//
// (approach LIKHA NAHI -- poora tera. copy-pen pe ARROW-diagram bana:
//  har node ke beech ke arrow ki DIRECTION ulti karni hai. 3 pointer socho.)
//
// Tests (// expected):
//   [1,2,3,4,5]  -> 5 4 3 2 1
//   [1,2]        -> 2 1
//   [1]          -> 1
//   []           -> (khali)
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
Node *reverseList(Node *head)
{
    Node *prev = NULL;
    Node *curr = head;
    Node *nextt = head;
    while (curr != NULL)
    {
        nextt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextt;
    }
    return prev;
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
    vector<vector<int>> tests = {
        {1, 2, 3, 4, 5},
        {1, 2},
        {1},
        {}};

    for (auto &t : tests)
        printList(reverseList(buildList(t)));
    // expected:
    // 5 4 3 2 1
    // 2 1
    // 1
    // (khali line)
    return 0;
}
