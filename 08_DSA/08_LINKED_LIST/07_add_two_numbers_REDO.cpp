// ============================================================
// REDO — ADD TWO NUMBERS   (blank se, dekhe bina)
// ============================================================
// do non-empty linked list, har node ek DIGIT, REVERSE order me (units-digit pehle).
// dono numbers ADD karke result bhi ussi tarah (reverse) linked-list me lautao.
//   l1 = 2->4->3 (=342),  l2 = 5->6->4 (=465)  ->  7->0->8  (=807)
//

// ---- ARPAN KI APPROACH ----
//  ye easy hai -- MERGE-TWO-SORTED jaisa hi (dummy + tail) -- bas isme sum aur CARRY ka thoda extra khel.
//  a aur b ki val jodo: sum += a->val (a hai to), same sum += b->val.
//  phir CARRY bhi add karo -> sum += carry.  (★ ye yaad rakhna wali cheez hai.)
//  node kaise banao? -> maan sum = 12 aaya. node "2" ki banegi, "1" carry jayega:
//     digit = sum % 10 (=2, node),   carry = sum / 10 (=1).
//  bas trick itni hi -- aur ye pattern kaafi aur questions me bhi aata hai.
//  (★ while(a || b || carry) -> ek list khatam ho to bhi doosri + carry process ho; 999+1 = naya digit.)


//
// Tests (l1, l2 -> expected):
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

// tera code yahan:

Node *addTwoNumbers(Node *a, Node *b)
{
    Node *dummy = new Node(0);
    Node *tail = dummy;
    int carry = 0;

    while (a != NULL || b != NULL || carry > 0)
    {
        int sum = 0;
        if (a != nullptr)
        {
            sum += a->val;
            a = a->next;
        }
        if (b != nullptr)
        {
            sum += b->val;
            b = b->next;
        }
        sum += carry;
        tail->next = new Node(sum % 10);
        tail = tail->next;
        carry = sum / 10;
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
    printList(addTwoNumbers(buildList({2, 4, 3}), buildList({5, 6, 4}))); // 7 0 8
    printList(addTwoNumbers(buildList({0}), buildList({0})));             // 0
    printList(addTwoNumbers(buildList({9, 9, 9}), buildList({1})));       // 0 0 0 1
    printList(addTwoNumbers(buildList({9, 9}), buildList({9, 9, 9})));    // 8 9 0 1
    printList(addTwoNumbers(buildList({5}), buildList({5})));             // 0 1
    return 0;
}
