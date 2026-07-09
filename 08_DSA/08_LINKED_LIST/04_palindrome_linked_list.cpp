// ============================================================
// PALINDROME LINKED LIST — Linked List (pattern #4)
// ============================================================
// head diya. TRUE agar list aage-se aur peeche-se SAME padhi jaati (palindrome).
//   1 -> 2 -> 2 -> 1        -> true
//   1 -> 2 -> 3 -> 2 -> 1   -> true
//   1 -> 2                  -> false
//
// (approach LIKHA NAHI -- 2 raaste:
//   (a) EASY: values vector me daalo -> two-pointer (aage/peeche) compare.
//   (b) ELEGANT (O(1) space): MIDDLE nikaalo (fast/slow) -> 2nd half REVERSE -> compare.
//              <- ye tere pichle 2 problems ka combo. jo pasand aaye.)
//   copy-pen pe [1,2,2,1] trace kar pehle.
//
// Tests (// expected):
//   [1,2,2,1]      -> 1  (true)
//   [1,2,3,2,1]    -> 1
//   [1,2]          -> 0  (false)
//   [1]            -> 1
//   [1,2,3]        -> 0
//   []             -> 1  (khali = palindrome)
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
Node *reverseList(Node *head)
{
    Node *prev = NULL;
    Node *curr = head;
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

bool isPalindrome(Node *head)
{
    if (head == nullptr)
    {
        return true;
    }

    Node *midNode = middleNode(head);
    Node *rev = reverseList(midNode);

    while (head != NULL && rev != NULL)
    {
        // cout << head->val << " " << rev->val << endl;
        if (head->val != rev->val)
        {
            return false;
        }
        head = head->next;
        rev = rev->next;
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

int main()
{
    vector<vector<int>> tests = {
        {1, 2, 2, 1},
        {1, 2, 3, 2, 1},
        {1, 2},
        {1},
        {1, 2, 3},
        {}
    };

    for (auto &t : tests)
        cout << isPalindrome(buildList(t)) << "\n";
    // expected: 1 1 0 1 0 1
    return 0;
}
