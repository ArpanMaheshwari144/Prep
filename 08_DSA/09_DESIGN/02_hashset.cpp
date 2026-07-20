// ============================================================
// DESIGN A HASHSET (internals) — LLD / data-structure   (C++)
// ============================================================
// Apna khud ka HashSet banao (built-in set ke BINA).
// SIRF keys (string). value nahi. add / contains / remove / size.
//
// METHODS:
//   void add(string key)        -> key daalo (pehle se ho -> kuch mat karo, duplicate nahi)
//   bool contains(string key)   -> key hai? true / false
//   void remove(string key)     -> key hatao
//   int  size()                 -> kitni distinct keys
//
// (HashMap jaisा hi: bucket array + chaining. bas value/update wala part nahi.)
//
// Tests (expected):
//   add("apple") add("banana") add("apple")   -> contains("apple")=1, size=2 (duplicate nahi)
//   contains("grape") -> 0
//   remove("apple")   -> contains("apple")=0, size ghata
//   50+ add -> sab contains milne chahiye
// ============================================================
//  ============ MENTAL MODEL (dekhne ke liye — no code) ============
//   HashMap jaisा hi: ARRAY (buckets) + har slot ek CHAIN. bas node me VALUE nahi -> Entry = | key | next |
//   index = hash(key) % cap.  same index (collision) -> chain me jode (next se).
//
//     buckets
//     ┌────┐
//   0 │ ●──┼──> NULL
//     ├────┤
//   1 │ ●──┼──> | apple | ●──|──> NULL
//     ├────┤
//   3 │ ●──┼──> | cat | ●──|──> | dog | ●──|──> NULL     <- COLLISION (chain)
//     └────┘
//     buckets[i] = us chain ka HEAD (ya NULL).
//
//   add:      key chain me hai? -> kuch mat karo (duplicate nahi). na ho -> node lagao + sz++.
//   contains: chain traverse -> key match -> true.  na mile -> false.
//   remove:   prev+curr traverse -> match: HEAD(prev null)->buckets[i]=curr->next · beech->prev->next=curr->next. sz--.
//   (HashMap se farak: value nahi, update nahi -- add me sirf "hai ya nahi".)
//  =================================================================
//
// ---- ARPAN KI APPROACH ----
//  ★ HashMap ka SAME design (bucket array + chaining), bas VALUE/update ka part hata.
//     Entry = |key|next| (value nahi). fields: buckets(Entry*) · cap=16 · sz.
//  index = hash<string>{}(key) % cap.
//  add(key):     bucket khaali -> buckets[index]=node, sz++. else chain traverse:
//                   key mil gayi -> return (duplicate nahi). tail (curr->next==null) -> append + sz++.
//  contains(key): chain traverse -> key match -> true. na mile/null -> false. (= HashMap ka get, bas bool.)
//  remove(key):  prev+curr traverse -> match: HEAD(prev null)->buckets[i]=curr->next · else prev->next=curr->next. sz--.
//     ★★ BUG-CATCH (khud dimaag me pakda): prev ko har step "prev = curr" karo (NA prev=prev->next --
//        prev NULL se shuru -> non-head remove pe NULL->next = CRASH). prev = curr; curr = curr->next;
//  size(): sz.

#include <bits/stdc++.h>
using namespace std;

struct Entry
{
    string key;
    Entry *next;
    Entry(string k) : key(k), next(nullptr) {}
};

class MyHashSet
{
    int sz;
    int cap;
    vector<Entry *> buckets;

public:
    MyHashSet()
    {
        sz = 0;
        cap = 16;
        buckets.resize(cap);
    }

    void add(string key)
    {
        int index = hash<string>{}(key) % cap;
        Entry *node = new Entry(key);
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

    bool contains(string key)
    {
        int index = hash<string>{}(key) % cap;
        Entry *curr = buckets[index];
        while (curr != nullptr)
        {
            if (curr->key == key)
            {
                return true;
            }
            curr = curr->next;
        }
        return false;
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
    MyHashSet s;
    s.add("apple");
    s.add("banana");
    s.add("apple"); // duplicate
    cout << "contains apple: " << s.contains("apple") << " (expected 1)\n";
    cout << "contains banana: " << s.contains("banana") << " (expected 1)\n";
    cout << "contains grape: " << s.contains("grape") << " (expected 0)\n";
    cout << "size: " << s.size() << " (expected 2)\n";

    s.remove("apple");
    cout << "contains apple after remove: " << s.contains("apple") << " (expected 0)\n";
    cout << "size: " << s.size() << " (expected 1)\n";

    for (int i = 0; i < 50; i++)
        s.add("key" + to_string(i));
    cout << "contains key7: " << s.contains("key7") << " (expected 1)\n";
    cout << "contains key49: " << s.contains("key49") << " (expected 1)\n";
    cout << "contains banana: " << s.contains("banana") << " (expected 1)\n";
    cout << "size: " << s.size() << " (expected 51)\n";
    return 0;
}
