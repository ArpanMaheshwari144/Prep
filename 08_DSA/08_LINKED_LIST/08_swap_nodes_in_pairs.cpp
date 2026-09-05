// ============================================================
// SWAP NODES IN PAIRS — Linked List  (fresh)
// ============================================================
// linked list di hai. har DO adjacent node ki JODI ko aapas me SWAP karo
// (values swap nahi -- actual NODES swap). naya head lauta.
//   1->2->3->4        ->  2->1->4->3
//   1->2->3->4->5     ->  2->1->4->3->5   (aakhri akela node waise hi)
//   1                 ->  1
//   (empty)           ->  (empty)



// Tests (list -> expected):
//   [1,2,3,4]     -> 2 1 4 3
//   [1,2,3,4,5]   -> 2 1 4 3 5
//   [1,2]        -> 2 1
//   [1]           -> 1
//   []            -> (empty)
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

Node *swapPairs(Node *head)
{
    if (!head)
    {
        return nullptr;
    }

    Node *dummy = new Node(0);
    Node *prev = dummy;
    prev->next = head;
    Node *first = head;
    while (prev->next != NULL && first->next != NULL)
    {
        Node *second = first->next;
        Node *temp = second->next;

        // rotate kar diya
        second->next = first;
        first->next = temp;

        // head badal diya ba ye hia head
        prev->next = second;

        // bus pointers shift
        prev = first;
        first = temp;
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
    printList(swapPairs(buildList({1, 2, 3, 4})));    // 2 1 4 3
    printList(swapPairs(buildList({1, 2, 3, 4, 5}))); // 2 1 4 3 5
    printList(swapPairs(buildList({1, 2})));          // 2 1
    printList(swapPairs(buildList({1})));             // 1
    printList(swapPairs(buildList({})));
    return 0;
}
