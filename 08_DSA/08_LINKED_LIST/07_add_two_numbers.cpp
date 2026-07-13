// ============================================================
// ADD TWO NUMBERS — Linked List  (fresh, dummy + carry)
// ============================================================
// do non-empty linked list, har node ek DIGIT. digits REVERSE order me store
// (yaani units-digit pehle). do numbers ADD karke result bhi ussi tarah (reverse) linked-list me lautao.
//   l1 = 2->4->3  (= 342),  l2 = 5->6->4  (= 465)  ->  7->0->8  (= 807)
//   (2+5=7, 4+6=10 -> 0 carry1, 3+4+1=8)
//
// (approach yahan LIKHA NAHI -- code TU likhega. Arpan ki approach BAAD me,
//  jab tu code kare. copy-pen pe trace pehle.)
//
// Tests (l1, l2 -> expected result list):
//   [2,4,3] + [5,6,4]   -> 7 0 8
//   [0] + [0]           -> 0
//   [9,9,9] + [1]       -> 0 0 0 1     (999+1 = 1000)
//   [9,9] + [9,9,9]     -> 8 9 0 1     (99 + 999 = 1098)
//   [5] + [5]           -> 0 1         (5+5 = 10)
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

// ---- tera kaam: yeh function ---- (RESET — dobara khud, blank + copy-pen)
Node *addTwoNumbers(Node *l1, Node *l2)
{
    // tera code yahan -- dummy + carry. copy-pen pe trace.
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
    printList(addTwoNumbers(buildList({2, 4, 3}), buildList({5, 6, 4}))); // 7 0 8
    printList(addTwoNumbers(buildList({0}), buildList({0})));             // 0
    printList(addTwoNumbers(buildList({9, 9, 9}), buildList({1})));       // 0 0 0 1
    printList(addTwoNumbers(buildList({9, 9}), buildList({9, 9, 9})));    // 8 9 0 1
    printList(addTwoNumbers(buildList({5}), buildList({5})));             // 0 1
    return 0;
}
