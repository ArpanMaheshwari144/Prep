## PATTERN 14 — HEAP

```
> ★ **YAHAN NAHI MILENGI** ("k-th / median" sun ke heap lagta hai, par nahi):
> ```
> median of 2 sorted arrays  ->  PATTERN 7 (BINARY SEARCH)
>       stream nahi hai — do SORTED array hain -> ek CUT dhoondo, O(log)
> kth smallest in BST        ->  PATTERN 12 (TREES)
>       BST ka INORDER khud sorted deta hai -> k-1 wala element, heap ki zaroorat nahi
> ``` (priority_queue: min/max top pe)

> ★★ **COMPARATOR — "B KO DEKHO" (Arpan ka apna rule, ratta nahi):**
> ```
> comparator me sirf ye dekho ki  b  kis taraf pada hai:
>
>     return a < b;      ->  b BADA wala hai     ->  MAX-heap
>     return a > b;      ->  b CHHOTA wala hai   ->  MIN-heap
> ```
> **Ye sirf trick nahi, theek bhi hai.** Comparator asal me poochta hai *"kya `a` ki priority `b`
> se KAM hai?"*. `a < b` likhne ka matlab hua "chhota = kam priority" -> **bada upar** = max-heap.
> "b ko dekho" seedha wahi baat pakad leta hai, bina poora vaakya sochre.
>
> Bade objects pe bhi wahi:
> ```
> return a->val > b->val;      ->  b chhota  ->  MIN-heap
> return a.dist  < b.dist;     ->  b bada    ->  MAX-heap
>
> priority_queue<int>                             -> max-heap (default)
> priority_queue<int, vector<int>, greater<int>>  -> min-heap
> ```
> (Doosra tareeka, agar kabhi shak ho: comparator = SORT ka niyam. `a<b` = ascending `[1,3,5,9]`.
> `sort()` **shuru** se padhta hai, `priority_queue` **aakhir** se uthata hai -> 9 top pe = max-heap.
> Dono raaste ek hi jagah pahunchte hain.)

 BROAD IDEA: baar-baar MIN ya MAX + data BADALTA -> HEAP (sort mahenga). top O(1), push/pop O(log n).
   C++ syntax:  priority_queue<int> = MAX-heap  ·  priority_queue<int, vector<int>, greater<int>> = MIN-heap.
   ops: pq.push(x) · pq.top() · pq.pop() · pq.size() · pq.empty().
   REAL: emergency-room (severity top pe).
   KAB: top-K · k-th largest/smallest · merge-k-lists · Dijkstra · median-stream.
   MIRROR-RULE: "k sabse BADE" -> MIN-heap size k ; "k sabse CHHOTE" -> MAX-heap size k. O(n log k).
 ★ EDGE (top K): MIN-heap of size k (sab max-heap me daal ke k pop = O(n log n), haar gaye).
 ★ EDGE (comparator): B KO DEKHO -> a < b = b bada = MAX-heap · a > b = b chhota = MIN-heap (neeche poora).
 ★ EDGE (pointer push): push se PEHLE null check (merge-k: khaali list push -> top()->val pe SEGFAULT).

```

#### ── FAMILY: heap of size k / top-K (size > k pe top EVICT, k bache) ──

```
 KAB: top K / k-th sabse bada ya chhota · baar-baar min / max aur data badal raha · merge K list · median stream
```

### ▸ LAST STONE WEIGHT (LC-1046, Easy)  = MAX-HEAP smash

```
stones[] me pattharon ka wazan diya hai. Har baari do sabse bhaari patthar takraate hain: barabar hon to dono khatam, warna bhaari wala farak jitna bachta hai.
Aakhir me jo patthar bache uska wazan lautao, koi na bache to 0.
  e.g. [3,7,2] -> 2
  kyun: 7,3 -> 4 bacha · 4,2 -> 2 bacha · ek hi bacha = 2
```

