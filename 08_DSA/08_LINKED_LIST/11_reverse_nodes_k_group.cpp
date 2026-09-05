// ============================================================
// REVERSE NODES IN K-GROUP — Linked List  (LC-25, fresh)
// ============================================================
// linked list di hai aur ek number k. list ke har k-node ke GROUP ko
// aapas me reverse karo (actual NODES, values nahi). naya head lauta.
//
// ★ RULE: agar aakhir me k se KAM node bache (poora group nahi banta),
//   to un bache-hue node ko WAISE HI chhod do (reverse mat karo).
//
//   1->2->3->4->5   k=2   ->  2 1 4 3 5     (aakhri akela 5 waise hi)
//   1->2->3->4->5   k=3   ->  3 2 1 4 5     (bache 4,5 < 3 -> waise hi)
//   1->2->3->4->5   k=1   ->  1 2 3 4 5     (koi change nahi)
//
// Tests (list, k -> expected):
//   [1,2,3,4,5], k=2   -> 2 1 4 3 5
//   [1,2,3,4,5], k=3   -> 3 2 1 4 5
//   [1,2,3,4,5], k=1   -> 1 2 3 4 5
//   [1,2,3,4,5,6], k=3 -> 3 2 1 6 5 4
//   [1,2,3], k=4       -> 1 2 3        (k > length -> koi group nahi)
//   [1], k=1           -> 1
//   [], k=2            -> (empty)
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

Node *reverseKGroup(Node *head, int k)
{
    if (head == NULL)
    {
        return NULL;
    }

    Node *check = head;
    int countNodes = 0;
    while (check != NULL && countNodes < k)
    {
        check = check->next;
        countNodes++;
    }

    if (countNodes != k)
    {
        return head;
    }

    int count = 0;
    Node *prev = NULL;
    Node *curr = head;
    Node *next = NULL;
    while (curr && count < k)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
        count++;
    }

    if (next != NULL)
    {
        head->next = reverseKGroup(next, k);
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
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5}), 2));    // 2 1 4 3 5
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5}), 3));    // 3 2 1 4 5
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5}), 1));    // 1 2 3 4 5
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5, 6}), 3)); // 3 2 1 6 5 4
    printList(reverseKGroup(buildList({1, 2, 3}), 4));          // 1 2 3
    printList(reverseKGroup(buildList({1}), 1));                // 1
    printList(reverseKGroup(buildList({}), 2));                 // (empty)
    return 0;
}
