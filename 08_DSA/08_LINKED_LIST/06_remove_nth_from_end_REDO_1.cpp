// ============================================================
// REMOVE Nth NODE FROM END — Linked List  (LC-19, REDO_1 — blank)
// ============================================================
// head aur n diya. list ke END se Nth node HATAO -> naya head lautao.
//   [1,2,3,4,5], n=2  ->  [1,2,3,5]   (end se 2nd = "4" hata)
//   [1], n=1          ->  []          (single node hata -> empty)
// n hamesha 1 se list ki length tak.
//
// Tests (values, n -> expected):
//   [1,2,3,4,5], n=2  -> 1 2 3 5
//   [1,2,3,4,5], n=5  -> 2 3 4 5   (head hata)
//   [1,2,3,4,5], n=1  -> 1 2 3 4   (tail hata)
//   [1], n=1          -> (khali)
//   [1,2], n=2        -> 2
//   [1,2], n=1        -> 1
//   [1,2,3], n=2      -> 1 3
//   [7,8,9,10], n=4   -> 8 9 10
//   [7,8,9,10], n=3   -> 7 9 10
//   [5,6,7,8,9,10], n=6 -> 6 7 8 9 10
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

// ---- kaam: yeh function ----
Node *removeNthFromEnd(Node *head, int n)
{
    Node *slow = head;
    Node *fast = head;
    for (int i = 1; i <= n; i++)
    {
        fast = fast->next;
    }

    if (fast == NULL)
    {
        return head->next;
    }

    while (fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next;
    }
    slow->next = slow->next->next;
    return head;
}

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
    printList(removeNthFromEnd(buildList({1, 2, 3, 4, 5}), 2));     // 1 2 3 5
    printList(removeNthFromEnd(buildList({1, 2, 3, 4, 5}), 5));     // 2 3 4 5
    printList(removeNthFromEnd(buildList({1, 2, 3, 4, 5}), 1));     // 1 2 3 4
    printList(removeNthFromEnd(buildList({1}), 1));                 // (khali)
    printList(removeNthFromEnd(buildList({1, 2}), 2));              // 2
    printList(removeNthFromEnd(buildList({1, 2}), 1));              // 1
    printList(removeNthFromEnd(buildList({1, 2, 3}), 2));           // 1 3
    printList(removeNthFromEnd(buildList({7, 8, 9, 10}), 4));       // 8 9 10
    printList(removeNthFromEnd(buildList({7, 8, 9, 10}), 3));       // 7 9 10
    printList(removeNthFromEnd(buildList({5, 6, 7, 8, 9, 10}), 6)); // 6 7 8 9 10
    return 0;
}
