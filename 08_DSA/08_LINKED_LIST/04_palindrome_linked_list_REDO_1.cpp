// ============================================================
// PALINDROME LINKED LIST — Linked List  (LC-234, REDO_1 — blank)
// ============================================================
// head diya. TRUE agar list aage-se aur peeche-se SAME padhi jaati (palindrome).
//   1 -> 2 -> 2 -> 1        -> true
//   1 -> 2 -> 3 -> 2 -> 1   -> true
//   1 -> 2                  -> false
//
// Tests (list -> expected, 1 = true, 0 = false):
//   [1,2,2,1]        -> 1
//   [1,2,3,2,1]      -> 1
//   [1,2]            -> 0
//   [1]              -> 1
//   [1,2,3]          -> 0
//   []               -> 1   (khali = palindrome)
//   [7,7]            -> 1
//   [1,1,2]          -> 0
//   [1,2,1,1]        -> 0
//   [5,4,3,3,4,5]    -> 1
//   [1,2,3,4,2,1]    -> 0
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

Node *reverseList(Node *head)
{
    Node *curr = head;
    Node *prev = NULL;
    Node *nextt = head;
    while (curr != NULL)
    {
        nextt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextt;
    }
    return prev;
}

bool isPalindrome(Node *head)
{
    Node *mid = middleNode(head);
    Node *rev = reverseList(mid);

    while (head != NULL && rev != NULL)
    {
        if (head->val == rev->val)
        {
            head = head->next;
            rev = rev->next;
        }
        else
        {
            return false;
        }
    }
    return true;
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

int pass = 0, total = 0;
void check(vector<int> v, bool expected)
{
    total++;
    bool got = isPalindrome(buildList(v));
    if (got == expected)
        pass++;
    else
    {
        cout << "FAIL: [";
        for (int x : v)
            cout << x << " ";
        cout << "] expected " << expected << " got " << got << "\n";
    }
}

int main()
{
    check({1, 2, 2, 1}, true);
    check({1, 2, 3, 2, 1}, true);
    check({1, 2}, false);
    check({1}, true);
    check({1, 2, 3}, false);
    check({}, true);
    check({7, 7}, true);
    check({1, 1, 2}, false);
    check({1, 2, 1, 1}, false);
    check({5, 4, 3, 3, 4, 5}, true);
    check({1, 2, 3, 4, 2, 1}, false);
    cout << pass << "/" << total << " passed\n";
    return 0;
}
