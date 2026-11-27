## PATTERN 9 — DESIGN (data-structure)


> [!IMPORTANT]
> **BROAD IDEA:** data-structure ko khud banao (built-in ke bina) -> array + pointers/nodes se.
>
> **KAB:** "class banao": HashMap / HashSet built-in ke bina · LRU cache · get / put O(1)

> [!IMPORTANT]
> **★ DHYAAN:** HashMap: array of buckets, har bucket ek list · index = hash(key) % size · same key aaye to update

> [!WARNING]
> **★ DHYAAN:** LRU me map se bhi hatana mat bhoolna



████████████████████████████████████████████████

### ▸ DESIGN HASHMAP (LLD)

**SAWAAL** Built-in map use kiye bina apna HashMap banao (string key, int value). put(key, val): key pehle se ho to value badlo, warna naya jodo. get(key): value lautao, na mile to -1. remove(key): key hatao.

- **e.g.** `put("a",1) put("b",2) put("a",5) get("a") -> 5 · size() -> 2 · remove("b") get("b") -> -1`
- **kyun:** "a" dobara aaya to naya node nahi, value 5 ho gayi -> keys sirf a,b = 2 · b hata to -1

### VISUAL

```
VISUAL:                                      Entry = | key | value | next |
   buckets
   ┌────┐
 0 │ - ──┼──> NULL
   ├────┤
 1 │ - ──┼──> | apple | 5 | - ─|──> NULL
   ├────┤
 3 │ - ──┼──> | cat | 9 | - ─|──> | dog | 4 | - ─|──> NULL     <- COLLISION (chain)
   └────┘
   buckets[i] = us chain ka HEAD (ya NULL).
```

### REMOVE head vs beech

```
REMOVE head (prev==null):            REMOVE beech (prev exist):
   buckets[i]->[A]->[B]->NULL           [A]->[B]->[C]->NULL   (remove B)
               ^curr                          ^prev ^curr
   buckets[i] = curr->next              prev->next = curr->next
   => buckets[i]->[B]->NULL             => [A]->[C]->NULL
(redo ke liye poora code: file 08_DSA/09_DESIGN/01_hashmap.cpp)
```

> [!IMPORTANT]
> **CHAABI:** ARRAY of buckets, har slot ek CHAIN. `index = hash<string>{}(key) % cap`. put: chain me key MILI -> update, na mili -> END pe jodo + `count++`. remove: `prev == NULL` -> `buckets[index] = curr->next`, warna `prev->next = curr->next`.

> [!WARNING]
> **Galti:** put me key MILI, value update ki par return nahi  ->  **Sahi:** update + TURANT `return`
> **Kyun:** loop aage chalta, chain ke end pe DUPLICATE node lag jaata aur size galat (test pass ho ke bhi code galat).
>
> **Galti:** remove me advance `prev = prev->next`  ->  **Sahi:** `prev = curr; curr = curr->next`
> **Kyun:** shuru me prev NULL hai, `NULL->next` crash.

**CODE**
```cpp
struct Entry {                              // Entry = | key | value | next |
    string key;
    int value;
    Entry *next;
    Entry(string k, int v) : key(k), value(v), next(nullptr) {}
};

class MyHashMap {                           // ARRAY (buckets), har slot ek CHAIN
    int count;                              // kitne pairs
    int cap;                                // slots = 16
    vector<Entry *> buckets;                // buckets[i] = chain ka HEAD, khud POINTER: isliye
                                            //   array-assign (buckets[i] = X) + node-link (X->next) MIX

public:
    MyHashMap() {
        count = 0;
        cap = 16;
        buckets.resize(cap);                // resize -> sab slot nullptr
    }

    void put(string key, int val) {
        int index = hash<string>{}(key) % cap;   // ★ % cap: hash bada number, slot 0..cap-1 me laana
        Entry *node = new Entry(key, val);
        if (buckets[index] == nullptr) {    // khaali slot: naya node hi chain ka head
            buckets[index] = node;
            count++;
        }
        else {                              // same index (collision) -> chain me jodo
            Entry *curr = buckets[index];
            // curr != NULL (curr->next != NULL nahi): AAKHRI node pe bhi khade hoke uski key dekhni hai;
            // wahan else ke if me curr->next == NULL dikhega -> usi ke next me naya node jod denge
            while (curr != NULL) {
                if (curr->key == key) {
                    curr->value = val;      // key MILI -> value update
                    return;                 // ★ TURANT return: warna END pe DUPLICATE node + count++
                }                           // (chain ke ANDAR update pe phat-ta; test-pass != code-sahi)
                else {
                    if (curr->next == nullptr) {   // next null = chain ka END: yahin jodo
                        curr->next = node;  //   (agle round naya node match hoke return)
                        count++;
                    }
                }
                curr = curr->next;
            }
        }
    }

    int get(string key) {
        int index = hash<string>{}(key) % cap;
        Entry *curr = buckets[index];
        while (curr != NULL) {
            if (curr->key == key)
                return curr->value;
            curr = curr->next;
        }
        return -1;                          // poori chain dekh li, key nahi mili
    }

    void remove(string key) {
        int index = hash<string>{}(key) % cap;
        Entry *curr = buckets[index];
        Entry *prev = NULL;                 // prev=NULL: head ke peeche koi node nahi
        while (curr != NULL) {
            if (curr->key == key) {
                if (prev != NULL)
                    prev->next = curr->next;        // BEECH: [A]->[B]->[C], B hata -> A->next = C
                else
                    buckets[index] = curr->next;    // ★ prev NULL = HEAD hi hata: bucket ka head badlo
                count--;
            }
            prev = curr;                    // prev = prev->next NAHI: prev NULL -> crash
            curr = curr->next;
        }
    }

    int size() {
        return count;
    }
};
```