```cpp
int lastStoneWeight(vector<int> &stones) {  // har turn 2 SABSE BHAARI smash, diff wapas, till <=1 bache
    priority_queue<int> pq;                 // ★ MAX: har turn 2 sabse BHAARI chahiye
    for (int i = 0; i < stones.size(); i++)
        pq.push(stones[i]);
    while (pq.size() > 1) {                 // >1: smash ko 2 pathar chahiye
        int x = pq.top();                   // sabse bada
        pq.pop();
        int y = pq.top();                   // doosra sabse bada
        pq.pop();
        int diff = abs(y - x);
        if (diff > 0)                       // barabar -> 0 = dono khatam
            pq.push(diff);                  // bacha pathar wapas heap me
    }
    return pq.empty() ? 0 : pq.top();       // ★ empty pehle: sab khatam to top() crash
}   // heap kyun (sort nahi): smash ke baad set BADALTA -> baar-baar "2 max"
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
TEMPLATE:
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     stones[]. har turn 2 sabse bhaari ladao: barabar -> dono gaye, warna diff bache. aakhri bacha lautao (warna 0).
```

### ▸ KTH LARGEST ELEMENT (LC-215, Medium)  = MIN-HEAP of size k

```
Ek array aur ek number k diya hai. Array ka k-va sabse bada number lautao.
  e.g. [3,2,1,5,6,4], k=2 -> 5
  kyun: bade se gino 6, 5 -> 2nd = 5
```

```cpp
int findKthLargest(vector<int> &nums, int k) {   // top-K ka core
    priority_queue<int, vector<int>, greater<int>> pq;   // ★ MIN: size>k pe sabse CHHOTA evict
    for (auto &it : nums) {
        pq.push(it);
        if (pq.size() > k)
            pq.pop();                       // sabse chhota nikaalo -> sirf k SABSE BADE bache
    }
    return pq.top();                        // k bade me se sabse chhota = k-th largest
}   // O(n log k) (sort O(n log n) se behtar)
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : k SABSE BADE min-heap me rakho -> unme se sabse chhota (heap TOP) = k-th largest.
TRICK (top-K ka core): MIN-heap of size k.  (kyun MIN: size>k pe sabse-chhota EVICT karna hai.)
TEMPLATE:
    priority_queue<int, vector<int>, greater<int>> pq;   // MIN-heap (min: size>k pe sabse chhota bahar nikle)
    for(num : nums){ pq.push(num);  if(pq.size() > k) pq.pop(); }   // k se zyada -> chhota nikaal
    return pq.top();                                      // k bade me se chhota = kth largest
O(n log k) (sort O(n log n) se behtar jab sirf k-th chahiye).  yehi pattern = "top-K" saare problems.
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     nums[] + k. k-th SABSE BADA element lautao.
```

### ▸ K CLOSEST POINTS TO ORIGIN (LC-973, Medium)  = MAX-HEAP of size k

```
points[] me [x, y] points diye hain aur ek number k. Origin (0, 0) ke sabse paas wale k points lautao. Doori = x*x + y*y, aur jawab kisi bhi kram me chalega.
  e.g. [[1,3],[-2,2]], k=1 -> [[-2,2]]
  kyun: [1,3] = 1+9 = 10, [-2,2] = 4+4 = 8 -> 8 chhota

KTH-LARGEST ka MIRROR: wahan k BADE (min-heap) · yahan k CHHOTE dist (MAX-heap, sabse BADI evict)
```

