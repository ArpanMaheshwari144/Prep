// ============================================================
// DESIGN A HASHMAP (internals) — LLD / data-structure   (C++)   [REDO-2, pakka]
// ============================================================
// Apna khud ka HashMap banao (built-in map/unordered_map ke BINA).
// string key -> int value. put / get / remove / size.
//
// METHODS:
//   void put(string key, int val)  -> key daalo. pehle se hai -> value UPDATE karo.
//   int  get(string key)           -> value do. key na ho -> -1.
//   void remove(string key)        -> key hatao.
//   int  size()                    -> kitni distinct keys.
//
// ============================================================
//  ============ MENTAL MODEL (picture pehle) ============
//   HashMap = ek ARRAY (buckets) + har slot pe ek LINKED LIST (chain).
//   buckets[i] KHUD ek POINTER hai -> chain ke HEAD pe point karta; uske aage se LL shuru.
//   node (Entry) = | key | value | next |
//   index = hash<string>{}(key) % cap.   do key same index (COLLISION) -> chain me aage jodo.
//
//     buckets (array)
//     ┌────┐
//   0 │ ●──┼──> NULL
//     ├────┤
//   1 │ ●──┼──> | "a"|50| ●──|──> NULL
//     ├────┤
//   3 │ ●──┼──> | "b"|20| ●──|──> | "z"|90| ●──|──> NULL   <- COLLISION (chain)
//     └────┘
//
//   => put/get/remove = us slot pe jao (hash%cap), phir chain me LINKED-LIST traverse.
//  =====================================================
// ============================================================
// ---- ARPAN KI APPROACH (SOLO — 2nd rep, faada) ----
//  ★ structure: buckets = vector<Entry*> (cap=16). buckets[i] = us slot ki chain ka head (ya NULL). Entry = |key|value|next|.
//  index = hash<string>{}(key) % cap.
//  put:    slot khaali -> node laga + sz++. warna chain traverse (while head != NULL):
//             key mili -> value UPDATE + RETURN (turant niklo). na mili + end (head->next==NULL) -> node append + sz++.
//  get:    index -> chain traverse -> key match -> value return. na mile -> -1.
//  remove: index -> prev+head traverse -> key match: HEAD(prev NULL) -> buckets[i]=head->next · beech -> prev->next=head->next. sz--.
//  ★★ TRAP (kal seekha): put me update ke baad RETURN zaroori (warna loop aage -> last pe DUPLICATE + sz galat). "test-pass != code-sahi".
// ============================================================

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

    void put(string key, int val)
    {
        int index = hash<string>{}(key) % cap; // bucket ka index nikala (hash % cap)
        Entry *node = new Entry(key, val);      // naya node banaya
        if (buckets[index] == nullptr)          // slot khaali -> seedha yahin laga do
        {
            buckets[index] = node;
            sz++; // nayi key add hui
        }
        else
        {
            // slot me pehle se chain hai -> traverse: key mili to value UPDATE, warna end me node jodo
            Entry *head = buckets[index];
            while (head != NULL)
            {
                if (head->key == key)
                {
                    head->value = val;
                    return;
                }
                else
                {
                    if (head->next == nullptr)
                    {
                        head->next = node;
                        sz++; // nayi key chain ke end me add hui
                    }
                }
                head = head->next;
            }
        }
    }

    int get(string key)
    {
        int index = hash<string>{}(key) % cap; // array me bucket ka index
        Entry *head = buckets[index];           // us slot ki chain (LL) ka head
        while (head != NULL) // chain traverse: key match hui to uski value return
        {
            if (head->key == key)
            {
                return head->value;
            }
            head = head->next;
        }
        return -1;
    }

    void remove(string key)
    {
        int index = hash<string>{}(key) % cap; // index nikala
        Entry *head = buckets[index];           // us slot ki chain (LL) ka head
        Entry *prev = NULL;                     // peeche wala node track karne ke liye
        while (head != NULL)
        {
            // andar tabhi jao jab key MATCH ho (jise hatana hai) -> tabhi sz--
            if (head->key == key)
            {
                if (prev != NULL) // prev != NULL -> ye beech/aage wali node hai
                {
                    prev->next = head->next;
                }
                else // prev == NULL -> ye HEAD node hai
                {
                    buckets[index] = head->next;
                }
                sz--; // node hata -> ginti ghatao
            }
            prev = head;
            head = head->next;
        }
    }

    int size()
    {
        return sz;
    }
};

int main()
{
    MyHashMap m;

    // --- basic put/get ---
    m.put("a", 10);
    m.put("b", 20);
    cout << m.get("a") << " (expected 10)\n";
    cout << m.get("b") << " (expected 20)\n";
    cout << m.get("c") << " (expected -1)\n"; // missing key
    cout << "size: " << m.size() << " (expected 2)\n";

    // --- UPDATE existing key (value badle, size NAHI badhe) ---
    m.put("b", 25);
    cout << m.get("b") << " (expected 25)\n";
    cout << "size: " << m.size() << " (expected 2)\n";

    // --- remove ---
    m.remove("a");
    cout << m.get("a") << " (expected -1)\n";
    cout << "size: " << m.size() << " (expected 1)\n";
    m.remove("zzz"); // non-existent -> crash NAHI hona chahiye
    cout << "size: " << m.size() << " (expected 1)\n";

    // --- collision + chain stress (bahut saari key) ---
    for (int i = 0; i < 50; i++)
        m.put("k" + to_string(i), i * i);
    cout << m.get("k7") << " (expected 49)\n";
    cout << m.get("k49") << " (expected 2401)\n";
    cout << "size: " << m.size() << " (expected 51)\n"; // "b"(1) + 50 keys = 51

    // --- UPDATE key jo collision-chain ke ANDAR ho (return-trap test) ---
    m.put("k0", 999);
    cout << m.get("k0") << " (expected 999)\n";
    cout << "size: " << m.size() << " (expected 51)\n"; // size badhna NAHI chahiye

    // --- remove from a chain: neighbor bacha rehna chahiye ---
    m.remove("k7");
    cout << m.get("k7") << " (expected -1)\n";
    cout << m.get("k8") << " (expected 64)\n";
    cout << "size: " << m.size() << " (expected 50)\n";

    return 0;
}
