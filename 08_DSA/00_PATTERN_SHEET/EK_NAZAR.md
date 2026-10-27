# EK NAZAR — OA / interview se pehli raat

> Sirf yaad dilane ke liye: har pattern ka KAB + CODE ka dhaancha + DHYAAN.
> Poora code (SAWAAL + TAREEKA + 174 problem) = [PATTERN_SHEET.md](PATTERN_SHEET.md). Roz ka redo wahi se.
> Is file me: PEHLE YE · OA tools · DECISION · EDGE kyun · NAAM-SE-CONFUSE A-E · phir 18 pattern.

---

## ★ PEHLE YE — har problem pe (30 sec)

```
1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
```
> Detail neeche: DECISION (input + signal table) · EDGE kyun fisalta · NAAM-SE-CONFUSE (A-E) · phir 18 pattern.

## ★ OA KE TOOLS — C++ syntax jo DSA nahi, par OA me aata (27-Oct Wise se)

```cpp
// STRING TODNA (log "s1 200 success", "name:score", CSV)
#include <sstream>
stringstream ss(line);  string a, b, c;
ss >> a >> b >> c;                       // space se tukde: a="s1", b="200", c="success"
int x = stoi(b);                         // string -> int
string part; stringstream ss2(line);
while (getline(ss2, part, ','))  { }     // comma (ya ':') se tukde
int pos = line.find(' ');                // pehla space
string left = line.substr(0, pos), right = line.substr(pos + 1);

// MINUS % (C++ me -2 % 26 = -2, 24 NAHI)
int r = ((x % m) + m) % m;               // hamesha 0..m-1  (Caesar peeche khiskana, remainder)
char sh = 'A' + ((c - 'A' - k) % 26 + 26) % 26;   // k > 26 bhi chalega
```

---

## ★★★ DECISION — kaunsa DS/technique KAB aur KYUN (11-Aug, Arpan-diagnosed; sabse-upar)

> Neeche wali sheet PATTERNS deti hai (kya hote hain). Ye section DERIVATION deta hai — *"naya problem dekh ke kaise pata karun kaunsa lagega."*
> Arpan-diagnosis: abhi tak "WHAT hashmap hai" pata tha, "WHEN/WHY lagta hai" kabhi nahi. Ye wahi bharta. Recognition pe depend NAHI — REASON karo.