```cpp
vector<vector<int>> kClosest(vector<vector<int>> &points, int k) {
    priority_queue<pair<int, vector<int>>> maxHeap;   // ★ MAX: sabse door top pe, size>k pe wahi nikle
    for (int i = 0; i < points.size(); i++) {
        maxHeap.push({{points[i][0] * points[i][0] + points[i][1] * points[i][1]}, {points[i]}});
                                            // {dist, point}: dist PEHLE (pair first pe compare)
                                            // dist = x²+y², √ NAHI (order same)
        if (maxHeap.size() > k)
            maxHeap.pop();                  // sabse door EVICT -> k paas bache
    }
    vector<vector<int>> ans;
    while (maxHeap.size() > 0) {
        ans.push_back(maxHeap.top().second);   // .second = {x,y} poora
        maxHeap.pop();
    }
    return ans;
}
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : origin (0,0) ke k SABSE PAAS points. paas = doori = x*x+y*y (√ NAHI, sirf compare).
= KTH-LARGEST ka MIRROR-TWIN. wahan k BADE (min-heap, chhota evict);
TEMPLATE (max-heap {dist, point}, size k):
    priority_queue<pair<int, vector<int>>> pq;   // MAX-heap: sabse BADI dist top pe (taaki size>k pe sabse door wala nikle)
    for(auto &p : points){
        int d = p[0]*p[0] + p[1]*p[1];           // √ skip -> compare-safe
        pq.push({d, p});                          // {dist, point} (dist PEHLE: heap pair ke first pe compare karta)
        if(pq.size() > k) pq.pop();               // sabse door (top) EVICT -> k paas bache
    while(!pq.empty()){ ans.push_back(pq.top().second); pq.pop(); }  // .second = {x,y} poora
KEY : (1) dist = x²+y², √ ki zaroorat nahi (order same).  (2) k CHHOTE chahiye -> MAX-heap
      (bada evict).  pair<int,vector<int>> = dist PEHLE (compare), point .second me poora.
```

### ▸ TOP K FREQUENT ELEMENTS (LC-347, Medium)  = MIN-HEAP size k on FREQUENCY

```
Ek array aur ek number k diya hai. Wo k numbers lautao jo array me sabse zyada baar aate hain. Kram kuch bhi chalega.
  e.g. [1,1,1,2,2,3], k=2 -> [1,2]
  kyun: 1 -> 3 baar, 2 -> 2 baar, 3 -> 1 baar -> top 2 = 1, 2

KTH-LARGEST ka COUSIN, farak 2: (1) pehle hashmap se freq (2) heap me {freq, num}
```

```cpp
vector<int> topKFrequent(vector<int> &nums, int k) {
    unordered_map<int, int> mp;
    for (int i = 0; i < nums.size(); i++)
        mp[nums[i]]++;                      // STEP 1: freq gino
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;
                                            // MIN: size>k pe sabse KAM freq bahar
    for (auto &it : mp) {
        minHeap.push({it.second, it.first});   // ★ {freq, num}: freq PEHLE -> heap freq pe sort
        if (minHeap.size() > k)
            minHeap.pop();                  // chhoti freq EVICT -> top-k bache
    }
    vector<int> ans;
    while (!minHeap.empty()) {
        int tp = minHeap.top().second;      // .second = actual number
        minHeap.pop();
        ans.push_back(tp);
    }
    return ans;
}   // O(n log k). max-heap me saare push -> k baar pop = O(n log n), slow
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : k sabse ZYADA baar aane wale elements return karo (order matter nahi).
= KTH-LARGEST ka COUSIN. bas "value" ki jagah "FREQUENCY" pe heap.  FARAK = 2:
TEMPLATE:
    unordered_map<int,int> mp;  for(x:nums) mp[x]++;              // STEP1: freq gino
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;  // MIN-heap (min: size>k pe sabse kam freq bahar nikle)
        if(pq.size() > k) pq.pop();                              // chhoti freq EVICT -> top-k bache
    while(!pq.empty()){ ans.push_back(pq.top().second); pq.pop(); }  // .second = actual num
KEY : pair<freq,num> daalte hain (freq PEHLE) taaki heap FREQUENCY pe compare kare.  O(n log k).
[maxHeap se bhi: saare push -> k baar top-pop.  par O(n log n), slow. min-heap-k better.]
```

#### ── FAMILY: MIN-heap of current candidates (har baar sabse chhota nikaalo, uska agla daalo) ──

### ▸ MERGE K SORTED LISTS (LC-23, Hard)  = MIN-HEAP of current heads

