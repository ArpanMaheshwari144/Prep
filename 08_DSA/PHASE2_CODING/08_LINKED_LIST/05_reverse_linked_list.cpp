// ============================================================
// REVERSE LINKED LIST   —   Linked List (3-pointer walk)
// ============================================================
// Singly linked list ko ULTA karo, naya head do.
//   1->2->3->4->5  ->  5->4->3->2->1
//
// SIGNAL: "list reverse / direction badlo" -> 3-pointer walk (prev, curr, next).
//   Soch (copy pe ARROWS bana ke, tera spatial mode):
//     - har node ka 'next' arrow ULTA karna hai (aage ki jagah PEECHE point kare).
//     - ek node ka next ulta karne se PEHLE uska aage wala kho jaayega ->
//       use ek temp mein bachao.
//     - end mein naya head = jo pehle aakhri tha.
//   No pseudo-code. Copy pe 1->2->3 ko arrows se ulta kar ke dekh.
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
ListNode *reverseList(ListNode *head)
{
    ListNode* prev = nullptr;
    ListNode* curr = head;
    ListNode* nextt = head;
    while(curr != nullptr){
        nextt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextt;
    }
    return prev;
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
void print(ListNode *head)
{
    while (head) { cout << head->val; if (head->next) cout << " "; head = head->next; }
    cout << endl;
}

int main()
{
    print(reverseList(build({1, 2, 3, 4, 5}))); // expected: 5 4 3 2 1
    print(reverseList(build({1, 2})));           // expected: 2 1
    print(reverseList(build({7})));              // expected: 7
    print(reverseList(build({})));               // expected: (empty line)
    return 0;
}
