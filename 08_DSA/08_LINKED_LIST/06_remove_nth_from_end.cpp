// ============================================================
// REMOVE Nth NODE FROM END — Linked List  (fresh, fast/slow GAP variant)
// ============================================================
// head aur n diya. list ke END se Nth node HATAO -> naya head lautao.
//   [1,2,3,4,5], n=2  ->  [1,2,3,5]   (end se 2nd = "4" hata)
//   [1], n=1          ->  []          (single node hata -> empty)
//
// ---- ARPAN KI APPROACH ----
//  fast/slow GAP variant: pehle fast ko n kadam aage bhejo (gap ban gaya).
//  edge: agar fast ab NULL -> matlab HEAD hi hatana tha -> return head->next.
//  warna fast->next NULL hone tak dono saath chalao; ab slow, hatane wale ke
//  THEEK PEHLE ruka -> slow->next = slow->next->next se node bypass. head lautao.
//
// Tests (values, n -> expected):
//   [1,2,3,4,5], n=2  -> 1 2 3 5
//   [1,2,3,4,5], n=5  -> 2 3 4 5   (head hata)
//   [1,2,3,4,5], n=1  -> 1 2 3 4   (tail hata)
//   [1], n=1          -> (khali)
//   [1,2], n=2        -> 2
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
Node *removeNthFromEnd(Node *head, int n)
{
    Node *slow = head;
    Node *fast = head; // {1, 2, 3, 4, 5}), 5)

    for (int i = 1; i <= n; i++)
    {
        fast = fast->next;
    }
    // cout << "->" << fast->val << endl;

    if (fast == nullptr)
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
    printList(removeNthFromEnd(buildList({1, 2, 3, 4, 5}), 2)); // 1 2 3 5
    printList(removeNthFromEnd(buildList({1, 2, 3, 4, 5}), 5)); // 2 3 4 5
    printList(removeNthFromEnd(buildList({1, 2, 3, 4, 5}), 1)); // 1 2 3 4
    printList(removeNthFromEnd(buildList({1}), 1));             // (khali)
    printList(removeNthFromEnd(buildList({1, 2}), 2));          // 2
    return 0;
}