**STEP-0 — INPUT-SHAPE padho (story se PEHLE; disguise ke against #1 hathiyaar):**

> Story jhooth bol sakti (disguise), input-shape aksar nahi. Input dekh ke family SHORTLIST karo, phir story ka transform kholo.

| INPUT diya                           | → shortlist pattern |
|--------------------------------------|---------------------|
| **SORTED** array                     | two-pointer / binary-search (SORTED-FORK dekho) |
| **"K"** (largest/closest/frequent)   | **HEAP** |
| array + **target sum/relation**      | hashmap (unsorted) / two-pointer (sorted) |
| **NEGATIVES** + subarray-sum         | **PREFIX+HASHMAP** (SW nahi — window toot-ta) |
| **2D grid / matrix**                 | DFS/BFS (islands) / DP (paths) |
| **tree / linked-list NODE**          | recursion/DFS / BFS / fast-slow |
| **intervals [[s,e]]**                | sort + merge/sweep |
| **prices/stock** + span/next-bigger  | monotonic STACK / kadane |
| **"MIN/MAX value jo condition sach"**| **BINARY SEARCH on ANSWER** |
| **edges / adjacency**                | graph BFS/DFS/topo |
| string + **dictionary/words**        | DP (break) / Trie |

> 2-LAYER method: (1) input-shape → family shortlist, (2) phir story ka transform kholo. **Input pehle, story baad me.** (6-Sep, Arpan-derived — disguised-problems ke live-test se.)

**THE METHOD (har NAYE problem pe 4 step):**
1. **BRUTE FORCE likho** — hamesha hota hai (partial answer + waqt).
2. **Poocho:** *"main baar-baar SAME kya compute kar raha?"* → **REDUNDANCY** dhoondo.
3. Us redundancy ko **CACHE/STORE** karo → recompute ki jagah **LOOK UP**.
4. **Cache-ka-DS hi PATTERN hai** (guess nahi — DERIVE).
> + "STORY hatao — structure kya? pairs? range? next-bigger/smaller? min/max? subproblems?"

**KAUNSA DS LAGEGA — SIGNAL se DERIVE karo** (pehchan se nahi; in sabka koi template hota hi nahi):

| Jab ye SIGNAL dikhe | Brute me REDUNDANCY | → DS | KYUN |
|---|---|---|---|
| do element **sum/diff/relation = target** (UNSORTED) | "partner pehle dekha?" scan | **HASHMAP** | seen store, O(1) lookup |
| **SORTED** + pair dhoondo | dono end scan | **TWO-POINTER** | sorted → converge |
| **CONTIGUOUS** + longest/max (saare **POSITIVE**) | window-sum baar-baar | **SLIDING WINDOW** | expand/shrink monotonic |
| subarray-sum / sum=k (**NEGATIVES** ho sakte) | prefix baar-baar | **PREFIX + HASHMAP** | prefix seen → O(1) |
| har element **"pehla bada/chhota kaun"** | aage/peeche scan | **MONOTONIC STACK** | chhote-bekaar pop |
| **"abhi tak min/max / top-k"** stream | baar-baar sort | **HEAP** | ordered store, log n |
| **"running median / do halves balance"** | baar-baar sort | **TWO HEAPS** | max-heap + min-heap |
| lookups by **prefix / word-set** | string scan baar-baar | **TRIE** | char-by-char tree |

> ★ **"SORTED" FORK** (5-Sep, drill se): sorted dekh ke seedha BS mat firing karo — **DUSRA cue** padho:
> pair / sum from **TWO ENDS** → **TWO-POINTER** (converge, O(n)) · **single position / threshold / "min X jisme condition sach"** → **BINARY SEARCH**. (two-sum-sorted = 2-pointer, NA BS.)

---

## ESSENCE — LABEL pe bharosa mat karo (31-Jul, Arpan-derived)

> Problem samjho -> phir pattern -> phir code. Par asli baat ye:
> **kisi aur ka LABEL blindly mat lo** — "reel ne SW bola" ka matlab SW nahi.
> Khud examine karo: *"sach me aisa hai?"*

---

## ★★ EDGE LIST — har template ke saath uski EK line

> **KYUN ye section hai:** template har problem me lagta hai -> 50 baar chhua -> baith gaya.
> Edge sirf us EK shakal me lagta hai -> 3-4 baar chhua -> fisal jaata. Mushkil hone ki wajah
> se nahi, **kam baar saamne aane** ki wajah se.
>
> Doosri wajah: template ka DHAANCHA hota hai — ek tukda yaad aaya, baaki chain khud kheech
> aayi. Edge AKELA padta hai, ek line, uske aage-peeche kuch nahi jo use upar kheenche.
>
> Teesri: code likhte waqt dimaag SHAKAL pe hota hai; edge sirf tab dikhta hai jab koi KHAAS
> input chalao. Isliye ye sochne se nahi, **DRY-RUN se** nikalte hain.
>
> ★★ **CHAUTHI — aur yahi sabse asli hai (Arpan ne khud pakdi):** hum problem ko aksar DIFF me
> rakhte hain — *"X jaisa hi hai, bas ye badla"*. Dimaag poora X yaad rakh leta hai + wo diff.
> Par jo line X me **THI HI NAHI**, uske latakne ki koi jagah nahi bachti — wo bhooli nahi
> jaati, wo **andar jaati hi nahi**.
> ```
> max-path-sum ko store kiya:  "DIAMETER/maxDepth wala hi hai, bas  1 +  ki jagah  node->val"
>
> DIAMETER me   left/right = DEPTH  ->  kabhi negative ho hi nahi sakti
>                                   ->  clamp ki zaroorat hi nahi  ->  template me wo line NAHI
> MAX-PATH-SUM  left/right = SUM    ->  negative ho sakti
>                                   ->  tabhi  max(0, ...)  paida hota hai
> ```
> **NIYAM:** jab bhi kuch "X jaisa hai, bas ye badla" karke rakho, ek sawaal aur poochho —
> **"X me jo cheez THI HI NAHI, wo kya hai?"** Edge hamesha wahin chhupa hota hai.
>
> ★ Ye theek karne wali "kami" nahi hai — koi saare edges sar me le kar nahi ghoomta, log inhe
> CHALA KE pakadte hain. Ilaaj = ye list roz aankh ke saamne se guzre + har code pe ek chhota
> dushman-example chalao. Sheet dekh lene me koi ghaata nahi — ye DERIVE hone wali cheezein
> hain hi nahi, PATA honi chahiye — aur PATA-hui cheez sirf REPETITION se tikti hai.

> **AB EDGE KAHAN HAI:** har edge PATTERN_SHEET ke apne pattern block me, `★ EDGE:` line pe (code block ke andar, BROAD IDEA ke
> neeche). Redo se pehle block padhte hi saamne aayegi. Code likhne ke BAAD, submit se pehle, us line pe ek nazar.
> Nayi edge mile to usi block me ek `★ EDGE:` line jodo — nayi file kabhi nahi.

---

## NAAM-SE-CONFUSE MAT HO — ek-jaisi DIKHNE wali jodiyaan

> ★ Ye section knowledge ka nahi, QUESTION-PADHNE ka hai. Har entry wo jagah hai jahan keyword sun ke
> sabse KAREEB wala pattern upar aa gaya, par sawaal kuch aur poochh raha tha.
> Pattern bolne se PEHLE do shabd khud se poochho:
> ```
> 1. SUBARRAY (jud-hua) · SUBSEQUENCE (tod sakte, order wahi) · SUBSET (order bekaar) ?
> 2. MAX / MIN / COUNT / EXACT — kya maang raha ?
> ```

### A. Container vs Trapping vs Histogram
```
                    CONTAINER Most Water    TRAPPING Rain Water      LARGEST Rectangle (Histogram)
                    (LC-11)                 (LC-42)                  (LC-84)
   ───────────────────────────────────────────────────────────────────────────────────────────────
   kya poochha       2 line chuno -> MAX     baarish -> dips me       sabse BADA SOLID rectangle
                     paani BEECH me          kitna paani TRAP         (paani NAHI!)
   paani ya solid?   PAANI (2 walls beech)   PAANI (har dip me)       SOLID block (paani nahi)
   kitne bar use     sirf 2 (chuni hui)      SAARE bar                har bar apna rectangle
   formula           min(L,R) * width        min(leftMax,rightMax)    height * (NS - PS - 1)
                                              - height[i]
   SUM ya MAX?       MAX area                SUM (total paani)        MAX area
   technique         two-pointer             two-ptr O(1) / prefix    MONOTONIC stack
                     (CHHOTI wall andar)     leftMax/rightMax         (prev + next smaller)

   1-line: Container = 2-deewar-BEECH-max-paani · Trapping = har-gaddha-SUM · Histogram = SOLID-block-MAX (paani nahi).
   bada farak: Container/Trapping = PAANI · Histogram = SOLID.   Trapping = SUM · baaki dono = MAX.
```

### B. LIS vs LONGEST CONSECUTIVE (dono me "longest" + "badhta hua")
```
                    LIS                             LONGEST CONSECUTIVE
                    (LC-300)                        (LC-128)
   ────────────────────────────────────────────────────────────────────────────────
   kya poochha       sabse lambi BADHTI chain        sabse lambi chain of
                                                     CONSECUTIVE integers (+1, +1)
   kitna badhna      bas BADA ho ( > )               exactly +1
   ORDER matter?     HAAN — left se right            NAHI — shuffle karo, jawab wahi
   array shuffle     jawab BADAL jaata               jawab SAME rehta
   technique         DP  O(n^2)                      SET  O(n)
                     ya tails+lower_bound O(n log n)  (num-1 set me nahi -> wahi start)

   ★ TELL (question me ye shabd = LIS):  "order aage ka hi rahe" · "beech ke chhod sakte ho"
   ★ TELL (= consecutive):               "+1" · "lagataar numbers" · order ka zikr HI nahi

   CHECK: [10, 9, 2, 5, 3, 7, 101, 18]
          LIS = 4  (2,3,7,18)          consecutive = 2  (2,3  ya  9,10)
          -> ek hi array, do alag jawab. Yahi jodi drill me phansi thi — isliye yahan likhi hai.
```

### C. KADANE vs PREFIX+HASHMAP (dono me "subarray" + "sum")
```
                    KADANE                          PREFIX + HASHMAP
                    (LC-53)                         (LC-560)
   ────────────────────────────────────────────────────────────────────────────────
   kya poochha       MAXIMUM sum wala subarray       KITNE subarray ka sum == K
   jawab ka type     ek VALUE (max)                  ek COUNT
   target diya hai?  NAHI — khud max dhoondhna       HAAN — exact K diya hua
   negative allowed  haan (isi ke liye bana hai)     haan — aur isliye SW nahi chalti
   technique         running sum, 0 se neeche        running sum + map[running-K]
                     gaya to reset                   map[0] = 1 se shuru (ye line chhoot-ti hai)

   ★ TELL: "MAX/sabse-bada sum"   -> Kadane
   ★ TELL: "KITNE / count"  ya  "= K / exactly"  -> prefix + hashmap

   ★ AUR EK: agar saare number POSITIVE hon aur "sum = K" poochhe -> SLIDING WINDOW bhi chalti.
             negative aate hi window ka shrink-rule toot jaata -> tabhi prefix+hash zaroori.
```

### D. SINGLE vs SINGLE-SORTED vs DISAPPEARED vs DUPLICATE (charon me "ek number alag hai")
```
                    SINGLE NUMBER        SINGLE ELEMENT       DISAPPEARED          FIND DUPLICATE
                    (LC-136)             (LC-540)             (LC-448)             (LC-287)
   ─────────────────────────────────────────────────────────────────────────────────────────────────
   shart            har num 2 baar,      SORTED, har num      values 1..n,         values 1..n,
                    ek 1 baar            2 baar, ek 1 baar    size n, kuch repeat  size n+1, ek repeat
   maang raha       AKELA                AKELA                GAYAB (sab)          DOHRA
   time / space     O(n), O(1)           O(log n)             O(n), O(1)           O(1), array MAT BADLO
   technique        XOR                  BINARY SEARCH        index ko MINUS       FLOYD cycle
                                         (jodi ka index)      (array khud map)     (value = pointer)
   teri file        11/01                07/10                03/10                08/12

   PEHCHAAN KA KRAM (3 shabd dhoondho):
     1. "SORTED" + log n        -> binary search (XOR bhi chalta, par O(n) = jo maanga wo nahi)
     2. "VALUES 1..n"           -> value hi index ban sakti
                                   GAYAB maanga -> index pe minus · DOHRA + array mat badlo -> Floyd
     3. range nahi, sorted nahi, bas "2 baar aata" -> jodi apne aap kat jaati = XOR

   1-line: sorted = BS · 1..n = index ka khel (gayab = minus, dohra = Floyd) · baaki "jodi" = XOR.
```

### E. "LAGAATAAR" sun ke SLIDING WINDOW? pehle 2 sawaal (Wise OA 27-Oct)
```
   "lagaataar / consecutive" = SW ka signal LAGTA hai, par SW tabhi jab dono jawab "haan":

   1. KISKA lagaataar?   EK hi line ka (array / string)          -> SW socho
                         alag-alag KEY (server / user / card) ki,
                         sab ek line me MILE hue                    -> HASHMAP: har key ki APNI ginti
   2. KYA maanga?        sabse LAMBI / CHHOTI window, window ka SUM -> SW (window khiskao)
                         bas "k tak pahuncha?" (streak)             -> chalti GINTI: ++ , toota to = 0

   misaal: SERVER ERROR STREAK (03/11)
     "s1 error, s2 success, s1 error, s3 error, s1 error" -> s1 ke 3 lagaataar
     ek window lagao to s2, s3 beech me ginti tod dete -> GALAT. map[server]++ , success pe 0 -> SAHI
   (test me khud pakda tha: "i++ kis condition pe? baith nahi raha" = yahi signal, SW chhodo)

   1-line: lagaataar EK line ka + window chahiye = SW · har KEY ka apna lagaataar = MAP + ginti.
```

---

# 18 PATTERN — EK NAZAR

## 01 TWO POINTER

**KAB:** array SORTED hai aur do number ki jodi chahiye · ya array me IN-PLACE kuch hatana / aage karna hai

**CODE (dono sire se):**
```cpp
int l = 0, r = n - 1;
while (l < r) {
    int sum = nums[l] + nums[r];
    if (sum == target) return {l, r};
    else if (sum < target) l++;      // chhota -> left aage
    else r--;                        // bada -> right peeche
}
```

**CODE (slow / fast, in-place):**
```cpp
int slow = 0;
for (int fast = 0; fast < n; fast++) {
    if (nums[fast] != 0) {           // jo RAKHNA hai
        swap(nums[slow], nums[fast]);
        slow++;
    }
}
```

**DHYAAN:**
- 3-sum me duplicate SKIP karo (2-sum me nahi)
- container / trapping / histogram me confuse -> upar NAAM-SE-CONFUSE A

**POORA:** [PATTERN 1](PATTERN_SHEET.md#pattern-1--two-pointer)

---

## 13 GRAPHS

**KAB:** edges / adjacency list / grid diya · "kitne group?" · "A se B pahunch sakte?" · "kam se kam kitne step?"

**CODE (graph banao):**
```cpp
unordered_map<int, vector<int>> adj;
for (auto &e : edges) {
    adj[e[0]].push_back(e[1]);
    adj[e[1]].push_back(e[0]);       // undirected = DONO taraf (directed me sirf ek)
}
// GRID ho to adj nahi: dirs = {{-1,0},{1,0},{0,-1},{0,1}}, cell khud node
```

**CODE (BFS):**
```cpp
vector<bool> vis(n, false);
queue<int> q;
vis[src] = true;
q.push(src);
while (!q.empty()) {
    int node = q.front();
    q.pop();
    for (int nbr : adj[node]) {
        if (!vis[nbr]) {
            vis[nbr] = true;         // PUSH karte waqt mark
            q.push(nbr);
        }
    }
}
```

**CODE (kitne group):**
```cpp
int count = 0;
for (int i = 0; i < n; i++) {
    if (!vis[i]) {
        count++;                     // naya group
        BFS(i);
    }
}
```

**DHYAAN:**
- visited PUSH karte waqt mark karo, nikaalte waqt nahi (warna ek node kai baar queue me)
- `auto &it = q.front(); q.pop();` = toota reference -> pehle padho, phir pop
- BFS = level / kam se kam step · DFS = recursion, code chhota
- dependency / kram -> topo (indegree + queue) · weight wale raaste -> Dijkstra (min-heap + dist[])

**POORA:** [PATTERN 13](PATTERN_SHEET.md#pattern-13--graphs-bfsdfs--visited)
