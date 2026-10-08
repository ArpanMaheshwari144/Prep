// ============================================================
// LRU CACHE — (LeetCode 146)   [DESIGN #3]   ★ REDO-2
// ============================================================
// KYA BANANA:
//   class LRUCache with fixed CAPACITY.
//   get(key)      -> value agar present, warna -1. (access = "recently used")
//   put(key,val)  -> insert/update. cache FULL -> LEAST-recently-used nikaalo (evict).
//   dono O(1) hone chahiye.
//
// Boilerplate (Node, map, head, tail, constructor) neeche bana hua hai.
// Tu sirf METHODS likh.
//
// ---- TEST SCENARIOS (sab neeche main me) ----
//   S1 cap=2  classic: put1 put2 get1 put3[evict 2] get2 put4[evict 1] get1 get3 get4
//   S2 cap=2  put se update bhi "recent" banata: put1 put2 put(1,10) put3[evict 2]
//   S3 cap=2  get se "recent": put1 put2 get1 put3[evict 2]
//   S4 cap=1  ek hi jagah: put1 put2[evict 1] · put(2,5) update
//   S5 cap=3  khaali cache pe get -> -1
//   S6 cap=3  lambi: put1 put2 put3 get1 put4[evict 2] put5[evict 3]
//   S7 cap=2  same key baar-baar update -> size nahi badhta
//   S8 cap=2  missing key ka get kram nahi badalta: put1 put2 get9 put3[evict 1]
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
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

    void addFront(Node *node)
    {
        Node *nxt = head->next;
        head->next = node;
        node->prev = head;

        node->next = nxt;
        nxt->prev = node;
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
            removeNode(node);
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
    {
        LRUCache c(2);
        c.put(1, 1);
        c.put(2, 2);
        check(c.get(1), 1, "S1 get(1)");
        c.put(3, 3); // evict 2
        check(c.get(2), -1, "S1 get(2)");
        c.put(4, 4); // evict 1
        check(c.get(1), -1, "S1 get(1)");
        check(c.get(3), 3, "S1 get(3)");
        check(c.get(4), 4, "S1 get(4)");
    }
    {
        LRUCache c(2);
        c.put(1, 1);
        c.put(2, 2);
        c.put(1, 10); // update -> 1 recent
        c.put(3, 3);  // evict 2
        check(c.get(1), 10, "S2 get(1)");
        check(c.get(2), -1, "S2 get(2)");
        check(c.get(3), 3, "S2 get(3)");
    }
    {
        LRUCache c(2);
        c.put(1, 1);
        c.put(2, 2);
        c.get(1);    // 1 recent
        c.put(3, 3); // evict 2
        check(c.get(2), -1, "S3 get(2)");
        check(c.get(1), 1, "S3 get(1)");
        check(c.get(3), 3, "S3 get(3)");
    }
    {
        LRUCache c(1);
        c.put(1, 1);
        c.put(2, 2); // evict 1
        check(c.get(1), -1, "S4 get(1)");
        check(c.get(2), 2, "S4 get(2)");
        c.put(2, 5);
        check(c.get(2), 5, "S4 get(2) after update");
    }
    {
        LRUCache c(3);
        check(c.get(5), -1, "S5 get(5) empty");
    }
    {
        LRUCache c(3);
        c.put(1, 1);
        c.put(2, 2);
        c.put(3, 3);
        c.get(1);    // order: 1 recent, 2 oldest
        c.put(4, 4); // evict 2
        check(c.get(2), -1, "S6 get(2)");
        c.put(5, 5); // evict 3
        check(c.get(3), -1, "S6 get(3)");
        check(c.get(1), 1, "S6 get(1)");
        check(c.get(4), 4, "S6 get(4)");
        check(c.get(5), 5, "S6 get(5)");
    }
    {
        LRUCache c(2);
        c.put(1, 1);
        c.put(1, 2);
        c.put(1, 3); // abhi bhi 1 hi key
        c.put(2, 2); // jagah thi, koi evict nahi
        check(c.get(1), 3, "S7 get(1)");
        check(c.get(2), 2, "S7 get(2)");
    }
    {
        LRUCache c(2);
        c.put(1, 1);
        c.put(2, 2);
        check(c.get(9), -1, "S8 get(9) missing");
        c.put(3, 3); // evict 1 (sabse purana)
        check(c.get(1), -1, "S8 get(1)");
        check(c.get(2), 2, "S8 get(2)");
        check(c.get(3), 3, "S8 get(3)");
    }
    return 0;
}
