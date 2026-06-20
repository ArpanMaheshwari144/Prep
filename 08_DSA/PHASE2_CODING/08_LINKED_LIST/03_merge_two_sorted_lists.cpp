// ============================================================
// MERGE TWO SORTED LISTS   —   Linked List
// ============================================================
// Do SORTED linked lists di hain. Inhe ek SORTED list mein merge karo, naya head do.
//   l1: 1->2->4      l2: 1->3->4    ->    1->1->2->3->4->4
//   l1: (empty)      l2: 0          ->    0
//
// SIGNAL: "do sorted lists ko ek sorted mein jodo" -> two-pointer on lists + DUMMY node.
//   Soch (structure TERA):
//     - dono lists ke aage ek-ek pointer. Jo chhota -> use result mein jodo, woh pointer aage.
//     - result banate waqt "head" alag se sambhalna jhanjhat -> ek DUMMY (fake) node
//       se shuru karo, uske aage jodte jao, end mein dummy->next return.
//     - ek list khatam -> doosri ki BACHI poonch seedha jod do (woh already sorted).
//   No code. Copy pe 1->2->4 aur 1->3->4 ko haath se merge kar.
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
ListNode *mergeTwoLists(ListNode *l1, ListNode *l2)
{
    if (l1 == nullptr && l2 == nullptr)
    {
        return nullptr;
    }
    if (l1 == nullptr)
    {
        return l2;
    }
    if (l2 == nullptr)
    {
        return l1;
    }

    ListNode *dummy = new ListNode(0);
    ListNode *curr = dummy;
    while (l1 != nullptr && l2 != nullptr)
    {
        if (l1->val <= l2->val)
        {
            curr->next = l1;
            curr = curr->next;
            l1 = l1->next;
        }
        else if (l1->val >= l2->val)
        {
            curr->next = l2;
            curr = curr->next;
            l2 = l2->next;
        }
    }
    while (l1)
    {
        curr->next = l1;
        curr = curr->next;
        l1 = l1->next;
    }
    while (l2)
    {
        curr->next = l2;
        curr = curr->next;
        l2 = l2->next;
    }
    return dummy->next;
}

// ---- helpers (mat chhu) ----
ListNode *build(vector<int> v)
{
    ListNode *head = nullptr, *tail = nullptr;
    for (int x : v)
    {
        ListNode *n = new ListNode(x);
        if (!head)
            head = tail = n;
        else
        {
            tail->next = n;
            tail = n;
        }
    }
    return head;
}
void print(ListNode *head)
{
    while (head)
    {
        cout << head->val;
        if (head->next)
            cout << " ";
        head = head->next;
    }
    cout << endl;
}

int main()
{
    print(mergeTwoLists(build({1, 2, 4}), build({1, 3, 4}))); // expected: 1 1 2 3 4 4
    print(mergeTwoLists(build({}), build({0})));              // expected: 0
    print(mergeTwoLists(build({}), build({})));               // expected: (empty line)
    print(mergeTwoLists(build({5}), build({1, 2, 3})));       // expected: 1 2 3 5
    print(mergeTwoLists(build({1, 3, 5}), build({2, 4, 6}))); // expected: 1 2 3 4 5 6
    return 0;
}
