// ============================================================
// DESIGN A HASHMAP (internals) — LLD / data-structure   (C++)   [REDO]
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
// (★ ye tere HASHSET ka bada bhai hai: SAME bucket-array + chaining, bas node me ek VALUE extra
//    aur put me "pehle se ho to UPDATE" wala part.)
// ============================================================
//  ============ MENTAL MODEL (ye picture pehle baithao — code nahi) ============
//   HashMap = ek ARRAY (buckets) + har slot pe ek LINKED LIST (chain).
//   node (Entry) = | key | value | next |
//   index nikaalo: index = hash<string>{}(key) % cap.   (key ko ek slot-number me badla.)
//   do key same index pe aa gaye (COLLISION) -> us slot ki chain me AAGE jod do (linked list).
//
//     buckets (array)
//     ┌────┐
//   0 │ ●──┼──> NULL
//     ├────┤
//   1 │ ●──┼──> | "a"|50| ●──|──> NULL
//     ├────┤
//   3 │ ●──┼──> | "b"|20| ●──|──> | "z"|90| ●──|──> NULL   <- COLLISION (b aur z same slot -> chain)
//     └────┘
//     buckets[i] = us chain ka HEAD (ya NULL).
//
//   => jab picture ye ban gaya, to put/get/remove sab bas us slot ki chain me
//      LINKED-LIST traverse hai (jo tu already jaanta hai). naya kuch nahi.
//  ============================================================================
//
// ============================================================
// ---- ARPAN KI APPROACH (SOLO — poora khud likha, kuch copy nahi) ----
// buckets mei jo shys mei wo bhi as pointers his and ukse baad LL shsru hia 
//  ★ structure: buckets = Entry* ka array (cap=16), har slot pe ek chain (linked list). Entry = |key|value|next|.
//  index = hash<string>{}(key) % cap.
//  put:    slot khaali -> newNode + sz++. warna chain traverse:
//             key mil gayi -> value UPDATE + ★ RETURN (turant niklo). na mili + last node per jaao -> append + sz++.
//  get:    index -> chain traverse -> key match -> value return. na mile -> -1.
//  remove: index -> prev+curr traverse -> match: while(LL != null) agra HEAD delete (mtlb prev null) -> (buckets[i] ye pointer hia)=LL->next · beech -> prev->next=LL->next. sz--.
//  ★★ TRAP (khud pakda): put me update ke baad RETURN na ho -> loop aage jaata -> last node pe DUPLICATE append + sz galat.
//     tests tab bhi PASS the -> seekh: "test-pass != code-sahi". return logically zaroori hai.
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
        int index = hash<string>{}(key) % cap;
        Entry *newNode = new Entry(key, val);
        if (buckets[index] == nullptr)
        {
            // first node in that index
            buckets[index] = newNode;
            sz++;
        }
        else
        {
            // is slot pe pehle se node hai -> chain me traverse: key mili to value UPDATE, na mili to aage node jodo
            Entry *LL = buckets[index];
            while (LL != NULL)
            {
                if (LL->key == key)
                {
                    LL->value = val;
                    return;
                }
                else
                {
                    if (LL->next == NULL)
                    {
                        LL->next = newNode;
                        sz++;
                    }
                }
                LL = LL->next;
            }
        }
    }

    int get(string key)
    {
        // index nikaalo -- buckets array me konse slot pe jaana hai
        int index = hash<string>{}(key) % cap;
        Entry *LL = buckets[index]; // us slot ka chain-head (linked list) lo, us par traverse karo -- bas aur kuch nahi
        while (LL != NULL)
        {
            if (LL->key == key)
            {
                return LL->value;
            }
            LL = LL->next;
        }
        return -1;
    }

    void remove(string key)
    {
        int index = hash<string>{}(key) % cap; // index nikaala
        Entry *LL = buckets[index];            // us slot ka chain-head le liya (LL)

        // ab wahi "remove from linked list" wala logic -- prev + curr traverse
        Entry *prev = nullptr;
        while (LL != NULL)
        {
            if (LL->key == key)
            {
                if (prev != NULL) // beech/aage wala node -> prev ko LL ke aage jod do (LL skip ho gaya)
                {
                    prev->next = LL->next;
                }
                else // prev == NULL -> ye HEAD node hai -> bucket ko seedha LL ke aage point karao
                {
                    buckets[index] = LL->next;
                }
                sz--;
            }
            prev = LL;
            LL = LL->next;
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

    // // --- remove from a chain: neighbor bacha rehna chahiye ---
    m.remove("k7");
    cout << m.get("k7") << " (expected -1)\n";
    cout << m.get("k8") << " (expected 64)\n"; // aas-paas wale bache rahein
    cout << "size: " << m.size() << " (expected 50)\n";

    return 0;
}
