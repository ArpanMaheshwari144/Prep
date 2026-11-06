## PATTERN 9 — DESIGN (data-structure)

```
 BROAD IDEA: data-structure ko khud banao (built-in ke bina) -> array + pointers/nodes se.

 KAB: "class banao": HashMap / HashSet built-in ke bina · LRU cache · get / put O(1)
 ★ DHYAAN: HashMap: array of buckets, har bucket ek list · index = hash(key) % size · same key aaye to update
 ★ DHYAAN: LRU me map se bhi hatana mat bhoolna
```

### ▸ DESIGN HASHMAP (LLD)

```
Built-in map use kiye bina apna HashMap banao (string key, int value). put(key, val): key pehle se ho to value badlo, warna naya jodo.
get(key): value lautao, na mile to -1. remove(key): key hatao.
  e.g. put("a",1) put("b",2) put("a",5) get("a") -> 5 · size() -> 2 · remove("b") get("b") -> -1
  kyun: "a" dobara aaya to naya node nahi, value 5 ho gayi -> keys sirf a,b = 2 · b hata to -1
```

```
PURANI SHEET SE (same to same):

PURANI SHEET SE (same to same):

MENTAL MODEL: ek ARRAY (buckets); har slot me ek LINKED-LIST (chain). buckets[index] = us chain ka HEAD.
   index = hash<string>{}(key) % cap.  (exact C++ syntax: hash<string>{}(key) -- string hash.)  same index pe 2 key (collision) -> chain me jode (next se).
fields: buckets (Entry* ka array) · cap (slots = 16) · sz (kitne pairs).   // ctor: sz=0, cap=16, buckets.resize(cap) -> sab slot nullptr

TRICK: index = hash<string>{}(key)%cap -> us bucket ki chain me traverse -> key match pe kaam; put me end tak na-mili -> naya node append. (collision = same index -> chain)
  put(key,val):  int index = hash<string>{}(key) % cap;   Entry* node = new Entry(key,val);   // % cap: hash bada number, slot 0..cap-1 me laana
                 if(buckets[index] == nullptr){ buckets[index] = node; sz++; }   // khaali slot: naya node hi chain ka head
                 else{
                    Entry* head = buckets[index];
                    while(head != NULL){
                       if(head->key == key){ head->value = val; return; }   // return: warna loop aage chalta, END pe DUPLICATE node + sz++
                       else if(head->next == nullptr){ head->next = node; sz++; }   // next null = chain ka END: yahin jodo (agle round naya node match hoke return)
                       head = head->next;
                    }
                 }
  get(key):      Entry* head = buckets[index];
                 while(head != NULL){ if(head->key == key) return head->value;  head = head->next; }
  remove(key):   Entry* head = buckets[index], *prev = NULL;   // prev=NULL: head ke peeche koi node nahi
                 while(head != NULL){
                    if(head->key == key){ if(prev) prev->next = head->next;  else buckets[index] = head->next;  sz--; }   // prev NULL = HEAD hi hata: tab bucket ka head badlo
                    prev = head;  head = head->next;   // prev = head (prev->next nahi): prev NULL ho to crash
                 }

PUT TRAP (7-redo se pakka): key MILI -> value update + TURANT return. return NA kiya to loop aage
   DUPLICATE node laga deta + sz galat. ("test-pass != code-sahi" -- chain-ke-ANDAR update pe phat-ta.)

COMBO (yahi naya/tricky tha): buckets[index] khud ek POINTER hai (chain ka head)
  -> isliye array-assign (buckets[i]=X) aur node-link (X->next) MIX ho jaate = array + linked-list.

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

REMOVE head (prev==null):            REMOVE beech (prev exist):
   buckets[i]->[A]->[B]->NULL           [A]->[B]->[C]->NULL   (remove B)
               ^curr                          ^prev ^curr
   buckets[i] = curr->next              prev->next = curr->next
   => buckets[i]->[B]->NULL             => [A]->[C]->NULL
(redo ke liye poora code: file 08_DSA/09_DESIGN/01_hashmap.cpp)

                    while(head != NULL){
                       head = head->next;
                 while(head != NULL){



   buckets
   ┌────┐
 0 │ - ──┼──> NULL
   ├────┤
   ├────┤
   └────┘
```

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

### ▸ DESIGN HASHSET (LLD)

```
Built-in set use kiye bina apna HashSet banao (sirf string key). add(key): dohrav nahi daalna. contains(key): hai to true, warna false. remove(key): hatao.
  e.g. add("apple") add("banana") add("apple") · contains("apple") -> true · size() -> 2 · remove("apple") -> contains false
  kyun: doosra "apple" pehle se tha, kuch nahi juda -> 2 key · remove ke baad apple gayab
```

```
PURANI SHEET SE (same to same):

= HashMap ka EXACT code, bas VALUE nikaal do. CONNECTION:
┌──────────┬──────────────────────────────┬──────────────────────────────┐
│ ASPECT   │ HashMap                      │ HashSet                      │
├──────────┼──────────────────────────────┼──────────────────────────────┤
│ Entry    │ | key | VALUE | next |       │ | key | next |   (value NAHI)│
├──────────┼──────────────────────────────┼──────────────────────────────┤
│ insert:  │ put(k,v)                     │ add(k)                       │
│  mili    │   value UPDATE + return      │   kuch nahi (duplicate skip) │
│  na-mili │   chain END me append + sz++ │   chain END me append + sz++ │  <- DONO SAME
│          │   UPDATE-then-RETURN trap  │   (koi trap nahi -- simpler) │
├──────────┼──────────────────────────────┼──────────────────────────────┤
│ lookup   │ get(k): value / -1           │ contains(k): true / false    │
│ delete   │ remove(k)                    │ remove(k): HUBAHU SAME       │
│ size     │ return sz                    │ SAME                         │
└──────────┴──────────────────────────────┴──────────────────────────────┘
SAME (dono me): bucket khaali -> node laga + sz++; index = hash<string>{}(key)%cap; chaining/collision; remove head-vs-beech; VISUAL/COMBO (upar HashMap entry).
BUG-CATCH (remove, khud pakda): advance = "prev = curr; curr = curr->next"
   (NA prev = prev->next -- prev NULL se non-head remove pe NULL->next = CRASH). (file 08_DSA/09_DESIGN/02_hashset.cpp)
```

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

