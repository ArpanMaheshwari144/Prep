// ============================================================
// DESIGN A HASHMAP (internals) — LLD / data-structure   (C++)
// ============================================================
// Apna khud ka HashMap banao (built-in map/unordered_map ke BINA).
// string key -> int value. put / get / remove / size support karo.
//
// METHODS:
//   void put(string key, int value)   -> daalo (key exist kare -> UPDATE value)
//   int  get(string key)              -> value lauta; key na ho -> -1
//   void remove(string key)           -> key hatao
//   int  size()                       -> kitne key-value pairs
//
// Tests (expected):
//   put("apple",1) put("banana",2) put("apple",5)   -> get("apple")=5 (update), get("banana")=2
//   get("grape")   -> -1
//   remove("apple") -> get("apple")=-1, size ghata
//   50+ put -> sab phir bhi milne chahiye
// ============================================================

// ---- ARPAN KI APPROACH ----
//  ★ CORE: ek ARRAY (buckets) jisme har slot ek CHAIN (linked-list) ka HEAD ho.
//     buckets[index] = us slot ki poori linked-list ka head (ya NULL agar khaali).
//     collision (2 key ka same index) -> chain me next se jode (chaining).
//  fields: vector<Entry*> buckets · cap=16 (slots) · sz=0 (actual pairs).
//  ★ INDEX: hash<string>{}(key) % cap  (C++ ka hash<string> unsigned deta -> Math.abs nahi chahiye).
//
//  put(key, value):
//     - bucket khaali (buckets[index]==null) -> seedha buckets[index]=node, sz++.
//     - warna chain TRAVERSE (curr=buckets[index]):
//          key mil gayi -> curr->value = value (UPDATE), return.  (naya node NAHI, sz++ NAHI)
//          last node (curr->next==null) tak na mili -> curr->next = node (TAIL pe append), sz++.
//  get(key):  chain traverse -> curr->key==key -> return curr->value.  chain khatam/null -> -1.
//  remove(key): traverse with PREV.  match pe:
//          prev==null (curr HEAD hai) -> buckets[index] = curr->next.   (naya head)
//          warna (beech/end)          -> prev->next = curr->next.        (bypass)
//          sz--, return.
//  size(): sz.
//  ★ KEY INSIGHT: buckets[index] khud ek POINTER hai (chain-head) -> isliye array-assign
//     (buckets[i]=X) aur node-link (X->next=..) MIX hote. yahi array + linked-list ka combo.
//
//  ============ VISUAL (redo ke waqt yahi dekhna) ============
//   STRUCTURE:  array (buckets) + har slot ek CHAIN ka head.   Entry = | key | value | next |
//
//     buckets
//     ┌────┐
//   0 │ ●──┼──> NULL
//     ├────┤
//   1 │ ●──┼──> | apple | 5 | ●──|──> NULL
//     ├────┤
//   3 │ ●──┼──> | cat | 9 | ●──|──> | dog | 4 | ●──|──> NULL      <- COLLISION (chain)
//     ├────┤
//   5 │ ●──┼──> | banana | 2 | ●──|──> NULL
//     └────┘
//     buckets[i] = us chain ka HEAD (ya NULL).
//
//   REMOVE head (prev==null):              REMOVE beech (prev exist):
//     pehle: buckets[i]->[A]->[B]->NULL       [A]->[B]->[C]->NULL   (remove B)
//                          ^curr                    ^prev ^curr
//     do:    buckets[i] = curr->next           prev->next = curr->next
//     baad:  buckets[i]->[B]->NULL             [A]->[C]->NULL   (B bypass)
//  ==========================================================

#include <bits/stdc++.h>
using namespace std;

struct Entry
{
    string key;
    int value;
    Entry *next;
    Entry(string k, int v) : key(k), value(v), next(nullptr) {}
};

class MyHashMap
{
    int sz;
    int cap;
    vector<Entry *> buckets;

public:
    MyHashMap()
    {
        sz = 0;
        cap = 16;
        buckets.resize(cap);
    }

    void put(string key, int value)
    {
        int index = hash<string>{}(key) % cap;
        Entry *node = new Entry(key, value);
        if (buckets[index] == nullptr)
        {
            buckets[index] = node;
            sz++;
        }
        else
        {
            Entry *curr = buckets[index];
            while (curr != nullptr)
            {
                if (curr->key == key)
                {
                    // value update kardo and return
                    curr->value = value;
                    return;
                }
                else
                {
                    if (curr->next == nullptr)
                    {
                        curr->next = node; // last mei jaake lagado
                        sz++;
                    }
                }
                curr = curr->next;
            }
        }
    }

    int get(string key)
    {
        int index = hash<string>{}(key) % cap;
        Entry *curr = buckets[index];
        while (curr != nullptr)
        {
            if (curr->key == key)
            {
                return curr->value;
            }
            curr = curr->next;
        }
        return -1;
    }

    void remove(string key)
    {
        int index = hash<string>{}(key) % cap;
        Entry *curr = buckets[index];
        Entry *prev = NULL;
        while (curr != nullptr)
        {
            if (curr->key == key)
            {
                // head ko delete karna bus bucket ko curr->next se jod do
                if (prev == nullptr)
                {
                    buckets[index] = curr->next;
                }
                else
                {
                    // warna koi dikkat nahi hia prev and curr bus prev->next = curr->next
                    prev->next = curr->next;
                }
                sz--;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
        return;
    }

    int size()
    {
        return sz;
    }
};

int main()
{
    MyHashMap m;
    m.put("apple", 1);
    m.put("banana", 2);
    m.put("apple", 5);
    cout << "apple: " << m.get("apple") << " (expected 5)\n";
    cout << "banana: " << m.get("banana") << " (expected 2)\n";
    cout << "grape: " << m.get("grape") << " (expected -1)\n";
    cout << "size: " << m.size() << " (expected 2)\n";

    m.remove("apple");
    cout << "apple after remove: " << m.get("apple") << " (expected -1)\n";
    cout << "size: " << m.size() << " (expected 1)\n";

    for (int i = 0; i < 50; i++)
        m.put("key" + to_string(i), i * 10);
    cout << "key7: " << m.get("key7") << " (expected 70)\n";
    cout << "key49: " << m.get("key49") << " (expected 490)\n";
    cout << "banana still: " << m.get("banana") << " (expected 2)\n";
    cout << "size: " << m.size() << " (expected 51)\n";
    return 0;
}
