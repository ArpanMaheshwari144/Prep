// ============================================================
// LRU CACHE — (LeetCode 146)   [DESIGN #3]   ★ REDO-1 (blank, 10-Aug)
// ============================================================
// Original mat dekh -- yaad se likh. Naya mechanic (DLL + hashmap) ko lock karne ke liye.
//
// KYA BANANA:
//   class LRUCache with fixed CAPACITY.
//   get(key)      -> value agar present, warna -1. (access = "recently used")
//   put(key,val)  -> insert/update. cache FULL -> LEAST-recently-used nikaalo (evict).
//   dono O(1) hone chahiye.
//
// ---- TEST CASES (capacity = 2) ----
//   put(1,1) · put(2,2) · get(1)->1 · put(3,3)[evict 2] · get(2)->-1
//   put(4,4)[evict 1] · get(1)->-1 · get(3)->3 · get(4)->4
// ============================================================

#include <bits/stdc++.h>
using namespace std;

struct Node
{
    int key, val;
    Node *prev, *next;
    Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
};

class LRUCache
{
    int cap;
    unordered_map<int, Node *> mp;
    Node *head, *tail;

public:
    LRUCache(int capacity)
    {
        cap = capacity;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    void removeNode(Node *node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void addFront(Node *node)
    {
        Node *temp = head->next;
        head->next = node;
        node->prev = head;

        node->next = temp;
        temp->prev = node;
    }

    int get(int key)
    {
        if (mp.count(key) > 0)
        {
            Node *node = mp[key];
            removeNode(node);
            addFront(node);
            return node->val;
        }
        return -1;
    }

    void put(int key, int value)
    {
        if (mp.count(key) > 0)
        {
            Node *node = mp[key];
            node->val = value;
            addFront(node);
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

// ---- test helper (ise mat chhed) ----
void check(int got, int exp, const string &label)
{
    cout << label << " -> got " << got << " | exp " << exp
         << (got == exp ? "   PASS" : "   *** FAIL ***") << "\n";
}

int main()
{
    LRUCache c(2);
    c.put(1, 1);
    c.put(2, 2);
    check(c.get(1), 1, "get(1)");  // 1
    c.put(3, 3);                   // evicts key 2
    check(c.get(2), -1, "get(2)"); // -1
    c.put(4, 4);                   // evicts key 1
    check(c.get(1), -1, "get(1)"); // -1
    check(c.get(3), 3, "get(3)");  // 3
    check(c.get(4), 4, "get(4)");  // 4
    return 0;
}
