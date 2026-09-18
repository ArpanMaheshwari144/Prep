// ============================================================
// MERGE K SORTED LISTS (LC 23) — REDO_1   [HEAP #3]
// khud se, bina dekhe — 03_merge_k_lists.cpp / sheet mat kholo; atko tabhi peek.
// ============================================================
//
// k sorted linked lists di hain. Sabko milaa ke EK sorted list lautao.
//
//   [[1,4,5],[1,3,4],[2,6]]  ->  [1,1,2,3,4,4,5,6]
//   []                       ->  []
//   [[]]                     ->  []
//   [[1]]                    ->  [1]
//   [[],[1],[]]              ->  [1]
//   [[5],[1],[3]]            ->  [1,3,5]
//
// ★ k bada ho sakta hai — ek-ek karke jodne se natija baar-baar traverse hota hai.
//
// ------------------------------------------------------------

#include <bits/stdc++.h>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

struct Compare
{
    bool operator()(ListNode *a, ListNode *b)
    {
        return a->val > b->val;
    }
};

ListNode *mergeKLists(vector<ListNode *> &lists)
{
    priority_queue<ListNode *, vector<ListNode *>, Compare> minHeap;
    for (auto &l : lists)
    {
        if (l)
            minHeap.push(l);
    }

    ListNode *dummy = new ListNode(0);
    ListNode *tail = dummy;
    while (!minHeap.empty())
    {
        ListNode *curr = minHeap.top();
        minHeap.pop();
        tail->next = curr;
        tail = curr;
        if (curr->next)
        {
            minHeap.push(curr->next);
        }
    }
    return dummy->next;
}

// ------------------------------------------------------------
// TEST HARNESS
// ------------------------------------------------------------
ListNode *build(const vector<int> &v)
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

vector<int> flatten(ListNode *head)
{
    vector<int> out;
    while (head)
    {
        out.push_back(head->val);
        head = head->next;
    }
    return out;
}

string show(const vector<int> &v)
{
    string s = "[";
    for (size_t i = 0; i < v.size(); i++)
    {
        if (i)
            s += ",";
        s += to_string(v[i]);
    }
    return s + "]";
}

int pass_count = 0, total = 0;

void check(vector<vector<int>> in, vector<int> exp)
{
    vector<ListNode *> lists;
    for (auto &v : in)
        lists.push_back(build(v));

    vector<int> got = flatten(mergeKLists(lists));
    bool ok = (got == exp);
    pass_count += ok;
    total++;
    cout << (ok ? "PASS" : "FAIL")
         << "  got=" << show(got) << "  exp=" << show(exp) << "\n";
}

int main()
{
    check({{1, 4, 5}, {1, 3, 4}, {2, 6}}, {1, 1, 2, 3, 4, 4, 5, 6});
    check({}, {});
    check({{}}, {});
    check({{1}}, {1});
    check({{}, {1}, {}}, {1});
    check({{5}, {1}, {3}}, {1, 3, 5});
    check({{1, 2, 3}, {}, {1, 2, 3}}, {1, 1, 2, 2, 3, 3});

    cout << "\n"
         << pass_count << "/" << total << " passed\n";
    return 0;
}
