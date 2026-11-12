// ============================================================
// PEHLE YE (har problem pe, 30 sec):
// 1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
// 2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
// 3. Kisi ka LABEL mat lo -> khud dekho: sach me aisa hai?
// 4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
// 5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
// 6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
// ============================================================
// DESIGN A HASHMAP — Design  (REDO_2 — blank, 12-Nov)
// ============================================================
// Apna khud ka HashMap banao (built-in map / unordered_map ke BINA).
// string key -> int value.
//
//   void put(string key, int val)  -> key daalo. pehle se hai -> value UPDATE karo (size nahi badhe).
//   int  get(string key)           -> value do. key na ho -> -1.
//   void remove(string key)        -> key hatao. key hai hi nahi -> kuch mat karo, crash nahi.
//   int  size()                    -> kitni distinct keys.
//
// Tests (neeche main me, sab PASS aane chahiye):
//   put a=10, b=20             -> get a = 10, get b = 20, get c = -1, size 2
//   put b=25 (update)          -> get b = 25, size 2
//   remove a                   -> get a = -1, size 1
//   remove "zzz" (hai hi nahi) -> size 1
//   put k0..k49 = i*i          -> get k7 = 49, get k49 = 2401, size 51
//   put k0=999 (update, bheed me) -> get k0 = 999, size 51
//   remove k7                  -> get k7 = -1, get k8 = 64, size 50
// ============================================================

#include <bits/stdc++.h>
using namespace std;

// ---- diya hua (LeetCode jaisa) ----
struct Entry
{
    string key;
    int value;
    Entry *next;
    Entry(string k, int v) : key(k), value(v), next(nullptr) {}
};

// ---- kaam: constructor + neeche ke 4 method ----
class MyHashMap
{
    int sz;
    int cap;
    vector<Entry *> buckets; // buckets[i] = us chain ka pehla node (ya nullptr)

public:
    MyHashMap()
    {
    }

    void put(string key, int val)
    {
    }

    int get(string key)
    {
        return -1;
    }

    void remove(string key)
    {
    }

    int size()
    {
        return 0;
    }
};

int t = 0, pass = 0;
void check(string naam, int got, int exp)
{
    t++;
    if (got == exp) pass++;
    cout << "T" << t << " " << naam << ": " << (got == exp ? "PASS" : "FAIL") << "  got=" << got << " exp=" << exp << "\n";
}

int main()
{
    MyHashMap m;

    m.put("a", 10);
    m.put("b", 20);
    check("get a", m.get("a"), 10);
    check("get b", m.get("b"), 20);
    check("get c (nahi hai)", m.get("c"), -1);
    check("size", m.size(), 2);

    m.put("b", 25);
    check("get b (update)", m.get("b"), 25);
    check("size (update pe nahi badhe)", m.size(), 2);

    m.remove("a");
    check("get a (hata)", m.get("a"), -1);
    check("size", m.size(), 1);
    m.remove("zzz");
    check("size (zzz tha hi nahi)", m.size(), 1);

    for (int i = 0; i < 50; i++)
        m.put("k" + to_string(i), i * i);
    check("get k7", m.get("k7"), 49);
    check("get k49", m.get("k49"), 2401);
    check("size", m.size(), 51);

    m.put("k0", 999);
    check("get k0 (update bheed me)", m.get("k0"), 999);
    check("size (nahi badhe)", m.size(), 51);

    m.remove("k7");
    check("get k7 (hata)", m.get("k7"), -1);
    check("get k8 (padosi bacha)", m.get("k8"), 64);
    check("size", m.size(), 50);

    cout << "\n" << pass << "/" << t << " PASS\n";
    return 0;
}
