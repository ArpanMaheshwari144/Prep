// ============================================================
// DESIGN A HASHSET (internals) — Design  (REDO_1 — blank)
// ============================================================
// Apna khud ka HashSet banao (built-in set / unordered_set ke BINA).
// SIRF keys (string), value nahi.
//
// METHODS:
//   void add(string key)        -> key daalo (pehle se ho -> kuch mat karo, duplicate nahi)
//   bool contains(string key)   -> key hai? true / false
//   void remove(string key)     -> key hatao (na ho -> kuch mat karo)
//   int  size()                 -> kitni distinct keys
//
// Tests (expected):
//   1. naya set                                    -> size 0, contains("apple") = 0
//   2. add apple, banana, apple                    -> size 2 (duplicate nahi)
//   3. contains("apple"), contains("banana")       -> 1, 1
//   4. contains("grape")                           -> 0
//   5. remove("apple")                             -> contains("apple") = 0, size 1
//   6. remove("grape") (jo hai hi nahi)            -> size wahi 1, kuch toote nahi
//   7. remove("apple") dobara                      -> size wahi 1
//   8. add("apple") hatane ke baad wapas           -> contains = 1, size 2
//   9. 100 alag key add ("k0".."k99")              -> sab contains = 1, size 102
//  10. un 100 me se har doosri (k0, k2 ..) remove  -> hati wali 0, baaki 1, size 52
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- diya hua (LeetCode jaisa) ----
struct Entry
{
    string key;
    Entry *next;
    Entry(string k) : key(k), next(nullptr) {}
};

// ---- kaam: neeche ke 4 method ----
class MyHashSet
{
    int sz;
    int cap;
    vector<Entry *> buckets; // buckets[i] = us chain ka pehla node (ya nullptr)

public:
    MyHashSet()
    {
        sz = 0;
        cap = 16;
        buckets.resize(cap, nullptr);
    }

    void add(string key)
    {
        int index = hash<string>{}(key) % cap;
        Entry *newNode = new Entry(key);
        if (buckets[index] == nullptr)
        {
            buckets[index] = newNode;
            sz++;
        }
        else
        {
            Entry *node = buckets[index];
            while (node != NULL)
            {
                if (node->key == key)
                {
                    return;
                }
                else
                {
                    if (node->next == nullptr)
                    {
                        node->next = newNode;
                        sz++;
                    }
                }
                node = node->next;
            }
        }
    }

    bool contains(string key)
    {
        int index = hash<string>{}(key) % cap;
        Entry *node = buckets[index];
        while (node != NULL)
        {
            if (node->key == key)
            {
                return true;
            }
            node = node->next;
        }
        return false;
    }

    void remove(string key)
    {
        int index = hash<string>{}(key) % cap;
        Entry *node = buckets[index];
        Entry *prev = NULL;
        while (node != NULL)
        {
            if (node->key == key)
            {
                if (prev == NULL)
                {
                    buckets[index] = node->next;
                }
                else
                {
                    prev->next = node->next;
                }
                sz--;
            }
            prev = node;
            node = node->next;
        }
    }

    int size()
    {
        return sz;
    }
};

// ---- tests (ise mat chhedo) ----
int pass = 0, total = 0;
void check(bool ok, string name)
{
    total++;
    if (ok)
        pass++;
    cout << (ok ? "PASS " : "FAIL ") << name << "\n";
}

int main()
{
    MyHashSet s;
    check(s.size() == 0 && !s.contains("apple"), "1 naya set khaali");

    s.add("apple");
    s.add("banana");
    s.add("apple");
    check(s.size() == 2, "2 duplicate nahi, size 2");
    check(s.contains("apple") && s.contains("banana"), "3 apple, banana hain");
    check(!s.contains("grape"), "4 grape nahi");

    s.remove("apple");
    check(!s.contains("apple") && s.size() == 1, "5 apple hata, size 1");

    s.remove("grape");
    check(s.size() == 1, "6 jo nahi hai use hatao, size wahi");

    s.remove("apple");
    check(s.size() == 1, "7 apple dobara hatao, size wahi");

    s.add("apple");
    check(s.contains("apple") && s.size() == 2, "8 apple wapas, size 2");

    for (int i = 0; i < 100; i++)
        s.add("k" + to_string(i));
    bool all = true;
    for (int i = 0; i < 100; i++)
        all = all && s.contains("k" + to_string(i));
    check(all && s.size() == 102, "9 100 key, sab mili, size 102");

    for (int i = 0; i < 100; i += 2)
        s.remove("k" + to_string(i));
    bool ok10 = true;
    for (int i = 0; i < 100; i++)
    {
        bool hai = s.contains("k" + to_string(i));
        if (i % 2 == 0 && hai)
            ok10 = false;
        if (i % 2 == 1 && !hai)
            ok10 = false;
    }
    check(ok10 && s.size() == 52, "10 har doosri hati, size 52");

    cout << "\n"
         << pass << "/" << total << " PASS\n";
    return 0;
}
