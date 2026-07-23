// ============================================================
// [COLD REDO] DESIGN A HASHMAP (internals) — LLD / data-structure
// ============================================================
// original/mental-model MAT dekh. fresh dimaag se, khud se bana.
// Apna khud ka HashMap (built-in map/unordered_map ke BINA). string key -> int value.
//
// METHODS:
//   void put(string key, int val)  -> key daalo. pehle se hai -> value UPDATE.
//   int  get(string key)           -> value do. key na ho -> -1.
//   void remove(string key)        -> key hatao.
//   int  size()                    -> kitni distinct keys.
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
    int count;
    int cap;
    vector<Entry *> buckets;

public:
    MyHashMap()
    {
        count = 0;
        cap = 16;
        buckets.resize(cap);
    }

    void put(string key, int val)
    {
        int index = hash<string>{}(key) % cap;
        Entry *node = new Entry(key, val);
        if (buckets[index] == nullptr)
        {
            buckets[index] = node;
            count++;
        }
        else
        {
            Entry *curr = buckets[index];
            while (curr != NULL)
            {
                if (curr->key == key)
                {
                    curr->value = val;
                    return;
                }
                else
                {
                    if (curr->next == nullptr)
                    {
                        curr->next = node;
                        count++;
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
        while (curr != NULL)
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
        while (curr != NULL)
        {
            if (curr->key == key)
            {
                if (prev != NULL)
                {
                    prev->next = curr->next;
                }
                else
                {
                    buckets[index] = curr->next;
                }
                count--;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    int size()
    {
        return count;
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
    m.remove("zzz"); // non-existent -> crash NAHI
    cout << "size: " << m.size() << " (expected 1)\n";

    // --- collision + chain stress (50 keys) ---
    for (int i = 0; i < 50; i++)
        m.put("k" + to_string(i), i * i);
    cout << m.get("k7") << " (expected 49)\n";
    cout << m.get("k49") << " (expected 2401)\n";
    cout << "size: " << m.size() << " (expected 51)\n"; // "b"(1) + 50 = 51

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
