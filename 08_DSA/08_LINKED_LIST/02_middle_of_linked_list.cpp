// ============================================================
// MIDDLE OF THE LINKED LIST — Linked List (pattern #2)
// ============================================================
// singly linked list ka head diya. BEECH wala (middle) node lautao.
// agar 2 middle ho (EVEN length) -> DOOSRA middle lautao.
//   [1,2,3,4,5]    -> 3
//   [1,2,3,4,5,6]  -> 4   (do middle 3 aur 4 -> doosra = 4)
//
// ---- ARPAN KI APPROACH ----
//  fast/slow: slow=head, fast=head. jab tak fast aur fast->next zinda ->
//  slow 1 kadam, fast 2 kadam. fast end pe pahunche tab slow BEECH me.
//  even length me slow apne aap DOOSRE middle pe ruk jaata. ek hi pass.
//
// Tests (// expected):
//   [1,2,3,4,5]    -> 3
//   [1,2,3,4,5,6]  -> 4
//   [1]            -> 1
//   [1,2]          -> 2
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
Node *middleNode(Node *head)
{
    Node *slow = head;
    Node *fast = head;

    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
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

int main()
{
    vector<vector<int>> tests = {
        {1, 2, 3, 4, 5},
        {1, 2, 3, 4, 5, 6},
        {1},
        {1, 2}};

    for (auto &t : tests)
    {
        Node *mid = middleNode(buildList(t));
        cout << (mid ? mid->val : -1) << "\n";
    }
    // expected:
    // 3
    // 4
    // 1
    // 2
    return 0;
}
