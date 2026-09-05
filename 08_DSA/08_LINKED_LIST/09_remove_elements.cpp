// ============================================================
// REMOVE LINKED LIST ELEMENTS — (LeetCode 203)   (fresh)
// ============================================================
// ek linked list ka head aur ek int val diya. list me se un SAARE nodes ko
// HATAO jinka val == diya-hua val. naya head return karo.
// (HEAD bhi ho sakta hai jise hatana hai -> isliye DUMMY node useful.)
//
//   [1,2,6,3,4,5,6], val=6   -> [1,2,3,4,5]
//   [], val=1                -> []
//   [7,7,7,7], val=7         -> []          (sab hat gaye)
//   [1,2,3], val=9           -> [1,2,3]      (koi match nahi)
//   [6,1,6], val=6           -> [1]          (head + tail dono hataye)
//
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int val;
    Node *next;
    Node(int v) : val(v), next(nullptr) {}
};

Node *removeElements(Node *head, int val)
{
    if (!head)
    {
        return NULL;
    }

    Node *prev = NULL;
    Node *curr = head;
    while (curr != NULL && head != NULL)
    {
        if (curr->val == val && prev == NULL) // head case or agar sab nodes same hai to ye case hi chalega
        {
            head = head->next;
        }
        else if (curr->val == val && prev != NULL)
        {
            // yahan prev = curr NAHI karenge. kyun: curr abhi HAT raha hai (skip ho raha).
            //   maan lo beech me 2 ya usse zyada consecutive same-val node hain (jaise [1,6,6,2], val=6).
            //   agar prev=curr kar diya to prev ek HATE hue node pe chala jayega -> agla 6 sahi se link nahi hoga.
            //   isliye prev waisa ka waisa rehta (uska next hi aage badhta), sirf curr aage jata.
            prev->next = curr->next;
        }
        else
        {
            prev = curr;
        }
        curr = curr->next;
    }
    return head;
}

// ---------- helpers (chhoo mat) ----------
Node *build(vector<int> v)
{
    Node dummy(0);
    Node *t = &dummy;
    for (int x : v)
    {
        t->next = new Node(x);
        t = t->next;
    }
    return dummy.next;
}
void print(Node *h)
{
    cout << "[";
    while (h)
    {
        cout << h->val << (h->next ? "," : "");
        h = h->next;
    }
    cout << "]\n";
}

int main()
{
    print(removeElements(build({1, 2, 6, 3, 4, 5, 6}), 6)); // [1,2,3,4,5]
    print(removeElements(build({}), 1));                    // []
    print(removeElements(build({7, 7, 7, 7}), 7));          // []
    print(removeElements(build({1, 2, 3}), 9));             // [1,2,3]
    print(removeElements(build({6, 1, 6}), 6));             // [1]
    print(removeElements(build({1, 6, 6, 2}), 6));          // [1]
    return 0;
}