```
k sorted linked list di hain. Sabko jod ke ek sorted linked list banao aur uska head lautao.
  e.g. [[1,4,5],[1,3,4],[2,6]] -> [1,1,2,3,4,4,5,6]
  kyun: har baar teeno ke current head me sabse chhota uthao: 1,1,2,3,4,4,5,6

struct Compare: ListNode* ko khud compare karna nahi aata
```

```cpp
struct Compare {                            // ListNode* ko khud compare karna nahi aata
    bool operator()(ListNode *a, ListNode *b) {
        return a->val > b->val;             // ★ > likha to MIN-heap (pq ka comparator ulta chalta)
    }
};

ListNode *mergeKLists(vector<ListNode *> &lists) {
    priority_queue<ListNode *, vector<ListNode *>, Compare> minHeap;
    for (auto &l : lists)
        if (l)
            minHeap.push(l);                // saare heads; NULL skip (warna ->val pe crash)
    ListNode *dummy = new ListNode(0);      // merge-2-lists jaisa: dummy + tail
    ListNode *tail = dummy;
    while (!minHeap.empty()) {
        ListNode *curr = minHeap.top();     // sabse chhota
        minHeap.pop();
        tail->next = curr;                  // jodo
        tail = curr;
        if (curr->next)
            minHeap.push(curr->next);       // ★ uska next; khatam list ka NULL mat daalo
    }
    return dummy->next;                     // ->next: dummy khud nakli node
}   // heap me max k node (1 per list) -> O(N log k)
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : min-heap me har list ka current HEAD. pop chhota -> result me jodo -> uska NEXT push. repeat.
TEMPLATE:
heap me max k nodes (1 per list) -> O(N log k).  ListNode* min-heap = custom Compare (a->val > b->val).
```

### ▸ SORT NEARLY-SORTED (k-sorted) ARRAY  = MIN-HEAP size k+1 (sliding buffer)

```
Ek array diya hai jisme har number apni sahi (sorted) jagah se zyada se zyada k khaane door hai. Poora sorted array lautao.
  e.g. [6,5,3,2,8,10,9], k=3 -> [2,3,5,6,8,9,10]
  kyun: 2 abhi index 3 pe, sahi jagah index 0 -> 3 door = k ke andar

har element sahi jagah se max k door. index-0 ka MIN max index k tak -> "pehle k+1 me MIN kaun?"
```

```cpp
vector<int> sortKSorted(vector<int> &arr, int k) {
    vector<int> ans;
    priority_queue<int, vector<int>, greater<int>> pq;   // MIN: sorted output me chhota pehle
    for (int i = 0; i < arr.size(); i++) {
        pq.push(arr[i]);                    // pehle daalo, phir size dekho
        if (pq.size() > k + 1) {            // ★ k+1 se bada: tab tak sahi MIN buffer me aa chuka
            int tp = pq.top();
            pq.pop();
            ans.push_back(tp);              // buffer bhara -> MIN nikaal
        }
    }
    while (!pq.empty()) {                   // bache = sorted tail
        int tp = pq.top();
        pq.pop();
        ans.push_back(tp);
    }
    return ans;
}   // [6,5,3,2,8,10,9], k=3 -> [2,3,5,6,8,9,10]. O(n log k). kth-largest ka cousin (sliding buffer)
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : har element sahi position se max k DOOR -> O(n log k) sort (n log n se behtar).
DERIVE (WHY heap): index-0 pe MIN aayega, wo max index k tak -> "pehle k+1 me se MIN kaun?"
   -> baar-baar MIN + data badalta -> DECISION-table = HEAP. min chahiye -> MIN-heap.
TEMPLATE (min-heap, k+1 sliding buffer):
  priority_queue<int,vector<int>,greater<int>> pq;              // MIN-heap (min: sorted output me chhota pehle chahiye)
EX: arr=[6,5,3,2,8,10,9], k=3 -> [2,3,5,6,8,9,10]  (buffer hamesha <=4; bharte hi MIN pop).
= KTH-LARGEST ("heap size k") ka COUSIN: wahan k bade RAKHE, yahan k+1 ka SLIDING buffer.  O(n log k).
```

#### ── FAMILY: TWO-HEAPS (do team, dono top = beech ke border) ──

