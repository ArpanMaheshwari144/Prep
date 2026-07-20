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
// ---- ARPAN KI APPROACH ----
//  ★ val wale SAARE node hatane hain. HEAD bhi ho sakta -> isliye prev/curr traverse + head special-case.
//  prev=NULL, curr=head. loop:
//     curr->val==val && prev==NULL (HEAD hatana) -> head=head->next, curr=curr->next.  (prev NULL hi rehne do)
//     curr->val==val && prev!=NULL (beech/end)   -> prev->next=curr->next, curr=curr->next.
//        ★★ BUG-CATCH: removal pe prev ADVANCE MAT karo (prev=curr galat -- curr to HATA diya).
//           prev waise ka waisa rehta (kyunki uska next ab curr-ke-aage pe hai). warna consecutive [1,6,6,2] fail.
//     warna (no match) -> prev=curr, curr=curr->next.   (yahan prev advance hota)
//  return head.
//  (dummy-node se bhi ho sakta: head se pehle dummy -> head special-case ki zaroorat nahi. yahan head-case use kiya.)

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
