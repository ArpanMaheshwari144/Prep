<h1 align="center">DSA PATTERN SHEET</h1>

## ★ PEHLE YE — har problem pe (30 sec)

```
1. INPUT dekho, story nahi: sorted? "K"? negatives? grid? tree/node? intervals? edges? -> family shortlist
2. Shabd pakdo: SUBARRAY / SUBSEQUENCE / SUBSET ?  ·  MAX / MIN / COUNT / EXACT ?
3. Kisi ka LABEL mat lo ("reel ne SW bola") -> khud dekho: sach me aisa hai?
4. Pattern nahi aaya -> BRUTE likho -> "baar-baar SAME kya nikal raha?" -> use STORE karo -> wo DS = pattern
5. "X jaisa hai, bas ye badla" laga -> poochho: X me jo THI HI NAHI, wo line kya hai? (edge wahin)
6. Code ke BAAD, submit se pehle: us pattern block ki ★ EDGE line + ek chhota dushman-example chalao
```
> Detail neeche: DECISION (input + signal table) · EDGE kyun fisalta · NAAM-SE-CONFUSE (A-D).

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

> **AB EDGE KAHAN HAI:** har edge apne pattern block me, `★ EDGE:` line pe (code block ke andar, BROAD IDEA ke
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

---

## PATTERN 1 — TWO POINTER

```

 BROAD IDEA: do pointer (dono taraf se, ya slow-fast) -> ek saath move karke O(n).
 ★ EDGE (sorted 2-ptr): duplicate skip karna hai kya? 3-sum HAAN, 2-sum nahi (bina skip ek triplet kai baar).
 ★ MILTE-JULTE (container / trapping / histogram): upar "NAAM-SE-CONFUSE" -> A dekh.

── FAMILY: slow/fast OVERWRITE (fast scan, rakhne wala slow pe; teeno same skeleton) ──

 ▸ MOVE ZEROES
     int slow = 0;                           // slow = agli jagah jahan non-zero rakhna hai
     int fast = 0;                           // fast = scan karta
     while (fast < nums.size())
         if (nums[fast] != 0)
             swap(nums[slow], nums[fast]);   // swap PEHLE
             slow++;                         // slow++ BAAD: non-zero apni jagah lag gaya tabhi aage
         fast++;                             // fast++ har baar (if ke BAHAR)

 ▸ REMOVE DUP (SORTED)
     int slow = 0; int fast = 0;             // slow = last UNIQUE ka index; 0 se: pehla khud unique
     while (fast < nums.size())
         if (nums[slow] != nums[fast])       // naya unique mila (compare nums[slow] se)
             slow++;                         // slow++ PEHLE: duplicate slot pe (move zeroes se ULTA)
             nums[slow] = nums[fast];        // overwrite BAAD
         fast++;                             // fast har baar
     return slow + 1;                        // unique count = slow + 1 (slow index hai)

 ▸ REMOVE ELEMENT
     int slow = 0, fast = 0;                 // slow = agli keep-jagah, fast scan
     while (fast < nums.size())
         if (nums[fast] != val)
             nums[slow] = nums[fast]; slow++;   // write PEHLE · slow++ BAAD (move zeroes jaisa)
         // nums[fast] == val -> skip, sirf fast++
         fast++;
     return slow;                            // slow = k, bache hue shuru ke k me

── FAMILY: left/right COMPARE-CONVERGE (dono chhor se andar, har step compare) ──

 ▸ VALID PALINDROME (SIMPLE)
     // sirf lowercase letters wala aasan version
     int left = 0; int right = s.size() - 1;   // size() - 1 : aakhri index, size nahi
     while (left <= right)
         if (s[left] != s[right])
             return false;                   // mismatch pe turant false
         left++; right--;
     return true;

 ▸ VALID PALINDROME (FULL, LC-125)
     // mixed char: sirf alnum ginna, case ignore (<cctype>)
     int left = 0; int right = s.size() - 1;   // aakhri index, size nahi
     while (left <= right)
         if (tolower(s[left]) == tolower(s[right]))   // 1. match -> dono andar
             left++; right--;
         else if (!isalnum(s[left]))  left++;   // 2. left alnum nahi -> skip
         else if (!isalnum(s[right]))  right--;   // 3. right alnum nahi -> skip
         else
             return false;                   // 4. dono alnum PAR alag -> false
     return true;                            // koi alnum na ho / khaali -> true

 ▸ VALID PALINDROME II
     // isPalindrome: helper: l..r palindrome hai?

     // validPalindrome: palindrome YA ek char delete karke ban jaaye
     int left = 0; int right = s.size() - 1;
     while (left <= right)
         if (s[left] != s[right])            // mismatch: left delete YA right delete, koi ek bane -> true
             return isPalindrome(s, left + 1, right) || isPalindrome(s, left, right - 1);
         left++; right--;
     return true;                            // koi mismatch nahi = pehle se palindrome

 ▸ REVERSE VOWELS
     // isVowel: a e i o u, lower + upper dono

     // reverseVowels: sirf vowels aapas me ulte, baaki char apni jagah
     int left = 0; int right = s.size() - 1;
     while (left < right)                    // < : milte hi ruko, beech ka akela char swap nahi
         if (isVowel(s[left]) && isVowel(s[right]))     // dono vowel -> swap + dono andar
             swap(s[left], s[right]); left++; right--;
         else if (!isVowel(s[left]))  left++;   // left vowel nahi -> left aage
         else  right--;                      // warna right peeche
     return s;

── FAMILY: left/right by-VALUE decision (sum / area ki value dekh ke kaunsa pointer chale) ──

 ▸ CONTAINER MOST WATER
     int low = 0; int high = height.size() - 1; int max_area = INT_MIN;
     while (low < high)                      // < : same index pe width 0
         int width = high - low;
         int area = width * min(height[low], height[high]);   // area = chhoti height x width
         max_area = max(max_area, area);
         if (height[low] < height[high])  low++;   // CHHOTI height wala andar (wahi bottleneck)
         else  high--;                       // barabar height -> high-- (else)
     return max_area;

 ▸ TWO SUM II (SORTED)
     // UNSORTED two sum = hashmap; SORTED = 2 pointer
     int left = 0, right = nums.size() - 1;
     while (left < right)                    // < : ek hi index do baar nahi
         if (nums[left] + nums[right] == target)
             return {left + 1, right + 1};   // 1-based lautao
         else if (nums[left] + nums[right] < target)  left++;   // chhota -> bada chahiye
         else  right--;                      // bada -> chhota chahiye
     return {-1, -1};                        // na mile; O(1) space

 ▸ SUM OF SQUARE NUMBERS
     // a² + b² == c ? (LC 633) 2 number ki jodi -> two pointer
     int low = 0, high = sqrt(c);
     while (low <= high)                     // <= : c=0 me 0+0 jaisa ek hi point bhi check ho
         long long ans = (long long)low * low + (long long)high * high;   // guna se PEHLE cast: overflow
         if (ans == c)
             return true;
         else if (ans < c)  low++;
         else  high--;
     return false;

── FAMILY: fill-from-END (result peeche se bharo, bada element end me) ──

 ▸ SQUARES OF SORTED ARR
     // sabse BADA square dono chhor pe (negative)
     vector<int> ans(nums.size(), -1);       // size n pehle se: k-- se index pe likho; O(n), sort nahi
     int left = 0, right = nums.size() - 1, k = nums.size() - 1;   // k = n-1 : sabse bada aakhri slot me
     while (left <= right)                   // <= : aakhri bacha 1 element bhi daalna hai
         if (nums[left] * nums[left] <= nums[right] * nums[right])
             ans[k] = nums[right] * nums[right]; right--;   // bada square aakhri khaali slot pe
             k--;                            // likho, phir peeche
         else
             ans[k] = nums[left] * nums[left]; left++; k--;   // left bada (negative tha), uska square peeche
     return ans;

 ▸ MERGE SORTED ARRAY
     // nums1 ke end me n khaali
     int i = m - 1; int j = n - 1; int k = m + n - 1;   // k = m+n-1 (m*n-1 NAHI) : nums1 ka aakhri index
     while (i >= 0 && j >= 0)                // && (|| nahi -> -1 index crash)
         if (nums1[i] < nums2[j])            // jo bada wahi peeche (aage se bharo to zinda element mitta)
             nums1[k] = nums2[j]; j--; k--;
         else                                // nums1[i] bada ya barabar -> wahi aakhri khaali slot pe
             nums1[k] = nums1[i]; i--; k--;
     while (i >= 0)                          // optional: nums1 ke bache pehle se apni jagah pe hain
         nums1[k] = nums1[i]; i--; k--;
     while (j >= 0)                          // zaroori: nums2[j] (nums1[j] NAHI), ye abhi copy nahi hue
         nums1[k] = nums2[j]; j--; k--;
     // in-place: naya array nahi -> O(1) extra, O(m+n)

── FAMILY: PARTITION / swap-in-place (category me baanto, galat jagah pe swap) ──

 ▸ SORT COLORS (0,1,2)
     // Dutch flag, 3 pointer
     int low = 0; int mid = 0; int high = nums.size() - 1;
     while (mid <= high)                     // <= : high wala abhi dekha nahi, use bhi check
         if (nums[mid] == 0)
             swap(nums[low], nums[mid]); low++;
             mid++;                          // mid++ bhi: low se aaya element pehle hi check ho chuka
         else if (nums[mid] == 1)  mid++;    // sirf mid++
         else if (nums[mid] == 2)
             swap(nums[mid], nums[high]); high--;   // mid++ NAHI: high se aaya element unknown, dobara check

 ▸ SORT ARRAY BY PARITY
     // even pehle, odd baad (aapas ka order koi bhi)
     int left = 0; int right = nums.size() - 1;
     while (left < right)
         if (nums[left] % 2 != 0 && nums[right] % 2 == 0)     // dono galat jagah -> swap + dono andar
             swap(nums[left], nums[right]); left++; right--;
         else if (nums[left] % 2 == 0)  left++;   // left even = sahi -> left aage
         else  right--;                      // warna right peeche
     // TRAP: left even + right odd = dono sahi, swap MAT
     return nums;

── (STANDALONE — apni alag trick) ──

 ▸ 3SUM (LC-15)  = SORT + ek fix + baaki 2-pointer (+ duplicate skip)
     // saare UNIQUE triplet jinka sum 0
     vector<vector<int>> ans;
     sort(begin(nums), end(nums));           // sort PEHLE: 2 pointer + duplicate skip sorted pe hi
     for (int i = 0; i < nums.size(); i++)   // har number ek baar FIX
         if (i > 0 && nums[i] == nums[i - 1])
             continue;                       // skip 1/3: duplicate FIXED (i>0 pehle, warna nums[-1])
         int low = i + 1, high = nums.size() - 1, sum = -nums[i];   // low=i+1 : fixed ke baad wale hi
         while (low < high)                  // < : low==high ek element; sum=-fixed = 2SUM-II
             if (nums[low] + nums[high] == sum)
                 ans.push_back({nums[low], nums[high], nums[i]});
                 while (low < high && nums[low] == nums[low + 1])
                     low++;                  // skip 2/3: duplicate LOW (low<high pehle: low+1 bahar)
                 while (low < high && nums[high] == nums[high - 1])
                     high--;                 // skip 3/3: duplicate HIGH (low<high pehle)
                 low++; high--;              // skip ke baad bhi 1 kadam: abhi purane number pe khade
             else if (nums[low] + nums[high] < sum)  low++;   // chhota -> low aage
             else  high--;                   // bada -> high peeche
     return ans;

 ▸ IS SUBSEQUENCE
     // s, t me order se hai? beech ke char skip chalega
     int i = 0, j = 0;                       // i -> s, j -> t
     while (i < s.size() && j < t.size())    // koi ek khatam -> ruko
         if (s[i] == t[j])                   // char mila -> dono aage
             i++; j++;
         else  j++;                          // nahi mila -> sirf t aage
     if (i >= s.size())                      // s ke saare char order me mil gaye
         return true;
     return false;

 ▸ TRAPPING RAIN WATER (LC-42)
     // bar i ke upar paani = min(leftMax[i], rightMax[i]) - height[i]   (min: chhoti side se bah jaata)
     // O(n) space wala: leftMax[] aage se, rightMax[] peeche se, ans += min(..) - height[i]
     // O(1) space, two pointer
     int n = height.size();
     int left = 0, right = n - 1;
     int leftMax = 0, rightMax = 0, ans = 0;   // max 0 se: abhi koi deewar dekhi nahi; empty -> 0
     while (left < right)                    // < : milne wali cell sabse unchi deewar, uspe paani nahi
         if (height[left] <= height[right])      // DEEWAR pe faisla, max pe NAHI ([4,1,4,1] -> 0)
             if (leftMax > height[left])     // chhoti deewar wali taraf niptao: doosri taraf badi pakki
                 ans += leftMax - height[left];  // chhota -> paani
             else  leftMax = height[left];   // bada -> nayi deewar
             left++;
         else
             if (rightMax > height[right])
                 ans += rightMax - height[right];   // chhota -> paani (rightMax tak)
             else  rightMax = height[right];   // bada -> nayi deewar
             right--;
     return ans;

 ▸ PALINDROMIC SUBSTRINGS (LC-647)  = EXPAND AROUND CENTER
     // kitne substring palindrome (single char bhi; alag position = alag count). har CENTER se phailo
     // "abcba": c -> bcb -> abcba = ek center se 3. ODD center = ek char, EVEN center = do char jodi
     // solve: &count : warna bahar count badhta hi nahi
     while (i >= 0 && j < s.size() && s[i] == s[j])     // range check PEHLE, phir s[i]==s[j]
         i--; j++;                           // dono taraf phailo
         count++;                            // HAR match = ek naya palindrome (ant me ek baar nahi)

     // countSubstrings
     int count = 0;
     for (int i = 0; i < s.size(); i++)      // n odd center + (n-1) even center
         solve(i, i, s, count);              // odd: center ek char (khud bhi palindrome)
         solve(i, i + 1, s, count);          // even: i+1 == n bhi theek, while me j < n pehle check
     return count;                           // O(n²), O(1); DP nahi: substring contiguous, choice nahi

```

---

---

## PATTERN 2 — SLIDING WINDOW

```

 KAB PEHCHANO: CONTIGUOUS subarray/substring + "longest / shortest / max / min / COUNT" maanga -> SW socho.
 ★ EDGE (variable window): shrink me count 0 ho jaye to map se ERASE karo, warna map.size() jhooth bolega.

 ONE SKELETON (fixed + variable DONO isi pe):
     int i = 0, j = 0;                       // dono 0 se: shuru me window khaali
     while (j < n) {                         // EXPAND: j hamesha aage (for(j..) = while(j<n){..; j++} SAME)
         arr[j] window me ADD;               // track: sum · freq-map (mp.size()) · zeros · prod · maxFreq
         while (WINDOW BAD) {                // SIZE fix (k / p.length()) -> IF (ek add, ek remove)
                                             // SIZE condition pe (sum / distinct / repeat / product) -> WHILE
                                             // TRAP: max-sum-K me k = SIZE (IF); product<K me k = LIMIT
             arr[i] REMOVE; i++;             // remove PEHLE, phir i++ (warna agla element hat jaata)
         }
         ans UPDATE;                         // shrink ke BAAD valid -> ans UNCONDITIONAL (if(==k) nahi)
                                             // length -> max(ans, j-i+1) · subarray COUNT -> count += j-i+1
         j++;                                // ans ke BAAD j++: warna j-i+1 galat
     }
     // sirf 2 cheez badalti: (1) inner IF ya WHILE  (2) "WINDOW BAD" kya + kya track. baaki SAME.
     // LONGEST  -> shrink jab INVALID (majboori), record shrink ke BAAD, ans = MAX
     // SHORTEST -> shrink jab tak VALID, record shrink loop ke ANDAR, ans = MIN

── FAMILY: variable-LONGEST (INVALID pe WHILE shrink, ans = MAX length) ──

 ▸ LONGEST SUBSTRING NO-REPEAT (LC-3)  = variable window + freq-map
     // longest substring jisme koi char REPEAT na ho
     if (s.empty())
         return 0;
     int ans = INT_MIN; unordered_map<char, int> mp;   // INT_MIN: end me check karke khaali pe 0
     int i = 0, j = 0;
     while (j < s.size())
         mp[s[j]]++;                         // window me add
         while (mp[s[j]] > 1)                // INVALID: jo abhi add hua (s[j]) wahi repeat
             mp[s[i]]--;                     // while: repeat wala char nikalne tak kai char hatane padte
             i++;                            // left shrink jab tak repeat khatam
         ans = max(ans, j - i + 1); j++;     // shrink ke BAAD record: window ab valid · record ke BAAD aage
     return ans == INT_MIN ? 0 : ans;        // khaali string -> INT_MIN reh jaata -> 0

 ▸ CHAR REPLACEMENT (LONGEST, LC-424)  = variable window + freq-map (k badal sakte)
     // longest window jisme <= k char badal ke sab SAME
     unordered_map<char, int> mp; int maxLen = INT_MIN, maxFreq = INT_MIN, i = 0, j = 0;
     while (j < s.size())
         mp[s[j]]++; maxFreq = max(maxFreq, mp[s[j]]);   // maxFreq update PEHLE, phir check
         while (j - i + 1 - maxFreq > k)     // windowLen - maxFreq = kitne badalne; > k -> INVALID
             mp[s[i]]--;                     // left wala char bahar, ginti kam
             if (mp[s[i]] == 0)  mp.erase(s[i]);   // 0 -> key hatao (distinct gino to zaroori)
             i++;                            // ginti kam karne ke BAAD hi left aage
         // CORE: maxFreq kabhi GHATAO mat (shrink pe recompute nahi);
                                             // window best-length se chhota nahi -> purana maxFreq theek
         if (j - i + 1 - maxFreq <= k)  maxLen = max(maxLen, j - i + 1);   // <= k -> VALID
         j++;
     return maxLen;

 ▸ MAX CONSECUTIVE ONES III (LC-1004)  = variable window + zeros-count (k flips)
     // max k zero FLIP -> longest all-1; sirf zeroCount
     int ans = INT_MIN; int zeroCount = 0; int i = 0, j = 0;   // INT_MIN: end me check karke khaali pe 0
     while (j < nums.size())
         if (nums[j] == 0)  zeroCount++;     // window me zero add
         while (zeroCount > k)               // INVALID: k se zyada zero
             if (nums[i] == 0)  zeroCount--;   // sirf zero nikla tabhi kam (1 nikalne pe nahi)
             i++;                            // check ke BAAD aage (pehle i++ to galat element dekhta)
         ans = max(ans, j - i + 1);          // UNCONDITIONAL: if(zeroCount==k) MAT lagao, shrink ke baad
                                             // window valid; ==k = sirf exactly-k, k=0 / k>zeros toot-te
         j++;
     return ans == INT_MIN ? 0 : ans;

 ▸ FRUIT INTO BASKETS
     // = longest subarray, AT MOST 2 DISTINCT (fruit = kahani)
     int ans = INT_MIN; unordered_map<int, int> mp; int i = 0, j = 0;
     while (j < fruits.size())
         mp[fruits[j]]++;                    // window me add
         while (mp.size() > 2)               // INVALID: > 2 distinct (while: ek type khatam hone tak)
                                             // IF bhi chalta sirf LONGEST me; COUNT / SHORTEST = while
             mp[fruits[i]]--;                // left wala bahar, ginti kam
             if (mp[fruits[i]] == 0)
                 mp.erase(fruits[i]);        // CRUX erase-on-0: warna 0 wali key bhi mp.size() me
             i++;                            // erase check ke BAAD hi left aage
         ans = max(ans, j - i + 1); j++;     // shrink ke BAAD record: window ab <= 2 distinct
     return ans == INT_MIN ? 0 : ans;

 ▸ LONGEST AT-MOST-K DISTINCT
     // totalFruit: FRUIT ka GENERAL version: wahan "2", yahan "k"
     int ans = INT_MIN; unordered_map<int, int> mp; int i = 0, j = 0;
     while (j < s.size())
         mp[s[j]]++;
         while (mp.size() > k)               // distinct > k -> left se shrink
             mp[s[i]]--;
             if (mp[s[i]] == 0)  mp.erase(s[i]);   // erase-on-0 (fruit jaisa)
             i++;
         ans = max(ans, j - i + 1); j++;     // ans = max length
     return ans == INT_MIN ? 0 : ans;

     // longestKDistinct
     return totalFruit(s, k);

 ▸ SUBARRAYS WITH K DISTINCT (LC-992)  = FRUIT (atMost-k) x2, ghata do  [exactly-K trick]
     // kitne SUBARRAYS jinme EXACTLY k different. [1,2,1,2,3], k=2 -> 7
     // exactly(k) seedha SW se ganda (boundary saaf nahi) -> exactly(k) = atMost(k) - atMost(k-1)
     // atMostK: = FRUIT ka template, bas ans = COUNT (max nahi)
     int i = 0, j = 0, count = 0;            // count = 0: ginti (max-template ka INT_MIN = garbage)
     unordered_map<int, int> mp;
     while (j < fruits.size())
         mp[fruits[j]]++;                    // naya number window me
         while (mp.size() > k)               // k se zyada distinct -> shrink
             mp[fruits[i]]--;
             if (mp[fruits[i]] == 0)
                 mp.erase(fruits[i]);        // erase: warna 0-count key bhi mp.size() me ginti
             i++;
         count += j - i + 1; j++;            // is j pe khatam hone wale saare valid subarray
     return count;

     // subarraysWithKDistinct
     return atMostK(nums, k) - atMostK(nums, k - 1);   // atMost(k) = distinct 1 + 2 + .. + k ka dabba;
                                                        // atMost(k-1) bina aakhri layer -> exactly k
                                                        // [1,2,1,2,3]: atMost(2) 12 - atMost(1) 5 = 7
                                                        // k=1 pe special case NAHI: atMost(0) apne aap 0
     // koi bhi "exactly K" -> atMost(k) - atMost(k-1)   (fruit · longest-k-distinct isi family)

 ▸ EQUAL SUBSTR WITHIN BUDGET (LC-1208)  = longest-subarray-sum <= k (string ke bhes me)
     // cost = |s[i]-t[i]|, SUM <= maxCost
     int i = 0, j = 0, cost = 0, ans = INT_MIN;          // track sirf cost (koi map nahi)
     while (j < t.size())                    // s="abcd", t="cdef", maxCost=3 -> 1
         cost += abs(s[j] - t[j]);
         while (cost > maxCost)              // INVALID -> WHILE shrink
             cost -= abs(s[i] - t[i]); i++;
         ans = max(ans, j - i + 1); j++;
     return ans;

── FAMILY: variable-SHORTEST (ULTA: VALID ho tab tak shrink, ans = MIN length) ──

 ▸ MIN SUBARRAY LEN (>=TARGET, LC-209)  = SHORTEST window (longest se ULTA)
     // positive; sum >= target SABSE CHHOTA
     int sum = 0; int size = INT_MAX;        // INT_MAX: min se ghatega; koi window na bani to end me 0
     int i = 0, j = 0;
     while (j < nums.size())
         sum += nums[j];                     // expand
         while (sum >= target)               // VALID hote hi -> record + SHRINK (aur chhota dhoondo)
             size = min(size, j - i + 1);    // record ANDAR (SHORTEST); LONGEST me record shrink ke BAAD
             sum -= nums[i]; i++;
         j++;                                // shrink loop ke BAHAR: har j pe ek baar
     return size == INT_MAX ? 0 : size;      // koi valid window nahi -> 0
     // char-matching wala SHORTEST = MIN WINDOW (LC-76) -> neeche need-map family

── FAMILY: FIXED window (size k pehle se diya, slide) ──

 ▸ MAX SUM OF K (FIXED)  = fixed window size-k + running sum
     // har k-size window ka sum -> MAX
     int sum = 0; int maxSum = INT_MIN; int i = 0, j = 0;   // INT_MIN: sab negative ho to bhi sahi max
     while (j < nums.size())
         sum += nums[j];
         if (j - i + 1 >= k)                 // IF (WHILE nahi): size fix -> har step 1 add + 1 remove
             maxSum = max(maxSum, sum); sum -= nums[i]; i++;   // FULL window -> record · phir 1 remove
         j++;
     return maxSum;

── FAMILY: COUNT (length nahi; valid window pe count += j-i+1) ──

 ▸ SUBARRAY PRODUCT < K  = variable window + COUNT (running product)
     // kitne subarray jinka product < k (GINTI)
     if (k == 0)
         return 0;                           // nums >= 1: product < 0 kabhi nahi, koi subarray nahi
     int count = 0; int i = 0, j = 0, prod = 1;   // prod = 1: guna ka shuru 1 (0 rakha to sab 0)
     while (j < nums.size())
         prod *= nums[j];                    // naya element window me
         while (prod >= k && i <= j)         // i <= j: k=1 pe khaali window prod 1, i bahar bhaagta
             prod /= nums[i]; i++;           // left wala guna se bahar: divide · divide ke BAAD hi left aage
         count += j - i + 1; j++;            // j pe khatam: [j], [j-1..j] .. [i..j] = j-i+1
     return count;                           // [10,5,2,6], k=100 -> 1+2+2+3 = 8 · [1], k=1 -> 0

── FAMILY: need-map + COUNT (t ke SAARE char chahiye; aana = --, jaana = ++ MIRROR) ──

 ▸ MIN WINDOW SUBSTRING (LC-76, Hard) -- answer = MIN-track, loop ke ANDAR
     // s ka sabse CHHOTA window jisme t ke saare char
     unordered_map<char, int> mp;
     for (auto &it : t)
         mp[it]++;                           // need-map: t ke har char pe ++
     int i = 0, j = 0, count = t.size(), minLen = INT_MAX, index = 0;   // count==0 VALID; index=start
     while (j < s.size())                    // OUTER: j aage
         if (mp[s[j]] > 0)  count--;         // t-char mila -> ek zaroorat puri
         mp[s[j]]--;                         // IF ke BAHAR: non-t char -ve -> count me "invisible"
                                             // (andar rakha to shrink me count++ galat = asymmetry bug)
         while (count == 0)                  // INNER: VALID -> shrink + SAVE (answer ANDAR)
             int len = j - i + 1;
             if (len < minLen)               // record PEHLE: abhi [i..j] valid, i++ ke baad shayad nahi
                 minLen = len; index = i;
             mp[s[i]]++;                     // MIRROR: wapas ++
             if (mp[s[i]] > 0)  count++;     // > 0 hua = t ka zaroori char gaya
             i++;
         j++;                                // shrink ke BAAD agla char
     return minLen == INT_MAX ? "" : s.substr(index, minLen);   // INT_MAX = koi valid nahi -> ""
     // "ADOBECODEBANC", "ABC" -> "BANC"

 ▸ SUBSTRINGS CONTAINING ALL (LC-1358) -- answer = COUNT, loop ke BAAD
     // minWindow: LC-76 ka SAME code; farak sirf answer ki line
     unordered_map<char, int> mp;
     for (auto &it : t)
         mp[it]++;
     int i = 0, j = 0, count = t.size(), ans = 0;   // ans = 0: ginti jod rahe
     while (j < s.size())
         if (mp[s[j]] > 0)  count--;         // EXPAND (same)
         mp[s[j]]--;
         while (count == 0)                  // INNER: shrink jab tak INVALID na ho
             mp[s[i]]++;
             if (mp[s[i]] > 0)  count++;
             i++;
         ans += i; j++;                      // inner-while ke BAAD, har j pe: start 0..i-1 sab valid the
     return ans;                             // "abcabc": i = 0,0,1,2,3,4 -> 10

     // numberOfSubstrings
     return minWindow(s, "abc");             // wahi function, bas t = "abc"

── FAMILY: ANAGRAM-window (fixed p-length window + isAnagram helper reuse) ──

 ▸ FIND ALL ANAGRAMS (LC-438)  = ANAGRAM-check + FIXED window (PARENT)
     // isAnagram: valid-anagram helper reuse
     if (s.size() != t.size())
         return 0;                           // size same chahiye
     unordered_map<char, int> mp;
     for (auto &it : s)
         mp[it]++;                           // a se ++
     for (auto &it : t)
         mp[it]--;                           // b se --
     for (auto &it : mp)
         if (it.second != 0)
             return false;                   // sab 0 -> anagram
     return true;

     // findAnagrams: s me p ke SAARE anagram ki START-INDEX
     vector<int> ans;
     int i = 0, j = 0;
     while (j < s.size())
         while (j - i + 1 > p.size())
             i++;                            // window ko p-size pe rakho (FIXED)
         if (j - i + 1 == p.size())          // == : window poora p-size hua tabhi check (shuru me chhota)
             if (isAnagram(s.substr(i, j - i + 1), p))  ans.push_back(i);   // anagram -> START-index push
         j++;                                // check ke BAAD aage
     return ans;

 ▸ PERMUTATION IN STRING (LC-567)  = FIND-ALL-ANAGRAMS ka EXACT same code
     // isAnagram = upar wala hi
     // findAnagrams: LC-438 ka skeleton, sirf answer-handle badla
     int i = 0, j = 0;
     while (j < s.size())
         while (j - i + 1 > p.size())
             i++;                            // FIXED p-size
         if (j - i + 1 == p.size())
             if (isAnagram(s.substr(i, j - i + 1), p))
                 return true;                // DELTA: push ki jagah return true (pehla match = bool)
         j++;
     return false;                           // loop-baad: koi na mila

     // checkInclusion: s2 me s1 ka koi permutation (= anagram) substring?
     return findAnagrams(s2, s1);            // dhyaan: ulta order (s2, s1)

── FAMILY: MONOTONIC DEQUE (window max/min; deque me INDICES, front = answer) ──

 ▸ SLIDING WINDOW MAXIMUM (LC-239)  = monotonic DEQUE of INDICES
     // har k-window ka MAX, O(n)
     vector<int> ans;
     deque<int> dq;                          // INDICES, values DECREASING -> front = max
     for (int i = 0; i < nums.size(); i++)
         if (!dq.empty() && dq.front() == i - k)   // 1) EXPIRY: == i-k window se bahar
             dq.pop_front();                 //    !dq.empty() pehle: khaali pe front() crash
                                             //    deque ki AKELI wajah yahi; baaki monotonic STACK
         while (!dq.empty() && nums[dq.back()] <= nums[i])   // 2) chhote back-wale BEKAAR (nums[i] zinda,
             dq.pop_back();                  //    wo max nahi ban sakte); <= : barabar purana bhi
                                             //    MIN chahiye -> condition ULTA (nums[back] >= nums[i])
         dq.push_back(i);                    // value nahi INDEX: expiry check ke liye
         if (i >= k - 1)                     // 3) record hamesha LAST; k-1 = pehle poore window ka aakhri
             ans.push_back(nums[dq.front()]);   // ORDER: expiry -> clean + push -> record (warna galat)
     return ans;

 ▸ MAX POINTS FROM CARDS (LC-1423)  = MAX-SUM-SIZE-K (01) + COMPLEMENT-twist
     // DONO ends se TOTAL k cards (har end se k nahi) -> sum MAX. [1,2,3,4,5,6,1], k=3 -> 12 (back se 5,6,1)
     // sirf back se k galat: [100,1,1,1,1] k=2 -> front 101. liye cards contiguous NAHI -> seedha SW fail
     // FLIP: jo NAHI liye = beech ka CONTIGUOUS window, size L-k. taken MAX = untaken MIN -> Total - min
     // minSumK: = max_sum_k ka MIN version (fixed window)
     int sum = 0; int minSum = INT_MAX; int i = 0, j = 0;   // INT_MAX se: min ghatega
     while (j < nums.size())
         sum += nums[j];
         if (j - i + 1 >= k)
             minSum = min(minSum, sum); sum -= nums[i]; i++;
         j++;
     return minSum;

     // maxScore
     int Total = 0;                          // saare cards ka sum: isme se beech ka min ghatayenge
     for (int i = 0; i < nums.size(); i++)
         Total += nums[i];
     if (nums.size() - k == 0)
         return Total;                       // k == L -> saare cards (window size 0)
     int minSum = minSumK(nums, nums.size() - k);   // window size = L-k
     return Total - minSum;                  // 22 - 10 = 12 (min window [1,2,3,4])

── FAMILY: FIXED window (size k) + HASHSET ──

 ▸ CONTAINS DUPLICATE II (LC-219)  = FIXED window (size k) + HASHSET
     // nums[i]==nums[j] AND |i-j| <= k ?
     if (k <= 0)
         return false;                       // k=0: check shrink se PEHLE -> [4,4] k=0 galat true
     int i = 0, j = 0; unordered_set<int> st;   // sirf LAST k element -> dup mila = farak apne aap <= k
     while (j < nums.size())
         if (st.count(nums[j]) > 0)
             return true;                    // window me pehle se -> mil gaya (within k)
         if (j - i + 1 > k)                  // window k se badi -> oldest hatao
             st.erase(nums[i]); i++;
         st.insert(nums[j]); j++;            // check ke BAAD insert: warna khud se match
     return false;                           // [1,2,3,1] k=3 -> true · [1,2,3,1,2,3] k=2 -> false

```

---

---

## PATTERN 3 — HASHING

```

 BROAD IDEA: hashmap/hashset = O(1) lookup. "pehle dekha?" / "complement hai?" / count -> O(n).
 ★ MILTE-JULTE (longest consecutive vs LIS): upar "NAAM-SE-CONFUSE" -> B dekh.

 COMMON SKELETON (seen / complement / count family -- ek pass):
    map/set banao                      // kabhi PRELOAD: two-sum khaali · subarray-sum-k -> mp{0:1}
    for x in arr:
        CHECK pehle                    // map me complement/seen/count hai? -> ans use/count
        UPDATE baad                    // map me x daalo / count++ (warna x KHUD se match kar leta)
    // har hashing Q one-pass nahi: group-anagrams = pehle poora map bharo phir output ·
    // longest-consec = set-membership. ye skeleton sirf "seen/complement/count" family ke liye.

── FAMILY: 2-MAP BIJECTION (mp1 + mp2 dono taraf; koi ek clash -> false, ek map kaafi nahi) ──

 ▸ ISOMORPHIC STRINGS
     // s->t har char ka 1-to-1 mapping? "egg"->"add" true; "foo"->"bar", "ab"->"aa" false
     unordered_map<char, char> mp1;          // s -> t
     unordered_map<char, char> mp2;          // t -> s ("ab"->"aa" clash sirf ULTI taraf pakadta)
     for (int i = 0; i < s.size(); i++)
         if ((mp1.count(s[i]) == 1 && mp1[s[i]] != t[i]) ||   // count() pehle: seedha mp1[x] khaali key
             (mp2.count(t[i]) == 1 && mp2[t[i]] != s[i]))     //   bana ke galat clash deta
             return false;                   // || : kisi bhi EK taraf clash = 1-to-1 nahi
         else                                // check ke BAAD dono taraf mapping save
             mp1[s[i]] = t[i]; mp2[t[i]] = s[i];
     return true;                            // bina clash poora chala = isomorphic

 ▸ WORD PATTERN
     // = ISOMORPHIC, bas char <-> WORD
     unordered_map<char, string> mp1; unordered_map<string, char> mp2;
     stringstream ss(s); string word;        // delta 1: input = word-stream (<sstream>)
     int i = 0; int j = 0;                   // i = pattern ka index · j = words ki ginti
     while (ss >> word)
         if (i < pattern.size())             // i<size: words zyada hon to pattern[i] bahar na padhe
             if ((mp1.count(pattern[i]) == 1 && mp1[pattern[i]] != word) ||   // delta 2: key =
                 (mp2.count(word) == 1 && mp2[word] != pattern[i]))           //   pattern[i] <-> word
                 return false;               // ISOMORPHIC ki EXACT clash line
             else                                // check ke BAAD save
                 mp1[pattern[i]] = word; mp2[word] = pattern[i];
             i++;                            // agla pattern char
         j++;                                // if ke BAHAR: har word gino, extra word bhi
     return (pattern.size() == j) ? true : false;   // delta 3: words ki ginti == pattern length
     //   warna "a","dog cat" galat true

── (STANDALONE — apni alag trick) ──

 ▸ TWO SUM
     unordered_map<int, int> mp;             // {value -> index}
     for (int i = 0; i < nums.size(); i++)
         if (mp.find(target - nums[i]) != mp.end())   // CHECK pehle: (target - num) pehle dekha?
             return {mp[target - nums[i]], i};        // mila -> dono index
         mp[nums[i]] = i;                    // daalo BAAD: taaki num khud se match na kare
     return {-1, -1};

 ▸ VALID ANAGRAM
     // har char ka count barabar? (count[26] ya map)
     if (s.size() != t.size())
         return 0;                           // length alag -> turant false
     unordered_map<char, int> mp;
     for (auto &it : s)
         mp[it]++;                           // s se ++
     for (auto &it : t)
         mp[it]--;                           // t se --
     for (auto &it : mp)
         if (it.second != 0)                 // != 0 (sirf > 0 nahi, -ve bhi aata)
             return false;
     return true;                            // sab ZERO -> anagram

 ▸ GROUP ANAGRAMS
     // "eat","tea","ate" -> teeno ka SORTED roop "aet" = same. Yahi KEY.
     // brute = har jodi "anagram hai?" O(n² · k) -> har word ki KEY bana ke map me CACHE -> O(n · k log k)
     unordered_map<string, vector<string>> mp;   // KEY = sorted(word) -> us group ke words
     for (auto &it : strs)
         string key = it;                    // copy lo: asli word sort hua to group me galat word jaata
         sort(begin(key), end(key)); mp[key].push_back(it);   // "tea" -> "aet" · same key = same group
     vector<vector<string>> ans;
     for (auto &it : mp)
         ans.push_back(it.second);           // map ki har list = ek group
     return ans;
     // KEY ka doosra roop (sirf a-z): 26 ki ginti "1#0#0#...#" -> sort nahi, O(n · k)
     //   (separator "#" zaroori: bina uske a=1,b=11 aur a=11,b=1 dono "111")
     // "" (khaali string) bhi chalta: sorted("") = "" -> ek hi group

 ▸ SUBARRAY SUM = K
     // prefix-sum + hashmap {prefixSum -> count}
     unordered_map<int, int> mp; int sum = 0, count = 0;
     mp[0] = 1;                              // START zaroori: jab sum khud == k, tab prefix-0 chahiye
     for (int i = 0; i < nums.size(); i++)
         sum += nums[i];                     // running sum
         count += mp[sum - k];               // sum - k (k - sum NAHI): abhi 15 km - aakhri tukda 5 =
                                             //   10 pe tukda shuru -> "10 kitni baar dikha?" BADA pehle
                                             //   (two sum ka "target - x" ulta yahan mat lagao)
         mp[sum]++;                          // check ke BAAD daalo
     return count;

 ▸ COUNT DIVISIBLE PAIRS  = two-sum/hashmap-complement, par KEY = REMAINDER
     // pairs (i<j): (A[i]+A[j])%k==0. brute O(n²) -> "partner pehle aaya?" ko hashmap me CACHE -> O(n)
     // A[i] KYUN, prefix NAHI: pair = 2 ALAG element (contiguous range nahi) -> har element ka apna rem
     unordered_map<int, int> mp; int count = 0;   // {remainder -> kitne aaye}
     for (auto &it : nums)
         int rem = (((it % k) + k) % k);     // +k phir %k: C++ me negative ka % negative aata, key galat
         count += mp[(k - rem) % k];         // partner rem: it=1,k=3 -> 1+? = 3 -> 2. rem=0 -> 3%3 = 0
                                             //   isliye (k-rem)%k. mp[2] jitne = utne pair
         mp[rem]++;                          // lookup PEHLE, insert BAAD: khud-se-pair na gine
     return count;                           // [1,2,3,4,5],k=3 -> 4 (it=5 pe mp[1]=2 -> ek saath 2 pair)

 ▸ LONGEST CONSECUTIVE SEQ (LC-128)
     // unsorted -> sabse LAMBI consecutive chain ki LENGTH. [100,4,200,1,3,2] -> 4 (1,2,3,4)
     // sort (n log n) mat karo -> SET me O(1) lookup, har CHAIN sirf uske START se gino -> O(n)
     int ans = INT_MIN; unordered_set<int> st;   // set: O(1) lookup + duplicate apne aap hat jaate
     for (auto &it : nums)
         st.insert(it);
     for (int i = 0; i < nums.size(); i++)   // set pe ghoomna behtar: nums pe duplicate START baar-baar
         int num = nums[i];
         if (!st.count(num - 1))             // START tabhi jab num-1 set me NAHI, warna beech ka: skip
             int currNum = num; int count = 1;   // count=1: START khud chain me
             while (st.count(currNum + 1))   // agla set me hai tab tak chain lambi
                 currNum++; count++;
             ans = max(ans, count);          // while ke BAAD: poori chain gin ke record
     return ans == INT_MIN ? 0 : ans;        // khaali array -> 0
     // 100 START(1) · 4 skip · 200 START(1) · 1 -> 1,2,3,4 = 4

 ▸ MAJORITY ELEMENT (LC-169, >n/2)
     // map se: mp[x]++, count > n/2 wala return -> O(n) time, O(n) space (seedha)
     // BOYER-MOORE VOTING, O(1) space
     int count = 1;                          // majority > n/2 -> baaki sab milke bhi kam ->
     int candidate = nums[0];                //   do ALAG element CANCEL karo, majority bachega
     for (int i = 1; i < nums.size(); i++)
         if (candidate == nums[i])  count++;   // == candidate -> count++
         else if (candidate != nums[i])
             count--;                        // != candidate -> count--
             if (count == 0)                 // 0 pe RESET (chhoda to -1 me jaata, reset kabhi nahi = BUG)
                 candidate = nums[i]; count = 1;
     return candidate;                       // end: candidate = ans

 ▸ FIND DISAPPEARED NUMBERS (LC-448)  = ARRAY-KHUD-hashmap · index-negation, O(1) space
     // size n, values 1..n (repeat ho sakte). jo MISSING wo sab. [4,3,2,7,8,2,3,1] -> [5,6]
     // extra set/map nahi: value v ka ghar = index v-1 -> dikha to us index ko NEGATIVE mark
     vector<int> ans;
     for (int i = 0; i < nums.size(); i++)
         int index = abs(nums[i]) - 1;       // abs(): value pehle hi -ve ho chuki ho sakti
         if (nums[index] > 0)                // >0 check: duplicate pe dobara minus = wapas plus
             nums[index] = -nums[index];
     for (int i = 0; i < nums.size(); i++)
         if (nums[i] > 0)                    // abhi bhi +ve = index+1 kabhi mark nahi hua = MISSING
             ans.push_back(i + 1);           // i+1: index i ka ghar value i+1 ka (1-based)
     return ans;
     // SAME trick: find-duplicate (287, sirf jab array badal sakte; asli 287 = Floyd) · first-missing-
     //   positive (41) · single-in-pairs. "values 1..n + O(1) space" = index-marking ka signal
     //   (array modify NA kar sakte -> Floyd cycle)

```

---

---

## PATTERN 4 — PREFIX SUM

```

 BROAD IDEA: running sum pehle se rakho -> range/left-right sum O(1) (baar-baar loop nahi).
 ★ EDGE (prefix + hashmap, sum == K): map[0] = 1 se SHURU karo. [3], K=3: bina iske poora-array wala case chhoot jaata.
 ★ MILTE-JULTE (max sum vs count sum == K): upar "NAAM-SE-CONFUSE" -> C dekh.
 ★ EDGE (C++ %): minus % minus deta (-2 % 5 = -2) -> ((x % m) + m) % m  (divisible-by-k remainder).

── FAMILY: "assign PEHLE, update BAAD" (pehle current index ka result rakho, PHIR running value badlo) ──

 ▸ PIVOT INDEX
     // index jahan left sum == right sum (khud kisi taraf nahi)
     int totalSum = 0; int leftSum = 0;
     for (int &it : nums)
         totalSum += it;                     // pehle poora total
     for (int i = 0; i < nums.size(); i++)
         int rightSum = totalSum - leftSum - nums[i];   // right = total - left - khud
         if (leftSum == rightSum)
             return i;                       // pehla aisa index
         leftSum += nums[i];                 // leftSum BAAD me: check ke baad hi khud ko left me jodo
     return -1;

 ▸ PRODUCT EXCEPT SELF (LC-238)
     // ans[i] = (i se PEHLE sabka product) x (i ke BAAD sabka product). division nahi, O(n)
     // O(1) extra (follow-up): left ko seedha ans me likho, right-pass me ans[i] *= prod (alag right[] nahi)
     int n = nums.size();
     vector<int> left(n, 1); vector<int> right(n, 1); vector<int> ans(n, 1);
     int prod = 1;                           // prod=1 se: index 0 ke left me koi nahi
     for (int i = 0; i < n; i++)
         left[i] = prod;                     // ASSIGN PEHLE
         prod *= nums[i];                    // UPDATE BAAD: warna khud ka element bhi product me
     prod = 1;                               // reset: left-pass ka bacha product saath mat lao
     for (int i = n - 1; i >= 0; i--)        // peeche se: n-1 se 0 tak
         right[i] = prod; prod *= nums[i];   // yahan bhi assign PEHLE
     for (int i = 0; i < n; i++)
         ans[i] = left[i] * right[i];        // combine
     return ans;

── (STANDALONE — apni alag trick) ──

 ▸ HIGHEST ALTITUDE
     // running sum chalao, max track (simple prefix)
     int sum = 0; int maxi = 0;              // sum=0, maxi=0: shuru ki altitude 0 bhi gino (sab -ve -> 0)
     for (int i = 0; i < gain.size(); i++)
         sum += gain[i]; maxi = max(maxi, sum);
     return maxi;

 ▸ SUBARRAY SUMS DIVISIBLE BY K (LC-974)  = SUBARRAY-SUM-K + REMAINDER-key
     // brute: har subarray sum baar-baar -> PREFIX -> do prefix ka remainder same = beech wala %k==0
     // PREFIX kyun, single A[i] nahi: subarray CONTIGUOUS range -> do prefix ka farak = wahi subarray
     // count subarrays jinka sum % k == 0
     unordered_map<int, int> mp; int sum = 0, count = 0;
     mp[0] = 1;                              // SEED (khaali prefix): shuru se hi divisible wala bhi gino
     for (auto &it : nums)
         sum += it;                          // pehle sum += , PHIR rem nikaalo
         int rem = ((sum) % k + k) % k;      // +k phir %k: negative sum ka % negative aata
         count += mp[rem]; mp[rem]++;        // = subarray-sum-k, bas KEY = remainder (sum ki jagah)
     return count;

 ▸ COUNT SUBARRAYS WITH XOR = K  = SUBARRAY-SUM-K, bas + ki jagah ^
     // [4,2,2,6,4], k=6 -> 4.   CHAABI: x ^ x = 0 (same number khud se XOR = gayab)
     // (shuru se j tak XOR) ^ (shuru se i tak XOR) = beech ka XOR (common hissa kat gaya)
     // beech wala = k chahiye -> pichhla XOR = abhi ka XOR ^ k -> map me dekho kitni baar aaya
     unordered_map<int, int> mp; mp[0] = 1;   // shuru se hi XOR == k wala subarray bhi gino
     int xorr = 0, count = 0;
     for (auto &it : nums)
         xorr ^= it; count += mp[xorr ^ k];   // abhi tak ka XOR · pichhla XOR jo chahiye, kitni baar dikha
         mp[xorr]++;                         // abhi wala map me, lookup ke BAAD
     return count;

 ▸ COUNT SUBARRAYS WITH MAX == K  = BOUNDARY-index (2 pointer track, NO stack/SW)
     // kitne subarray jinka MAX bilkul K. [8,2,1,3,4,5,1,10],K=3 -> 3 ([2,1,3],[1,3],[3])
     // valid = (har element <= K) AND (>=1 element == K)
     // TRAP: "subarray"->SW, "max"->STACK, "count"->PQ sab galat: max FIXED K se compare -> simple scan
     // K se BADE element = WALL (array todte). segment ke andar K wale subarray gino
     int lastWall = -1;                      // aakhri index jahan arr[i] > K  (-1: abhi koi wall nahi)
     int lastK = -1;                         // aakhri index jahan arr[i] == K (-1: abhi koi K nahi)
     long long count = 0;                    // LONG LONG: count ~n^2/2 (5e9) -> int overflow
     for (int i = 0; i < arr.size(); i++)
         if (arr[i] > K)  lastWall = i;      // pehle lastWall / lastK update, PHIR count
         if (arr[i] == K)  lastK = i;
         if (lastK > lastWall)               // guard: warna lastK - lastWall NEGATIVE ([5,3,5] pe dikhta)
             count += lastK - lastWall;      // start choices = lastWall+1 .. lastK
     return count;

 ▸ DIFFERENCE ARRAY / RANGE UPDATE (LC-1109 Corporate Flight Bookings)
     // = prefix-sum ka ULTA · SWEEP-LINE (MEETING ROOMS II, PATTERN 15) ka sagaa bhai
     // KAB: har RANGE pe kuch ADD karo, 20,000 range. seedha = O(range x length), chahiye O(n + ranges)
     // har element mat chhuo: sirf DO jagah nishaan (shuru + khatam ke baad), phir ek baar chalti-sum = value
     // n=5, [[1,2,10],[2,3,20],[2,5,25]] -> [10,55,45,25,25]
     vector<int> diff(n + 2);                // n+2: end+1 sabse bada n+1 ho sakta (n+1 rakha to crash)
     for (auto &it : bookings)
         int start = it[0]; int end = it[1]; int count = it[2]; diff[start] += count;   // yahan se shuru
         diff[end + 1] -= count;             // +1: last din bhi inclusive, uske AGLE din se hatao
     vector<int> ans;
     int cummSum = 0;                        // chalti-sum abhi khaali
     for (int i = 1; i <= n; i++)            // 1..n: flights 1-based, diff[0] / diff[n+1] answer me nahi
         cummSum += diff[i]; ans.push_back(cummSum);
     return ans;
     // SWEEP-LINE se farak (machine EK, teen cheez badalti):
     //   rakho : meeting rooms = map (time bada / bikhra, 10^9) · yahan = plain array (index chhota, 1..n)
     //   ghatao: meeting rooms [start,end) exclusive -> -1 at end
     //           yahan [first,last] inclusive -> -count at end+1
     //           (inclusive me end pe ghataya = aakhri element chhoot gaya)
     //   nikalo: meeting rooms = chalti-sum ka MAX ("kitne kamre")
     //           yahan = poora array ("har flight ka total")
     // FAMILY: range-add · range-flip · car-pooling · "kitne ek-saath"
     //   (JP OA bank me 3-4 baar: "Interval Flip Operations", "Minimum Cores", "Maximum Concurrent Tasks")

```

---

---

## PATTERN 5 — MATRIX
> ★ **YAHAN NAHI MILENGI** (grid dikhti hai, par technique alag hai):
> ```
> number of islands · rotting oranges · max area of island   ->  PATTERN 13 (GRAPHS)
>       grid hai, par kaam BFS/DFS ka hai — visit + mark + neighbors
> search 2D matrix                                           ->  PATTERN 7 (BINARY SEARCH)
>       naam me matrix hai, par rows sorted hain -> ek lambi sorted array maan lo
> ```

```

 BROAD IDEA: 2D grid pe index-navigation (boundary/direction careful; copy-pen pe grid banao).

── (STANDALONE — har ek apni index-navigation) ──

 ▸ SET MATRIX ZEROES
     // in-place, 2 marker array, O(m+n) space
     int row = matrix.size(); int col = matrix[0].size(); vector<int> zeroRow(row, -1);   // -1 = unmarked
     vector<int> zeroCol(col, -1);
     for (int i = 0; i < row; i++)           // pass-1: sirf MARK karo
         for (int j = 0; j < col; j++)
             if (matrix[i][j] == 0)          // 2 pass: ek hi pass me 0 kiya to naye 0 bhi mark ho jaate
                 zeroRow[i] = 0; zeroCol[j] = 0;
     for (int i = 0; i < row; i++)           // pass-2: marked ho to 0
         for (int j = 0; j < col; j++)
             if (zeroRow[i] == 0 || zeroCol[j] == 0)   // || : row ya col koi ek bhi marked -> 0
                 matrix[i][j] = 0;

 ▸ SPIRAL MATRIX (LC-54)  = 4 boundary andar simat-te (clockwise)
     int row = matrix.size(); int col = matrix[0].size();
     int top = 0, bottom = row - 1, left = 0, right = col - 1;   // -1 : inclusive, isliye loop <=
     vector<int> ans;
     while (top <= bottom && left <= right)     // <= : ek akeli row / col bachi ho tab bhi chhaapni hai
         for (int i = left; i <= right; i++)
             ans.push_back(matrix[top][i]);  // top row L->R : [top][i]
         top++;                              // turant: right col wala loop ye corner dobara na le
         for (int i = top; i <= bottom; i++)
             ans.push_back(matrix[i][right]);   // right col T->B : [i][right]
         right--;                            // turant: bottom row wala loop ye corner dobara na le
         if (top <= bottom)                  // GUARD: bottom row bachi? (ek hi row -> ulti dobara)
             for (int i = right; i >= left; i--)
                 ans.push_back(matrix[bottom][i]);   // bottom row R->L : [bottom][i]
         bottom--;
         if (left <= right)                  // GUARD: left col bachi? (ek hi col -> dobara)
             for (int i = bottom; i >= top; i--)
                 ans.push_back(matrix[i][left]);     // left col B->T : [i][left]
         left++;
     return ans;
     // guard sirf 3rd / 4th loop pe: 1st / 2nd ko while(top<=bottom && left<=right) bacha leta
     // [[1,2,3]]: top row 1 2 3 -> top=1 > bottom=0 -> bottom row skip (warna 2 1 dobara)
     // [[1],[2],[3]]: right col ke baad right=-1 < left=0 -> left col skip (warna 2 dobara)

 ▸ TRANSPOSE
     int row = matrix.size(); int col = matrix[0].size();
     vector<vector<int>> ans(col, vector<int>(row, -1));   // naya col x row size (non-square bhi)
     for (int i = 0; i < col; i++)           // outer i < col
         for (int j = 0; j < row; j++)       // inner j < row
             ans[i][j] = matrix[j][i];
     return ans;

 ▸ COUNT SERVERS THAT COMMUNICATE (LC-1267, JP)  = SET MATRIX ZEROES ka cousin (row/col COUNT 2-pass)
     // 1 = server. communicate = uski ROW ya COL me koi DOOSRA server. row + col dono me akela -> nahi gino
     // [[1,0],[1,1]] -> 3  ·  [[1,0],[0,1]] -> 0
     // set zeroes ki 2 marker trick, MARK ki jagah COUNT
     int m = grid.size(); int n = grid[0].size();
     vector<int> rowCount(m, 0);             // 0 se: abhi kisi row / col me koi server gina nahi
     vector<int> colCount(n, 0);
     for (int i = 0; i < m; i++)             // pass-1: poori ginti, tabhi pass-2 me pata akela ya nahi
         for (int j = 0; j < n; j++)
             if (grid[i][j] == 1)
                 rowCount[i]++; colCount[j]++;
     int count = 0;
     for (int i = 0; i < m; i++)             // pass-2: check
         for (int j = 0; j < n; j++)
             if (grid[i][j] == 1 && (rowCount[i] > 1 || colCount[j] > 1))   // >1 : khud ke alawa koi aur
                 count++;                    // || : row ya col koi ek kaafi
     return count;                           // O(m*n)

```

---

---

## PATTERN 6 — STACK
> ★ **YAHAN NAHI MILEGI:**
> ```
> generate parentheses   ->  PATTERN 16 (BACKTRACKING)
>       valid-parentheses CHECK karta hai (stack) · generate BANATA hai (choose/un-choose)
> ```

```

 BROAD IDEA: stack = LIFO. "kholo push, band karo pop-match" · ya MONOTONIC (next-greater/smaller).
 ★ EDGE (monotonic stack): stack me INDEX rakho, value nahi (DOORI chahiye: i - st.top()).
 ★ MILTE-JULTE (histogram vs container / trapping): upar "NAAM-SE-CONFUSE" -> A dekh.

── FAMILY: MONOTONIC stack (INDEX stack, curr vs top compare -> pop + us index ka ans) ──
     // ek hi skeleton, sirf 3 cheez badalti:
     // (1) cmp: `<` = GREATER dhoondh rahe, `>` = SMALLER (ulta lagta: top ko jab curr hara de)
     // (2) ans me kya: VALUE (next greater / smaller) · DISTANCE i - top (daily temp) · INDEX (histogram)
     // (3) disha: next = 0..n-1 · prev = n-1..0 (loop ulta) · circular = 2n loop, i % n

 ▸ NEXT GREATER ELEMENT
     // har element ke DAAYE pehla BADA, koi nahi -> -1.  [2,1,2,4,3] -> [4,2,4,-1,-1]
     vector<int> ans(nums.size(), -1);       // -1 init: daaye bada na mile wo -1 hi rahe
     stack<int> st;                          // INDEX rakho: ans[topp] me likhne ko position chahiye
     for (int i = 0; i < nums.size(); i++)   // left->right: daaye wala bada aage aayega
         while (!st.empty() && nums[st.top()] < nums[i])     // !empty pehle (warna top() crash)
             int topp = st.top();            // < : curr BADA -> top ko jawab mila (barabar = bada nahi)
             st.pop(); ans[topp] = nums[i];   // top pehle bachao: pop ke baad top() badal jaata
         st.push(i);                         // while ke BAAD push: curr ab apne jawab ka intezaar kare
     return ans;                             // stack me bache = koi bada nahi = -1 (init)

 ▸ NEXT SMALLER ELEMENT
     // = next greater, sirf condition ULTI
     vector<int> ans(nums.size(), -1); stack<int> st;
     for (int i = 0; i < nums.size(); i++)
         while (!st.empty() && nums[st.top()] > nums[i])     // > strict (>= nahi): [2,2,2] -> saare -1
             int topp = st.top(); st.pop(); ans[topp] = nums[i];
         st.push(i);
     return ans;

 ▸ PREVIOUS SMALLER ELEMENT
     // = next smaller, sirf LOOP ULTA (condition wahi >)
     vector<int> ans(nums.size(), -1); stack<int> st;
     for (int i = nums.size() - 1; i >= 0; i--)     // right->left: right ka next smaller = left ka prev
         while (!st.empty() && nums[st.top()] > nums[i])
             int topp = st.top(); st.pop(); ans[topp] = nums[i];
         st.push(i);
     return ans;
     // prev smaller + next smaller = histogram ka building block

 ▸ LARGEST RECTANGLE IN HISTOGRAM (LC-84)
     // har bar pe KHADE ho: left me pehla CHHOTA (pS), right me pehla CHHOTA (nS) = dono STOPPER
     // [2,1,5,6,2,3] -> 10 (bars 5,6 -> min 5 * width 2). next GREATER se nahi: rectangle chhote pe rukta
     // nextSmaller
     int n = heights.size();
     stack<int> st; vector<int> ans(n, n);   // default n (-1 NAHI): right ki bahar deewar
     for (int i = 0; i < n; i++)
         while (!st.empty() && heights[st.top()] > heights[i])     // > strict: [3,3,3,3] >= -> 3, > -> 12
             int tp = st.top(); st.pop(); ans[tp] = i;   // INDEX store, value nahi: width index se banti
         st.push(i);
     return ans;

     // prevSmaller
     int n = heights.size();
     stack<int> st; vector<int> ans(n, -1);   // default -1: left ki bahar deewar (index 0 se pehle)
     for (int i = n - 1; i >= 0; i--)
         while (!st.empty() && heights[st.top()] > heights[i])
             int tp = st.top(); st.pop(); ans[tp] = i;
         st.push(i);
     return ans;

     // largestRectangleArea
     int n = heights.size();
     vector<int> ns = nextSmaller(heights); vector<int> ps = prevSmaller(heights);
     int ans = INT_MIN;                      // INT_MIN: pehla area bhi max me aaye (0 bhi chalega)
     for (int i = 0; i < n; i++)             // har bar pe khade ho: wahi is rectangle ki height
         int width = ns[i] - ps[i] - 1;      // bars ps+1 .. ns-1; stopper khud andar nahi
         ans = max(ans, heights[i] * width);   // heights[i] : range me ye bar sabse chhota
     return ans;
     // [2,4]: nS default -1 hota to bar0 width = -1-(-1)-1 = -1 (galat); n=2 se width 2 -> area 4 (sahi)

 ▸ MAXIMAL RECTANGLE (LC-85, 2D)
     // = LC-84 ka 2D bhai: HAR ROW = histogram -> upar wala largestRectangleArea REUSE -> sab rows ka MAX
     int row = matrix.size(); int col = matrix[0].size();
     vector<int> heights(col, 0); int maxi = INT_MIN;   // COL size (row nahi): har column ki running height
     for (int i = 0; i < row; i++)           // outer i = row
         for (int j = 0; j < col; j++)       // inner j = col
             if (matrix[i][j] == 1)  heights[j] += 1;   // upar wale 1s ke saath build-up
             else  heights[j] = 0;           // column toot gaya -> RESET
         maxi = max(maxi, largestRectangleArea(heights));   // call j-loop ke BAHAR, har row ke baad
     return maxi;

 ▸ DAILY TEMPERATURES
     int n = temps.size();
     vector<int> ans(n, 0); stack<int> st;   // 0 init (-1 nahi): garam din na mile to 0
     for (int i = 0; i < n; i++)
         while (!st.empty() && temps[st.top()] < temps[i])     // < strict: barabar = garam nahi
             int tp = st.top(); st.pop(); ans[tp] = i - tp;   // VALUE nahi, DISTANCE (kitne din baad)
         st.push(i);
     return ans;

 ▸ NEXT GREATER ELEM II
     // CIRCULAR: aakhri ka greater shuru me bhi
     if (nums.size() == 0)
         return {};
     int n = nums.size();
     stack<int> st;                          // INDEX (0..n-1) jinka greater abhi nahi mila
     vector<int> ans(n, -1);                 // size n (2n nahi); na mile -> -1
     for (int i = 0; i < 2 * n; i++)         // 2n: doosre chakkar me aakhri elements ko shuru wale dikhte
         while (!st.empty() && nums[st.top()] < nums[i % n])     // i % n = asli index (i 2n tak)
             int tp = st.top(); st.pop(); ans[tp] = nums[i % n];   // jawab TOP ka -> ans[tp] (ans[i] nahi)
         st.push(i % n);                     // i % n push, i nahi (i >= n pe index bahar)
     return ans;
     // [1,2,1]: aakhri 1 (idx 2) pehle chakkar me bacha -> i=4 -> 4%3=1 -> nums[1]=2 -> ans[2]=2. isliye 2n

 ▸ ONLINE STOCK SPAN (LC-901)  = monotonic-stack + SUM-span DELTA
     // STREAMING next(price). span = aaj samet peeche kitne LAGATAAR din price <= aaj
     // next(100,80,60,70,60,75,85) -> 1,1,1,2,1,4,6
     // class StockSpanner
     stack<pair<int, int>> st;               // INDEX nahi, PAIR {price, span}
     StockSpanner() {}

     // next
     int span = 1;                       // 1 se: aaj ka din khud bhi span me
     while (!st.empty() && st.top().first <= price)     // <= : barabar price wala din bhi span me
         span += st.top().second; st.pop();   // pop wale ka span JODO: wo apne peeche ke din nigal chuka
     st.push({price, span});             // poora span saath push: aage bada din ise nigle to sab jude
     return span;
     // 75 aaya, top pe ...60,70,60: 60 -> span 2 · 70 -> span 4 · 80 > 75 ruko -> push {75,4}

── (STANDALONE — apni alag trick) ──

 ▸ VALID PARENTHESES
     stack<int> st;
     for (auto &it : s)
         if (it == '(' || it == '[' || it == '{')  st.push(it);   // opening -> push
         else
             if (st.empty())
                 return false;               // closing aaya, stack khaali -> false
             char ch = st.top(); st.pop();   // top + pop PEHLE, phir match
             if ((ch == '(' && it == ')') || (ch == '[' && it == ']') || (ch == '{' && it == '}'))
                 continue;
             else
                 return false;               // match nahi -> turant false
     return st.empty();                      // end me stack KHAALI = valid ("" -> true)

 ▸ MIN STACK
     // class MinStack: push / pop / top / getMin sab O(1)
     stack<pair<int, int>> st;               // {val, yahan tak ka MIN}: min hamesha ready, scan nahi
     MinStack() {}

     // push
     int minVal = st.empty() ? val : min(st.top().second, val);   // empty pehle: pehle pe top() nahi
     st.push({val, minVal});
     void pop() { st.pop(); }
     int top() { return st.top().first; }
     int getMin() { return st.top().second; }   // O(1), koi loop nahi
     // ek single min var kyun NAHI: pop pe purana min WAPAS aata, single var wo history nahi rakhta
     // push 5,3,7 -> min 3 · pop 7 -> 3 · pop 3 -> 5 WAPAS (pair me neeche wala min apne aap sahi)

 ▸ MIN ADD MAKE VALID
     // COUNTER, stack nahi (sirf '(' ')')
     if (s.empty())
         return 0;
     int open_needed = 0;                    // kitne ')' UNMATCHED aaye (inhe pehle '(' chahiye tha)
     int close_needed = 0;                   // kitne '(' abhi UNMATCHED khade (inhe aage ')' chahiye)
     for (auto &it : s)
         if (it == '(')  close_needed++;     // ek open, iska close pending
         else
             if (close_needed > 0)  close_needed--;   // pending open se MATCH = 0 insertion ("(())" -> 0)
             else  open_needed++;            // koi open nahi -> ye ')' extra
     return open_needed + close_needed;      // dono bache = utne insert

 ▸ QUEUE USING STACKS (LC 232)  — Walmart R2 me aaya ("fully working code")
     // class MyQueue
     stack<int> input; stack<int> output;    // push yahan · pop / peek yahan se
     MyQueue() {}

     // push
     input.push(x);

     // pop
     if (output.empty())                 // KHAALI ho tabhi palto: har element ek hi baar input -> output
         while (!input.empty())          // -> amortized O(1) (har pop pe palatna = O(n))
             output.push(input.top()); input.pop();
     int x = output.top(); output.pop();     // x bachao, phir pop, phir return
     return x;

     // peek
     if (output.empty())                 // TRAP: output me kuch bacha ho tab palta -> kram toot jaata
         while (!input.empty())
             output.push(input.top()); input.pop();
     return output.top();

     // empty
     if (input.empty() && output.empty())   // dono khaali
         return true;
     return false;

     broad-trick: MONOTONIC stack (next-greater + daily-temp = same, bas ans me kya bharo alag).

```

---

---

## PATTERN 7 — BINARY SEARCH

```

 BROAD IDEA: sorted space -> mid -> aadha kaato -> O(log n). (space array ho ya ANSWER-range).
 BS ko poori sorted array nahi chahiye: bas ek comparison se "kaunsa half phenko" tay ho (find-peak bhi).
 BOUNDARY RULE: jis boundary pe element KHUD answer ho sakta -> equality INCLUDE (>= / <=).
    jahan mid==target pehle hi return ho gaya -> < ya <= farak nahi. galat >/< = off-by-one -> chhota dry-run.
 ★ EDGE (BS on answer): lo = max(arr) (isse kam me bhaari item uthta hi nahi), hi = sum(arr).
         ship-in-D-days [3,10], D=2: lo=0 rakha -> 5 aata, sahi 10.

 high=mid  vs  high=mid-1 (EK SAWAAL: "mid KHUD answer ho sakta abhi bhi?"):
    TARGET-REJECT (basic / rotated search: mid != target pakka)   -> high = mid-1  + while (low <= high)
    KHUD-ANSWER (find-peak / find-min-rotated, koi target nahi)  -> high = mid    + while (low < high)
    high=mid + while(low<=high) = INFINITE LOOP: mid neeche round, low==high==mid pe range shrink nahi
       ([2,1] peak-logic: mid=0 -> high=0 -> low=high=0 kabhi khatam nahi)
 3 broad trick: BS-on-ANSWER (koko) · sorted-half-check (rotated) · 2D-index-map (matrix).

── FAMILY: BASIC / BOUNDARY (target ko mid se compare, low<=high, mid±1; mila to ruko mat = boundary) ──

 ▸ BINARY SEARCH (BASIC)
     int low = 0, high = nums.size() - 1;
     while (low <= high)                         // <= : 1 element bacha tab bhi check
         int mid = low + (high - low) / 2;       // (low+high)/2 nahi: bade low+high pe overflow
         if (nums[mid] == target) return mid;
         else if (nums[mid] < target) low = mid + 1;   // target right me; mid khud nahi -> +1
         else high = mid - 1;                    // target left me; mid khud nahi -> -1
     return -1;                                  // loop ke bahar: nahi mila

 ▸ SEARCH INSERT POSITION
     // [1,3,5,6], 2 -> 1 (1 aur 3 ke beech)
     int low = 0, high = nums.size() - 1;
     while (low <= high)                         // basic BS hubahu
         int mid = low + (high - low) / 2;
         if (nums[mid] == target) return mid;
         else if (nums[mid] < target) low = mid + 1;
         else high = mid - 1;
     return low;                                 // na mila: low = pehla nums >= target = insert jagah

 ▸ FIRST & LAST POSITION (LC-34)
     // sorted (duplicate ho sakte) me target ka PEHLA aur AAKHRI index; nahi -> {-1,-1}, O(log n)
     // [5,7,7,8,8,10], 8 -> {3,4}. normal BS, bas MILNE pe RUKO MAT -> ans likho, ek taraf aur chalo. 2 BS
     vector<int> ans(2, -1);                     // size fixed 2 -> ans(2,-1) + index set, push_back NAHI
     int low = 0, high = nums.size() - 1;        // n-1: aakhri valid index, dono sire inclusive
     while (low <= high)                         // PASS 1 = FIRST
         int mid = low + (high - low) / 2;
         if (nums[mid] == target)
             ans[0] = mid; high = mid - 1;       // mila -> aur LEFT (mid save, usse pehle wala dhoondo)
         else if (nums[mid] < target) low = mid + 1;
         else high = mid - 1;
     low = 0, high = nums.size() - 1;            // RESET, warna PASS 2 galat jagah se shuru
     while (low <= high)                         // PASS 2 = LAST
         int mid = low + (high - low) / 2;
         if (nums[mid] == target)            // mila -> aur RIGHT (sirf EK pointer hilao)
             ans[1] = mid; low = mid + 1;
         else if (nums[mid] < target) low = mid + 1;
         else high = mid - 1;
     return ans;                                 // PASS 1: mid 4 -> 3 = FIRST 3 · PASS 2: mid 4 -> LAST 4

── FAMILY: ROTATED (ek comparison se kaunsa half sorted / pivot kidhar, wahi half rakho) ──

 ▸ SEARCH IN ROTATED (LC-33)  = rotated sorted array me target, O(log n)
     // [4,5,6,7,0,1,2] me target ka index, nahi -> -1
     int n = nums.size(); int low = 0; int high = n - 1;
     while (low <= high)                         // <= : akela element bacha tab bhi check
         int mid = low + (high - low) / 2;
         if (nums[mid] == target) return mid;
         if (nums[low] <= nums[mid])             // LEFT half sorted (<= : low==mid pe akela bhi sorted)
             if (target >= nums[low] && target <= nums[mid])   // >= nums[low] ZARURI: target low pe
                 high = mid - 1;                 // khud ho sakta, warna miss ([5,1,3], 5)
             else low = mid + 1;
         else                                    // warna RIGHT half sorted
             if (target >= nums[mid] && target <= nums[high])   // <= nums[high]: target high pe ho sakta
                 low = mid + 1;
             else high = mid - 1;
     return -1;                                  // har baar aadha kaata -> O(log n)

 ▸ FIND MIN IN ROTATED
     // target nahi, khud MIN -> mid ko HIGH se compare
     int low = 0, high = nums.size() - 1;
     while (low < high)                          // low<high: high=mid ke saath <= = infinite loop
         int mid = low + (high - low) / 2;
         if (nums[mid] > nums[high]) low = mid + 1;   // mid bada -> min right me, mid khud nahi
         else high = mid;                        // high=MID (mid-1 NAHI): mid khud min ho sakta
         // ([3,1,2] pe mid-1 se galat 3 aata)
     return nums[low];

── FAMILY: BS-on-ANSWER (koko · ship · split: SHELL same, sirf RANGE + solve(mid) alag) ──

     // array nahi, ANSWER-range pe BS. solve(mid) feasible? -> chhota try (high=mid-1), warna low=mid+1
     // SHARED SHELL (teeno bilkul same):
     //    ans = -1                                   // koi mid feasible na nikle to -1
     //    while (low <= high)                         // <= : low==high wala aakhri candidate bhi test
     //        mid = low + (high - low) / 2
     //        solve(mid) ? (ans = mid, high = mid - 1) : low = mid + 1   // feasible nahi: mid bekaar
     // FARAK sirf 2: (a) RANGE (low, high)  (b) solve(mid) ka formula (greedy count <= limit?)

 ▸ KOKO BANANAS (LC-875) -- min eating-speed
     bool solve(vector<int> &piles, int h, int mid)
         long long hours = 0;                    // long long: bade piles pe sum overflow
         for (int i = 0; i < piles.size(); i++)
             hours += ceil((double)piles[i] / mid);   // double cast PEHLE: int/int pehle hi floor (7/3=2)
                                                 // ya integer: (piles[i] + mid - 1) / mid (safe, prefer)
         return hours <= h;                      // hours = sum ceil(piles[i] / mid) <= h ?

     int minEatingSpeed(vector<int> &piles, int h)
         int low = 1;                            // low=1: 0 pe piles[i]/mid = divide by zero
         int high = *max_element(begin(piles), end(piles));   // high = max: ek ghante me sabse badi dheri
         int ans = -1;                           // [3,6,7,11], h=8 -> 4
         while (low <= high)
             int mid = low + (high - low) / 2;
             if (solve(piles, h, mid))           // feasible -> chhota try
                 ans = mid; high = mid - 1;
             else low = mid + 1;
         return ans;

 ▸ SHIP WITHIN D DAYS (LC-1011) -- min capacity
     bool solve(vector<int> &weights, int D, int mid)
         int days = 1; int sum = 0;              // day=1: aakhri din ka load bhi gino
         for (auto &w : weights)
             if (sum + w > mid)                  // aaj w chadhaya to capacity tooti -> agla din
                 days++; sum = 0;
             sum += w;
         return days <= D;

     int shipWithinDays(vector<int> &weights, int days)
         int low = *max_element(begin(weights), end(weights));   // low=max: sabse bhaari packet akela chadhe
         int high = accumulate(begin(weights), end(weights), 0); int ans = -1;   // high=sum: ek din me sab
         while (low <= high)                     // SHELL wahi
             int mid = low + (high - low) / 2;
             if (solve(weights, days, mid))
                 ans = mid; high = mid - 1;
             else low = mid + 1;
         return ans;

 ▸ SPLIT ARRAY LARGEST SUM (LC-410) -- min largest part-sum
     bool solve(vector<int> &nums, int k, int mid)
         int sum = 0, count = 1;                 // count=1 se START: warna aakhri tukda chhoot jaata
         for (int i = 0; i < nums.size(); i++)
             sum += nums[i];
             if (sum > mid)
                 count++; sum = nums[i];         // sum = x (0 nahi): limit todne wala x naye tukde ka pehla
         return count <= k;

     int splitArray(vector<int> &nums, int k)
         int low = *max_element(begin(nums), end(nums));   // low=max: bada element akela
         int high = accumulate(begin(nums), end(nums), 0); int ans = -1;   // high=sum: k=1 poora
         while (low <= high)                     // SHELL wahi
             int mid = low + (high - low) / 2;
             if (solve(nums, k, mid))
                 ans = mid; high = mid - 1;
             else low = mid + 1;
         return ans;

── FAMILY: HALF-DISCARD (array pura sorted nahi, phir bhi ek comparison se aadha safe discard) ──

 ▸ FIND PEAK ELEMENT (LC-162)
     // peak = dono padosi se bada; edges ke bahar -inf
     int n = nums.size(); int low = 0, high = n - 1;
     while (low < high)                          // low<high: high=mid se atke nahi, aur mid+1 hamesha andar
         int mid = low + (high - low) / 2;
         if (nums[mid] < nums[mid + 1])
             low = mid + 1;                      // CHADHAAI -> peak RIGHT me (mid peak nahi, right bada)
         else if (nums[mid] > nums[mid + 1])
             high = mid;                         // DHALAAN -> mid KHUD ya left: high = mid (mid-1 NAHI)
     return low;                                 // low==high pe converge = peak, edges apne aap

 ▸ SINGLE ELEMENT (SORTED, LC-540)  = INDEX-PARITY binary search
     // har element 2x, sirf EK akela. O(log n) (XOR = O(n), yahan log n chahiye)
     // single se PEHLE pairs (even,odd) idx pe -> single ke BAAD (odd,even) SHIFT. mid ko mid-1 se compare
     if (nums.size() == 1) return nums[0];       // akela element hi single, BS ki zarurat nahi
     int n = nums.size(); int low = 0; int high = n - 1;   // poori array inclusive
     while (low <= high)                         // <= : low>high pe hi ruko, tab high single pe
         int mid = low + (high - low) / 2;
         if (mid == 0) return nums[mid];         // edge: mid-1 = -1 bahar; yahan aaye to single idx 0 pe
         if (mid % 2 == 0)                       // even mid: single se pehle iska saathi AAGE hota
             if (nums[mid] == nums[mid - 1])
                 high = mid - 1;                 // saathi peeche = pairing shift ho chuki -> single PEECHE
             else low = mid + 1;                 // pairing theek -> single aage
         else                                    // odd mid: single se pehle iska saathi PEECHE hota
             if (nums[mid] != nums[mid - 1])
                 high = mid - 1;                 // saathi peeche nahi = shift ho chuki -> single PEECHE
             else low = mid + 1;                 // saathi peeche mila = theek -> single aage
     return nums[high];                          // HIGH (low nahi): dono branch mid reject, high single pe
     // [1,1,2,3,3,4,4,8,8]: mid 4 -> 1 -> 2 -> 3 -> high=2 -> 2

── FAMILY: 2D-index-map (2D matrix = 1D sorted array, index -> (row, col)) ──

 ▸ SEARCH 2D MATRIX
     int row = matrix.size(); int col = matrix[0].size();   // col = matrix[0].size(), row nahi
     int low = 0, high = row * col - 1;          // poore matrix ko 1D maano
     while (low <= high)
         int mid = low + (high - low) / 2;
         if (matrix[mid / col][mid % col] == target) return true;   // row = mid / col, col = mid % col
         else if (matrix[mid / col][mid % col] < target) low = mid + 1;
         else high = mid - 1;
     return false;

── FAMILY: BS-on-PARTITION / CUT (value pe nahi, 2 sorted array me CUT-position pe BS) ──

 ▸ MEDIAN OF 2 SORTED ARRAYS (LC-4, Hard)
     // MERGE mat karo -> CUT: LEFT aadha | RIGHT aadha, left ka HAR <= right ka HAR. 4 kinaare = median
     // A=[1,3,8,9,15], B=[7,11,18,19,21,25] -> Px=4 (1 3 8 9 | 15), Py=2 (7 11 | 18 ..) -> max(9,11) = 11
     // Px=2 try: x2=19 > x3=8 -> A ne KAM liya -> low = Px+1. cousin = BS-on-ANSWER (wahan VALUE pe)
     int m = A.size(); int n = B.size();
     if (m > n) return findMedianSortedArrays(B, A);   // chhote pe BS -> Py kabhi -ve nahi, O(log(min(m,n)))
     int low = 0, high = m;                      // high=m: A ke 0 se m tak kitne bhi left me
     while (low <= high)                         // <= : Px=low==high wala cut bhi try
         int Px = low + (high - low) / 2;        // Px = A-left count (BS isi pe)
         int Py = ((m + n + 1) / 2) - Px;        // +1: odd total pe left me ek zyada
         int x1 = (Px == 0) ? INT_MIN : A[Px - 1];   // left khaali -> -INF (A[-1] bahar)
         int x2 = (Py == 0) ? INT_MIN : B[Py - 1];
         int x3 = (Px == m) ? INT_MAX : A[Px];   // right khaali -> +INF (A[m] bahar)
         int x4 = (Py == n) ? INT_MAX : B[Py];
         if (x1 <= x4 && x2 <= x3)               // && : dono cross-check sahi tabhi cut valid
             if ((m + n) % 2 == 0)
                 return (max(x1, x2) + min(x3, x4)) / 2.0;   // /2.0: int division se .5 kat jaata
             else return max(x1, x2);            // odd -> left ka max
         if (x1 > x4) high = Px - 1;             // A ne ZYADA liya -> cut left khiskao
         else low = Px + 1;                      // A ne kam liya
     return 0.0;
```

---

---

## PATTERN 8 — LINKED LIST
> **YAHAN NAHI MILEGI:**
> ```
> merge K sorted lists   ->  PATTERN 14 (HEAP)
>       node/list dikhta hai, par asli kaam MIN-HEAP of current heads ka hai
>       (jodne ka engine — dummy + tail — wahi hai jo merge-TWO me hai)
> ```

```

 BROAD IDEA: pointer manipulation. 3 broad-trick: FAST/SLOW · REVERSE (3-pointer) · DUMMY node.
 ★ EDGE (fast-slow): loop ka SHURU chahiye -> milne ke baad slow ko HEAD pe, dono 1-1 step.
         sirf "loop hai ya nahi" poocha to ye step nahi chahiye.

 LOOP-CONDITION (while me kya likhun) -- ek sawaal: aakhri node ka bhi kaam hai, ya sirf uske NEXT ko dekhna?
     while (curr != NULL)        // har node ka apna kaam (get / remove / count / print) -> ZYADATAR yahi
     while (curr->next != NULL)  // curr ka NEXT dekhna, ya aakhri node PE rukna (tail pakad ke aage jodna)
     // A->B->NULL: curr!=NULL B tak jaake B ka kaam karta; curr->next!=NULL B se pehle ruk jaata

── FAMILY: FAST/SLOW (slow 1 kadam, fast 2 kadam ya gap; middle / cycle / palindrome / nth-from-end) ──

 ▸ MIDDLE OF LIST
     // fast end pe -> slow MIDDLE pe
     Node *slow = head; Node *fast = head;       // dono head se: even list me 2nd middle milta
     while (fast != NULL && fast->next != NULL)   // fast pehle check (NULL->next crash);
         slow = slow->next; fast = fast->next->next;   // fast->next check hataya -> fast->next->next crash
     return slow;                                // [1,2,3,4] -> 3 (2nd middle) · [1,2,3] -> 2

 ▸ DETECT CYCLE
     // Floyd: cycle ho to tez-dheema MILENGE
     Node *slow = head; Node *fast = head;       // dono head se: cycle ho to fast ghoom ke slow ko pakdega
     while (fast != NULL && fast->next != NULL)
         slow = slow->next; fast = fast->next->next;
         if (slow == fast)                       // check move ke BAAD: dono head se shuru,
             return true;                        // pehle check karte to turant true
     return false;                               // fast NULL tak pahuncha = list ka end, cycle nahi

 ▸ FIND DUPLICATE NUMBER (LC-287)   [= DETECT CYCLE ka ARRAY-application]
     // n+1 numbers, values 1..n, ek repeat. O(1) space + array READ-ONLY
     // PEHCHAN: "array + duplicate + O(1) space + no-modify" -> value = POINTER (i -> nums[i])
     // values 1..n + n+1 slot -> koi value 2 baar point -> loop; cycle ki ENTRY = duplicate
     // [3,1,3,4,2]: 0->3->4->2->3->4->2... (loop, entry 3 = dup)
     int slow = 0, fast = 0;                     // 0 se: value 1..n, koi 0 pe point nahi -> 0 cycle ke bahar
     slow = nums[slow], fast = nums[nums[fast]];   // PEHLE ek kadam: 0 pe hi rahe to loop chalega nahi
     while (slow != fast)                        // PHASE-1: cycle me tez-dheema milne tak
         slow = nums[slow]; fast = nums[nums[fast]];
     slow = 0;                                   // sirf slow wapas 0 pe, fast meet point pe hi rahe
     while (slow != fast)                        // PHASE-2 entry: ab fast bhi 1 kadam, 2 nahi
         slow = nums[slow]; fast = nums[fast];   // (meet se aur 0 se entry tak doori BARABAR = Floyd)
     return slow;                                // jahan mile wahi cycle entry = duplicate

 ▸ LINKED LIST CYCLE II (LC-142)   [= DETECT CYCLE + phase-2 ENTRY]
     // cycle ho to uske START node ka pointer, warna NULL. = find-duplicate ka Node* version (->next)
     if (head == NULL) return NULL;
     ListNode *slow = head; ListNode *fast = head;
     bool cycle = false;                         // flag: 1 node pe bina cycle bhi slow==fast hota
     while (fast != NULL && fast->next != NULL)
         slow = slow->next; fast = fast->next->next;
         if (slow == fast)                   // break: fast meet point pe hi ruke
             cycle = true; break;
     if (!cycle)                                 // flag se decide, `slow != fast` se NAHI:
         return NULL;                            // 1 node pe loop chalta hi nahi, dono head pe = galat head
     slow = head;                                // sirf slow head pe, fast meet point pe
     while (slow != fast)                        // PHASE-2: dono 1-1 kadam (meet aur head se entry barabar)
         slow = slow->next; fast = fast->next;
     return slow;
     // detect cycle + find dup + cycle II = teeno wahi Floyd

 ▸ PALINDROME LL (LC-234)
     // COMBO: middle -> 2nd half REVERSE -> head (aage se) & rev (peeche se) saath compare
     // middleNode = upar wala MIDDLE OF LIST · reverseList = neeche wala REVERSE LINKED LIST
     Node *mid = middleNode(head);               // slow / fast
     Node *rev = reverseList(mid);               // 2nd half ulta (rev = naya head)
     while (head != NULL && rev != NULL)         // && : even me rev half chhota, jo pehle NULL wahin ruko
         if (head->val == rev->val)
             head = head->next; rev = rev->next;
         else return false;
     return true;                                // khaali list -> true
     // list TOOTI nahi: pehle half ka ->next nahi badla, odd me middle SHARED -> bina cut odd + even dono

 ▸ REMOVE NTH FROM END
     // FAST/SLOW GAP: pehle fast n kadam aage, phir dono saath
     Node *slow = head; Node *fast = head;       // dummy nahi, dono head se
     for (int i = 1; i <= n; i++) fast = fast->next;   // fast n kadam aage -> dono me n ka gap
     if (fast == NULL) return head->next;        // fast list se bahar = head hi hatana hai
     while (fast->next != NULL)                  // fast AAKHRI node pe ruke -> slow hatane wale ke PEHLE
         slow = slow->next; fast = fast->next;   // dono saath, gap wahi
     slow->next = slow->next->next;              // while ke BAHAR: loop khatam, ek hi baar hatao
     return head;                                // head nahi hata

 ▸ REMOVE ELEMENTS (delete by val, LC-203)
     // val wale SAARE node hatao
     if (!head) return NULL;
     Node *prev = NULL; Node *curr = head;       // prev=NULL: head ke peeche koi node nahi
     while (curr != NULL && head != NULL)        // head NULL = sab node hat gaye, aage kuch nahi
         if (curr->val == val && prev == NULL) head = head->next;   // HEAD hata (ya saare node same val)
         else if (curr->val == val && prev != NULL)
             prev->next = curr->next;            // beech ka hata; prev = curr NAHI (curr hata hua node)
         else prev = curr;                       // prev SIRF no-match pe aage, warna [1,6,6,2] val=6 fail
         curr = curr->next;                      // har case ke end me
     return head;

── FAMILY: DUMMY node (nakli head + tail; naye node jodte jao; ans = dummy->next; head ka case apne aap) ──

 ▸ MERGE TWO SORTED (LC-21)
     Node *dummy = new Node(0);                  // dummy: head ka special case nahi
     Node *tail = dummy;                         // tail dummy se shuru
     while (a != NULL && b != NULL)              // && : ek list khatam hote hi ruko, baaki neeche jodte
         if (a->val <= b->val)
             tail->next = a; a = a->next; tail = tail->next;
         else if (b->val < a->val)
             tail->next = b; b = b->next; tail = tail->next;
     while (a != NULL)                   // baaki a jod do (already sorted)
         tail->next = a; a = a->next; tail = tail->next;
     while (b != NULL)                   // baaki b jod do
         tail->next = b; b = b->next; tail = tail->next;
     return dummy->next;                         // dummy skip -> asli head

 ▸ ADD TWO NUMBERS
     // number list me ULTA (units digit HEAD pe) -> head se head jodo = school wala right-to-left add
     // (2->4->3) + (5->6->4) = 342 + 465 = 807 -> (7->0->8)
     Node *dummy = new Node(0); Node *tail = dummy; int carry = 0;   // dummy + tail (MERGE jaisa) + ek CARRY
     while (a != NULL || b != NULL || carry > 0)   // || carry: 999 + 1 -> list khatam, carry 1 bacha
         int sum = 0;                            // -> ek aur node (leading 1)
         if (a != nullptr)                   // a hai to jodo, a aage
             sum += a->val; a = a->next;
         if (b != nullptr)                   // b hai to jodo, b aage
             sum += b->val; b = b->next;
         sum += carry; tail->next = new Node(sum % 10);   // pichhle step ka carry · digit jodo
         tail = tail->next; carry = sum / 10;    // agle ke liye
     return dummy->next;                         // dummy nakli, asli head uske next pe

 ▸ SWAP NODES IN PAIRS  = DUMMY + 3-player rewire (order + temp-save)
     // har do adjacent NODE ki jodi swap (values nahi, pointer)
     if (!head) return nullptr;                  // khaali list: swap ko kuch nahi
     Node *dummy = new Node(0);                  // dummy: head bhi badlega, usse pehle ek node chahiye
     Node *prev = dummy; prev->next = head;      // prev = jodi se PEHLE wala (pehli jodi ke pehle = dummy)
     Node *first = head;
     while (prev->next != nullptr && first->next != nullptr)   // jodi (2 node) bache tab tak
         Node *second = first->next;             // (pehle prev->next, warna NULL->next)
         Node *temp = second->next; second->next = first;   // BACHA lo, warna agli line me gum · 2 -> 1
         first->next = temp; prev->next = second;   // 1 -> baaki list · prev -> 2 (= is jodi ka naya head)
         prev = first; first = temp;             // ADVANCE: prev = purana first · first = agli jodi ka pehla
         // har iteration SIRF ek jodi; rewire ORDER + temp = crux
     return dummy->next;                         // naya head = pehli jodi ka second

── FAMILY: REVERSE (3-pointer: prev / curr / next se link ulta; reverse khud + palindrome ka 2nd half) ──

 ▸ REVERSE LINKED LIST
     Node *prev = NULL; Node *curr = head;       // prev=NULL: purana head naya tail, uska next NULL
     Node *nextt = head;
     while (curr != NULL)
         nextt = curr->next;                     // nextt pehle BACHAO: link ulta karte hi aage ka raasta gum
         curr->next = prev; prev = curr; curr = nextt;   // curr ka link ULTA (-> prev) · dono aage
     return prev;                                // curr NULL pe ruka, prev = aakhri node = naya head

 ▸ REVERSE NODES IN K-GROUP (LC-25)  = REVERSE-LL (3-ptr) + RECURSION
     // har k-node group ulta (NODES), k se KAM bache -> waise hi. 1->2->3->4->5, k=2 -> 2->1->4->3->5
     // PEHCHAN: "reverse in groups of k" -> 3-ptr + count + recurse (copy-random se ALAG: ulatna, naya nahi)
     if (head == NULL) return NULL;
     int countNode = 0; Node *currNode = head;
     while (currNode && countNode < k)           // (1) count-FIRST: pehle k node GINO
         currNode = currNode->next; countNode++;
     if (countNode != k) return head;            // k se kam bache (leftover) -> waise hi
     int count = 0; Node *prev = NULL; Node *curr = head; Node *nextt = NULL;
     while (curr && count < k)                   // (2) 3-pointer se sirf k node ulte
         nextt = curr->next; curr->next = prev; prev = curr; curr = nextt; count++;
     if (nextt != NULL)                          // (3) head ab is group ka TAIL -> baaki pe RECURSE + stitch
         head->next = reverseKGroup(nextt, k);   // nextt = agle group ka head
     return prev;                                // prev = is group ka naya head

── FAMILY: HASHMAP-AS-GLUE (map<old*, new*> = purane node ko naye se jodne wala gond; LRU me bhi) ──

 ▸ COPY LIST w/ RANDOM POINTER (LC-138) = HASHMAP old->new + 2 PASS
     // har node me val + next + RANDOM (kahin bhi / NULL). DEEP copy: copy ke random COPY ke andar
     // 2 PASS kyun: random aage bhi ho sakta, 1 pass me target abhi bana hi na ho -> pehle sab bana lo (+map)
     // PEHCHAN: "random pointer / deep copy" -> MAP (old -> new). T = O(n), S = O(n)
     if (head == NULL) return NULL;
     unordered_map<Node *, Node *> mp;           // old -> new: pass-2 me random ka naya node yahin se
     Node *curr = head; Node *prev = NULL; Node *newHead = NULL;
     while (curr)                                // PASS 1: clone + next + map
         Node *temp = new Node(curr->val); mp[curr] = temp;   // 7 => 7'
         if (newHead == NULL)                    // pehla clone = naya head, peeche jodne ko koi nahi
             newHead = temp; prev = newHead;
         else                                // pehle prev->next pe jodo, phir prev aage
             prev->next = temp; prev = temp;
         curr = curr->next;
     curr = head; Node *currRand = newHead;      // PASS 2: random set; dono list shuru se, same position
     while (curr)
         if (curr->random == NULL) currRand->random = NULL;   // random NULL to NULL
         else currRand->random = mp[curr->random];   // ORIGINAL ka random padho (curr), uska NAYA copy
         curr = curr->next;                      // copy (currRand) se padhte to abhi NULL hota
         currRand = currRand->next;              // dono saath aage
     return newHead;
     // O(1) space alt = interleave A->A'->B->B' clone (optional, code nahi)
```

---

---

## PATTERN 9 — DESIGN (data-structure)

```

 BROAD IDEA: data-structure ko khud banao (built-in ke bina) -> array + pointers/nodes se.

 ▸ DESIGN HASHMAP (LLD)
     struct Entry                                // Entry = | key | value | next |
         string key; int value; Entry *next;
         Entry(string k, int v) : key(k), value(v), next(nullptr) {}

     class MyHashMap                             // ARRAY (buckets), har slot ek CHAIN
         int count; int cap;                     // kitne pairs · slots = 16
         vector<Entry *> buckets;                // buckets[i] = chain ka HEAD, khud POINTER: isliye
                                                 // array-assign (buckets[i] = X) + node-link (X->next) MIX

     public:
         MyHashMap()
             count = 0; cap = 16; buckets.resize(cap);   // resize -> sab slot nullptr

         void put(string key, int val)
             int index = hash<string>{}(key) % cap;   // % cap: hash bada number, slot 0..cap-1 me laana
             Entry *node = new Entry(key, val);
             if (buckets[index] == nullptr)      // khaali slot: naya node hi chain ka head
                 buckets[index] = node; count++;
             else                                // same index (collision) -> chain me jodo
                 Entry *curr = buckets[index];
                 while (curr != NULL)
                     if (curr->key == key)
                         curr->value = val;      // key MILI -> value update
                         return;                 // TURANT return: warna END pe DUPLICATE node + count++
                     // (chain ke ANDAR update pe phat-ta; test-pass != code-sahi)
                     else
                         if (curr->next == nullptr)   // next null = chain ka END: yahin jodo
                             curr->next = node; count++;   // (agle round naya node match hoke return)
                     curr = curr->next;

         int get(string key)
             int index = hash<string>{}(key) % cap; Entry *curr = buckets[index];
             while (curr != NULL)
                 if (curr->key == key) return curr->value;
                 curr = curr->next;
             return -1;                          // poori chain dekh li, key nahi mili

         void remove(string key)
             int index = hash<string>{}(key) % cap; Entry *curr = buckets[index];
             Entry *prev = NULL;                 // prev=NULL: head ke peeche koi node nahi
             while (curr != NULL)
                 if (curr->key == key)
                     if (prev != NULL)
                         prev->next = curr->next;   // BEECH: [A]->[B]->[C], B hata -> A->next = C
                     else buckets[index] = curr->next;   // prev NULL = HEAD hi hata: bucket ka head badlo
                     count--;
                 prev = curr; curr = curr->next;   // prev = prev->next NAHI: prev NULL -> crash

         int size()
             return count;

 ▸ DESIGN HASHSET (LLD)
     struct Entry                                // HashMap ka EXACT code, bas VALUE nahi: | key | next |
         string key; Entry *next;
         Entry(string k) : key(k), next(nullptr) {}

     class MyHashSet
         int sz; int cap; vector<Entry *> buckets;

     public:
         MyHashSet()
             sz = 0; cap = 16; buckets.resize(cap);

         void add(string key)                    // HashMap ke put() jaisa
             int index = hash<string>{}(key) % cap; Entry *node = new Entry(key);
             if (buckets[index] == nullptr)      // bucket khaali -> node laga + sz++
                 buckets[index] = node; sz++;
             else
                 Entry *curr = buckets[index];
                 while (curr != nullptr)
                     if (curr->key == key) return;   // mili -> kuch nahi (duplicate skip)
                     else
                         if (curr->next == nullptr)
                             curr->next = node;   // na-mili -> chain END me append + sz++ (HashMap jaisa)
                             sz++;
                     curr = curr->next;

         bool contains(string key)               // get() jaisa, value ki jagah true / false
             int index = hash<string>{}(key) % cap; Entry *curr = buckets[index];
             while (curr != nullptr)
                 if (curr->key == key) return true;
                 curr = curr->next;
             return false;

         void remove(string key)                 // HashMap ka remove HUBAHU
             int index = hash<string>{}(key) % cap; Entry *curr = buckets[index]; Entry *prev = NULL;
             while (curr != nullptr)
                 if (curr->key == key)
                     if (prev == nullptr)
                         buckets[index] = curr->next;   // HEAD hata -> bucket ko curr->next se jodo
                     else prev->next = curr->next;   // beech: prev->next = curr->next
                     sz--;
                     return;
                 prev = curr; curr = curr->next;   // prev = prev->next NAHI: NULL->next CRASH
             return;

         int size()
             return sz;

 ▸ LRU CACHE (LC146) — COMBO: hashmap + doubly-linked-list
     struct Node                                 // Node = | key | val | prev | next |
         int key, val; Node *prev, *next;
         Node(int k, int v) : key(k), val(v), prev(nullptr), next(nullptr) {}

     class LRUCache                              // get + put DONO O(1): hashmap (KAHAN) + DLL (ORDER)
         int cap; unordered_map<int, Node *> mp;   // key -> DLL node ka POINTER (find O(1))
         Node *head, *tail;                      // dummy [HEAD] <-> MRU .. LRU <-> [TAIL] dummy (edge clean)

     public:
         LRUCache(int capacity)
             cap = capacity;
             head = new Node(-1, -1);            // TRAP: yahan "Node *head =" likha = shadowing (naya local)
             tail = new Node(-1, -1); head->next = tail; tail->prev = head;

         void removeNode(Node *node)             // beech se nikal, O(1)
             node->next->prev = node->prev; node->prev->next = node->next;

         void addFront(Node *node)
             Node *nxt = head->next;             // nxt PEHLE pakdo: head->next overwrite se pehle (TRAP)
             head->next = node; node->prev = head;

             node->next = nxt; nxt->prev = node;   // node ab head ke turant baad = sabse naya (MRU)

         int get(int key)
             if (mp.count(key) > 0)              // count se: mp[key] chup-chaap entry bana deta
                 Node *node = mp[key]; removeNode(node); addFront(node);   // access = recent -> front pe
                 return node->val;
             return -1;

         void put(int key, int value)
             if (mp.count(key) > 0)
                 Node *node = mp[key]; node->val = value;
                 removeNode(node);               // DONO, get() jaisa. sirf addFront = node purani
                 addFront(node);                 // jagah bhi juda -> wahi evict (put1 put2
             // put(1,10) put3 -> get(1) = -1, sahi 10)
             else
                 if (mp.size() == cap)           // FULL: pehle evict, phir naya (check add se PEHLE)
                     Node *toBeRemoved = tail->prev;   // tail->prev = sabse purana (tail khud dummy)
                     removeNode(toBeRemoved);
                     mp.erase(toBeRemoved->key);   // KEY se (->val NAHI: wo key hi nahi,
                 // kuch nahi hatta; test me key != val, 1,100)
                 Node *newNode = new Node(key, value); addFront(newNode); mp[key] = newNode;
```

---

---

## PATTERN 10 — KADANE (running value + max update)

```

 BROAD IDEA: ek value AAGE carry karo (min/sum), har step MAX update. single pass, O(1) space.
 ★ EDGE (circular): ans = max(normal kadane, total - minSubarray). SAB NEGATIVE -> total-min = 0 aata,
         tab normal kadane hi sahi (alag se dekho).
 ★ MILTE-JULTE (max sum vs count sum == K): upar "NAAM-SE-CONFUSE" -> C dekh.

 ▸ BUY/SELL STOCK (LC-121)
     // aaj becho = aaj ka daam MINUS ab tak ka sasta
     int ans = INT_MIN; int minSoFar = INT_MAX;   // INT_MAX: pehla din hi sasta ban jaaye
     for (int i = 0; i < prices.size(); i++)
         minSoFar = min(minSoFar, prices[i]);    // min PEHLE: diff minus nahi, sell buy ke BAAD
         ans = max(ans, prices[i] - minSoFar);
     return ans < 0 ? 0 : ans;
     // brute = saare pair (i<j), O(n^2). REDUNDANCY = har sell-din pe peeche ka sasta baar-baar
     // scan -> us min ko VARIABLE me CACHE -> O(n) (redundancy -> cache = running min)

 ▸ MAX SUBARRAY SUM (LC-53)
     // running sum NEGATIVE = bojh -> sum = 0
     int maxSum = INT_MIN;                       // INT_MIN: all-negative [-1] bhi pakde
     int sum = 0;                                // 0 se: abhi tak kuch joda nahi
     for (auto &it : nums)
         sum += it; maxSum = max(maxSum, sum);   // max PEHLE, reset BAAD: warna all-negative [-1] pe 0 aata
         if (sum < 0) sum = 0;
     return maxSum;

── FAMILY: flip ho to MIN bhi + 3 candidate + FREEZE (temp) · 53 sum · 152 product · 1749 abs ──

 ▸ MAX PRODUCT SUBARRAY (LC-152)
     // neg*neg = BADA -> MIN bhi track (chhota kal sabse bada)
     int mini = 1;                               // SHURUAAT: mini = maxi = 1, ans = INT_MIN, loop 0 se
     int maxi = 1;                               // (loop 1 se ho to ans bhi nums[0] se; warna [4,-3] pe 0)
     int ans = INT_MIN;                          // INT_MIN: 0 se [-2] pe 0 aata, sahi -2
     for (int i = 0; i < nums.size(); i++)
         int temp = max({nums[i], mini * nums[i], maxi * nums[i]});   // 3 candidate, temp me:
         mini = min({nums[i], mini * nums[i], maxi * nums[i]});   // FREEZE: min purane max se
         maxi = temp; ans = max(ans, maxi);      // max min ke BAAD
     return ans;

 ▸ MAX ABSOLUTE SUM (LC-1749)
     // = MAX PRODUCT skeleton, 2 tweak: * -> +, ans me ABS
     int n = nums.size();
     if (n == 1) return abs(nums[0]);            // n==1 pe abs ([-7] -> 7)
     int mini = 0;                               // SHURUAAT: 0, 0, 0, loop 0 se: + me 0 kuch nahi jodta, abs
     int maxi = 0; int ans = 0;                  // kabhi minus nahi (loop 1 se tha to [9,-6] pe 6, sahi 9)
     for (int i = 0; i < n; i++)
         int temp = max({nums[i], mini + nums[i], maxi + nums[i]});
         mini = min({nums[i], mini + nums[i], maxi + nums[i]}); maxi = temp;   // temp FREEZE, product jaisa
         ans = max(ans, max(abs(mini), abs(maxi)));   // answer minSum se bhi aa sakta -> abs dono
     return ans;                                 // [2,-5,1,-4,3,-2] -> [-5,1,-4] = -8 -> 8

── (STANDALONE — apni alag trick) ──

 ▸ MAX SUM CIRCULAR SUBARRAY (LC-918)  = TRICK: total − MIN-subarray
     // circular (end -> start wrap), max-sum NON-EMPTY subarray. TRICK: PATA hona chahiye
     // A) no-wrap = normal Kadane  B) wrap = poora array MINUS beech ka chunk = TOTAL - min
     int maxKadaneK(vector<int> &nums)
         int sum = nums[0]; int maxSum = nums[0];   // init sum = maxSum = nums[0], loop 1 se (ya 0 + INT_MIN)
         for (int i = 1; i < nums.size(); i++)
             sum = max(sum + nums[i], nums[i]); maxSum = max(maxSum, sum);
         return maxSum;

     int minKadaneK(vector<int> &nums)           // Kadane ka MIN version (prefix-sum se NAHI)
         int sum = nums[0]; int minSum = nums[0];   // init nums[0], loop 1 se (ya 0 + INT_MAX)
         for (int i = 1; i < nums.size(); i++)
             sum = min(sum + nums[i], nums[i]); minSum = min(minSum, sum);
         return minSum;

     int maxSubarraySumCircular(vector<int> &nums)
         int totalSum = 0;
         for (int i = 0; i < nums.size(); i++) totalSum += nums[i];
         int maxSum = maxKadaneK(nums); int minSum = minKadaneK(nums);
         int circularSum = totalSum - minSum;    // wrap = poora - sabse bura (min) beech ka chunk
         if (maxSum > 0)
             return max(circularSum, maxSum);    // normal: dono me bada. [5,-3,5]: 7, 7-(-3)=10 -> 10
         return maxSum;                          // sab non-positive: total-min = 0 = EMPTY, galat
     // house robber II jaisa "circular twist", par mechanism alag (HR-II = pehla / aakhri chhodo)

 ▸ 2 MAX-SUM NON-OVERLAPPING SUBARRAYS (Google)  = bestLeft + bestRight + WALL
     // har WALL i pe: [0..i] ka best + [i+1..n-1] ka best -> sabka MAX
     // = KADANE (per index) + PREFIX / SUFFIX max
     // [2,1,-99,3,3] = 9: wall index 1 -> left [2,1] = 3 + right [3,3] = 6
     vector<int> maxEndingAtForLeft(vector<int> &nums)   // STEP 1: kadane per index (aage)
         int n = nums.size(); vector<int> ans(n, 0);
         ans[0] = nums[0];                       // index 0 pe khatam = sirf nums[0], isliye loop 1 se
         for (int i = 1; i < n; i++)
             ans[i] = max(ans[i - 1] + nums[i], nums[i]);   // pichhle me judo ya akela naya shuru, jo bada
         return ans;

     vector<int> bestLeftPrefix(vector<int> &nums)   // STEP 2: STEP 1 ka running max (aage)
         int n = nums.size(); vector<int> temp = maxEndingAtForLeft(nums); vector<int> ans(n, 0);
         ans[0] = temp[0];                       // 0 tak ka best = khud temp[0]
         for (int i = 1; i < n; i++)
             ans[i] = max(ans[i - 1], temp[i]);   // 0..i ka best = pichhla best ya temp[i]
         return ans;

     vector<int> maxEndingAtForRight(vector<int> &nums)   // STEP 3: STEP 1 ka mirror (peeche)
         int n = nums.size(); vector<int> ans(n, 0);
         ans[n - 1] = nums[n - 1];               // last pe khatam = sirf wahi; right part peeche se banta
         for (int i = n - 2; i >= 0; i--)
             ans[i] = max(ans[i + 1] + nums[i], nums[i]);   // agle (i+1) me judo ya akela shuru
         return ans;

     vector<int> bestRightPrefix(vector<int> &nums)   // STEP 4: STEP 3 ka running max (peeche)
         int n = nums.size(); vector<int> temp = maxEndingAtForRight(nums); vector<int> ans(n, 0);
         ans[n - 1] = temp[n - 1];
         for (int i = n - 2; i >= 0; i--)        // peeche bharo: ans[i+1] pehle se ready chahiye
             ans[i] = max(ans[i + 1], temp[i]);   // i..n-1 ka best = agla best ya temp[i]
         return ans;

     int maxTwoNonOverlap(vector<int> &nums)     // STEP 5: WALL
         int n = nums.size(); vector<int> bestForleft = bestLeftPrefix(nums);
         vector<int> bestForRight = bestRightPrefix(nums);
         int ans = INT_MIN;                      // INT_MIN: sab minus ho to jod bhi minus, 0 se galat
         for (int i = 0; i < n - 1; i++)         // n-2 tak: i+1 bahar na jaaye; aakhri split (right = akela
             ans = max(ans, bestForleft[i] + bestForRight[i + 1]);   // last) bhi aaye: [-5,10,10] -> 20
         return ans;                             // i+1 = NON-OVERLAP: bestRight[i] liya to index i dono me =
     // do baar gina ([..,8,..] + [14..] = 22 jhootha)
     // [1,2,-7,8,6,-4]: maxEndAt [1,3,-4,8,14,10] · bestLeft [1,3,3,8,14,14]
     // bestRight [14,14,14,14,6,-4] · i=1: 3 + 14 = 17
```

---

---

## PATTERN 11 — BIT MANIPULATION (XOR)

```

 BROAD IDEA: XOR: a^a=0 (pair cancel), a^0=a (single bacha). pairs khud gayab, extra memory nahi.
 ★ MILTE-JULTE (sorted / 1..n / dohra): upar "NAAM-SE-CONFUSE" -> D dekh, XOR lagane se PEHLE.

── FAMILY: XOR-cancel (pair khud kat, akela / missing bacha = ans; Missing-Number, Single-Number-II bhi) ──

 ▸ SINGLE NUMBER (LC-136)
     // sab 2 baar, ek 1 baar -> sab XOR = wahi ek
     int XORR = nums[0];                         // nums[0] se shuru, isliye loop 1 se
     for (int i = 1; i < nums.size(); i++) XORR ^= nums[i];   // a^a = 0 pair kat jaata, a^0 = a
     return XORR;                                // O(n), O(1), koi map nahi
```

---

---

## PATTERN 12 — TREES (recursion)

```

 BROAD IDEA: har tree-Q = RECURSION on TreeNode(val, left, right).
             3 cheez -> BASE (null pe return) · dono taraf RECURSE · COMBINE.
 ★ EDGE (max path sum): left = max(0, left) · right = max(0, right) -> "0 = wo arm li hi nahi".
         2 ke neeche -1: arm lo to 1, chhodo to 2 -> negative arm LENA nuksaan.

 ORDER: kaam recursion-RESULT pe depend kare -> recurse-PEHLE (post-order); independent -> free.

 CORE (har tree-Q bas yehi 4 line; sirf BASE + COMBINE badalte):
     solve(node) {
         if (node == NULL) return BASE;      // (1) base -> null pe ruko
         left  = solve(node->left);          // (2) node->left = NEECHE bhejo (call)
         right = solve(node->right);         // (3) left / right = jo VALUE wapas aayi (child ka jawab)
         return COMBINE(left, right);        // (4) apna jawab banao -> parent ko do (jawab neeche se upar)
     }
     // Q1 root->left (asli bachcha: recurse · LEAF check · swap) vs left (return value: COMBINE me)
     // Q1b return type: bool -> seedha call ||/&& (variable nahi) · node -> LCA
     //     · int dobara chahiye -> variable
     // Q2 COMBINE: gehra/bada -> max · dono side match -> && · koi ek path -> || · node dhundo -> node return
     //    special condition kuch Q me: path-sum -> LEAF pe check · LCA -> root == p || q
     // Q3 answer seedha bane -> RETURN · parent ko height chahiye -> height RETURN + answer GLOBAL (diameter)
     // Q4 subtree se bane (height / sum) -> UPAR return · target path pe -> value NEECHE carry (path-sum)
     // DEBUG: poora trace mat karo. (1) maano left/right mil gaya, bas "is node pe kya" socho
     //   (2) base + 2-3 node ka chhota tree haath se check (3) print: enter-print null-check ke BAAD
     //   (warna null->val crash), result variable me lo -> print + return dono

 ▸ 3 DFS TRAVERSALS (LC-144 / 94 / 145)
     // preorder: PRE = VISIT -> left -> right (root pehle)
     if (!root) return;
     res.push_back(root->val);               // VISIT
     preorder(root->left, res), preorder(root->right, res);

     // inorder: IN = left -> VISIT -> right (beech)
     if (!root) return;
     inorder(root->left, res);
     res.push_back(root->val);               // VISIT
     inorder(root->right, res);

     // postorder: POST = left -> right -> VISIT (baad)
     if (!root) return;
     postorder(root->left, res), postorder(root->right, res);
     res.push_back(root->val);               // VISIT
     // teeno SAME recursion, sirf VISIT ki JAGAH badalti

── FAMILY: recurse + COMBINE (answer = return-value; jo combine karo wahi return) ──

 ▸ MAX DEPTH (LC-104)
     if (!root) return 0;   // base: null -> 0
     int left = maxDepth(root->left); int right = maxDepth(root->right);
     return 1 + max(left, right);            // khud = 1 + gehri branch

 ▸ INVERT TREE (LC-226)
     if (!root) return nullptr;   // base: null -> null
     invertTree(root->left);                 // recurse dono (order FREE)
     invertTree(root->right);
     swap(root->left, root->right);          // TRAP: POINTERS swap (poori branch badalti), values NAHI
     return root;                            //   value-swap = sirf 2 number + leaf pe null->val CRASH

 ▸ SAME TREE (LC-100)
     if (p == NULL && q == NULL) return true;   // dono null -> true
     if (p == NULL && q != NULL)             // null checks PEHLE: warna null->val crash
         return false;                       // ek null -> false
     if (p != NULL && q == NULL) return false;   //   (clean base: if (!p || !q) return p == q;)
     if (p->val != q->val) return false;   // val alag -> yahin false, recurse nahi
     else if (p->val == q->val)
         return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);   // && : DONO side match
     return false;

 ▸ SYMMETRIC TREE (LC-101)  = SAME-TREE + CROSS
     // isSameTree: same tree ka hi base (dono null / ek null / val alag)
     if (p == NULL && q == NULL) return true;
     if (p == NULL && q != NULL) return false;   // null checks PEHLE: warna null->val crash
     if (p != NULL && q == NULL) return false;
     if (p->val != q->val) return false;
     else if (p->val == q->val)
         return isSameTree(p->left, q->right) && isSameTree(p->right, q->left);   // CROSS = mirror
     return false;

     // isSymmetric
     if (!root) return true;
     return isSameTree(root->left, root->right);   // root khud nahi, uske dono bachche aapas me mirror

── FAMILY: return-ONE + track-ANOTHER (parent ko HEIGHT return, asli ANSWER global / ref me; post-order) ──

 ▸ DIAMETER (LC-543)  = MAX-DEPTH + 1 line
     // height: max-depth ka height(), maxDia by-REFERENCE
     if (!root) return 0;   // null ki height 0: uske neeche koi edge nahi
     int left = height(root->left, maxDia);  // recurse PEHLE: kaam peeche aate hue (post-order)
     int right = height(root->right, maxDia);
     maxDia = max(maxDia, left + right);     // EXTRA line: is node se guzarti diameter = leftH + rightH
                                             //   MAX (overwrite NAHI: test pass != code sahi)
     return 1 + max(left, right);            // HEIGHT return (parent ko yahi chahiye)

     // diameterOfBinaryTree
     int maxDia = 0;                         // 0 se theek: diameter edge-ginti hai, kabhi minus nahi
     height(root, maxDia);
     return maxDia;                          // diameter = BYPRODUCT -> global maxDia (return nahi)

 ▸ MAX PATH SUM (LC-124, Hard)  = DIAMETER ka SUM-version
     // skeleton HUBAHU diameter (return EK arm / record DONO arm / post-order / global max). SIRF 3 badle:
     //   (1) arm = height -> GAIN (sum): return me 1 ki jagah root->val
     //   (2) record me + root->val: junction pe node bhi ginta
     //   (3) max(0, ..): minus arm SKIP (diameter me sab positive the)
     // maxDepth
     if (!root) return 0;   // base: null -> 0
     int left = max(0, maxDepth(root->left, maxSum));     // (3) minus arm -> 0 (lene se sum ghatta)
     int right = max(0, maxDepth(root->right, maxSum));
     maxSum = max(maxSum, root->val + left + right);      // (2) answer: junction pe DONO sadak + node
     return root->val + max(left, right);    // (1) parent ko: node + EK sadak (upar ek hi ja sakti)

     // maxPathSum
     int maxSum = INT_MIN;                   // INT_MIN (0 NAHI): akela -3 bhi valid path
     maxDepth(root, maxSum);
     return maxSum;                          // [-10,9,20,_,_,15,7]: @20 -> 15+20+7 = 42 (answer), upar 35

 ▸ BUILD TREE from PREORDER + INORDER (LC-105)
     // preorder[rootIndex] = ROOT · inorder me us root pe todo -> LEFT-part | root | RIGHT-part, recurse
     TreeNode *solve(int &rootIndex, vector<int> &preorder, vector<int> &inorder, int l, int r,
                     unordered_map<int, int> &mp)     // &rootIndex: ek pointer SAB calls me shared (neeche)
         if (l > r) return NULL;   // > (>= nahi): l==r pe ek node bacha, wo banana hai
         int pivot = mp[preorder[rootIndex]];    // inorder me root kahan (map = O(1))
         rootIndex++;                            // ++ turant: agla preorder element = agla root
         TreeNode *newNode = new TreeNode(inorder[pivot]);
         newNode->left = solve(rootIndex, preorder, inorder, l, pivot - 1, mp);    // LEFT pehle: preorder
         newNode->right = solve(rootIndex, preorder, inorder, pivot + 1, r, mp);   //   = Root, Left, Right
         return newNode;

     int rootIndex = 0;                      // 0: preorder ka pehla = root
     unordered_map<int, int> mp;
     for (int i = 0; i < inorder.size(); i++) mp[inorder[i]] = i;   // value -> inorder index (ek baar)
     return solve(rootIndex, preorder, inorder, 0, inorder.size() - 1, mp);   // 0..n-1 = poora inorder
     // rootIndex by-REFERENCE (&): LEFT poora consume hone ke BAAD hi RIGHT ko sahi agla root milta.
     //   value-pass -> copy -> galat tree. pre [3,9,20,15,7] in [9,3,15,20,7] -> 3 (9) (20 (15) (7))

 ▸ BALANCED TREE (LC-110)  = MAX-DEPTH + (-1 SENTINEL)
     // HAR node pe abs(leftH - rightH) <= 1? alag "balanced?" pass nahi -> height nikaalte-nikaalte HI check
     // maxDepth: return = HEIGHT, ya -1 (neeche kahin imbalance)
     if (!root) return 0;   // null ki height 0 (balanced hai, -1 nahi)
     int left = maxDepth(root->left);
     if (left == -1) return -1;   // left me imbalance -> -1 UPAR bhejo, aage mat dekho
     int right = maxDepth(root->right);
     if (right == -1) return -1;   // right me imbalance -> turant -1 upar
     if (abs(left - right) > 1)
         return -1;                          // IS node pe imbalance (har node pe check, sirf root nahi:
                                             //   root balanced par andar imbalanced ho sakta)
     int result = 1 + max(left, right);      // balanced -> asli HEIGHT
     return result;

     // isBalanced
     if (!root) return true;   // redundant (maxDepth(null)=0 != -1), saaf padhne ko
     return maxDepth(root) != -1 ? true : false;   // -1 nahi aaya = kahin imbalance nahi. O(n) ek pass

── FAMILY: search + BUBBLE-UP (jise dhundh rahe wo mila -> UPAR return; DONO taraf se mila = answer) ──

 ▸ LCA -- Lowest Common Ancestor (LC-236)
     // p, q ALAG side split
     if (!root) return NULL;   // khaali subtree -> p/q yahan nahi
     if (root == p || root == q) return root;   // mil gaya -> UPAR bhejo
     TreeNode *left = lowestCommonAncestor(root->left, p, q);
     TreeNode *right = lowestCommonAncestor(root->right, p, q);
     if (left == NULL)                       // ek side khali -> doosri return. DO case ek saath:
         return right;                       //   left khali + right mila · dono khali -> NULL (sahi)
                                             //   "&& right != NULL" jodo to dono-khali pe root = GALAT
     else if (right == NULL) return left;   // right khali -> dono left me, left wala hi jawab
     else if (left == p && right == q) return root;
     else if (left == q && right == p) return root;
     return root;                            // DONO non-null -> yahi split point = LCA

── FAMILY: carry-value DOWN + root-to-LEAF (target ghatao, LEAF pe check, combine = OR) ──

 ▸ PATH SUM (LC-112)
     // root se LEAF tak sum == target?
     if (!root) return false;   // base: null -> false
     if (!root->left && !root->right)        // LEAF = ACTUAL bachche (root->left), recursion-result nahi
         return root->val == targetSum;      // leaf pe hi check: beech me mile to path adhoora
     return hasPathSum(root->left, targetSum - root->val) ||    // value NEECHE carry (target ghatao)
            hasPathSum(root->right, targetSum - root->val);     // || : koi ek path bana de to kaafi

── FAMILY: BFS / LEVEL-ORDER (QUEUE FIFO, recursion nahi; while ka ek chakkar = ek LEVEL) ──

 ▸ LEVEL ORDER (LC-102)
     // har level ki alag list. FIFO -> L-to-R apne aap
     vector<vector<int>> ans;
     if (!root) return ans;   // warna NULL push hota, phir node->val crash
     queue<TreeNode *> q;
     q.push(root);                           // level-0 (root) se BFS shuru
     while (!q.empty())
         int sz = q.size();                  // SNAPSHOT for se PEHLE: loop me push se size badalta,
         vector<int> temp;                   //   warna agle level ke node is level me ghus jaate
         for (int i = 0; i < sz; i++)        // sirf sz baar: isi level ke node
             TreeNode *curr = q.front(); q.pop(); temp.push_back(curr->val);
             if (curr->left != NULL) q.push(curr->left);   // bachche -> AGLE level me
             if (curr->right != NULL) q.push(curr->right);
         ans.push_back(temp);                // level poora tab push (for ke bahar)
     return ans;
     // TEMPLATE: right view · level average · zigzag · min depth -> isi skeleton pe, bas "level me kya"

 ▸ RIGHT SIDE VIEW (LC-199)  = LEVEL-ORDER + 1 line
     // daaye se har level ka SIRF right-most dikhta
     if (!root) return {};
     queue<TreeNode *> q; q.push(root);
     vector<int> ans;
     while (!q.empty())
         int sz = q.size();                  // snapshot (level-order jaisa)
         for (int i = 0; i < sz; i++)
             TreeNode *curr = q.front(); q.pop();
             if (i == sz - 1)                // level ka LAST: FIFO me L-to-R -> aakhri = sabse RIGHT
                 ans.push_back(curr->val);   //   (1-based "position sz" = 0-based i == sz-1)
                                             //   left side view: i == 0
             if (curr->left != NULL) q.push(curr->left);
             if (curr->right != NULL) q.push(curr->right);
     return ans;

 ▸ ZIGZAG LEVEL ORDER (LC-103)  = LEVEL-ORDER + reverse
     // 0: L->R · 1: R->L · 2: L->R ...
     vector<vector<int>> ans;
     if (!root) return ans;
     queue<TreeNode *> q; q.push(root);
     int levelCounter = 0;                   // 0 se: root level seedha
     while (!q.empty())
         int sz = q.size();
         vector<int> temp;                   // level HAMESHA normal L->R banao
         for (int i = 0; i < sz; i++)
             TreeNode *curr = q.front(); q.pop(); temp.push_back(curr->val);
             if (curr->left != NULL) q.push(curr->left);
             if (curr->right != NULL) q.push(curr->right);
         if (levelCounter % 2 != 0)          // DELTA (inner loop ke BAAD): ODD level -> ulta
             reverse(begin(temp), end(temp));    // (alt: deque / front insert, reverse simplest)
         levelCounter++;                     // reverse check ke BAAD ++: warna root level ulta
         ans.push_back(temp);
     return ans;

 ▸ MINIMUM DEPTH (LC-111)   = LEVEL-ORDER + EARLY-STOP
     // root se sabse PAAS wale LEAF tak
     if (!root) return 0;   // khaali tree -> 0 (warna NULL push)
     queue<TreeNode *> q; q.push(root);
     int depth = 0;                          // ab tak POORE hue level (current = depth+1)
     while (!q.empty())
         int sz = q.size();                  // for se PEHLE pakdo
         for (int i = 0; i < sz; i++)
             TreeNode *curr = q.front(); q.pop();
             if (!curr->left && !curr->right)
                 return depth + 1;           // PEHLA leaf = sabse paas -> turant return
             if (curr->left != NULL) q.push(curr->left);
             if (curr->right != NULL) q.push(curr->right);
         depth++;                            // level POORA -> depth++ (for ke BAHAR)
     return -1;                              // yahan pahunchte nahi (root hai to leaf milega)
     // DFS TRAP (isliye BFS): "1 + min(leftD, rightD)" GALAT -> NULL child ko depth 0 maan ke
     //   skew tree (2->3->4) pe galat deta

── FAMILY: BST (left < node < right; INORDER = SORTED -> "sorted / kth chahiye? -> INORDER") ──

 ▸ KTH SMALLEST IN BST (LC-230)
     // inorder: BST pe inorder = SORTED (ascending)
     if (!root) return;
     inorder(root->left, res);               // pehle poora LEFT (chhote)
     res.push_back(root->val);               // phir NODE
     inorder(root->right, res);              // phir RIGHT (bade)

     // kthSmallest
     vector<int> ans;
     inorder(root, ans);                     // saari values SORTED
     return ans[k - 1];                      // k-1 TRAP: k 1-based, vector 0-based
     // optimize: counter, k-va node pe EARLY-STOP

 ▸ VALIDATE BST (LC-98)  = RANGE carry-DOWN
     // TRAP: sirf turant child check KAAFI NAHI -> node ko SAARE ancestors ka rule maanna
     //   -> har node ki WINDOW (mini, maxi), neeche jaate tight hoti
     // solve
     if (root == NULL) return true;   // khaali subtree valid
     if (root->val <= mini || root->val >= maxi)
         return false;                       // <= aur >= : barabar bhi galat (BST me duplicate nahi)
     bool left = solve(root->left, mini, root->val);    // LEFT jao -> maxi = val (chhota hona)
     bool right = solve(root->right, root->val, maxi);  // RIGHT jao -> mini = val (bada hona)
     return left && right;                   // && : dono subtree valid tabhi valid

     // isValidBST
     return solve(root, LLONG_MIN, LLONG_MAX);   // long long: INT_MIN / INT_MAX se shuru kiya to node
     //   khud INT_MAX ho ([2147483647]) -> strict = galat false
     // alt: inorder strictly increasing? -> BST (kth smallest ka near-transfer)

```

---

---

## PATTERN 13 — GRAPHS (BFS/DFS + visited)

```

 BROAD IDEA: graph = nodes + edges. TREE bhi ek graph hai (special) -> Graph = Tree + 2 cheez:
   (1) CYCLE ho sakti -> VISITED chahiye (warna A->B->C->A infinite loop)
   (2) pointers ki jagah ADJACENCY LIST.
 ★ EDGE (BFS grid / word ladder): visited ANDAR DAALTE waqt mark karo, nikaalte waqt nahi
         (warna ek node kai baar queue me).

 TREE se transfer: BFS = queue (level-order jaisa) · DFS = recursion (left/right ki jagah adj[node]).
   bas 'visited' add.

 INPUT-FORM pehchano (pehle ye -- kab adj-list banani, kab NAHI):
      unordered_map<int, vector<int>> adj;    // ya  vector<int> adj[n];
      edges[] + undirected -> adj[u]+=v; adj[v]+=u;  DONO taraf      (path-exists, CC, dijkstra{v,w})
      edges[] + directed   -> sirf adj[u]+=v; + indegree[v]++        (course-schedule/topo)
      GRID (2D) diya       -> adj-list NAHI -> padosi = 4-dir `dirs`, cell khud node
                              (islands, rotting, max-area)
      graph[]/node.neighbors already -> build SKIP, seedha traverse  (bipartite, clone-graph)

 DECISION-GUIDE (koi graph-Q se pehle):
   Q1  OUTER-LOOP chahiye? (SABSE bada -- yehi trick)
        kitne group / poora graph / disconnected -> HAAN:
             for (i : all nodes)  if (!vis[i]) { count++; BFS(i); }   // sirf unvisited pe: naya group
                                                                      //   (CC, Islands)
        ek source -> target / ek hi component -> NAHI: sirf source se EK traversal   (Path-Exists)

   Q2  DFS ya BFS? (connectivity ke liye dono same)
        BFS = shortest-path / level (unweighted) · DFS = recursion, code chhota
        BFS (queue):                          DFS (recursion):
            vis[start] = true;                    void dfs(int node) {
            q.push(start);                            vis[node] = true;
            while (!q.empty()) {                      for (int nbr : adj[node])
                int node = q.front();                     if (!vis[nbr]) dfs(nbr);
                q.pop();                              }
                for (int nbr : adj[node])
                    if (!vis[nbr]) {
                        vis[nbr] = true;   // PUSH pe mark: warna ek node queue me do baar
                        q.push(nbr);
                    }
            }
        GRID pe: adj[node] ki jagah 4-direction (Q3 dirs); visited = cell ko SINK ('0'/'2').

   Q2b BFS ke ANDAR: PLAIN ya LEVEL-ORDER? (rotting me chhoota tha)
        "reach / connected / count / islands"   -> PLAIN BFS (level-count nahi)
        "kitne STEPS / MINUTES / shortest-dist"  -> LEVEL-ORDER, ek queue-size snapshot = ek level = ek step:
             while (!q.empty()) { int sz = q.size(); while (sz--) { ... push nbrs } steps++; }
             // sz pehle pakdo; steps++ level ke baad, andar nahi   (rotting, min-steps-in-grid, word-ladder)

   Q3  neighbours kaise? edges -> adj[node] · grid -> 2 TRICK (ratta, apne aap nahi aate):
          (a) vector<vector<int>> dirs = {{1,0},{-1,0},{0,1},{0,-1}};   // neeche, upar, daaye, baaye
              for (auto &d : dirs) { int nr = r + d[0], nc = c + d[1]; ... }
          (b) if (nr >= 0 && nr < m && nc >= 0 && nc < n && grid[nr][nc] == ?)   // bounds PEHLE, grid baad:
              //   bahar index pe && pehle hi ruk jaata. HAR grid-Q me; '==?' per-Q (islands/rotting == '1')
   visited HAMESHA (cycle rok) -> push/enter ke waqt mark.

── FAMILY: single-source traversal (ek source se ek BFS, target mila? koi OUTER-loop nahi) ──

 ▸ PATH EXISTS (LC-1971)  = single BFS (reachability)
     // = tree LEVEL-ORDER ka skeleton + 3 ADD: (1) neighbours = adj[node] (edges se banao)
     //   (2) vis[] (tree me cycle nahi, graph me hai)  (3) level-loop NAHI: sirf reachability
     unordered_map<int, vector<int>> adj;
     for (auto &it : edges)
         int u = it[0]; int v = it[1]; adj[u].push_back(v);
         adj[v].push_back(u);                // undirected -> dono taraf
     queue<int> q; vector<bool> vis(n, false); q.push(source);
     vis[source] = true;                     // src bhi mark: warna cycle se ghoom ke src dobara push
     while (!q.empty())
         int node = q.front(); q.pop();
         if (node == destination) return true;   // pop pe check: src == dst wala case bhi pakda jaata
         for (auto &it : adj[node])
             if (!vis[it])
                 vis[it] = true;             // push ke saath hi mark: warna ek node queue me do baar
                 q.push(it);
     return false;                           // queue khaali -> koi path nahi (alag group)

── FAMILY: COUNT components (outer-loop + FLOOD each; Islands = grid, CC = adj-list, bas neighbour badla) ──

 ▸ NUMBER OF ISLANDS (LC-200)  = Connected-Components on a GRID
     // DFS
     if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == '0')   // BASE ek hi if, sab || se
         return;                             // bounds PEHLE, '0' baad: bahar index pe grid padhne se
                                             //   pehle ruk jaata. '0' base me: warna poora grid kha jaata
     grid[i][j] = '0';                       // SINK = visited, dobara na gino
     DFS(grid, i + 1, j, m, n);              // 4-dir: neeche
     DFS(grid, i - 1, j, m, n);              // upar
     DFS(grid, i, j + 1, m, n);              // right
     DFS(grid, i, j - 1, m, n);              // left

     // numIslands
     int m = grid.size(); int n = grid[0].size(); int count = 0;
     for (int i = 0; i < m; i++)             // outer: har cell scan
         for (int j = 0; j < n; j++)
             if (grid[i][j] == '1')          // har naya '1' = naya island
                 DFS(grid, i, j, m, n);      // poora island dubo do
                 count++;
     return count;

 ▸ CONNECTED COMPONENTS (LC-323)  = ISLANDS par ADJ-LIST graph (grid nahi) + BFS
     // ISLANDS ka SAME skeleton (outer-loop + count), 2 farak: graph = ADJ-LIST, aur BFS (islands DFS)
     unordered_map<int, vector<int>> adj;
     for (auto &it : edges)
         int u = it[0]; int v = it[1]; adj[u].push_back(v);
         adj[v].push_back(u);                // undirected -> dono taraf
     queue<int> q;
     vector<bool> vis(n, false);             // sab unvisited
     int count = 0;                          // abhi koi group nahi gina
     for (int i = 0; i < n; i++)             // outer-loop: DISCONNECTED groups bhi cover
         if (!vis[i])                        // count++ AUR BFS dono iske ANDAR: warna har node gine (= n)
             q.push(i);
             vis[i] = true;                  // start bhi push ke saath mark
             count++;                        // har UNVISITED node = naya component
             while (!q.empty())              // = PATH-EXISTS ka body
                 int node = q.front(); q.pop();
                 for (auto &it : adj[node])     // neighbours = adj[node] (islands me 4-dir) = yahi farak
                     if (!vis[it])
                         vis[it] = true, q.push(it);
     return count;

 ▸ MAX AREA OF ISLAND (LC-695)  = Islands, par MAX AREA (count nahi)
     // ISLANDS ka SAME skeleton, 2 DELTA: (1) DFS void nahi, AREA return  (2) outer count++ nahi, MAX
     // DFS
     if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0)
         return 0;                           // bounds / paani -> 0 (bounds PEHLE)
     grid[i][j] = 0;                         // sink (visited)
     int neeche = DFS(grid, i + 1, j, m, n); int upar = DFS(grid, i - 1, j, m, n);
     int right = DFS(grid, i, j + 1, m, n); int left = DFS(grid, i, j - 1, m, n);
     return 1 + left + right + upar + neeche;   // 1 = ye cell khud + chaaron taraf ka area

     // maxAreaOfIsland
     int m = grid.size(); int n = grid[0].size();
     int ans = 0;                            // 0 se: area >= 0, koi island nahi -> 0
     for (int i = 0; i < m; i++)
         for (int j = 0; j < n; j++)
             if (grid[i][j] == 1)
                 ans = max(ans, DFS(grid, i, j, m, n));   // har island ka area, sabse bada rakho
     return ans;

── FAMILY: MULTI-SOURCE / level-BFS (saare sources EK saath queue; sz-snapshot = ek level = ek minute) ──

 ▸ ROTTING ORANGES (LC-994)  = grid MULTI-SOURCE BFS + levels
     // tree LEVEL-ORDER grid pe; har level = 1 minute
     queue<pair<int, int>> q; int m = grid.size(); int n = grid[0].size();
     for (int i = 0; i < m; i++)
         for (int j = 0; j < n; j++)
             if (grid[i][j] == 2) q.push({i, j});   // MULTI-SOURCE = level-0 (saare rotten PEHLE)
     vector<vector<int>> dirs = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};   // 4-dir
     int mins = 0;
     while (!q.empty())
         int sz = q.size();                  // while se PEHLE pakdo: andar push se size badalta
         while (sz--)                        // sz baar = sirf is minute wale rotten
             auto &it = q.front(); q.pop(); int row = it.first; int col = it.second;
             for (auto &it : dirs)
                 int nR = it[0] + row; int nC = it[1] + col;
                 if (nR >= 0 && nR < m && nC >= 0 && nC < n && grid[nR][nC] == 1)     // bounds PEHLE
                     grid[nR][nC] = 2;       // push ke saath hi 2: warna doosra rotten bhi push karta
                     q.push({nR, nC});
         mins++;                             // poora level = 1 minute (while(sz--) ke BAHAR)
     for (int i = 0; i < m; i++)
         for (int j = 0; j < n; j++)
             if (grid[i][j] == 1) return -1;   // fresh bacha -> impossible
     return mins == 0 ? mins : mins - 1;     // last level extra -> -1. EDGE: koi santra hi nahi ->
     //   mins = 0 -> mins-1 = -1 galat, sahi 0

── FAMILY: TOPO-SORT / cycle-detect (DIRECTED; prereq + can-finish -> Kahn's = BFS + indegree) ──

 ▸ COURSE SCHEDULE (LC-207)  = TOPO-SORT (Kahn's), cycle-detect
     // "prereq + can-finish-ALL?" = koi CYCLE to nahi? cycle -> false
     // [[1,0],[2,1],[3,2]]: har [a,b] = a lene se pehle b -> edge b -> a
     unordered_map<int, vector<int>> adj;
     vector<int> indegree(numCourses, 0);    // indegree = kitne prereq BACHE; 0 = ready
     for (auto &it : prerequisites)
         int u = it[1];
         int v = it[0];                      // edge u -> v (pre -> course)
         adj[u].push_back(v);
         indegree[v]++;                      // v ka ek prereq aur
     queue<int> q;
     vector<int> ans;
     for (int i = 0; i < numCourses; i++)    // har course check: kai ek saath start ho sakte
         if (indegree[i] == 0) q.push(i);   // koi prereq nahi -> queue
     while (!q.empty())
         int node = q.front(); q.pop(); ans.push_back(node);
         for (auto &it : adj[node])
             indegree[it]--;                 // ek prereq pura
             if (indegree[it] == 0) q.push(it);   // sab pure -> ready
     return ans.size() == numCourses;        // cycle wale ka indegree KABHI 0 nahi -> queue me aate hi
     //   nahi -> size < n -> false

 ▸ BUILD ORDER — ready me se SABSE CHHOTA pehle (topo + MIN-HEAP)
     // PEHCHAAN: dependency -> TOPO. output line padho: "koi bhi valid order" -> queue · "sabse chhota /
     //   lexicographic" -> MIN-HEAP. topo ke KAI sahi order ([5->2,5->0,4->0,4->1,2->3,3->1]: "4 5 0 2 3 1"
     //   aur "5 4 2 0 3 1" dono sahi) -> khaas order = heap. queue = jo pehle ready · heap = ready me chhota
     // BADLAAV bas itna: queue -> min-heap, front() -> top(). indegree / adj / ghatana / end-check SAME
     vector<int> indegree(n, 0); unordered_map<int, vector<int>> adj;
     for (auto &it : deps)
         int u = it[0]; int v = it[1]; adj[u].push_back(v); indegree[v]++;
     priority_queue<int, vector<int>, greater<int>> pq;   // min-heap: sabse chhota top
     for (int i = 0; i < n; i++)             // (id string ho -> heap <string>)
         if (indegree[i] == 0) pq.push(i);
     vector<int> ans;                        // KHAALI se: ans(n,0) + push_back = size 2n, galat
     while (!pq.empty())
         int node = pq.top();                // n=4, [0->1]: heap {0,2,3} -> 0 -> 1 ready -> {1,2,3}
         pq.pop();                           //   = 0 1 2 3 (queue hota to 0 2 3 1 -> GALAT)
         ans.push_back(node);
         for (auto &it : adj[node])
             indegree[it]--;
             if (indegree[it] == 0) pq.push(it);
     if (ans.size() == n) return ans;
     return {};                              // loop / self-loop [0->0] -> indegree kabhi 0 nahi -> khaali

── FAMILY: CLONE / deep-copy (BFS + map purana -> naya; map = cycle-rok + clone-dhundo) ──

 ▸ CLONE GRAPH (LC-133)  = BFS/DFS + map(old->new)
     // saare node ka clone map me (A->A', B->B'), phir clones ko CONNECT (A'--B'). 2 kaam:
     //   (a) clone BANANA (if ke ANDAR, sirf naye pe)  (b) JODNA push_back (if ke BAHAR, har edge pe)
     //   bina jodna -> loose clones -> adhoora graph
     if (!node) return nullptr;   // khaali graph
     unordered_map<Node *, Node *> mp;       // purana (A) -> clone (A'); value hi clone
     queue<Node *> q; Node *clone = new Node(); clone->val = node->val;
     mp[node] = clone;                       // seed bhi map me: warna cycle pe isi node ka doosra clone
     q.push(node);
     while (!q.empty())
         Node *curr = q.front(); q.pop();
         for (auto &it : curr->neighbors)
             if (mp.find(it) == mp.end())    // (a) BANANA. find se check: mp[it] khaali entry bana deta
                 Node *newClone = new Node(); newClone->val = it->val; mp[it] = newClone;
                 q.push(it);                 // push sirf naye pe: warna cycle me infinite loop
             mp[curr]->neighbors.push_back(mp[it]);   // (b) JODNA: A' ki list me B'
         // clone pehle se ho tab bhi edge jodni hai
     return mp[node];                        // start node ka clone = naye graph ka shuru
     // DFS: recursion + same map -> map me hai? return; nahi -> clone + map -> har nbr recurse + jodo

── FAMILY: GRAPH-COLOURING / bipartite (2 group me baant sakte? odd-cycle? -> 2-COLOUR BFS) ──

 ▸ IS BIPARTITE (LC-785)  = 2-colour BFS + outer-loop (CC jaisa)
     // har node ko rang 0/1; koi 2 JUDE node SAME rang na hon. odd cycle (triangle) -> false
     // PEHCHAAN: "DO team me baanto + jo JUDE (dushman / edge) wo ek group me NAHI". output "team A / team B"
     //   = 2 rang. DSU ULTA hai: DSU me jude -> SAME group, yahan jude -> ALAG
     // graph[i] = node i ke neighbours (adj-list KHUD di hai). [[1,3],[0,2],[1,3],[0,2]] -> true (square)
     //   [[1,2,3],[0,2],[0,1,3],[0,2]] -> false (0-1-2 triangle)
     // BFS
     queue<int> q; q.push(node);
     color[node] = 0;                        // START ko rang ZAROORI, warna flip galat
     while (!q.empty())
         int node = q.front(); q.pop();
         for (auto &it : graph[node])
             if (color[it] == -1)            // (a) uncolored -> ULTA rang + push
                 color[it] = 1 - color[node];   // 0 -> 1, 1 -> 0
                 q.push(it);
             else
                 if (color[it] == color[node]) return false;   // (b) pehle se rang + SAME = conflict
     return true;                            // is component me conflict nahi

     // isBipartite
     int m = graph.size();
     vector<int> color(m, -1);               // -1 = abhi rang nahi (0 aur 1 asli rang)
     for (int i = 0; i < m; i++)             // har node se: graph disconnected ho sakta
         if (color[i] == -1)                 // -1 check PEHLE: rang wale pe BFS dobara nahi
             if (BFS(graph, color, i) == false) return false;   // ek bhi component fail -> poora false
     return true;

── FAMILY: WEIGHTED shortest-path (edges pe weight -> Dijkstra; kam edges != kam weight, BFS kaafi nahi) ──

 ▸ DIJKSTRA  = BFS template + 2 swap + relaxation
     // BFS ka WEIGHTED bhai: queue -> MIN-HEAP (chhoti dist pehle) · visited[] -> dist[] (INF, src=0) + RELAX
     // visited kyun nahi: dist[] + STALE-skip = visited ka kaam; strict relax -> undirected me bhi loop nahi
     // negative weight -> Dijkstra FAIL (dist ghatta rehta) -> Bellman-Ford. REAL: Maps / GPS, routing
     // pq me {dist, node} PAIR: heap FIRST se sort -> HAMESHA {jispe-sort, saath-wala}
     const int INF = 1e9; unordered_map<int, vector<pair<int, int>>> adj;
     for (auto &it : edges)
         int u = it[0]; int v = it[1]; int w = it[2]; adj[u].push_back({v, w});
         adj[v].push_back({u, w});           // undirected DONO taraf
     priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;   // {dist,node}
     vector<int> dist(n, 1e9);               // INF se: pehla rasta hamesha chhota lage
     dist[src] = 0;                          // src khud 0
     pq.push({0, src});
     while (!pq.empty())
         int wt = pq.top().first;            // node tak ki dist
         int node = pq.top().second; pq.pop();
         if (wt > dist[node]) continue;   // STALE skip (sirf > : barabar wala taaza entry)
         for (auto &it : adj[node])          // it.first = nbr, it.second = EDGE weight (dist[node] NAHI)
             if (wt + it.second < dist[it.first])     // RELAX: naya rasta chhota? (< strict: barabar pe
                 dist[it.first] = wt + it.second;     //   dobara push bekaar, loop ka khatra)
                 pq.push({wt + it.second, it.first});   // pehle dist update, phir push
     return dist;

── FAMILY: DSU / Union-Find (groups / same-set + edges ek-ek MERGE; BFS = pura traverse, DSU = incremental) ──

 ▸ DSU (Union-Find)  = parent[] + find + unite
     // KAB: groups / connected / same-set? · UNDIRECTED cycle-detect · provinces · redundant-conn
     //   · MST-Kruskal
     // REAL: dost-circle ("A-B same circle?"), accounts merge
     // class DSU
     vector<int> parent, rnk;                // rnk = tree ki approx height (union-by-rank isi se)
     // DSU
     parent.resize(n), rnk.assign(n, 0);
     for (int i = 0; i < n; i++) parent[i] = i;   // shuru me har node khud apna leader
     // find
     if (x == parent[x]) return x;   // khud ka parent = ROOT, yahin ruko
     return parent[x] = find(parent[x]);   // PATH COMPRESSION: agli baar seedha root
     // unite
     int u_x = find(x);                  // ROOT jodo, x-y khud nahi: warna group toot jaata
     int u_y = find(y);
     if (u_x == u_y) return;   // already same set
     if (rnk[u_x] < rnk[u_y]) parent[u_x] = u_y;   // UNION BY RANK: chhota bade ke neeche
     else if (rnk[u_y] < rnk[u_x]) parent[u_y] = u_x;
     else
         parent[u_y] = u_x;
         rnk[u_x]++;                     // barabar rank: jodne se ek level badha
     // 2 opt (path-compression + union-by-rank) -> find / unite ~O(1)
     // same group? -> find(x) == find(y)   (root compare, parent nahi: parent beech ka ho sakta)

     // countComponents
     DSU dsu(n);
     for (auto &e : edges) dsu.unite(e[0], e[1]);   // har edge -> do node merge
     int count = 0;
     for (int i = 0; i < n; i++)
         if (dsu.find(i) == i) count++;   // sirf root ka parent khud wahi = ek group
     return count;

 ▸ WORD LADDER (LC-127)  = BFS shortest-path (unweighted)
     // begin -> end, har step EK letter badlo, beech ka har word dict me. "min steps" -> BFS level-by-level
     // word = NODE · edge = ek letter farak + dict me. graph IMPLICIT (words), islands / Kahn's wali family
     // hit -> hot -> dot / lot -> dog / log -> cog = 5 WORDS (begin = 1, steps nahi)
     // level track 2 tareeke: steps PAIR me (ye code) · ya size-loop + level++ (poora layer khatam pe)
     unordered_set<string> st(begin(wordList), end(wordList));   // dict = lookup + visited dono
     queue<pair<string, int>> q;
     q.push({beginWord, 1});                 // begin = 1 word
     st.erase(beginWord);                    // begin dobara na aaye
     while (!q.empty())
         string word = q.front().first; int steps = q.front().second; q.pop();
         if (word == endWord) return steps;
         for (int i = 0; i < word.size(); i++)
             char original = word[i];
             for (char ch = 'a'; ch <= 'z'; ch++)
                 word[i] = ch;
                 if (st.find(word) != st.end())
                     st.erase(word);         // use karte hi ERASE = visited, alag visited-set nahi
                     q.push({word, steps + 1});
             word[i] = original;             // RESTORE
     return 0;                               // endWord dict me nahi -> 0

```

---

---

## PATTERN 14 — HEAP
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

```
 BROAD IDEA: baar-baar MIN ya MAX + data BADALTA -> HEAP (sort mahenga). top O(1), push/pop O(log n).
   C++ syntax:  priority_queue<int> = MAX-heap  ·  priority_queue<int, vector<int>, greater<int>> = MIN-heap.
   ops: pq.push(x) · pq.top() · pq.pop() · pq.size() · pq.empty().
   REAL: emergency-room (severity top pe).
   KAB: top-K · k-th largest/smallest · merge-k-lists · Dijkstra · median-stream.
   MIRROR-RULE: "k sabse BADE" -> MIN-heap size k ; "k sabse CHHOTE" -> MAX-heap size k. O(n log k).
 ★ EDGE (top K): MIN-heap of size k (sab max-heap me daal ke k pop = O(n log n), haar gaye).
 ★ EDGE (comparator): B KO DEKHO -> a < b = b bada = MAX-heap · a > b = b chhota = MIN-heap (neeche poora).
 ★ EDGE (pointer push): push se PEHLE null check (merge-k: khaali list push -> top()->val pe SEGFAULT).

── FAMILY: heap of size k / top-K (size > k pe top EVICT, k bache) ──

 ▸ LAST STONE WEIGHT (LC-1046, Easy)  = MAX-HEAP smash
     // har turn 2 SABSE BHAARI smash, diff wapas, till <=1 bache
     priority_queue<int> pq;   // MAX: har turn 2 sabse BHAARI chahiye
     for (int i = 0; i < stones.size(); i++) pq.push(stones[i]);
     while (pq.size() > 1)   // >1: smash ko 2 pathar chahiye
         int x = pq.top(); pq.pop(); int y = pq.top(); pq.pop();   // sabse bada · doosra sabse bada
         int diff = abs(y - x);
         if (diff > 0) pq.push(diff);        // barabar -> 0 = dono khatam · bacha pathar wapas heap me
     return pq.empty() ? 0 : pq.top();   // empty pehle: sab khatam to top() crash
 // heap kyun (sort nahi): smash ke baad set BADALTA -> baar-baar "2 max"

 ▸ KTH LARGEST ELEMENT (LC-215, Medium)  = MIN-HEAP of size k
     // top-K ka core
     priority_queue<int, vector<int>, greater<int>> pq;   // MIN: size>k pe sabse CHHOTA evict
     for (auto &it : nums)
         pq.push(it);
         if (pq.size() > k) pq.pop();        // sabse chhota nikaalo -> sirf k SABSE BADE bache
     return pq.top();   // k bade me se sabse chhota = k-th largest
 // O(n log k) (sort O(n log n) se behtar)

 ▸ K CLOSEST POINTS TO ORIGIN (LC-973, Medium)  = MAX-HEAP of size k
     // KTH-LARGEST ka MIRROR: wahan k BADE (min-heap) · yahan k CHHOTE dist (MAX-heap, sabse BADI evict)
     priority_queue<pair<int, vector<int>>> maxHeap;   // MAX: sabse door top pe, size>k pe wahi nikle
     for (int i = 0; i < points.size(); i++)
         maxHeap.push({{points[i][0] * points[i][0] + points[i][1] * points[i][1]}, {points[i]}});
                                             // {dist, point}: dist PEHLE (pair first pe compare)
                                             // dist = x²+y², √ NAHI (order same)
         if (maxHeap.size() > k) maxHeap.pop();   // sabse door EVICT -> k paas bache
     vector<vector<int>> ans;
     while (maxHeap.size() > 0) ans.push_back(maxHeap.top().second), maxHeap.pop();   // .second = {x,y} poora
     return ans;

 ▸ TOP K FREQUENT ELEMENTS (LC-347, Medium)  = MIN-HEAP size k on FREQUENCY
     // KTH-LARGEST ka COUSIN, farak 2: (1) pehle hashmap se freq (2) heap me {freq, num}
     unordered_map<int, int> mp;
     for (int i = 0; i < nums.size(); i++) mp[nums[i]]++;   // STEP 1: freq gino
     priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> minHeap;
                                             // MIN: size>k pe sabse KAM freq bahar
     for (auto &it : mp)
         minHeap.push({it.second, it.first});   // {freq, num}: freq PEHLE -> heap freq pe sort
         if (minHeap.size() > k) minHeap.pop();   // chhoti freq EVICT -> top-k bache
     vector<int> ans;
     while (!minHeap.empty())
         int tp = minHeap.top().second; minHeap.pop(); ans.push_back(tp);   // .second = actual number
     return ans;
 // O(n log k). max-heap me saare push -> k baar pop = O(n log n), slow

── FAMILY: MIN-heap of current candidates (har baar sabse chhota nikaalo, uska agla daalo) ──

 ▸ MERGE K SORTED LISTS (LC-23, Hard)  = MIN-HEAP of current heads
     // struct Compare: ListNode* ko khud compare karna nahi aata
     return a->val > b->val;                 // > likha to MIN-heap (pq ka comparator ulta chalta)

     priority_queue<ListNode *, vector<ListNode *>, Compare> minHeap;
     for (auto &l : lists)
         if (l) minHeap.push(l);             // saare heads; NULL skip (warna ->val pe crash)
     ListNode *dummy = new ListNode(0); ListNode *tail = dummy;   // merge-2-lists jaisa: dummy + tail
     while (!minHeap.empty())
         ListNode *curr = minHeap.top(); minHeap.pop(); tail->next = curr;   // sabse chhota · jodo
         tail = curr;
         if (curr->next) minHeap.push(curr->next);   // uska next; khatam list ka NULL mat daalo
     return dummy->next;                     // ->next: dummy khud nakli node
     // heap me max k node (1 per list) -> O(N log k)

 ▸ SORT NEARLY-SORTED (k-sorted) ARRAY  = MIN-HEAP size k+1 (sliding buffer)
     // har element sahi jagah se max k door. index-0 ka MIN max index k tak -> "pehle k+1 me MIN kaun?"
     vector<int> ans;
     priority_queue<int, vector<int>, greater<int>> pq;   // MIN: sorted output me chhota pehle
     for (int i = 0; i < arr.size(); i++)
         pq.push(arr[i]);   // pehle daalo, phir size dekho
         if (pq.size() > k + 1)   // k+1 se bada: tab tak sahi MIN buffer me aa chuka
             int tp = pq.top(); pq.pop(); ans.push_back(tp);   // buffer bhara -> MIN nikaal
     while (!pq.empty())                 // bache = sorted tail
         int tp = pq.top(); pq.pop(); ans.push_back(tp);
     return ans;
 // [6,5,3,2,8,10,9], k=3 -> [2,3,5,6,8,9,10]. O(n log k). kth-largest ka cousin (sliding buffer)

── FAMILY: TWO-HEAPS (do team, dono top = beech ke border) ──

 ▸ FIND MEDIAN FROM DATA STREAM (LC-295)  = TWO-HEAPS balanced
     // stream me kabhi bhi median turant. add 5,15,1,3 -> 4.0 ((3+5)/2)
     // TRAP: heap ka NAAM top batata, number kaun rakhta wo NAHI. "bada -> maxHeap" GALAT:
     //   bada -> BADI team -> minHeap. naam bhool, team soch.
     // class MedianFinder
     priority_queue<double> maxHeap;         // CHHOTI team: top = chhoto ka sabse bada (border)
     priority_queue<double, vector<double>, greater<double>> minHeap;   // BADI team: top = bado ka min

     // addNum
     if (maxHeap.empty() || maxHeap.top() >= num)   // empty() pehle: khaali pe top() crash
         maxHeap.push(num);                  // 1. PLACEMENT: chhota -> chhoti team
     else minHeap.push(num);                 // bada -> badi team (minHeap)
     if (maxHeap.size() < minHeap.size())    // 2. BALANCE (dono if zaroori: num kisi bhi heap me)
         double it = minHeap.top();          // minHeap bada: extra hamesha maxHeap me -> ek wapas
         minHeap.pop(), maxHeap.push(it);
     else if (maxHeap.size() > minHeap.size() + 1)   // 2 ka farak: maxHeap sirf 1 aage chalta
         double it = maxHeap.top(); maxHeap.pop(); minHeap.push(it);

     // findMedian
     if (maxHeap.size() == minHeap.size())
         return (maxHeap.top() + minHeap.top()) / 2.0;   // even: dono border ka average
     return maxHeap.top();                   // odd: extra maxHeap me, uska top = median
     // 5 -> 5 · 15 -> 10 · 1 -> 5 · 3 -> farak 2, 5 minHeap me -> {3,1} {5,15} -> 4

── FAMILY: MAX-heap + greedy (har round sabse zyada wala lagao) ──

 ▸ REORGANIZE STRING (LC-767)  = MAX-HEAP + POP-TWO-AT-A-TIME
     // koi 2 ADJACENT same na ho, na bane -> "". top-k-frequent ka cousin: har round 2 nikaalo
     // possible tabhi: (maxCount-1) gap <= (n-maxCount) baaki. aab OK · aaab -> ""
     //   (alag check na bhi rakho: aakhri bacha count>1 khud pakad leta)
     string ans = ""; unordered_map<char, int> mp;
     for (auto &it : s) mp[it]++;
     priority_queue<pair<int, char>> maxHeap;   // {count, char}: count PEHLE, us pe sort
     for (auto &it : mp) maxHeap.push({it.second, it.first});
     while (maxHeap.size() > 1)   // >1: har round 2 ALAG char (1 nikaal wapas = "aa")
         auto it1 = maxHeap.top(); maxHeap.pop(); auto it2 = maxHeap.top(); maxHeap.pop();
         ans += it1.second, ans += it2.second, it1.first -= 1, it2.first -= 1;   // -- pehle, phir check
         if (it1.first >= 1) maxHeap.push(it1);   // 0 bacha to wapas mat daalo
         if (it2.first >= 1) maxHeap.push(it2);
     if (!maxHeap.empty())
         if (maxHeap.top().first > 1) return "";   // akela bacha count>1: do same paas aayenge, impossible
         else ans += maxHeap.top().second;
     return ans;   // heap khaali: saare char lag gaye
 // "aaabbb" -> ab, abab, ababab · "aaab" -> ab, (2,a) akela count 2 -> ""

── FAMILY: MIN-heap + vis[] (lazy delete) ──

 ▸ THROTTLING — MIN UTHAO, PADOSI HATAO (Amazon OA)  = MIN-HEAP {val,idx} + vis[] (lazy skip)
     // har step bache me SABSE CHHOTA (tie -> chhota index), jodo, khud + ORIGINAL padosi (i-1, i+1) hatao
     // [4,2,9,1,7,3] -> 1 (9,7 gaye) -> 2 (4 gaya) -> 3 = 6
     // BURST BALLOONS NAHI: wahan kaunsa phodna TU chunta + padosi badalte -> DP.
     //   yahan niyam tay (hamesha min) + ORIGINAL padosi -> koi choice nahi -> heap se simulate
     int n = responseTimes.size();
     vector<bool> vis(n, false); long long ans = 0;   // 1e9 x lakh -> long long
     priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;   // MIN
     for (int i = 0; i < n; i++) pq.push({responseTimes[i], i});   // pair: tie pe chhota idx apne aap pehle
     while (!pq.empty())
         int index = pq.top().second; pq.pop();   // .second = index (first sirf sort ke liye)
         if (vis[index] == false)   // hat chuka -> chhodo (LAZY delete, Dijkstra stale-skip)
             ans += responseTimes[index], vis[index] = true;   // khud bhi hatao, dobara na uthe
             if (index - 1 >= 0) vis[index - 1] = true;   // >=0: pehle ka left padosi nahi
             if (index + 1 < n) vis[index + 1] = true;   // < n: aakhri index n-1
     return ans;
 // O(n log n). heap se beech ka element nikaal nahi sakte -> vis[] se pop pe skip
```

---

---

## PATTERN 15 — INTERVALS
> ★ **YAHAN NAHI MILEGI:**
> ```
> non-overlapping intervals  ->  PATTERN 18 (GREEDY)
>       intervals hain, par sawaal "kam se kam kitne HATAO" = greedy choice
>       (sort by END, aur takraane wala chhodo)
> ``` (sort-by-start + overlap)

```
 BROAD IDEA: interval = [start,end]. dikha -> PEHLA reflex = SORT. phir left-to-right handle.
   REAL: calendar/meeting-scheduling, booking, resource-alloc.

 ★ DECISION — input same [start,end], SAWAAL (verb) decide karta kaunsa tool:
     "MERGE overlapping / combine"           -> sort by START + merge
     "MAX non-overlapping pick / min remove" -> GREEDY: sort by END, free ho to pick
     "kitne ek-SAATH / MIN rooms chahiye"    -> LINE-SWEEP (start +1, end -1, running-count ka MAX)
   greedy = wahi sort+loop, bas KEY = END (jaldi-khatam wali max room chhodti -> zyada fit).
      A[1,10] B[2,3] C[4,5] -> START-sort = 1 · END-sort = 2. END jeeta.

── FAMILY: sort-by-start + overlap check (bas overlap pe ACTION aur boundary < / <= badalta) ──

 ▸ MERGE INTERVALS (LC-56)  = sort-by-start + merge
     sort(begin(intervals), end(intervals));   // default FIRST element (start) se. HAMESHA pehla step
     vector<vector<int>> ans;
     ans.push_back(intervals[0]);   // pehla baseline
     for (int i = 1; i < intervals.size(); i++)   // 1 se: [0] pehle hi ans me
         if (ans.back()[1] >= intervals[i][0])   // OVERLAP: curr.start <= last.END
                                             //   (<= : [1,3],[3,5] chhoote bhi merge)
             ans.back()[1] = max(ans.back()[1], intervals[i][1]);   // max zaroori: [1,10],[2,3] -> 10
         else ans.push_back(intervals[i]);   // no overlap -> naya
     return ans;   // ans.back() = last; [0]=start [1]=END
 // [[1,3],[2,6],[8,10],[15,18]] -> [[1,6],[8,10],[15,18]]

 ▸ MEETING ROOMS (LC-252, Easy)  = Merge ka SAME template, bas action alag
     // MERGE: overlap -> end max, return list · MEETING: overlap -> return false, end me true
     if (intervals.empty()) return true;     // EDGE: warna intervals[0] pe crash
     sort(intervals.begin(), intervals.end());   // start se sort
     vector<vector<int>> ans;
     ans.push_back(intervals[0]);   // pehla baseline
     for (int i = 1; i < intervals.size(); i++)
         if (intervals[i][0] < ans.back()[1])   // < (<= NAHI): ek khatam usi pal doosri shuru = chalta
             return false;   // 2 meeting takraye -> saare attend nahi
         else ans.push_back(intervals[i]);   // agla overlap isi ke end se check
     return true;   // koi overlap nahi
 // boundary farak: Merge touching = MERGE (<=) · Meeting touching = OK (<)

 ▸ INSERT INTERVAL (LC-57)  = already-SORTED -> 3 phase (no sort)
     // pehle se sorted + non-overlap -> SORT nahi, ek pass. newInterval = "GROWING BOX"
     vector<vector<int>> ans;
     int count = 0;   // kitne process hue, doosra loop wahin se
     for (int i = 0; i < intervals.size(); i++)
         if (intervals[i][1] < newInterval[0])   // 1. new se PEHLE khatam -> push
             ans.push_back(intervals[i]);   //    (sirf < : chhoota hua merge hoga)
         else if (intervals[i][0] > newInterval[1])
             break;   // 3. new ke AAGE -> BREAK (sorted, aage sab door)
         else   // 2. OVERLAP -> new ko GROW
             newInterval[0] = min(newInterval[0], intervals[i][0]);
             newInterval[1] = max(newInterval[1], intervals[i][1]);   // end bhi max
         count++;   // break wala yahan nahi aata -> count se doosra loop
     ans.push_back(newInterval);   // 4. bada hua new daalo
     for (int i = count; i < intervals.size(); i++) ans.push_back(intervals[i]);   // phir count se aage baaki
     return ans;

── FAMILY: SWEEP LINE (events + running count: kitna overlap, sirf "hai?" nahi) ──

 ▸ MEETING ROOMS II (LC-253)  = SWEEP LINE / event-count
     // min ROOMS = kisi ek pal pe MAX kitni meeting saath. rooms object mat gino -> +1 / -1 GINTI
     map<int, int> mp;   // ORDERED map: time -> delta, keys khud sorted
     int n = start.size();
     for (int i = 0; i < n; i++)
         mp[start[i]]++;   // shuru = +1 (room bhara)
         mp[end[i]]--;   // khatam = -1. same time pe +1 -1 kat -> room REUSE
     int overlap = 0, maxOverlap = 0;   // 0 se: shuru me koi meeting nahi
     for (auto &it : mp)   // keys sorted -> time ke order me
         overlap += it.second;   // is pal kitni meeting chal rahi
         if (overlap > maxOverlap) maxOverlap = overlap;   // peak = rooms
     return maxOverlap;
 // start=[2,9,6] end=[4,12,10]: t=9 pe [6,10]+[9,12] = 2 -> 2 rooms
     // FAMILY: max-concurrent, CPU-load, planes-in-sky. SAGAA BHAI = DIFFERENCE ARRAY (LC-1109)
     //   -> PATTERN 4: map ki jagah ARRAY · end ki jagah END+1 (inclusive) · max ki jagah POORA array
```

---

---

## PATTERN 16 — BACKTRACKING (choose / explore / un-choose)

```
 BROAD IDEA: saari possibilities explore -> solution EK-EK step build;
   galat lage -> PEECHE aao (backtrack) -> agla raasta.
   MAZE: raasta pakdo -> dead-end -> WAPAS last-junction -> doosra try. har element pe CHOICE(s).
   TEMPLATE (3 step): CHOOSE (path me daalo) -> EXPLORE (aage recurse)
                      -> UN-CHOOSE (pop / undo -> agli choice).
   DECISION TREE: har node = choice-point; har leaf tak ka raasta = ek possible answer.
   KAB: "saare subsets / permutations / combinations", "N-queens", "har combination try karo".

 GOTCHA 1 — EARLY RETURN 2ND BRANCH MAAR DETA:
     if (cond1) return solve(...);           // GALAT: function yahin exit -> neeche wali branch kabhi nahi
     if (cond2) solve(...);
     // FIX: har branch bina return:  if (cond1) solve(...);  if (cond2) solve(...);
     // return HARMFUL sirf jab uske BAAD chalne wala code ho. LAST line pe return = safe.
     // BEECH me return = agla branch maar deta (void / non-void se lena-dena NAHI, FLOW ka rule).

 GOTCHA 2 — `else if` MAT LAGAO (dono branch ALAG if):
     if (cond1) solve(...);                  // sahi: dono chalti
     if (cond2) solve(...);
     // else if = "ek mil gaya, bas" -> cond1 sach to cond2 KABHI nahi. backtracking = HAR combination banana.
     // dhoondhne wale code me else if theek (ek jawab). BANANE wale code me GALAT (saare jawab).

 2 FORMS = SAME cheez, alag likhawat (dono me core: CHOOSE -> EXPLORE -> UN-CHOOSE):
   A) INCLUDE / EXCLUDE : 2 call (liya / nahi liya), record BASE (index == n) pe
   B) FOR-LOOP (start)  : loop se har element pe branch, record HAR node pe, alag base nahi
                          (index == n -> loop chalta hi nahi -> khud ruk jaata)

── FAMILY: SUBSETS-family (start-aage = kaunse; used[] = kis order) ──

 ▸ SUBSETS (LC-78)  = har element pe 2 choice (INCLUDE / EXCLUDE)
     void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp)
         if (index >= nums.size())   // har element pe faisla ho gaya -> ek poora subset
             ans.push_back(temp); return;
         temp.push_back(nums[index]);   // INCLUDE: le liya
         solve(nums, ans, index + 1, temp);   // index+1: agle element pe faisla
         temp.pop_back();   // UN-CHOOSE, exclude se PEHLE: warna exclude me bhi chipka rehta
         solve(nums, ans, index + 1, temp);   // EXCLUDE: element chhod ke aage

     vector<vector<int>> ans;
     vector<int> temp;   // BAHAR banao, by-ref pass: helper me banate to har call naya
     solve(nums, ans, 0, temp);   //   (ek SHARED path pe push / pop -> tabhi backtrack chalta)
     return ans;   // 2^n subsets (har element in / out = power set)

     // ALT FORM (start-loop), same output:
     void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp)
         ans.push_back(temp);   // HAR node = ek subset (alag base-case NAHI)
         for (int i = index; i < nums.size(); i++)   // index se: peeche wale dobara nahi
             temp.push_back(nums[i]), solve(nums, ans, i + 1, temp);   // CHOOSE · EXPLORE: i+1 -> agla aage se
             temp.pop_back();   // UN-CHOOSE agle i se pehle: wahi jagah agle element ko mile
     // tree [1,2,3]: har edge pe neeche jaate PUSH, wapas aate POP. undo na karo -> galat subset
     // ([1,2,3] -> pop3 -> [1,2] -> pop2 -> [1] -> loop aage +3 -> [1,3]). right side patli: node sirf aage
     //   pe branch

 ▸ PERMUTATIONS (LC-46)  = used[] + FULL-LENGTH base (har position pe koi bhi bacha)
     void solve(vector<int> &nums, vector<int> &temp, vector<bool> &used, vector<vector<int>> &ans)
         if (temp.size() == nums.size())   // base = full length (poora arrangement)
             ans.push_back(temp); return;
         for (int i = 0; i < nums.size(); i++)   // 0 se, start NAHI: order matter, peeche wala bhi kisi
                                                 //   position pe
             if (used[i]) continue;          // already liya -> skip
             temp.push_back(nums[i]), used[i] = true;   // CHOOSE
             solve(nums, temp, used, ans);   // EXPLORE (i+1 nahi -> phir 0 se, used skip karta)
             temp.pop_back();   // UN-CHOOSE: DONO undo
             used[i] = false;   //   used false bhi: warna aage ke arrangements me ye band
     // caller: temp + used(n, false) BAHAR bana ke by-ref. n! permutations.
     // SUBSETS = 'start' (sirf AAGE, [1,2]==[2,1] repeat nahi) · PERMUTATION = used[] + loop 0 se, start NAHI

 ▸ COMBINATIONS (LC-77)  = SUBSETS ka start-loop, bas base size==k
     void solve(int n, int k, vector<vector<int>> &ans, int index, vector<int> &temp)
         if (temp.size() == k)   // fixed-size combo poora (subsets har node pe, yahan sirf ==k)
             ans.push_back(temp); return;
         for (int i = index; i <= n; i++)   // 1..n numbers, index se aage (<= n: n khud bhi number)
             temp.push_back(i);   // CHOOSE
             solve(n, k, ans, i + 1, temp);   // EXPLORE (i+1 -> order nahi, aage hi)
             temp.pop_back();   // UN-CHOOSE: agla i try karne se pehle path saaf
     // caller: solve(n, k, ans, 1, temp)  (1 se: numbers 1..n hain, 0 nahi)
     // SUBSETS = start-aage, har node · COMBINATIONS = start-aage, size==k · PERM = used[] + loop 0, size==n

 ▸ COMBINATION SUM (LC-39)  = REUSE allowed + TARGET-base
     void solve(vector<int> &nums, vector<vector<int>> &ans, int index, vector<int> &temp, int target)
         if (target < 0) return;             // overshoot: aage jaana bekaar (prune)
         if (index >= nums.size())   // end pe check
             if (target == 0) ans.push_back(temp);
             return;
         temp.push_back(nums[index]);   // INCLUDE
         solve(nums, ans, index, temp, target - nums[index]);   // index SAME: yahi element dobara le sakte
         temp.pop_back();   // exclude se pehle undo: warna exclude path me bhi rehta
         solve(nums, ans, index + 1, temp, target);   // EXCLUDE: is candidate ko ab chhod diya

     // ALT FORM (for-loop), same output:
     void solve2(vector<int> &candidates, int target, vector<vector<int>> &ans, int index,
                 vector<int> &temp)
         if (target < 0) return;             // prune
         if (target == 0)                    // for-loop form: target==0 pe turant record
             ans.push_back(temp); return;
         for (int i = index; i < candidates.size(); i++)   // start se: peeche wale nahi, combo repeat nahi
             temp.push_back(candidates[i]);   // CHOOSE
             solve2(candidates, target - candidates[i], ans, i, temp);   // 'i' (i+1 NAHI) -> REUSE allowed
             temp.pop_back();   // UN-CHOOSE
     // combinations se FARAK: (1) recurse 'i' na i+1 (2) base = target (0 = record, <0 = prune), size==k nahi
     // candidates positive -> target ghatta -> infinite nahi

── FAMILY: GRID PATH (element choose / skip nahi, grid pe raasta: mark -> recurse -> un-mark) ──

 ▸ WORD SEARCH (LC-79)  = grid-DFS + BACKTRACKING
     bool solve(vector<vector<char>> &board, string word, int index, int i, int j, int m, int n,
                vector<vector<bool>> &vis)   // vis REFERENCE (&), copy nahi
         if (i < 0 || i >= m || j < 0 || j >= n || vis[i][j] || board[i][j] != word[index])
             return false;   // bounds PEHLE (warna board[i][j] bahar), visited bhi isi if me
         vis[i][j] = true, index++;          // mark PEHLE: is raaste me ye cell dobara na aaye
         if (solve(board, word, index, i + 1, j, m, n, vis) ||   // || : ek true -> baaki call nahi
             solve(board, word, index, i, j + 1, m, n, vis) ||
             solve(board, word, index, i - 1, j, m, n, vis) ||
             solve(board, word, index, i, j - 1, m, n, vis))
             return true;
         vis[i][j] = false;   // BACKTRACK (un-mark): doosre raaste ke liye cell free
         return index == word.size() ? true : false;   // base: aakhri char match tabhi == size

     int m = board.size(); int n = board[0].size();
     vector<vector<bool>> vis(m + 1, vector<bool>(n + 1, false));   // m x n kaafi (m+1 zaroori nahi)
     for (int i = 0; i < m; i++)
         for (int j = 0; j < n; j++)
             if (board[i][j] == word[0])   // pehla char mile tabhi DFS
                 if (solve(board, word, 0, i, j, m, n, vis) == true) return true;   // KOI EK mila = kaafi
     return false;   // SAARE start try, kahin nahi -> tab false
     // BUG-TRAP: loop me `if (solve(..) == false) return false;` -> pehla start fail hote hi poora false.
     //   "AAB" on [CAA / AAA / BCD]: (0,1) ka A fail, par (1,1)->(1,0)->(2,0) = AAB tha. + ulta bug: word[0]
     //   kahin na mila aur end me `return true` = jhootha "mila"
     // ANY vs ALL: KOI EK? -> loop me true pe return, BAAD me false · SAB? -> loop me false pe return, BAAD
     //   me true
     // islands-DFS se FARAK: islands = fill / count. word search = mark + recurse + UN-mark + return-bubble

── FAMILY: CONSTRAINT / LEVEL choices (har step pe choice, rule ya level se) ──

 ▸ GENERATE PARENTHESES (LC-22)  = CONSTRAINT-guided (open / close count)
     // n jode ke saare VALID strings. brute (2^2n + filter) NAHI -> 2 RULE se hi banao
     void solve(int open, int close, string path, vector<string> &ans, int n)   // path BY VALUE: path+'('
         if (open == n && close == n)   //   naya copy -> pop_back nahi chahiye
             ans.push_back(path); return;    // base: saare khule + band (= path.size() == 2*n)
         if (open < n) solve(open + 1, close, path + '(', ans, n);   // '(' : n se zyada '(' nahi
         if (close < open)   // ')' : band utne hi jitne khule. ALAG if, else if NAHI:
             solve(open, close + 1, path + ')', ans, n);   //   dono sach ho sakti, tab DONO chalni chahiye
     // return solve(..) MAT: '(' ke return se ')' branch kabhi nahi
     // caller: solve(0, 0, "", ans, n). n=3 -> 5, n=4 -> 14 (kam aaye = koi branch kat rahi)

 ▸ LETTER COMBINATIONS OF A PHONE NUMBER (LC-17)
     // "23" -> har ank ke akshar me se EK-EK. har LEVEL (index) = ek ank, us ke SAARE akshar try (for loop)
     void solve(int index, string &temp, unordered_map<char, string> &mp, vector<string> &ans,
                string &digits)
         if (index >= digits.size())         // saare ank ho gaye
             ans.push_back(temp); return;
         string str = mp[digits[index]];   // is ank ke akshar
         for (int i = 0; i < str.size(); i++)   // har akshar ek alag branch
             temp.push_back(str[i]), solve(index + 1, temp, mp, ans, digits);   // choose · explore (agla ank)
             temp.pop_back();   // un-choose: agle akshar se pehle path saaf

     if (digits.size() == 0) return {};      // EDGE: "" -> khaali LIST, [""] nahi
     unordered_map<char, string> mp;   // key = char ('2'): digits[index] char hai -> seedha chalta
     mp['2'] = "abc";  mp['3'] = "def";  mp['4'] = "ghi";  mp['5'] = "jkl";
     mp['6'] = "mno";  mp['7'] = "pqrs"; mp['8'] = "tuv";  mp['9'] = "wxyz";
     vector<string> ans;   // ans GLOBAL mat rakho: function do baar bula to pichhle jawab
     string temp = ""; solve(0, temp, mp, ans, digits);   // bhi jud jaate ("2" pe 3 ki jagah 12)
     return ans;
```

---

---

## PATTERN 17 — DP (dynamic programming = recursion + memo)

```

 BROAD IDEA: same SUBPROBLEM baar-baar aaye (OVERLAP) -> dobara mat nikaalo, ek baar karke dp[] me rakho.
   DP = RECURSION + memo (recursion ko smart tareeke se). OVERLAP na ho to DP ka fayda nahi.
   har DP me wahi 3-part skeleton: BASE + TRANSITION (choices = solve(i-1) / solve(j-1) / ...) + MEMO.
   transition = wahi recursion jo trees (left/right) / DFS-grid (4 dir) me thi -- bas branch ki ginti alag.
   ARC: 1) plain recursion (relation + base) -> 2) overlap dekho (same call 2x) -> 3) memo -> [4) bottom-up]
   TOP-DOWN = recursion + dp[] cache (sochne me aasaan) · BOTTOM-UP = base bharo, loop se dp[i] chhote se bade
   KAB DP: "kitne WAYS" · "min/max cost" · "kya possible" + har step CHOICE + subproblem OVERLAP.
 ★ EDGE (min / max DP): min le raha -> init INT_MAX · max le raha -> INT_MIN · 0 sirf jab 0 VALID answer ho.
         coin change: take = 0 daala -> 0 hamesha min jeet gaya. ★ init likhte waqt HAATH RUKE.
 ★ MILTE-JULTE (LIS vs longest consecutive): upar "NAAM-SE-CONFUSE" -> B dekh.

 MEMO SKELETON (har DP me SAME 3 line):
     vector<int> dp(n + 1, -1);                  // -1 = abhi nahi nikala (0 bhi asli jawab ho sakta)
     if (dp[state] != -1) return dp[state];      // cache HIT
     return dp[state] = <relation>;              // compute + STORE
 // RELATION lagbhag same (take/not-take, min/max), aasaan. MUSHKIL = BASE + invalid handling, har problem
 //   me ALAG (climbing / robber = base simple; coin change = i<0 vs i==0, INT_MAX, -1 -> wahi phasaya).
 //   => relation likho, BASE pe RUK ke socho.

── FAMILY: 1D index DP (Fibonacci shape / take-skip) ──

 ▸ CLIMBING STAIRS (LC-70)  = Fibonacci-shape (ways = ways(n-1)+ways(n-2))
     int solve(int n, vector<int> &dp)   // step n pe: (n-1) se 1 step YA (n-2) se 2 step
         if (n == 0 || n == 1) return 1;     // base: n==0 bhi 1 (2-step se seedha top = ek tareeka)
         if (dp[n] != -1) return dp[n];      // cache HIT
         return dp[n] = solve(n - 1, dp) + solve(n - 2, dp);   // compute + STORE (= Fibonacci)

     vector<int> dp(n + 1, -1); return solve(n, dp);   // n+1: dp[n] tak index chahiye
     // BOTTOM-UP: dp[0]=dp[1]=1; for (i=2..n) dp[i]=dp[i-1]+dp[i-2]; return dp[n]  (0,1 base pehle bhare)
     // plain recursion = solve(n-2) BAAR-BAAR (overlap) -> exponential. memo se O(n)

 ▸ DECODE WAYS (LC-91)  = climbing-stairs ka bhai, par har STEP pe VALIDITY check
     // A=1 .. Z=26. sirf ank mile ("226") -> kitne alag shabd? GINTI. 2|2|6=BBF · 22|6=VF · 2|26=BZ -> 3
     // har i pe 2 choice (1 ank -> i+1 · 2 ank -> i+2); FARAK: yahan har choice pehle VALID honi chahiye
     // "226" ka tree: "6" do jagah aata -> dusri baar memo se seedha (yahi dp[] ka kaam)
     int solve(int i, string s, int n, vector<int> &dp)
         if (i == n) return 1;               // i==n PEHLE (warna s[n] padhta); khatam = ek poora decoding
         if (s[i] == '0') return 0;          // RULE-1: 0 se letter shuru nahi ("01" bhi A nahi) -> mara
         if (dp[i] != -1) return dp[i];      // -1 = abhi nahi nikala
         int count = solve(i + 1, s, n, dp);   // 1 ank: hamesha valid (0 upar hi kat gaya)
         if (i + 1 < n)   // pehle i+1<n: aakhri char pe aage mat padho
             if (s[i] == '1' || (s[i] == '2' && s[i + 1] <= '6'))   // RULE-2: 2 ank sirf 10..26
                 count += solve(i + 2, s, n, dp);
         return dp[i] = count;

     int n = s.size();
     vector<int> dp(n + 1, -1); return solve(0, s, n, dp);
     // KAB: string ko TUKDON me kaato + har tukda RULE pass kare + kitne tareeke -> index pe DP
     //   (word-break bhi yahi shakl: wahan rule "dictionary me hai?", yahan "1..26 me hai?")

 ▸ HOUSE ROBBER (LC-198)  = har step pe TAKE/SKIP choice + max
     int solve(vector<int> &A, int i, vector<int> &dp)   // LAST index se peeche (top-down)
         if (i < 0) return 0;                // i-2 se array ke bahar: koi ghar nahi
         if (dp[i] != -1) return dp[i];      // cache HIT
         int take = A[i] + solve(A, i - 2, dp);   // LOOT: padosi i-1 chhodna padega -> i-2
         int notTake = 0 + solve(A, i - 1, dp);   // SKIP: pichhle ghar tak ka best
         return dp[i] = max(take, notTake);

     int n = A.size();
     vector<int> dp(n + 1, -1);
     return solve(A, n - 1, dp);   // n-1 = LAST index (0..n-1); climbing me n = step-count tha
     // climbing se FARAK: wahan sirf ADD; yahan har step CHOICE -> max(take, skip)
     // base i==0 -> nums[0] bhi likh sakte (robber II wala); i<0 -> 0 ke saath zaroori nahi, nateeja same

 ▸ HOUSE ROBBER II (LC-213)  = House Robber I + CIRCLE -> I ko 2 baar chala
     // ghar GOL: pehla (0) aur aakhri (n-1) PADOSI -> dono saath nahi. [2,3,2] -> 3 (2+2=4 GALAT)
     int solve(vector<int> &nums, int i, vector<int> &dp)   // = House Robber I, bina badle
         if (i < 0) return 0;
         if (i == 0) return nums[0];
         if (dp[i] != -1) return dp[i];
         int notTake = 0 + solve(nums, i - 1, dp); int Take = nums[i] + solve(nums, i - 2, dp);
         return dp[i] = max(notTake, Take);

     int rob1(vector<int> &nums)
         int n = nums.size();
         vector<int> dp(n + 1, -1);   // har run ka dp alag (rob1 ke andar naya)
         return solve(nums, n - 1, dp);

     int n = nums.size();
     if (n == 1) return nums[0];             // ek ghar: dono temp khaali rehte -> 0 aata, galat
     vector<int> temp1, temp2;
     for (int i = 0; i < n; i++)
         if (i != 0) temp1.push_back(nums[i]);   // RUN-A: pehla ghar hata
         if (i != n - 1) temp2.push_back(nums[i]);   // RUN-B: aakhri ghar hata
     return max(rob1(temp1), rob1(temp2));   // ek ghar hatao -> gol toot ke seedhi line -> normal robber

── FAMILY: TAKE / NOT-TAKE on index (coin / LIS / knapsack / subset) ──

 ▸ COIN CHANGE (LC-322)  = MIN coins + REUSE (dono form, backtracking jaisa)
     // amount banane ko MIN coins (har coin UNLIMITED). na ban sake -> -1
     // amount==0 = THEEK bana -> 0 coins · amount<0 = overshoot -> INVALID (ban-na = theek 0 pe rukna)
     // 3 TRAP: (1) INT_MAX = "ban nahi sakta" (min me haarta) (2) overflow guard (3) caller INT_MAX -> -1
     // ── FORM A: TAKE / NOT-TAKE (index i, 2D dp[i][amount]) ──
     int solve(vector<int> &coins, int amount, int i, vector<vector<int>> &dp)
         if (amount < 0) return INT_MAX;     // INVALID (INT_MAX: min me apne aap haar jaata)
         if (i < 0)
             return amount == 0 ? 0 : INT_MAX;   // koi coin nahi bacha (i<0, NA i==0 -- coin[0] usable)
         if (dp[i][amount] != -1) return dp[i][amount];
         int notTake = solve(coins, amount, i - 1, dp);   // coin i chhoda -> agla
         int take = INT_MAX;   // INT_MAX se: na le paaye to min pe asar nahi
         if (solve(coins, amount - coins[i], i, dp) != INT_MAX)   // overflow guard (1+INT_MAX -> negative)
             take = 1 + solve(coins, amount - coins[i], i, dp);   // coin i liya, i SAME = reuse
         return dp[i][amount] = min(take, notTake);

     int n = coins.size();
     vector<vector<int>> dp(n + 1, vector<int>(amount + 1, -1));   // amount+1: dp[..][amount] tak
     int ans = solve(coins, amount, n - 1, dp); return ans == INT_MAX ? -1 : ans;   // na ban sake -> -1

     // ── FORM B: FOR-LOOP (saare coins, 1D dp[amount]) ── SAME idea, likhawat alag (03_coin_change_REDO_1)
     int solve(vector<int> &coins, int amount, int i, vector<int> &dp)
         if (amount < 0) return INT_MAX;         // overshoot: ye raasta nahi banta
         if (amount == 0) return 0;              // theek ban gaya
         if (dp[amount] != -1) return dp[amount];
         int Take = INT_MAX;   // koi coin kaam na aaye -> nahi-banta signal
         for (int i = 0; i < coins.size(); i++)   // har coin TRY (choose khud loop me)
             if (solve(coins, amount - coins[i], i, dp) != INT_MAX)   // overflow guard
                 Take = min(Take, 1 + solve(coins, amount - coins[i], i, dp));   // sabme se MIN
         return dp[amount] = Take;   // caller: dp(amount+1, -1); INT_MAX -> -1
     // dono SAME: TAKE/NOT-TAKE = index pe 2 branch · FOR-LOOP = sirf take, NOT-TAKE apne aap (loop chunta)
     //   for-loop me 'i' state nahi -> 1D dp; 2D me state = (i, amount)

 ▸ LONGEST INCREASING SUBSEQUENCE (LC-300)  = TAKE/NOT-TAKE + prev-track
     int solve(int index, int prevIndex, vector<int> &nums, vector<vector<int>> &dp)
         if (index == nums.size()) return 0; // base saaf (coin change jaisa INT_MAX / -1 nahi)
         if (dp[index][prevIndex + 1] != -1)   // +1: prevIndex -1 se shuru, dp[-1] crash hota
             return dp[index][prevIndex + 1];
         int notTake = 0 + solve(index + 1, prevIndex, nums, dp);   // chhoda -> prev wahi
         int Take = 0;
         if (prevIndex == -1 || nums[index] > nums[prevIndex])   // STRICTLY increasing rahe TABHI take
             Take = 1 + solve(index + 1, index, nums, dp);   // liya -> ab prev = index
         return dp[index][prevIndex + 1] = max(Take, notTake);

     int n = nums.size();
     vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));   // n+1 column: prev -1..n-1 shift -> 0..n
     return solve(0, -1, nums, dp);
     // coin change se TWIST: (1) state me PREV bhi (2) take pe CONDITION (nums[i] > nums[prev])

── FAMILY: 2D GRID DP (up + left) ──

 ▸ UNIQUE PATHS (LC-62)  = 2D DP intro (climbing-stairs ka 2D cousin)
     int solve(int i, int j, vector<vector<int>> &dp)   // robot (0,0) -> (m-1,n-1), sirf RIGHT / DOWN
         if (i < 0 || j < 0) return 0;       // grid ke bahar -> koi raasta nahi
         if (i == 0 && j == 0) return 1;     // start cell
         if (dp[i][j] != -1) return dp[i][j];
         int up = solve(i - 1, j, dp);   // upar se aaye (robot sirf down chalta)
         int left = solve(i, j - 1, dp);   // left se aaye (robot sirf right chalta)
         return dp[i][j] = up + left;   // dono taraf ke raaste jodo

     vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
     return solve(m - 1, n - 1, dp);
     // climbing se CONNECT: ways(n-1)+ways(n-2) -> ways(i-1,j)+ways(i,j-1). state (i,j), base do, memo 2D

 ▸ MIN PATH SUM (LC-64)  = UNIQUE-PATHS reuse, bas MIN + cost
     int solve(int i, int j, vector<vector<int>> &dp, vector<vector<int>> &grid)
         if (i < 0 || j < 0) return INT_MAX; // unique paths me 0 tha -> ab INVALID, min ignore karega
         if (i == 0 && j == 0) return grid[0][0];   // unique paths me 1 tha -> ab start ki COST
         if (dp[i][j] != -1) return dp[i][j];
         int up = solve(i - 1, j, dp, grid);   // sirf upar aur left se aa sakte (right / down chaal)
         int left = solve(i, j - 1, dp, grid);
         return dp[i][j] = grid[i][j] + min(up, left);   // up + left ki jagah: apni cost + CHHOTA raasta

     int m = grid.size(); int n = grid[0].size();
     vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
     return solve(m - 1, n - 1, dp, grid);
     // INT_MAX = coin change ka INVALID signal (asli cell pe >=1 taraf valid -> overflow nahi)

── FAMILY: 2 STRING DP (i, j peeche se) ──

 ▸ LONGEST COMMON SUBSEQUENCE (LC-1143)  = 2D DP on 2 STRINGS (match/no-match)
     int solve(string s1, string s2, int i, int j, vector<vector<int>> &dp)   // s1 i tak, s2 j tak
         if (i < 0 || j < 0) return 0;       // koi string khatam -> common kuch nahi
         if (dp[i][j] != -1) return dp[i][j];
         if (s1[i] == s2[j])
             return dp[i][j] = 1 + solve(s1, s2, i - 1, j - 1, dp);   // MATCH -> DONO peeche (diagonal)
         return dp[i][j] = max(solve(s1, s2, i - 1, j, dp), solve(s1, s2, i, j - 1, dp));   // ek char chhodo

     int m = s1.size();
     int n = s2.size();
     vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
     return solve(s1, s2, m - 1, n - 1, dp);
     // "2-string DP" ka base (edit distance isi pe)

 ▸ EDIT DISTANCE (LC-72, Hard)  = LCS reuse -- no-match 3-choice + base 2-sided
     int solve(int i, int j, string s1, string s2, vector<vector<int>> &dp)   // s1 -> s2 min ops
         if (i < 0) return j + 1;            // s1 khatam -> bache s2 ke j+1 char INSERT
         if (j < 0) return i + 1;            // s2 khatam -> s1 ke i+1 char DELETE
         if (dp[i][j] != -1) return dp[i][j];
         if (s1[i] == s2[j]) return dp[i][j] = 0 + solve(i - 1, j - 1, s1, s2, dp);   // MATCH -> koi op nahi
         return dp[i][j] = 1 + min({solve(i - 1, j, s1, s2, dp),   // DELETE: s1 ka char NIKAALA -> i--
                                    solve(i, j - 1, s1, s2, dp),   // INSERT: s2 ke liye DAALA -> j--
                                    solve(i - 1, j - 1, s1, s2, dp)});   // REPLACE: dono nipte -> i--, j--

     int longestCommonSubsequence(string s1, string s2)   // LCS wala caller hi reuse
         int m = s1.size();
         int n = s2.size();
         vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
         return solve(m - 1, n - 1, s1, s2, dp);

     return longestCommonSubsequence(w1, w2);
     // LCS se FARAK: (1) no-match = 1 + MIN of 3 (LCS me max of 2) (2) base = j+1 / i+1 (LCS me 0)
     // CRUX: DELETE = s1 se nikaala (i ghata) · INSERT = s2 ke liye daala (j ghata) -> ulta index

── FAMILY: 0/1 KNAPSACK (item ONCE -> i-1) ──

 ▸ 0/1 KNAPSACK (classic)  = PURE take/not-take (item ONCE)
     // MAX value, weight <= W, har item ek baar
     int solve(vector<int> &wt, vector<int> &val, int W, int i, vector<vector<int>> &dp)
         if (i < 0) return 0;                // koi item nahi (i<0: index-0 normal item, cleaner)
         if (dp[i][W] != -1) return dp[i][W];
         int notTake = 0 + solve(wt, val, W, i - 1, dp);   // item chhoda
         int take = 0;   // fit na ho to take ka koi value nahi
         if (wt[i] <= W)   // fit tabhi (warna W negative -> dp index crash)
             take = val[i] + solve(wt, val, W - wt[i], i - 1, dp);   // liya -> i-1 (ONCE)
         return dp[i][W] = max(take, notTake);

     int n = wt.size();
     vector<vector<int>> dp(n + 1, vector<int>(W + 1, -1));
     return solve(wt, val, W, n - 1, dp);
     // coin change se FARAK: item ONCE -> take me i-1 (coin me REUSE -> i same)

 ▸ SUBSET SUM (boolean)  = 0/1 KNAPSACK ka BOOLEAN bhai
     // knapsack ka max-value ki jagah true/false
     bool solve(vector<int> &nums, int target, int index, vector<vector<int>> &dp)
         if (target == 0) return true;       // SUCCESS -- SABSE UPAR (BASE-ORDER TRAP, neeche dekh)
         if (index < 0) return false;        // numbers khatam, target bacha
         if (index == 0) return nums[0] == target;   // aakhri number: sirf isse target ban sakta?
         if (dp[index][target] != -1) return dp[index][target];   // -1 = nahi nikala (0 = asli false)
         bool notTake = solve(nums, target, index - 1, dp);
         bool take = false;
         if (nums[index] <= target)   // warna target negative -> dp crash
             take = solve(nums, target - nums[index], index - 1, dp);
         return dp[index][target] = take || notTake;   // koi ek raasta true -> true

     int n = nums.size();
     vector<vector<int>> dp(n + 1, vector<int>(target + 1, -1));
     return solve(nums, target, n - 1, dp);
     // BASE-ORDER TRAP: target==0 UPAR nahi to index==0 wale case me target 0 aaya -> nums[0]==0 = false
     //   -> success MISS. [5,3], T=3 -> {3} true hona chahiye, galat false aata

 ▸ PARTITION EQUAL SUBSET SUM (LC-416)  = SUBSET-SUM ka delta
     // [1,5,11,5] -> S=22 -> {11} = {1,5,5} = 11 -> true. ek subset S/2 bana -> baaki apne aap S - S/2
     // solve() = upar SUBSET SUM wala, hubahu
     bool subsetSum(vector<int> &nums, int target, int n, vector<vector<int>> &dp)
         return solve(nums, target, n - 1, dp);

     int S = 0;
     for (int i = 0; i < nums.size(); i++) S += nums[i];
     int n = nums.size();
     vector<vector<int>> dp(n + 1, vector<int>((S / 2) + 1, -1));   // column S/2+1: target ab S/2
     if (S % 2 != 0) return false;           // ODD -> aadha ho hi nahi sakta
     return subsetSum(nums, S / 2, n, dp);   // target = S/2 -> wahi boolean subset sum

── FAMILY: STATE me COUNTER (choice per char) ──

 ▸ VALID PAREN with '*' (LC-678)  = choice-per-char, 2D dp[index][open]
     // '*' = '(' YA ')' YA khaali. open = ab tak kitne '(' UNMATCHED. state = (index, open)
     bool solve(string &s, int index, int open, vector<vector<int>> &dp)
         if (index == s.size()) return open == 0;   // sab '(' match ho gaye?
         if (dp[index][open] != -1) return dp[index][open];
         if (s[index] == '(')
             return dp[index][open] = solve(s, index + 1, open + 1, dp);   // ek hi raasta, open badhao
         else if (s[index] == ')')
             if (open > 0)   // band karne ko koi khula chahiye
                 return dp[index][open] = solve(s, index + 1, open - 1, dp);
             else return false;
         bool open_for_astrisk = solve(s, index + 1, open + 1, dp);   // '*' as '('
         bool close_for_astrisk;
         if (open > 0)   // GUARD: open-1 se pehle open>0 (warna dp[-1] crash)
             close_for_astrisk = solve(s, index + 1, open - 1, dp);   // '*' as ')'
         else close_for_astrisk = false;     // '*' pe seedha return false MAT: baaki 2 option zinda
         bool none = solve(s, index + 1, open, dp);   // '*' khaali
         return dp[index][open] = open_for_astrisk || close_for_astrisk || none;

     int n = s.size();
     vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));   // open 0..n (worst = saare '(')
     return solve(s, 0, 0, dp);
     // = coin change ka cousin (choice + memo); DELTA: state me 'open' + '*' pe 3 branch OR (true/false)

── FAMILY: INTERVAL DP (range (i, j), WALL k pe loop + recurse) ──

 ▸ MATRIX CHAIN MULT / MCM  = INTERVAL-DP  = two_max WALL + loop + recurse
     // chain multiply: result SAME, COST order pe -> MIN. p[] = dims, matrix A_i = p[i-1] x p[i] (i = 1..n)
     //   p = [10,30,5,60]: A1 = 10x30, A2 = 30x5 (p1 = 30 SHARED), A3 = 5x60
     // = two_max ka WALL; FARAK: wahan left/right precomputed, yahan ranges overlap -> loop + recurse + memo
     int solve(int i, int j, vector<int> &p, vector<vector<int>> &dp)   // A_i..A_j ko 1 banane ka MIN cost
         if (i == j) return 0;               // ek hi matrix bacha -> multiply ko kuch nahi
         if (dp[i][j] != -1) return dp[i][j];
         int ans = INT_MAX;   // MIN chahiye: bade se shuru
         for (int k = i; k <= j - 1; k++)   // k = WALL: (i..k)|(k+1..j), har side >=1 -> k max j-1
             ans = min(ans, solve(i, k, p, dp) + solve(k + 1, j, p, dp) + p[i - 1] * p[k] * p[j]);
         // left p[i-1]xp[k] guna right p[k]xp[j]
         return dp[i][j] = ans;

     int n = p.size();
     vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
     return solve(1, n - 1, p, dp);   // 1 se: i=0 pe p[i-1] = p[-1] crash; n-1 = matrices
     // p[i-1] = poore left ka baayaan chhor · p[k] = WALL · p[j] = poore right ka daayaan chhor
     // [10,30,5,60], k=1: left A1 = 10x30, right A2A3 -> 30x60, combine = 10*30*60 = p0*p1*p3
     // real: DB JOIN-order. interval-DP family: burst balloons, stone merge, optimal BST

 ▸ BURST BALLOONS (LC 312)  = INTERVAL-DP  = MCM WALL + loop + recurse
     // phodne pe coins = left * khud * right (padosi = us waqt ZINDA) -> saare phod ke MAX
     // k = range me SABSE AAKHRI phootne wala -> baaki sab phoot chuke -> padosi = range ke BAHAR: i-1, j+1
     //   [1,3,1,5,8,1], solve(1,4), k=3 (5) aakhri -> coins = p[0]*p[3]*p[5] = 1*5*1
     int solve(int i, int j, vector<int> &p, vector<vector<int>> &dp)   // i..j sab phodne ka MAX coins
         if (i > j) return 0;                // khaali range. i==j base NAHI: wo balloon abhi phootega
         if (dp[i][j] != -1) return dp[i][j];
         int ans = INT_MIN;   // MAX chahiye: chhote se shuru
         for (int k = i; k <= j; k++)   // j TAK: har balloon aakhri ban sakta (MCM me j-1)
             ans = max(ans, solve(i, k - 1, p, dp) + solve(k + 1, j, p, dp) + p[i - 1] * p[k] * p[j + 1]);
         // k REMOVED -> dono side se bahar: (i,k-1) + (k+1,j)
         return dp[i][j] = ans;

     int matrixChain(vector<int> &p, int n, vector<vector<int>> &dp)   // MCM jaisa caller naam
         return solve(1, n, p, dp);   // asli balloon idx 1..n

     int n = balloons.size();   // n PAD se PEHLE (asli count)
     vector<vector<int>> dp(n + 1, vector<int>(n + 1, -1));
     balloons.insert(balloons.begin(), 1);   // dono taraf 1 PAD: kinaare ka gaayab padosi = 1
     balloons.push_back(1);   //   (1 = guna ki identity; 0 se pad -> coin 0, galat)
     return matrixChain(balloons, n, dp);
     // [1,3,2,5,1], solve(1,3), k=2: left 1*3*2=6 + right 2*5*1=10 + k 1*2*1=2 = 18 (MCM me guna, yahan JOD)
```

---

---

## PATTERN 18 — GREEDY

```

 BROAD IDEA: har step pe ABHI-KA-BEST choice pakdo (local best), aage ki poori planning nahi.
   Kaam karta SIRF jab abhi-ka-best = aage-bhi-best (greedy-choice property); warna DP (jo sab explore kare).
   SIGNATURE: aksar pehle SORT (kisi key pe: size / finish-time) -> phir ek LINEAR pass me pick / skip.

 ▸ ASSIGN COOKIES (LC-455)  = greedy intro (sort + 2-pointer)
     // g[] = bachhe ki GREED, s[] = cookie SIZE. s[j] >= g[i] -> khush. max kitne khush?
     // chhoti greed wale ko jitni ZAROORAT utni (chhoti se chhoti jo kaam kare) -> badi cookies bade greed ko
     int n = g.size(), m = s.size(), count = 0, i = 0, j = 0;   // i = bachha, j = cookie
     if (s.size() == 0) return 0;            // cookie hi nahi -> 0
     sort(begin(g), end(g)), sort(begin(s), end(s));   // dono SORT
     while (i < n && j < m)   // koi ek khatam -> ruk
         if (s[j] >= g[i]) i++, j++, count++;   // >= : barabar size bhi khush karti · khush -> dono aage
         else j++;                           // cookie chhoti -> badi dhoondho
     return count;   // [10,9,8,7], [5,6,7,8] -> 2 (7->7, 8->8)

 ▸ JUMP GAME (LC-55)  = greedy REACHABILITY (farthest-reach track)
     // nums[i] = us index se max kitna kood sakte. last tak pahunche?
     int maxReach = 0;   // ab tak sabse DOOR kahan tak pahunch sakte (0 se)
     for (int i = 0; i < nums.size(); i++)
         if (i > maxReach) return false;     // index i KHUD maxReach se aage = yahan pahunch hi nahi
         maxReach = max(maxReach, i + nums[i]);   // i + nums[i]: nums[i] = kitne STEP (relative), i jodo
                                                  //   ->
     //   ABSOLUTE index ([2,3,1,1,4]: index 1 -> 1+3 = 4)
     return maxReach >= nums.size() - 1;   // O(n), koi DP nahi. [2,3,1,1,4] -> true
     // cookies se FARAK: wahan sort + 2-pointer; yahan sort nahi, ek scan me farthest-reach (greedy ke 2
     //   rang)

 ▸ NON-OVERLAPPING INTERVALS (LC-435)  = greedy interval-scheduling (keep SMALLER-END)
     // MIN kitne HATAO ki overlap na bache
     sort(begin(intervals), end(intervals));   // default sort (start pe)
     int start = 0, end = 1, count = 0;   // start = rakha hua, end = candidate
     while (end < intervals.size())
         if (intervals[start][1] <= intervals[end][0])   // NO overlap (<= : chhoona overlap nahi)
             start = end, end++;             // candidate safe, rakho
         else   // OVERLAP
             count++;   // ek to hatana hi
             if (intervals[start][1] > intervals[end][1])
                 start = end;   // chhota-END wala rakho: aage zyada jagah -> zyada fit
             end++;   // agla candidate
     return count;   // removals. O(n log n)
     // EDGE: overlap pe BLINDLY start rakhna GALAT -> [[1,100],[2,3],[3,4]] pe 2 aata (sahi 1). end-compare
     //   zaroori.
     //   (alt: END pe sort -> pehla wala hamesha chhota-end -> ye if gayab)

 ▸ GAS STATION (LC-134)  = greedy running-tank + reset-start
     // N pump CIRCLE me. gas[i] = pump i pe milta, cost[i] = i se i+1 tak kharcha. khaali tank, kahan se
     //   chalu ki poora ghoom ke wapas aa jau? koi nahi -> -1. har pump ka NET = gas[i] - cost[i]
     int n = gas.size();
     int index = 0, meriGaddiKaTank = 0, kyeNegativeHua = 0;   // 0: pehle pump ko start maano, tank khaali
     for (int i = 0; i < n; i++)
         meriGaddiKaTank += gas[i] - cost[i];   // abhi wale start se tank -> doobe to reset ("start
                                                //   kahan")
         kyeNegativeHua += gas[i] - cost[i];   // poore circle ka total, KABHI reset nahi ("possible hai?")
         if (meriGaddiKaTank < 0)   // is start se pump i+1 tak nahi pahunche
             meriGaddiKaTank = 0;   // naya start, khaali tank
             index = i + 1;   // AGLE pump pe khiska do: s..i ke beech se shuru karke bhi
         //   i paar nahi hota (beech ka tank >= 0 tha, uske bina aur kam)
     return kyeNegativeHua < 0 ? -1 : index;   // total kam -> -1; warna LAST reset index
     // gas=[1,2,3,4,5] cost=[3,4,5,1,2]: net -2 -2 -2 +3 +3 -> p0,p1,p2 pe reset, total 0 -> index 3
     // cookies / jump / interval se FARAK: sort NAHI. circular + 2 accumulator (possible? + start kahan)

 ▸ TASK SCHEDULER (LC-621)  = greedy IDLE-SLOTS (most-frequent frame)
     // SAME task dobara se pehle n gap. minimum total TIME (idle bhi)? signal: "cooldown" + "minimum time"
     //   -> greedy (most-frequent frame) YA max-heap. sabse ZYADA wala task (maxFreq) DHANCHA banata,
     //   beech ke gaps doosre tasks se bharo, khaali bache = IDLE
     vector<int> mp(26);
     for (int i = 0; i < tasks.size(); i++) mp[tasks[i] - 'A']++;   // freq count (26-size)
     sort(begin(mp), end(mp)); int maxFreq = mp[25];   // ascending: sabse badi freq aakhri index 25 pe
     int gadde = maxFreq - 1;   // gaps sirf BEECH me: A _ _ A _ _ A (3 A -> 2). aakhri A ke baad
                                             // koi agla A nahi -> cooldown kisi ko nahi rokta (maxFreq
                                             //   NAHI)
     int idleSlotes = n * gadde;   // har gadde me n slot. gadda (gap) alag, slot (1 unit) alag
     for (int i = 24; i >= 0; i--)   // 24 se: 25 = maxFreq khud, use chhodo
         idleSlotes -= min(mp[i], gadde);   // ek task har gadde me SIRF EK copy (2 = apna cooldown toota)
                                             //   -> max gadde slots bhar sakta; kam copies -> utne hi
     if (idleSlotes > 0) return tasks.size() + idleSlotes;   // kuch gap sach me KHAALI -> length + idle
     return tasks.size();   // <= 0: saare gap bhar gaye -> LENGTH wins
     // [A,A,A,B,B,B] n=2: gadde 2, idle 4, B bhare min(3,2)=2 -> idle 2 -> 6+2 = 8  (A B idle A B idle A B)
     // length-wins: [A3,B3,C3,D2,E2] n=3 -> idle -2 -> 13
     // alt: max-heap by freq + cooldown-queue (jab "actual schedule" poochein)
```
