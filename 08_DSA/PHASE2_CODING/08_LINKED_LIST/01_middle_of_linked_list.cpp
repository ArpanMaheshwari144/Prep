// ============================================================
// MIDDLE OF THE LINKED LIST   —   Linked List (slow / fast pointer)
// ============================================================
// Singly linked list ka MIDDLE node return karo (uski value).
// Do middle ho (even length) -> DOOSRA (second) middle do.
//   1->2->3->4->5        -> 3        (5 nodes, beech wala)
//   1->2->3->4->5->6     -> 4        (6 nodes, 2 middle [3,4] -> doosra = 4)
//
// SIGNAL: "middle / half / cycle without counting length" -> SLOW + FAST pointer.
//   Soch (structure TERA -- copy pe 2 pointer ARROWS chala ke dekh, tera spatial mode):
//     - ek POINTER 1 kadam, doosra 2 kadam chale.
//     - fast aakhir tak pahunche -> slow theek BEECH mein hoga.
//     - kab rukna (fast NULL? fast->next NULL?) -- dono test case khud dry-run kar.
//   No pseudo-code. Copy pe odd aur even dono chala ke dekh.
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// ---- TERA kaam: yeh function ----
ListNode *middleNode(ListNode *head)
{
    ListNode* slow = head;
    ListNode* fast = head;
    while(fast != NULL && fast->next != NULL){
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// ---- helpers (mat chhu) ----
ListNode *build(vector<int> v)
{
    ListNode *head = nullptr, *tail = nullptr;
    for (int x : v)
    {
        ListNode *n = new ListNode(x);
        if (!head) head = tail = n;
        else { tail->next = n; tail = n; }
    }
    return head;
}

int main()
{
    cout << middleNode(build({1, 2, 3, 4, 5}))->val << endl;       // expected 3
    cout << middleNode(build({1, 2, 3, 4, 5, 6}))->val << endl;    // expected 4
    cout << middleNode(build({1}))->val << endl;                    // expected 1
    cout << middleNode(build({1, 2}))->val << endl;                 // expected 2
    cout << middleNode(build({10, 20, 30}))->val << endl;          // expected 20
    return 0;
}