### ▸ FIND MEDIAN FROM DATA STREAM (LC-295)  = TWO-HEAPS balanced

```
Numbers ek-ek karke aate hain (addNum). Kabhi bhi findMedian() bulaya jaaye to ab tak aaye saare numbers ka median lautao.
Odd ginti pe beech wala, even pe beech ke do ka average.
  e.g. add 5, 15, 1, 3 -> findMedian = 4.0
  kyun: sorted 1,3,5,15 -> beech ke do 3,5 -> (3+5)/2 = 4

stream me kabhi bhi median turant. add 5,15,1,3 -> 4.0 ((3+5)/2)
═════════════════════════ ★ EDGE ═════════════════════════════════════
heap ka NAAM top batata, number kaun rakhta wo NAHI. "bada -> maxHeap" GALAT:
  bada -> BADI team -> minHeap. naam bhool, team soch.
```

```
TASVEER:

★ 2 balance-if KYUN (dono zaroori): placement num ko DONO heap me daal sakti (chhota->maxH, bada->minH).
   FIRST  (maxH < minH)    -> bada-num minH me gaya, minH bada -> ek wapas maxH (extra maxH me rahe)
   SECOND (maxH > minH+1)  -> chhota-num maxH me gaya, maxH 2 bada -> ek minH
   dono milke: farak <=1, extra hamesha maxHeap.

DRY: add 5,15,1,3
   5  -> maxH{5}                              med 5
   15 -> 15>5 minH{15}                        med (5+15)/2 = 10
   1  -> 1<=5 maxH{5,1}                        med 5  (maxH bada)
   3  -> 3<=5 maxH{5,3,1} farak2 -> 5 minH me -> maxH{3,1} minH{5,15}
                                              med (3+5)/2 = 4 ✓

PURANI SHEET KI KYUN LINE (code ke saath wali):
CHAAL: median = beech wala; poora sorted rakhna mehnga. Numbers ko DO team me baanto:
       maxHeap = CHHOTI team   top = chhoto ka SABSE BADA    <- naam se ULTA (chhote yahan)
       minHeap = BADI team     top = bado ka SABSE CHHOTA
       dono top = beech ke 2 border -> median inhi se.
```

```cpp
class MedianFinder {
public:
    priority_queue<double> maxHeap;         // CHHOTI team: top = chhoto ka sabse bada (border)
    priority_queue<double, vector<double>, greater<double>> minHeap;   // BADI team: top = bado ka min

    void addNum(int num) {
        if (maxHeap.empty() || maxHeap.top() >= num)   // ★ empty() pehle: khaali pe top() crash
            maxHeap.push(num);              // 1. PLACEMENT: chhota -> chhoti team
        else
            minHeap.push(num);              //    bada -> badi team (minHeap)
        if (maxHeap.size() < minHeap.size()) {   // 2. BALANCE (dono if zaroori: num kisi bhi heap me)
            double it = minHeap.top();      // minHeap bada: extra hamesha maxHeap me -> ek wapas
            minHeap.pop();
            maxHeap.push(it);
        }
        else if (maxHeap.size() > minHeap.size() + 1) {   // ★ 2 ka farak: maxHeap sirf 1 aage chalta
            double it = maxHeap.top();
            maxHeap.pop();
            minHeap.push(it);
        }
    }

    double findMedian() {
        if (maxHeap.size() == minHeap.size())
            return (maxHeap.top() + minHeap.top()) / 2.0;   // even: dono border ka average
        return maxHeap.top();               // odd: extra maxHeap me, uska top = median
    }
};   // 5 -> 5 · 15 -> 10 · 1 -> 5 · 3 -> farak 2, 5 minHeap me -> {3,1} {5,15} -> 4
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR : stream me numbers aate rehte; kabhi bhi ab-tak ka MEDIAN turant chahiye.
addNum (2 kadam):
findMedian: barabar(even) -> (maxHeap.top+minHeap.top)/2 ; warna(odd) -> maxHeap.top
TEMPLATE:
       return maxHeap.top();  // odd: extra maxHeap me rakha, uska top hi median
★ TRAP (yahin confuse hua): heap ka NAAM top batata, "kaise number rakhta" NAHI.
FAMILY: heap (kth-largest / k-sorted / merge-k) · running-value + do-heap-balance
```

