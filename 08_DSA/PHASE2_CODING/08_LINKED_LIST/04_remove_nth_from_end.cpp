// ============================================================
// REMOVE Nth NODE FROM END   —   Linked List (two-pointer GAP + dummy)
// ============================================================
// List ka END se Nth node hata do, naya head do.
//   1->2->3->4->5,  n=2   ->  1->2->3->5     (end se 2nd = 4 hata)
//   1,               n=1   ->  (empty)         (akela node hata)
//   1->2,            n=1   ->  1               (end se 1st = 2 hata)
//   1->2,            n=2   ->  2               (end se 2nd = head hata)
//
// SIGNAL: "end se Nth / ek hi pass mein" -> two-pointer with a GAP of n + dummy.
//   Soch (slow/fast + dummy dono tere owned tools):
//     - fast ko PEHLE n kadam aage bhej do. Phir slow+fast SAATH chalao.
//       fast end pe -> slow theek us node ke AAGE jise hatana hai (uska pichla).
//     - head hi hat sakta (n = length) -> DUMMY node head ke aage lagao,
//       taaki "pichla node" hamesha mile (edge clean).
//   No code. Copy pe 1->2->3->4->5, n=2 haath se chala.
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
ListNode *removeNthFromEnd(ListNode *head, int n)
{
    // ListNode *dummy = head;
    ListNode *slow = head;
    ListNode *fast = head;

    for (int i = 1; i <= n; i++)
    {
        fast = fast->next;
    }

    if (fast == nullptr)
    {
        return head->next;
    }

    while (fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next;
    }
    slow->next = slow->next->next;

    return head;
}

// ---- helpers (mat chhu) ----
ListNode *build(vector<int> v)
{
    ListNode *head = nullptr, *tail = nullptr;
    for (int x : v)
    {
        ListNode *node = new ListNode(x);
        if (!head)
            head = tail = node;
        else
        {
            tail->next = node;
            tail = node;
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
    print(removeNthFromEnd(build({1, 2, 3, 4, 5}), 2)); // expected: 1 2 3 5
    print(removeNthFromEnd(build({1}), 1));             // expected: (empty line)
    print(removeNthFromEnd(build({1, 2}), 1));          // expected: 1
    print(removeNthFromEnd(build({1, 2}), 2));          // expected: 2   (head hata)
    print(removeNthFromEnd(build({1, 2, 3}), 3));       // expected: 2 3 (head hata)
    return 0;
}