████████████████████████████████████████████████

### ▸ DESIGN HASHSET (LLD)  = HASHMAP ka EXACT code, bas VALUE nikaal do

**SAWAAL** Built-in set use kiye bina apna HashSet banao (sirf string key). add(key): dohrav nahi daalna. contains(key): hai to true, warna false. remove(key): hatao.

- **e.g.** `add("apple") add("banana") add("apple") · contains("apple") -> true · size() -> 2 · remove("apple") -> contains false`
- **kyun:** doosra "apple" pehle se tha, kuch nahi juda -> 2 key · remove ke baad apple gayab

> [!IMPORTANT]
> **CHAABI:** HASHMAP ka hi code, Entry me sirf `| key | next |`. add: key mili -> kuch nahi, na mili -> chain END. contains = get ka true/false. remove HUBAHU same.

> [!WARNING]
> **Galti:** remove me advance `prev = prev->next`  ->  **Sahi:** `prev = curr; curr = curr->next`
> **Kyun:** prev NULL se non-head remove pe `NULL->next` crash.

**CODE**
```cpp
struct Entry {                              // HashMap ka EXACT code, bas VALUE nahi: | key | next |
    string key;
    Entry *next;
    Entry(string k) : key(k), next(nullptr) {}
};

class MyHashSet {
    int sz;
    int cap;
    vector<Entry *> buckets;

public:
    MyHashSet() {
        sz = 0;
        cap = 16;
        buckets.resize(cap);
    }

    void add(string key) {                  // HashMap ke put() jaisa
        int index = hash<string>{}(key) % cap;
        Entry *node = new Entry(key);
        if (buckets[index] == nullptr) {    // bucket khaali -> node laga + sz++
            buckets[index] = node;
            sz++;
        }
        else {
            Entry *curr = buckets[index];
            while (curr != nullptr) {
                if (curr->key == key)
                    return;                 // ★ mili -> kuch nahi (duplicate skip)
                else {
                    if (curr->next == nullptr) {
                        curr->next = node;  // na-mili -> chain END me append + sz++ (HashMap jaisa)
                        sz++;
                    }
                }
                curr = curr->next;
            }
        }
    }

    bool contains(string key) {             // get() jaisa, value ki jagah true / false
        int index = hash<string>{}(key) % cap;
        Entry *curr = buckets[index];
        while (curr != nullptr) {
            if (curr->key == key)
                return true;
            curr = curr->next;
        }
        return false;
    }

    void remove(string key) {               // HashMap ka remove HUBAHU
        int index = hash<string>{}(key) % cap;
        Entry *curr = buckets[index];
        Entry *prev = NULL;
        while (curr != nullptr) {
            if (curr->key == key) {
                if (prev == nullptr)
                    buckets[index] = curr->next;    // HEAD hata -> bucket ko curr->next se jodo
                else
                    prev->next = curr->next;        // beech: prev->next = curr->next
                sz--;
                return;
            }
            prev = curr;                    // ★ prev = prev->next NAHI: NULL->next CRASH
            curr = curr->next;
        }
        return;
    }

    int size() {
        return sz;
    }
};
```



████████████████████████████████████████████████

### ▸ LRU CACHE (LC146) — COMBO: hashmap + doubly-linked-list

**SAWAAL** Ek fixed capacity wala cache banao. get(key): value lautao, na ho to -1. put(key, val): daalo ya badlo. Dono O(1) me. Cache bhar jaaye to wo key hatao jo sabse lambe samay se use nahi hui (least recently used).

