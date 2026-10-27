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

## 02 SLIDING WINDOW

**KAB:** CONTIGUOUS subarray / substring + "sabse lambi / sabse chhoti / max / count" maanga

**CODE (ek hi dhaancha, fixed + variable dono):**
```cpp
int i = 0, j = 0, ans = 0;
while (j < n) {
    // arr[j] window me ADD (sum / freq-map / zeros ...)
    while (window KHARAB) {          // size fixed (k) ho to IF, condition pe ho to WHILE
        // arr[i] window se REMOVE
        i++;
    }
    ans = max(ans, j - i + 1);       // LONGEST: shrink ke BAAD record
    j++;
}
// SHORTEST: jab tak VALID hai tab tak shrink, aur record shrink loop ke ANDAR (ans = min)
// COUNT:    count += j - i + 1
```

**DHYAAN:**
- shrink me count 0 ho jaaye to map se ERASE karo, warna `mp.size()` galat bolega
- NEGATIVE numbers ho to SW nahi chalti -> PREFIX + MAP
- "lagaataar" par har server / user ka alag -> upar NAAM-SE-CONFUSE E (map + ginti)

**POORA:** [PATTERN 2](PATTERN_SHEET.md#pattern-2--sliding-window)

---

## 03 HASHING

**KAB:** "pehle dekha?" · "jodi / complement hai?" · "kitni baar aaya?" · grouping (anagram)

**CODE (seen / complement / count, ek pass):**
```cpp
unordered_map<int, int> mp;          // kabhi PRELOAD: subarray-sum-k me mp[0] = 1
for (int i = 0; i < n; i++) {
    if (mp.count(target - nums[i]))  // CHECK pehle
        return {mp[target - nums[i]], i};
    mp[nums[i]] = i;                 // UPDATE baad (warna x khud se match)
}
```

**CODE (per-key ginti, jaise server streak):**
```cpp
unordered_map<string, int> mp;       // key = kiska (server / user)
for (auto &s : logs) {
    stringstream ss(s);
    string server, code, msg;
    ss >> server >> code >> msg;
    if (msg == "error") {
        mp[server]++;
        if (mp[server] >= 3) { count++; mp[server] = 0; }
    }
    else mp[server] = 0;             // toota = 0, ghatao nahi
}
```

**DHYAAN:**
- 1-to-1 mapping (isomorphic / word pattern) -> DO map, ek kaafi nahi
- longest consecutive vs LIS -> upar NAAM-SE-CONFUSE B

**POORA:** [PATTERN 3](PATTERN_SHEET.md#pattern-3--hashing)

---

## 04 PREFIX SUM

**KAB:** range sum baar-baar · "kitne subarray ka sum = K" (negative bhi) · left sum = right sum

**CODE (prefix + map, sum == K):**
```cpp
unordered_map<int, int> mp;
mp[0] = 1;                           // SHURU se: poora prefix khud K ho to
int sum = 0, count = 0;
for (int x : nums) {
    sum += x;
    if (mp.count(sum - k)) count += mp[sum - k];
    mp[sum]++;
}
```

**DHYAAN:**
- `mp[0] = 1` bhoolna nahi ([3], K=3)
- divisible by k: C++ me minus % minus deta -> `((sum % k) + k) % k`
- max sum vs count sum == K -> upar NAAM-SE-CONFUSE C

**POORA:** [PATTERN 4](PATTERN_SHEET.md#pattern-4--prefix-sum)

---

## 05 MATRIX

**KAB:** 2D matrix pe index ka khel: spiral, transpose, set zeroes, row / col ginti

**CODE (spiral, 4 boundary):**
```cpp
int top = 0, bottom = m - 1, left = 0, right = n - 1;
while (top <= bottom && left <= right) {
    for (int j = left; j <= right; j++) ans.push_back(mat[top][j]);
    top++;
    for (int i = top; i <= bottom; i++) ans.push_back(mat[i][right]);
    right--;
    if (top <= bottom) {
        for (int j = right; j >= left; j--) ans.push_back(mat[bottom][j]);
        bottom--;
    }
    if (left <= right) {
        for (int i = bottom; i >= top; i--) ans.push_back(mat[i][left]);
        left++;
    }
}
```

**DHYAAN:**
- islands / rotting / max area matrix jaise dikhte hain par GRAPH (BFS / DFS) hain -> 13
- search 2D matrix = sorted -> BINARY SEARCH (07)
- copy-pen pe grid bana ke index chalao

**POORA:** [PATTERN 5](PATTERN_SHEET.md#pattern-5--matrix)

---

## 06 STACK

**KAB:** brackets match · "har element ka agla / pichhla bada ya chhota" · span · histogram

**CODE (monotonic stack, next greater):**
```cpp
stack<int> st;                       // INDEX rakho
vector<int> ans(n, -1);
for (int i = 0; i < n; i++) {
    while (!st.empty() && nums[st.top()] < nums[i]) {   // < = GREATER dhoondh rahe
        ans[st.top()] = nums[i];     // ya doori: i - st.top()
        st.pop();
    }
    st.push(i);
}
// SMALLER -> > · PREVIOUS -> loop ulta · CIRCULAR -> 2n loop, i % n
```

**DHYAAN:**
- stack me INDEX rakho, value nahi (doori chahiye hoti hai)
- histogram vs container / trapping -> upar NAAM-SE-CONFUSE A
- generate parentheses stack nahi, BACKTRACKING (16)

**POORA:** [PATTERN 6](PATTERN_SHEET.md#pattern-6--stack)

---

## 07 BINARY SEARCH

**KAB:** SORTED + ek jagah / threshold dhoondhna · "MIN / MAX value jisme condition sach" (answer pe BS)

**CODE (basic):**
```cpp
int low = 0, high = n - 1;
while (low <= high) {
    int mid = low + (high - low) / 2;
    if (nums[mid] == target) return mid;
    else if (nums[mid] < target) low = mid + 1;
    else high = mid - 1;
}
```

**CODE (answer pe BS, jaise koko / ship):**
```cpp
int low = *max_element(...), high = accumulate(...);   // lo = max, hi = sum
while (low < high) {
    int mid = low + (high - low) / 2;
    if (possible(mid)) high = mid;   // mid khud answer ho sakta
    else low = mid + 1;
}
return low;
```

**DHYAAN:**
- mid KHUD answer ho sakta -> `high = mid` + `while (low < high)` · nahi -> `high = mid - 1` + `<=`
- `high = mid` ke saath `<=` = infinite loop
- answer pe BS: `lo = max(arr)`, 0 nahi

**POORA:** [PATTERN 7](PATTERN_SHEET.md#pattern-7--binary-search)

---

## 08 LINKED LIST

**KAB:** node / next diya · middle · cycle · ulta karo · nth from end · do list jodo

**CODE (reverse):**
```cpp
ListNode *prev = NULL, *curr = head;
while (curr != NULL) {
    ListNode *next = curr->next;     // pehle aage ka bachao
    curr->next = prev;
    prev = curr;
    curr = next;
}
return prev;
```

**CODE (fast / slow):**
```cpp
ListNode *slow = head, *fast = head;
while (fast != NULL && fast->next != NULL) {
    slow = slow->next;
    fast = fast->next->next;
    if (slow == fast) return true;   // cycle
}
```

**DHYAAN:**
- cycle ka SHURU chahiye -> milne ke baad slow ko head pe, dono 1-1 step
- jodna / hatana -> DUMMY node (head bhi badal sakta)
- har node ka kaam -> `curr != NULL` · aakhri pe rukna -> `curr->next != NULL`

**POORA:** [PATTERN 8](PATTERN_SHEET.md#pattern-8--linked-list)

---

## 09 DESIGN

**KAB:** "class banao": HashMap / HashSet built-in ke bina · LRU cache · get / put O(1)

**CODE (LRU ka dhaancha):**
```cpp
// list<pair<int,int>> dll;                          // aage = taaza, peeche = purana
// unordered_map<int, list<pair<int,int>>::iterator> mp;   // key -> list me jagah
// get(key): nahi -> -1 · hai -> uthake AAGE rakho, value lautao
// put(key, val): hai -> hatao · bhar gaya -> PEECHE wala hatao (mp se bhi) · naya AAGE
```

**DHYAAN:**
- HashMap: array of buckets, har bucket ek list · `index = hash(key) % size` · same key aaye to update
- LRU me map se bhi hatana mat bhoolna

**POORA:** [PATTERN 9](PATTERN_SHEET.md#pattern-9--design-data-structure)

---

## 10 KADANE

**KAB:** subarray ka MAX sum / product · stock ka ek baar kharido-becho

**CODE (max sum):**
```cpp
int sum = 0, maxi = INT_MIN;
for (int x : nums) {
    sum += x;
    maxi = max(maxi, sum);
    if (sum < 0) sum = 0;            // minus me gaya -> naya shuru
}
```

**CODE (max product: min bhi saath):**
```cpp
int maxP = nums[0], minP = nums[0], ans = nums[0];
for (int i = 1; i < n; i++) {
    if (nums[i] < 0) swap(maxP, minP);   // minus x minus = bada
    maxP = max(nums[i], maxP * nums[i]);
    minP = min(nums[i], minP * nums[i]);
    ans = max(ans, maxP);
}
```

**DHYAAN:**
- circular: `max(normal, total - minSubarray)`, par SAB negative ho to sirf normal
- max sum vs count sum == K -> upar NAAM-SE-CONFUSE C

**POORA:** [PATTERN 10](PATTERN_SHEET.md#pattern-10--kadane-running-value--max-update)

---

## 11 BIT MANIPULATION

**KAB:** har number 2 baar, ek akela · extra memory nahi

**CODE:**
```cpp
int x = 0;
for (int n : nums) x ^= n;           // a ^ a = 0, a ^ 0 = a
return x;
```

**DHYAAN:**
- sorted / 1..n / dohra -> XOR se pehle upar NAAM-SE-CONFUSE D dekh

**POORA:** [PATTERN 11](PATTERN_SHEET.md#pattern-11--bit-manipulation-xor)

---

## 12 TREES

**KAB:** TreeNode diya · depth / height / path / same / mirror · level-wise · BST

**CODE (har tree sawaal ka dhaancha):**
```cpp
int solve(TreeNode *root) {
    if (root == NULL) return BASE;   // null pe ruko
    int left = solve(root->left);
    int right = solve(root->right);
    return COMBINE(left, right);     // max / && / + ...
}
```

**CODE (level order):**
```cpp
queue<TreeNode*> q;
q.push(root);
while (!q.empty()) {
    int sz = q.size();               // is level ke kitne
    for (int i = 0; i < sz; i++) {
        TreeNode *node = q.front();
        q.pop();
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}
```

**DHYAAN:**
- max path sum: `left = max(0, left)` (minus wali arm mat lo)
- parent ko height chahiye aur answer kuch aur -> height RETURN, answer GLOBAL (diameter)
- BST ka inorder = sorted

**POORA:** [PATTERN 12](PATTERN_SHEET.md#pattern-12--trees-recursion)

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

---

## 14 HEAP

**KAB:** top K / k-th sabse bada ya chhota · baar-baar min / max aur data badal raha · merge K list · median stream

**CODE (top K bade = MIN-heap size k):**
```cpp
priority_queue<int, vector<int>, greater<int>> pq;   // MIN-heap
for (int x : nums) {
    pq.push(x);
    if (pq.size() > k) pq.pop();     // sabse chhota bahar, k bade bache
}
return pq.top();                     // k-th largest
```

**DHYAAN:**
- k sabse BADE -> MIN-heap · k sabse CHHOTE -> MAX-heap
- comparator: B KO DEKHO -> `a < b` = MAX-heap · `a > b` = MIN-heap
- pointer push se pehle NULL check (merge K)
- median stream = DO heap (max + min), size barabar rakho

**POORA:** [PATTERN 14](PATTERN_SHEET.md#pattern-14--heap)

---

## 15 INTERVALS

**KAB:** [start, end] diye · merge · meeting · kitne room · insert

**CODE (merge):**
```cpp
sort(intervals.begin(), intervals.end());          // START pe sort
vector<vector<int>> ans;
for (auto &it : intervals) {
    if (ans.empty() || ans.back()[1] < it[0])      // overlap nahi
        ans.push_back(it);
    else
        ans.back()[1] = max(ans.back()[1], it[1]); // overlap -> end badhao
}
```

**DHYAAN:**
- MERGE -> start pe sort · MAX pick / MIN hatao -> END pe sort (greedy) · kitne ek saath -> line sweep
- touching (2 aur 2) overlap hai ya nahi -> `<` ya `<=` sawaal se tay karo

**POORA:** [PATTERN 15](PATTERN_SHEET.md#pattern-15--intervals)

---

## 16 BACKTRACKING

**KAB:** "SAARE" subsets / permutations / combinations · grid me word dhoondo · N-queens

**CODE (choose -> explore -> un-choose):**
```cpp
void solve(int start, vector<int> &temp) {
    ans.push_back(temp);                 // subsets: har node pe record
    for (int i = start; i < n; i++) {
        temp.push_back(nums[i]);         // CHOOSE
        solve(i + 1, temp);              // EXPLORE (combination sum me reuse -> i)
        temp.pop_back();                 // UN-CHOOSE
    }
}
// permutations: start nahi, used[] + temp.size() == n pe record
```

**DHYAAN:**
- beech me `return solve(...)` doosri branch maar deta hai
- `else if` mat lagao, dono branch alag `if`
- grid: visit mark -> explore -> UNMARK

**POORA:** [PATTERN 16](PATTERN_SHEET.md#pattern-16--backtracking-choose--explore--un-choose)

---

## 17 DP

**KAB:** "kitne tareeke" · "min / max cost" · "ho sakta hai?" + har step pe choice + same sawaal baar-baar

**CODE (house robber: paas-paas ke 2 ghar nahi loot sakte, MAX paisa):**
```cpp
// [2,7,9,3,1] -> 12 (2 + 9 + 1)
int solve(vector<int> &nums, int i, vector<int> &dp)
{
    if (i < 0)
        return 0;                                // koi ghar nahi bacha
    if (i == 0)
        return nums[0];
    if (dp[i] != -1)                             // -1 = abhi nahi nikala
    {
        return dp[i];
    }
    int LOOT = nums[i] + solve(nums, i - 2, dp); // ye loota -> bagal wala chhodo
    int SKIP = 0 + solve(nums, i - 1, dp);       // ye chhoda -> pichhla khula
    return dp[i] = max(LOOT, SKIP);
}
int rob(vector<int> &nums)
{
    int n = nums.size();
    vector<int> dp(n + 1, -1);
    return solve(nums, n - 1, dp);
}
```

**DHYAAN:**
- min le raha -> invalid = INT_MAX · max -> INT_MIN · 0 sirf tab jab 0 sach me valid ho (coin change)
- recursion me `string` / `vector` `&` se bhejo, warna har call pe copy (bade test pe TLE)
- 2 string (LCS / edit distance) -> `dp[i][j]`
- LIS vs longest consecutive -> upar NAAM-SE-CONFUSE B

**POORA:** [PATTERN 17](PATTERN_SHEET.md#pattern-17--dp-dynamic-programming--recursion--memo)

---

## 18 GREEDY

**KAB:** har step pe abhi ka best chuno aur wo aage bhi best rahe · aksar pehle SORT, phir ek pass

**CODE (jump game: kahan tak pahunch sakte):**
```cpp
int reach = 0;
for (int i = 0; i < n; i++) {
    if (i > reach) return false;     // yahan tak pahunche hi nahi
    reach = max(reach, i + nums[i]);
}
return true;
```

**DHYAAN:**
- greedy tabhi jab abhi ka best = aage ka best, warna DP
- intervals me "kam se kam hatao" -> END pe sort
- gas station: tank minus hua -> agla station naya start

**POORA:** [PATTERN 18](PATTERN_SHEET.md#pattern-18--greedy)
