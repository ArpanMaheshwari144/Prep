// ============================================================
// MERGE K SORTED LISTS — (LeetCode 23, Hard)   [HEAP #3 · min-heap of heads]
// ============================================================
// k sorted linked lists diye -> sabko merge karke EK sorted list lautao.
//
//   [[1,4,5],[1,3,4],[2,6]]  ->  [1,1,2,3,4,4,5,6]
//   []                       ->  []
//   [[]]                     ->  []
//   [[1]]                    ->  [1]
//
//   ★ idea: min-heap me har list ka current HEAD. pop smallest -> result me jodo -> uska NEXT push. repeat.
//   heap me max k nodes -> O(N log k).
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// min-heap of ListNode* -> value se compare (a->val > b->val = MIN-heap: chhota top pe)
struct Compare
{
    bool operator()(ListNode *a, ListNode *b) { return a->val > b->val; }
};

// ---- APPROACH ----  (MIN-HEAP of current HEADS)
//  ★ idea: min-heap me har list ka current HEAD. pop SABSE CHHOTA -> result me jodo -> uska NEXT push. repeat.
//  1. saare lists ke heads pq me push (NULL skip).
//  2. dummy head + tail (merge-2-lists jaisa build).
//  3. while(!pq.empty): pop smallest -> tail->next = node -> tail aage -> node->next hai to push.
//  4. return dummy->next.
//  ★ heap me max k nodes (ek har list se) -> pop=O(log k), total N nodes -> O(N log k).
//  ★ min-heap of ListNode* -> custom Compare (a->val > b->val = chhota top pe). [upar defined]
// ============================================================
ListNode *mergeKLists(vector<ListNode *> &lists)
{
    priority_queue<ListNode *, vector<ListNode *>, Compare> pq; // min-heap ready

    // STEP 1: k lists ke heads pq me (NULL skip)
    for (auto &it : lists)
    {
        if (it)
        {
            pq.push(it);
        }
    }

    // STEP 2: dummy + tail (merge-sorted-lists jaisa: tail aage badhta jaata)
    ListNode *dummy = new ListNode(-1);
    ListNode *tail = dummy;

    // STEP 3: har baar sabse chhota node nikaal ke result me jodo, uska next push
    while (!pq.empty())
    {
        ListNode *curr = pq.top(); // sabse chhota (min-heap top)
        pq.pop();

        tail->next = curr; // result me joda
        tail = curr;       // tail aage

        if (curr->next) // is node ke aage aur hai -> pq me daalo
        {
            pq.push(curr->next);
        }
    }
    return dummy->next; // STEP 4: merged list ka head
}

// ─── TEST HARNESS (isko haath mat lagana) ───────────────────
ListNode *buildList(vector<int> v)
{
    ListNode dummy(0);
    ListNode *tail = &dummy;
    for (int x : v)
    {
        tail->next = new ListNode(x);
        tail = tail->next;
    }
    return dummy.next;
}

vector<int> collect(ListNode *head)
{
    vector<int> out;
    while (head)
    {
        out.push_back(head->val);
        head = head->next;
    }
    return out;
}

void check(vector<vector<int>> in, vector<int> exp, int t)
{
    vector<ListNode *> lists;
    for (auto &v : in)
        lists.push_back(buildList(v));
    vector<int> got = collect(mergeKLists(lists));
    cout << "T" << t << ": " << (got == exp ? "PASS" : "FAIL") << "  got=[";
    for (int i = 0; i < (int)got.size(); i++)
        cout << got[i] << (i + 1 < (int)got.size() ? "," : "");
    cout << "]\n";
}

int main()
{
    check({{1, 4, 5}, {1, 3, 4}, {2, 6}}, {1, 1, 2, 3, 4, 4, 5, 6}, 1);
    check({}, {}, 2);
    check({{}}, {}, 3);
    check({{1}}, {1}, 4);
    return 0;
}