- **e.g.** `cap=2: put(1,1) put(2,2) get(1) -> 1 · put(3,3) · get(2) -> -1 · put(4,4) · get(1) -> -1 · get(3) -> 3 · get(4) -> 4`
- **kyun:** get(1) se 1 taaza, put(3) pe 2 sabse purana -> hata · put(4) pe ab 1 purana -> hata, bache 3,4

### VISUAL

```
VISUAL:                     Node = | key | val | prev | next |
   mp:{ 1->●, 3->● }
          │    │
          v    v
    [HEAD]<->|1|<->|3|<->[TAIL]
    (dummy)  MRU   LRU   (dummy)
```

> [!IMPORTANT]
> **CHAABI:** hashmap = KAHAN (`key -> Node*`), DLL dummy `head`/`tail` = ORDER (head-side MRU, tail-side LRU). get aur put dono me `removeNode` + `addFront`. Full pe `tail->prev` hatao + map se USI node ki key se (`mp.erase(toBeRemoved->key)`).

> [!WARNING]
> **Galti:** key check ke liye `mp[key]`  ->  **Sahi:** `mp.count(key)` / `mp.find(key)`
> **Kyun:** `mp[key]` missing key pe chup-chaap nayi entry daal deta.
>
> **Galti:** DLL rewire me seedha overwrite  ->  **Sahi:** overwrite se PEHLE `nxt` temp me pakdo
> **Kyun:** link badalte hi aage wala node haath se chala jaata.
>
> **Galti:** constructor me member ka type dobara likha (`int cap = capacity;`)  ->  **Sahi:** sirf `cap = capacity;`
> **Kyun:** type likha to naya local ban jaata (shadowing), member khaali reh jaata.
>
> **Galti:** put me key pehle se ho to sirf `addFront`  ->  **Sahi:** pehle `removeNode`, phir `addFront`
> **Kyun:** node purani jagah pe bhi juda rehta, `tail->prev` use hi dekhta, abhi update hua wahi evict (cap=2: put1, put2, put(1,10), put3 -> get(1) = -1, sahi 10).
>
> **Galti:** `mp.erase(lru->val)`  ->  **Sahi:** `mp.erase(toBeRemoved->key)`
> **Kyun:** map me val naam ki key hi nahi, kuch nahi hatta; gaya hua key wapas milta, size badhta rehta. Test me key != val rakho (1,100), warna ye chhup jaata.

**CODE**
```cpp
struct Node {                               // Node = | key | val | prev | next |
    int key, val;
    Node *prev, *next;
    Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {                            // get + put DONO O(1): hashmap (KAHAN) + DLL (ORDER)
    int cap;
    unordered_map<int, Node *> mp;          // key -> DLL node ka POINTER (find O(1))
    Node *head, *tail;                      // dummy [HEAD] <-> MRU .. LRU <-> [TAIL] dummy (edge clean)

public:
    LRUCache(int capacity) {
        cap = capacity;
        head = new Node(-1, -1);            // TRAP: yahan "Node *head =" likha = shadowing (naya local)
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }

    void removeNode(Node *node) {           // beech se nikal, O(1)
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

    void addFront(Node *node) {
        Node *nxt = head->next;             // ★ nxt PEHLE pakdo: head->next overwrite se pehle (TRAP)
        head->next = node;
        node->prev = head;

        node->next = nxt;
        nxt->prev = node;                   // node ab head ke turant baad = sabse naya (MRU)
    }

    int get(int key) {
        if (mp.count(key) > 0) {            // count se: mp[key] chup-chaap entry bana deta
            Node *node = mp[key];
            removeNode(node);
            addFront(node);                 // access = recent -> front pe
            return node->val;
        }
        return -1;
    }

    void put(int key, int value) {
        if (mp.count(key) > 0) {
            Node *node = mp[key];
            node->val = value;
            removeNode(node);               // ★ DONO, get() jaisa. sirf addFront = node purani
            addFront(node);                 //   jagah bhi juda -> wahi evict (put1 put2
        }                                   //   put(1,10) put3 -> get(1) = -1, sahi 10)
        else {
            if (mp.size() == cap) {         // ★ FULL: pehle evict, phir naya (check add se PEHLE)
                Node *toBeRemoved = tail->prev;     // tail->prev = sabse purana (tail khud dummy)
                removeNode(toBeRemoved);
                mp.erase(toBeRemoved->key); // ★ KEY se (->val NAHI: wo key hi nahi,
            }                               //   kuch nahi hatta; test me key != val, 1,100)
            Node *newNode = new Node(key, value);
            addFront(newNode);
            mp[key] = newNode;
        }
    }
};
```


