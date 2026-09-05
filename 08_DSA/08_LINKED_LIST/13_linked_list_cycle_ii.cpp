// ============================================================
// LINKED LIST CYCLE II (LC-142) — Linked List / FAST-SLOW (Floyd phase-2, ENTRY node)
// ============================================================
// head diya. Agar list me CYCLE hai to us cycle ke START (entry) node ka
// POINTER return karo. Cycle nahi -> return NULL.
//
// ---- ARPAN KI APPROACH (VERIFIED 5/5) ----
//  = DETECT CYCLE (03) + DELTA: sirf true/false nahi, cycle-ENTRY node chahiye.
//  ye WAHI phase-2 hai jo find-duplicate (12) me kiya — bas ab Node* pe (nums[i] ki jagah ->next).
//  PHASE-1 (meet): slow=slow->next (1), fast=fast->next->next (2) -> slow==fast (cycle).
//                  fast ya fast->next NULL -> no cycle -> return NULL.
//  PHASE-2 (entry): ek pointer head pe reset -> dono 1-1 kadam (p=p->next, slow=slow->next) -> mile = ENTRY.
//  KYUN: meet-point se aur head se cycle-entry tak distance BARABAR (Floyd).
//
// INPUT format: values[] + pos (cyclePos). pos = -1 -> no cycle; warna last node ka
//               ->next us index (pos) wale node se jud jaata. Return = ENTRY node ka val.
//
// TEST CASES (values, pos -> expected entry-val;  -1 = NULL/no-cycle):
//   [3,2,0,-4], pos=1    -> 2
//   [1,2],      pos=0    -> 1
//   [1],        pos=0    -> 1   (self loop)
//   [1,2,3],    pos=-1   -> -1  (no cycle -> NULL)
//   [1,2,3,4,5],pos=2    -> 3
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int v) : val(v), next(nullptr) {}
};

ListNode *detectCycle(ListNode *head)
{
    if (head == NULL)
        return NULL;

    ListNode *slow = head;
    ListNode *fast = head;
    while (fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast)
        {
            break;
        }
    }

    if (slow != fast)
    {
        return NULL;
    }

    slow = head;
    while (slow != fast)
    {
        slow = slow->next;
        fast = fast->next;
    }
    return slow;
}

// ---- test harness (list build with optional cycle) ----
void check(vector<int> vals, int pos, int expected)
{
    vector<ListNode *> nodes;
    for (int v : vals)
        nodes.push_back(new ListNode(v));
    for (int i = 0; i + 1 < (int)nodes.size(); i++)
        nodes[i]->next = nodes[i + 1];
    if (pos >= 0 && !nodes.empty())
        nodes.back()->next = nodes[pos]; // cycle jodo

    ListNode *got = detectCycle(nodes.empty() ? nullptr : nodes[0]);
    int gotVal = got ? got->val : -1;
    cout << (gotVal == expected ? "PASS" : "FAIL")
         << "  got=" << gotVal << "  exp=" << expected << "\n";
}

int main()
{
    check({3, 2, 0, -4}, 1, 2);
    check({1, 2}, 0, 1);
    check({1}, 0, 1);
    check({1, 2, 3}, -1, -1);
    check({1, 2, 3, 4, 5}, 2, 3);
    return 0;
}