### ▸ LRU CACHE (LC146) — COMBO: hashmap + doubly-linked-list

```
Ek fixed capacity wala cache banao. get(key): value lautao, na ho to -1. put(key, val): daalo ya badlo. Dono O(1) me.
Cache bhar jaaye to wo key hatao jo sabse lambe samay se use nahi hui (least recently used).
  e.g. cap=2: put(1,1) put(2,2) get(1) -> 1 · put(3,3) · get(2) -> -1 · put(4,4) · get(1) -> -1 · get(3) -> 3 · get(4) -> 4
  kyun: get(1) se 1 taaza, put(3) pe 2 sabse purana -> hata · put(4) pe ab 1 purana -> hata, bache 3,4
```

```
PURANI SHEET SE (same to same):

MENTAL MODEL: get+put DONO O(1). 2 zaroorat -> 2 structure JODO:
   hashmap: key -> DLL-node ka POINTER  (find O(1))
   DLL:     dummy [HEAD] <-> MRU .. LRU <-> [TAIL] dummy  (reorder/evict O(1))
   head-side=recent · tail-side=purana(evict) · dummy head+tail = edge-case clean.
   => hashmap = KAHAN hai (find), DLL = ORDER (reorder/evict). jodne se dono O(1).

VISUAL:                     Node = | key | val | prev | next |
   mp:{ 1->●, 3->● }
          │    │
          v    v
    [HEAD]<->|1|<->|3|<->[TAIL]
    (dummy)  MRU   LRU   (dummy)

fields: unordered_map<int,Node*> mp · Node{key,val,prev,next} · Node *head,*tail (dummy) · cap.
ctor:   dummy head,tail bana -> head<->tail link · cap=capacity.   (TRAP-3)   // dummy = new Node(-1,-1) dono

CRUX = 2 DLL helper (yehi asli mechanic, get/put inko call karte):
   removeNode(n): n->prev->next=n->next;  n->next->prev=n->prev;      // beech se nikal, O(1)
   addFront(n):   nxt=head->next;  head->next=n; n->prev=head;        // nxt PEHLE pakdo (TRAP-2)
                  n->next=nxt; nxt->prev=n;                           // n ab head ke turant baad = sabse naya (MRU)

get(key):  mp.count(key)==0 -> -1.  warna: Node* node = mp[key]; removeNode(node); addFront(node); return node->val;   // count se check: mp[key] chup-chaap khaali entry bana deta
put(k,v):  hai  -> node->val=v  -> removeNode + addFront.   ★ DONO, get() jaisa (neeche TRAP-4) (sirf addFront kiya to node purani jagah bhi juda, wahi evict hota)
           naya -> FULL? Node* lru = tail->prev;  removeNode(lru);   // tail->prev = sabse purana (tail khud dummy hai) · FULL = mp.size()==cap
                         mp.erase(lru->key);          // ★ KEY se (map key se dhoondhta). ->val NAHI (TRAP-5)
                  phir new node addFront + mp[k]=node.   // pehle evict, phir naya: size==cap check add se PEHLE

TRAP: (1) mp[key] missing pe CHUP-CHAAP entry INSERT -> check ke liye mp.count()/find(). (C++ jaal)
      (2) DLL rewire: overwrite se PEHLE temp me pakdo (nxt).  (3) ctor me type-dobara = shadowing (naya local).
      (4) ★ (24-Sep, base + REDO_1 dono me tha) put() me key pehle se ho to sirf addFront kiya, removeNode NAHI
          -> node purani jagah pe BHI juda reh gaya, tail->prev use hi dekhta -> jo abhi update hua wahi evict.
          cap=2: put1, put2, put(1,10), put3 -> get(1) = -1 aaya (sahi 10), get(2) = 2 (sahi -1).
      (5) mp.erase(lru->val) likha -> map me val naam ki key hi nahi, kuch nahi hata -> gaya hua key
          get() pe wapas milta (100 aata, sahi -1), mp.size() badhta rehta -> aage evict hi band.
          test me key == val ho (1,1 · 2,2) to ye chhup jaata -> test me key != val rakho (1,100).

PEHLI SHEET (Aug) se bhi, same to same:

CRUX = 2 DLL helper (yehi asli mechanic, get/put inko call karte):
   removeNode(n): n->prev->next=n->next;  n->next->prev=n->prev;      // beech se nikal, O(1)
   addFront(n):   nxt=head->next;  head->next=n; n->prev=head;        // nxt PEHLE pakdo (TRAP-2)
                  n->next=nxt; nxt->prev=n;                           // n = MRU

get(key):  mp.count(key)==0 -> -1.  warna: Node* node = mp[key]; removeNode(node); addFront(node); return node->val;
put(k,v):  hai  -> node->val=v  -> removeNode + addFront.
           naya -> FULL? tail->prev(=LRU) removeNode + mp.erase(uski key); phir new node addFront + mp[k]=node.
```

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

