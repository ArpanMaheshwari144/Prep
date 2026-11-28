// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// REORDER LIST   (LC-143 — NAYA)
// ============================================================
// Linked list di hai: L0 -> L1 -> L2 -> ... -> Ln-1 -> Ln
// Use aise badlo:     L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 -> ...
// (pehla, aakhri, doosra, aakhri se pehla, ... aise baari-baari)
//
// ★ NODE ki value mat badlo, NODE hi idhar-udhar karo.
//   Kuch lautana nahi (void), wahi list badlo.
//
//   1->2->3->4      ->  1 4 2 3
//   1->2->3->4->5   ->  1 5 2 4 3
//
// Tests (list -> expected):
//   [1,2,3,4]               -> 1 4 2 3
//   [1,2,3,4,5]             -> 1 5 2 4 3
//   [1]                     -> 1
//   [1,2]                   -> 1 2
//   [1,2,3]                 -> 1 3 2
//   []                      -> (khaali)
//   [1,2,3,4,5,6]           -> 1 6 2 5 3 4
//   [1,2,3,4,5,6,7]         -> 1 7 2 6 3 5 4
//   [10,20,30,40,50,60,70,80] -> 10 80 20 70 30 60 40 50
//   [5,5,1,1]               -> 5 1 5 1
// ============================================================
#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int val;
    Node *next;
    Node(int v) : val(v), next(nullptr) {}
};

void reorderList(Node *head)
{
}

// ---------- checker (ise mat chhed) ----------
int passed = 0, total = 0;
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
void test(vector<int> in, vector<int> expected)
{
    total++;
    Node *head = buildList(in);
    // har node ka pata + uski value yaad rakho (value badli to pakda jaayega)
    vector<pair<Node *, int>> before;
    for (Node *c = head; c; c = c->next)
        before.push_back({c, c->val});

    reorderList(head);

    vector<int> got;
    int steps = 0;
    for (Node *c = head; c && steps <= (int)in.size() + 1; c = c->next, steps++)
        got.push_back(c->val);
    bool valuesSame = true;
    for (auto &p : before)
        if (p.first->val != p.second)
            valuesSame = false;

    bool ok = (got == expected) && valuesSame;
    if (ok)
        passed++;
    cout << "test " << total << "  got=";
    for (int x : got)
        cout << x << " ";
    cout << " expected=";
    for (int x : expected)
        cout << x << " ";
    if (!valuesSame)
        cout << " (node ki VALUE badli hai)";
    cout << (ok ? "  PASS" : "  *** FAIL ***") << "\n";
}

int main()
{
    test({1, 2, 3, 4}, {1, 4, 2, 3});
    test({1, 2, 3, 4, 5}, {1, 5, 2, 4, 3});
    test({1}, {1});
    test({1, 2}, {1, 2});
    test({1, 2, 3}, {1, 3, 2});
    test({}, {});
    test({1, 2, 3, 4, 5, 6}, {1, 6, 2, 5, 3, 4});
    test({1, 2, 3, 4, 5, 6, 7}, {1, 7, 2, 6, 3, 5, 4});
    test({10, 20, 30, 40, 50, 60, 70, 80}, {10, 80, 20, 70, 30, 60, 40, 50});
    test({5, 5, 1, 1}, {5, 1, 5, 1});
    cout << "\n"
         << passed << "/" << total << " tests passed\n";
    return 0;
}
