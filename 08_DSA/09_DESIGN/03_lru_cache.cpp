// ============================================================
// LRU CACHE (LC 146) — design / COMBO: hashmap + doubly-linked-list (C++)
// ============================================================
// Fixed-capacity cache. get + put DONO O(1). Full -> LRU (least-recently-used) evict.
//
// METHODS:
//   int  get(int key)          -> value do + item ko MRU bana. key na ho -> -1.
//   void put(int key, int val) -> hai -> val UPDATE + MRU. naya -> add + MRU. FULL -> LRU evict phir add.
//
// ============================================================
//  ============ MENTAL MODEL (picture pehle) ============
//   2 zaroorat -> 2 structure JODO:
//     hashmap: key -> us key ke DLL-node ka POINTER   (find O(1))
//     DLL    : dummy [HEAD] <-> MRU ... LRU <-> [TAIL] dummy   (reorder/evict O(1))
//              head-side = sabse recent  ·  tail-side = sabse purana (evict yahan se)
//
//     mp: { 1 -> ●,  3 -> ● }        (har key -> DLL node ka pata)
//               │       │
//               v       v
//        [HEAD] <-> |1|v| <-> |3|v| <-> [TAIL]
//        (dummy)     MRU        LRU      (dummy)
//
//   => get/put: node hashmap se O(1) uthao -> DLL me HEAD ke paas (front) le jao.
//      FULL pe: tail->prev (=LRU) nikaalo + mp.erase.
//      DUMMY head/tail: koi real node kabhi NULL-padosi na dekhe -> edge-case clean.
//  =====================================================
// ============================================================
// ============================================================

#include <bits/stdc++.h>
using namespace std;

class LRUCache
{
    struct Node
    {
        int key, val;
        Node *prev;
        Node *next;
        Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
    };

    int cap;
    unordered_map<int, Node *> mp; // key -> node
    Node *head;                    // dummy: most-recent side (head.next = MRU)
    Node *tail;                    // dummy: least-recent side (tail.prev = LRU)

    void removeNode(Node *node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void addFront(Node *node)
    {
        Node *nxt = head->next;
        head->next = node;
        node->prev = head;

        node->next = nxt;
        nxt->prev = node;
    }

public:
    LRUCache(int capacity)
    {
        cap = capacity;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    int get(int key)
    {
        if (mp.count(key) == 0)
        {
            return -1;
        }
        Node *node = mp[key];
        removeNode(node);
        addFront(node);
        return node->val;
    }

    void put(int key, int value)
    {
        if (mp.count(key) > 0)
        {
            Node *node = mp[key];
            node->val = value;
            addFront(node);
            return;
        }
        else
        {
            if (mp.size() == cap)
            {
                Node *toBeRemoved = tail->prev;
                removeNode(toBeRemoved);
                mp.erase(toBeRemoved->key);
            }
            Node *newNode = new Node(key, value);
            addFront(newNode);
            mp[key] = newNode;
        }
    }
};

// ---- test harness ----
static int pass = 0, fail = 0;
void check(int got, int exp, const string &label)
{
    bool ok = (got == exp);
    cout << (ok ? "PASS " : "FAIL ") << label << "  got=" << got << " exp=" << exp << "\n";
    ok ? pass++ : fail++;
}

int main()
{
    LRUCache cache(2);
    cache.put(1, 1);
    cache.put(2, 2);
    check(cache.get(1), 1, "T1 get(1)");          // 1  (1 recent hua)
    cache.put(3, 3);                              // cap full -> LRU(2) evict
    check(cache.get(2), -1, "T2 get(2) evicted"); // -1
    cache.put(4, 4);                              // LRU(1) evict
    check(cache.get(1), -1, "T3 get(1) evicted"); // -1
    check(cache.get(3), 3, "T4 get(3)");          // 3
    check(cache.get(4), 4, "T5 get(4)");          // 4

    cout << "\n"
         << pass << " passed, " << fail << " failed\n";
    return 0;
}
