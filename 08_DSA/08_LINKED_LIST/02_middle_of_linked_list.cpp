// ============================================================
// MIDDLE OF THE LINKED LIST — Linked List (pattern #2)
// ============================================================
// singly linked list ka head diya. BEECH wala (middle) node lautao.
// agar 2 middle ho (EVEN length) -> DOOSRA middle lautao.
//   [1,2,3,4,5]    -> 3
//   [1,2,3,4,5,6]  -> 4   (do middle 3 aur 4 -> doosra = 4)
//
// (approach LIKHA NAHI -- poora tera. apne tarike se: count karke ja sakta,
//  ya ek elegant one-pass trick hai (2 pointer, ek tez ek dheema) -- socho.
//  copy-pen pe [1,2,3,4,5] trace kar pehle.)
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
