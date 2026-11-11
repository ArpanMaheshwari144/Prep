// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// REVERSE NODES IN K-GROUP — Linked List  (LC-25, REDO_2 — blank)
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
//   [1,2,3,4], k=2     -> 2 1 4 3      (poore group, kuch nahi bachta)
//   [1,2,3,4,5,6,7,8], k=3 -> 3 2 1 6 5 4 7 8
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

    Node *currNode = head;
    int countNodes = 0;
    while (currNode && countNodes < k)
    {
        currNode = currNode->next;
        countNodes++;
    }

    if (countNodes != k)
    {
        return head;
    }

    int count = 0;
    Node *prev = NULL;
    Node *curr = head;
    Node *nextt = head;
    while (curr && count < k)
    {
        nextt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextt;
        count++;
    }

    if (nextt != NULL)
    {
        head->next = reverseKGroup(nextt, k);
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
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5}), 2));          // 2 1 4 3 5
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5}), 3));          // 3 2 1 4 5
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5}), 1));          // 1 2 3 4 5
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5, 6}), 3));       // 3 2 1 6 5 4
    printList(reverseKGroup(buildList({1, 2, 3}), 4));                // 1 2 3
    printList(reverseKGroup(buildList({1}), 1));                      // 1
    printList(reverseKGroup(buildList({}), 2));                       // (empty)
    printList(reverseKGroup(buildList({1, 2, 3, 4}), 2));             // 2 1 4 3
    printList(reverseKGroup(buildList({1, 2, 3, 4, 5, 6, 7, 8}), 3)); // 3 2 1 6 5 4 7 8
    return 0;
}