#### ── FAMILY: MAX-heap + greedy (har round sabse zyada wala lagao) ──

### ▸ REORGANIZE STRING (LC-767)  = MAX-HEAP + POP-TWO-AT-A-TIME

```
Ek string s di hai. Uske akshar aise lagao ki koi bhi do paas-paas wale akshar same na hon. Koi bhi ek sahi string lautao, aisa na ban sake to khaali string "".
  e.g. "aab" -> "aba" · "aaab" -> ""
  kyun: aab: b dono a ke beech · aaab: 3 a alag karne 2 diwaar chahiye, b sirf 1

koi 2 ADJACENT same na ho, na bane -> "". top-k-frequent ka cousin: har round 2 nikaalo
possible tabhi: (maxCount-1) gap <= (n-maxCount) baaki. aab OK · aaab -> ""
  (alag check na bhi rakho: aakhri bacha count>1 khud pakad leta)
```

```
TASVEER:

VISUAL ( "aaabbb" ):        heap (3,a)(3,b)
   R1: pop a,b -> "ab"   -> (2,a)(2,b)
   R2: pop a,b -> "abab" -> (1,a)(1,b)
   R3: pop a,b -> "ababab" -> empty      => "ababab"
  ( "aaab": (3,a)(1,b) -> R1 "ab",(2,a) -> size1, count2>1 -> "" )

PURANI SHEET KI KYUN LINE (code ke saath wali):
PEHCHAN: "rearrange so adjacent not same" / "sabse-zyada wala spread karo"
         -> MAX-HEAP {count,char}. (top-k-frequent ka cousin: wahan k nikaale,
          yahan har round 2 nikaalo taaki adjacent-safe.)
FEASIBILITY (dry-run se khud nikli): maxCount-1 gaps, (n-maxCount) se bharo
```

```cpp
string reorganizeString(string s) {
    string ans = "";
    unordered_map<char, int> mp;
    for (auto &it : s)
        mp[it]++;
    priority_queue<pair<int, char>> maxHeap;   // {count, char}: count PEHLE, us pe sort
    for (auto &it : mp)
        maxHeap.push({it.second, it.first});
    while (maxHeap.size() > 1) {            // ★ >1: har round 2 ALAG char (1 nikaal wapas = "aa")
        auto it1 = maxHeap.top();
        maxHeap.pop();
        auto it2 = maxHeap.top();
        maxHeap.pop();
        ans += it1.second;
        ans += it2.second;
        it1.first -= 1;                     // -- pehle, phir check
        it2.first -= 1;
        if (it1.first >= 1)                 // 0 bacha to wapas mat daalo
            maxHeap.push(it1);
        if (it2.first >= 1)
            maxHeap.push(it2);
    }
    if (!maxHeap.empty()) {
        if (maxHeap.top().first > 1)        // ★ akela bacha count>1: do same paas aayenge, impossible
            return "";
        else
            ans += maxHeap.top().second;
    }
    return ans;                             // heap khaali: saare char lag gaye
}   // "aaabbb" -> ab, abab, ababab · "aaab" -> ab, (2,a) akela count 2 -> ""
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR: chars rearrange -> koi 2 ADJACENT same na ho. na bane -> "".
   aab: a2 -> 1 gap <= 1 baaki -> OK.   aaab: a3 -> 2 gap > 1 -> "".
   (alag check na bhi rakho -> last-bacha-count>1 khud pakad leta.)
KYUN POP-TWO: 1 nikaal turant wapas -> phir wahi top -> "aa".
TEMPLATE: // pehle mp[ch]++, phir maxHeap.push({cnt,ch}) (cnt PEHLE: us pe sort)
   while(heap.size()>1){ t1=pop; t2=pop; ans+=t1.ch; ans+=t2.ch;  // >1: har round 2 ALAG char chahiye
      if(--t1.cnt) push(t1); if(--t2.cnt) push(t2); }  // -- pehle phir check: 0 bacha to wapas mat daalo
   if(heap.size()==1) return top.cnt>1 ? "" : ans+top.ch;  // akela bacha count>1: do same paas aayenge, impossible
   return ans;   // heap khaali: saare char jagah pe lag gaye
crux: MAX-HEAP + har round 2 alag nikaalo + last-1 count>1 = impossible.
FAMILY: heap (top-k-frequent cousin) · greedy-place-most-frequent
```

