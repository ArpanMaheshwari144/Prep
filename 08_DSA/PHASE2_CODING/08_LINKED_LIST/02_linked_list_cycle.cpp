// ============================================================
// LINKED LIST CYCLE   —   Linked List (slow / fast pointer)
// ============================================================
// Pata karo list mein CYCLE (loop) hai ya nahi. true / false.
//   1->2->3->4->2(wapas 2 pe)   -> true   (cycle)
//   1->2->3->4->NULL            -> false  (no cycle)
//
// SIGNAL: "cycle hai ya nahi (bina extra space)" -> SLOW + FAST pointer (Floyd's).
//   Soch (Middle-of-LL wala SAME tool -- slow 1, fast 2):
//     - agar cycle HAI -> fast (tej) ghoomte-ghoomte slow ko PAKAD lega (mil jayenge).
//     - agar cycle NAHI -> fast NULL pe pahunch jayega (end mil gaya).
//     - kab true, kab false -- ye tu condition se nikaalega. No code.
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
bool hasCycle(ListNode *head)
{
    ListNode *slow = head;
    ListNode *fast = head;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            return true;
        }
    }
    return false;
}

// ---- helpers (mat chhu) ----
// values se list banao; cyclePos = -1 (no cycle) ya us index pe tail ko jodo
ListNode *build(vector<int> v, int cyclePos)
{
    ListNode *head = nullptr, *tail = nullptr;
    vector<ListNode *> nodes;
    for (int x : v)
    {
        ListNode *n = new ListNode(x);
        nodes.push_back(n);
        if (!head)
            head = tail = n;
        else
        {
            tail->next = n;
            tail = n;
        }
    }
    if (cyclePos != -1 && tail)
        tail->next = nodes[cyclePos]; // loop jodo
    return head;
}

int main()
{
    cout << hasCycle(build({3, 2, 0, -4}, 1)) << endl; // expected 1 (cycle at idx1)
    cout << hasCycle(build({1, 2}, 0)) << endl;        // expected 1 (cycle at idx0)
    cout << hasCycle(build({1, 2, 3, 4}, -1)) << endl; // expected 0 (no cycle)
    cout << hasCycle(build({1}, -1)) << endl;          // expected 0 (single, no cycle)
    cout << hasCycle(build({1}, 0)) << endl;           // expected 1 (single -> self loop)
    cout << hasCycle(build({}, -1)) << endl;           // expected 0 (empty)
    return 0;
}
