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

// ---- ARPAN KI APPROACH ----
//  = REVERSE-LL (3-pointer engine) + RECURSION. do known tukde.
//
//  STEP 1 — count-FIRST (leftover guard): k node GINO. countNodes != k -> return head
//     (bache node WAISE HI, reverse nahi).
//  STEP 2 — REVERSE k: 3-pointer (prev/curr/next), count<k tak.  [= reverse-LL wala loop]
//     baad me -> prev = NAYA HEAD · head = is group ka TAIL · curr/next = agle group ka pehla.
//  STEP 3 — RECURSE stitch: head->next = reverseKGroup(next, k);  return prev;
//
//  VISUAL ( 1 -> 2 -> 3 -> 4 -> 5 , k=2 ):
//     GRP1 [1,2]:  rev -> 2 -> 1   (prev=2 = HEAD, 1 = TAIL) ; 1 -> reverseKGroup(3 4 5)
//     GRP2 [3,4]:  rev -> 4 -> 3   (3 = TAIL)                ; 3 -> reverseKGroup(5)
//     GRP3 [5]:    count=1 < 2 -> return 5   (leftover, waise hi)
//     JODO:  2 -> 1 -> 4 -> 3 -> 5
//
//  crux: count-FIRST + head-BANTA-TAIL + RECURSE-stitch.

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