```
PURANI SHEET KI LINE (28-Oct PATTERN_SHEET se, upar is roop me nahi thi):
     string s ke akshar aise lagao ki koi 2 PADOSI same na ho. koi bhi ek valid lautao, na bane -> "".
```

#### ── FAMILY: MIN-heap + vis[] (lazy delete) ──

### ▸ THROTTLING — MIN UTHAO, PADOSI HATAO (Amazon OA)  = MIN-HEAP {val,idx} + vis[] (lazy skip)

```
responseTimes[] diya hai. Har kadam pe bache hue numbers me se sabse chhota uthao (barabar ho to chhota index), use jawab me jodo,
aur use aur uske dono padosi (i-1, i+1, jo bache hon) hata do. Ant me kul jod lautao.
  e.g. [4,2,9,1,7,3] -> 6
  kyun: 1 uthaya (9,7 hate) · 2 uthaya (4 hata) · 3 uthaya -> 1+2+3 = 6

[4,2,9,1,7,3] -> 1 (9,7 gaye) -> 2 (4 gaya) -> 3 = 6
BURST BALLOONS NAHI: wahan kaunsa phodna TU chunta + padosi badalte -> DP.
  yahan niyam tay (hamesha min) + ORIGINAL padosi -> koi choice nahi -> heap se simulate
```

```cpp
long long totalResponseTime(vector<int> &responseTimes) {
    int n = responseTimes.size();
    vector<bool> vis(n, false);
    long long ans = 0;                      // 1e9 x lakh -> long long
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;   // MIN
    for (int i = 0; i < n; i++)
        pq.push({responseTimes[i], i});     // pair: tie pe chhota idx apne aap pehle
    while (!pq.empty()) {
        int index = pq.top().second;        // .second = index (first sirf sort ke liye)
        pq.pop();
        if (vis[index] == false) {          // ★ hat chuka -> chhodo (LAZY delete, Dijkstra stale-skip)
            ans += responseTimes[index];
            vis[index] = true;              // khud bhi hatao, dobara na uthe
            if (index - 1 >= 0)             // >=0: pehle ka left padosi nahi
                vis[index - 1] = true;
            if (index + 1 < n)              // < n: aakhri index n-1
                vis[index + 1] = true;
        }
    }
    return ans;
}   // O(n log n). heap se beech ka element nikaal nahi sakte -> vis[] se pop pe skip
```

```
PURANI SHEET KI LINE (PATTERN_SHEET_2 se, upar is roop me nahi thi):
SAAR: har step bache hue me SABSE CHHOTA uthao (tie -> chhota index), total me jodo,
      use + ORIGINAL padosi (i-1, i+1) hata do. sab hatne tak.
PEHCHAN: kahani hata do -> "baar-baar MIN + beech me cheezein hat-ti" -> HEAP + hat-chuka array.
   sawaal: "kya mujhe chunna padta hai?" haan -> DP · nahi -> simulate.
TEMPLATE:
   priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;  // MIN {val,idx} (min: har step sabse chhota uthana)
       if(vis[i]) continue;                          // pehle hi hat chuka -> chhodo (LAZY delete)
       if(i+1 <  n) vis[i+1] = true;   // < n: aakhri index n-1, uske aage bahar
             (Dijkstra ke "stale-skip" jaisa hi).
FAMILY: heap (emergency-room: baar-baar min) · visited[] (graph jaisa) · lazy delete
```

