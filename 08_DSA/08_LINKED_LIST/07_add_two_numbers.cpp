// ============================================================
// ADD TWO NUMBERS — Linked List  (fresh, dummy + carry)
// ============================================================
// do non-empty linked list, har node ek DIGIT. digits REVERSE order me store
// (yaani units-digit pehle). do numbers ADD karke result bhi ussi tarah (reverse) linked-list me lautao.
//   l1 = 2->4->3  (= 342),  l2 = 5->6->4  (= 465)  ->  7->0->8  (= 807)
//   (2+5=7, 4+6=10 -> 0 carry1, 3+4+1=8)
//


// ---- ARPAN KI APPROACH ----
//  ye MERGE-TWO-SORTED jaisa hi laga (dummy + tail) -- bas isme SUM aur CARRY ka khel extra.
//  pehle dummy aur tail liye.
//  while loop me a, b AUR carry -- ★ || condition kyun? -> ek list khatam ho to bhi doosri (aur carry)
//     ko process karna hai; carry last tak values badal sakta hai (999+1 -> naya digit).
//  andar: sum lo. a hai to sum += a->val + a ko a->next; same b ke liye. phir sum me CARRY bhi add karo.
//  ab naya node kaise? -> maan sum = 12: node "2" ka banega, "1" carry jayega.
//     ★ digit = sum % 10 (2 wali node -> tail se jodo, tail aage).  carry = sum / 10 (1 nikla).
//  baaki while loop ye dobara karta jaata. return dummy->next.




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

Node *addTwoNumbers(Node *a, Node *b)
{
    Node *dummy = new Node(0);
    Node *tail = dummy;
    int carry = 0;

    while (a || b || carry > 0)
    {
        int sum = 0;
        if (a != nullptr)
        {
            sum += (a->val);
            a = a->next;
        }
        if (b != nullptr)
        {
            sum += (b->val);
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
