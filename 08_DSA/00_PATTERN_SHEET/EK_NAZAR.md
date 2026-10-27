# EK NAZAR — OA / interview se pehli raat

> Sirf yaad dilane ke liye: har pattern ka KAB + har sawaal ka card (Maanga · Pehchaan · Chaabi kadam) + DHYAAN.
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

**SAWAAL (18):**
```text
MOVE ZEROES
  Maanga:    saare 0 END me, baaki ka order wahi, IN-PLACE
  Pehchaan:  in-place "rakhna / hatana" -> slow / fast overwrite
  Chaabi:
    1. slow = 0, fast = 0                         (slow = agli jagah jahan non-zero rakhna)
    2. while (fast < n): nums[fast] != 0 ho to swap(nums[slow], nums[fast]), PHIR slow++
    3. fast++ har baar                            (if ke BAHAR)

REMOVE DUP (SORTED)
  Maanga:    SORTED array, dohra IN-PLACE hatao, unique ginti k lautao
  Pehchaan:  sorted + in-place -> slow / fast (dohra hamesha bagal me)
  Chaabi:
    1. slow = 0, fast = 0                         (slow = aakhri UNIQUE ka index; pehla khud unique)
    2. while (fast < n): nums[slow] != nums[fast] ho to PEHLE slow++, PHIR nums[slow] = nums[fast]
       (move zeroes se ULTA kram)
    3. fast++ har baar
    4. return slow + 1                            (slow index hai, ginti nahi)

REMOVE ELEMENT
  Maanga:    saare val IN-PLACE hatao, bache ki ginti k lautao
  Pehchaan:  in-place hatana -> slow / fast
  Chaabi:
    1. slow = 0, fast = 0
    2. while (fast < n): nums[fast] != val ho to nums[slow] = nums[fast], PHIR slow++   (move zeroes jaisa)
    3. val mila to kuch nahi, sirf fast++
    4. return slow

VALID PALINDROME (SIMPLE)
  Maanga:    sirf lowercase string, aage-peeche same? true / false
  Pehchaan:  dono sire se compare -> left / right
  Chaabi:
    1. left = 0, right = s.size() - 1             (aakhri index, size nahi)
    2. while (left <= right): s[left] != s[right] -> turant false
    3. warna left++, right--
    4. loop ke baad return true

VALID PALINDROME (FULL, LC-125)
  Maanga:    kuch bhi char; sirf letter + digit gino, case ignore; palindrome?
  Pehchaan:  dono sire se compare + kachra skip
  Chaabi:
    1. left = 0, right = s.size() - 1, while (left <= right) me 4 raaste:
    2. tolower(s[left]) == tolower(s[right]) -> dono andar
    3. else if !isalnum(s[left]) -> left++ ; else if !isalnum(s[right]) -> right--
    4. else (dono alnum par alag) -> return false
    5. loop ke baad return true                   (khaali / koi alnum nahi -> true)

VALID PALINDROME II
  Maanga:    palindrome hai, ya ek char hata ke ban sakti? true / false
  Pehchaan:  dono sire se compare + ek galti ki chhoot
  Chaabi:
    1. helper isPalindrome(s, left, right): simple wala hi, bas range di hui
    2. main: left = 0, right = n - 1, while (left <= right)
    3. mismatch pe return isPalindrome(s, left + 1, right) || isPalindrome(s, left, right - 1)
       (left hatao YA right hatao)
    4. match -> left++, right--; loop ke baad true

REVERSE VOWELS
  Maanga:    sirf vowels aapas me ulte, baaki char apni jagah
  Pehchaan:  dono sire se, sirf kuch char swap -> left / right
  Chaabi:
    1. helper isVowel(ch): a e i o u, lower + UPPER dono
    2. left = 0, right = n - 1, while (left < right)   (< : beech ka akela swap nahi)
    3. dono vowel -> swap + left++ right--
    4. else if left vowel nahi -> left++ ; else -> right--
    5. return s

CONTAINER MOST WATER
  Maanga:    do line chuno, paani = min(dono) x doori, MAX paani
  Pehchaan:  do sire + value dekh ke kaunsa pointer chale
  Chaabi:
    1. low = 0, high = n - 1, max_area = INT_MIN
    2. while (low < high): width = high - low, area = width * min(height[low], height[high])
    3. max_area = max(max_area, area)
    4. CHHOTI height wala andar: height[low] < height[high] -> low++, warna high--   (barabar -> high--)
    5. return max_area

TWO SUM II (SORTED)
  Maanga:    SORTED array, do number ka jod = target, 1-BASED index
  Pehchaan:  sorted + jodi -> 2 pointer (unsorted hota to hashmap)
  Chaabi:
    1. left = 0, right = n - 1, while (left < right)
    2. jod == target -> return {left + 1, right + 1}
    3. jod < target -> left++ ; bada -> right--
    4. loop ke baad {-1, -1}

SUM OF SQUARE NUMBERS
  Maanga:    c diya, koi a, b hain jinke a*a + b*b == c? true / false
  Pehchaan:  do number ki jodi -> 2 pointer (0 se sqrt(c) tak)
  Chaabi:
    1. low = 0, high = sqrt(c)
    2. while (low <= high)                        (<= : c=0 jaisa ek hi point bhi)
    3. ans = (long long)low * low + (long long)high * high   (guna se PEHLE cast, overflow)
    4. == c -> true ; < c -> low++ ; > c -> high--
    5. loop ke baad false

SQUARES OF SORTED ARR
  Maanga:    sorted (minus bhi) ke square, result bhi sorted, O(n)
  Pehchaan:  sabse bada square dono SIRE pe -> peeche se bharo
  Chaabi:
    1. ans(n, -1), left = 0, right = n - 1, k = n - 1    (k = aakhri slot)
    2. while (left <= right)                     (<= : aakhri bacha ek bhi daalna)
    3. left ka square <= right ka square -> ans[k] = right ka square, right--, k--
    4. warna ans[k] = left ka square, left++, k--
    5. return ans

MERGE SORTED ARRAY
  Maanga:    nums1 (m asli + peeche n khaali) me nums2 IN-PLACE milao, sorted
  Pehchaan:  khaali jagah peeche -> peeche se bharo
  Chaabi:
    1. i = m - 1, j = n - 1, k = m + n - 1        (m*n-1 NAHI)
    2. while (i >= 0 && j >= 0): jo bada wo nums1[k] pe, uska pointer-- aur k--   (&& , || nahi)
    3. while (i >= 0): nums1 ke bache copy        (optional, pehle se jagah pe)
    4. while (j >= 0): nums1[k] = nums2[j]        (ZAROORI, nums2 se, nums1 se nahi)

SORT COLORS (0,1,2)
  Maanga:    sirf 0 1 2, in-place sort, ek pass, sort() nahi
  Pehchaan:  3 category me baanto -> Dutch flag, 3 pointer
  Chaabi:
    1. low = 0, mid = 0, high = n - 1, while (mid <= high)   (<= : high wala bhi dekhna)
    2. nums[mid] == 0 -> swap(low, mid), low++, mid++
    3. nums[mid] == 1 -> sirf mid++
    4. nums[mid] == 2 -> swap(mid, high), high--  (mid++ NAHI, aaya hua unknown)

SORT ARRAY BY PARITY
  Maanga:    saare EVEN pehle, phir ODD (aapas ka order kuch bhi)
  Pehchaan:  do category, galat jagah pe swap -> left / right
  Chaabi:
    1. left = 0, right = n - 1, while (left < right)
    2. left ODD aur right EVEN (dono galat) -> swap + left++ right--
    3. else if left even -> left++ ; else -> right--
       (left even + right odd = dono sahi, swap MAT)
    4. return nums

3SUM (LC-15)
  Maanga:    saare UNIQUE triplet jinka jod 0
  Pehchaan:  sort + ek FIX + baaki do pe two sum II + duplicate skip
  Chaabi:
    1. sort PEHLE
    2. for (i = 0..n-1): i > 0 && nums[i] == nums[i-1] -> continue   (skip 1: fixed ka dohra)
    3. andar low = i + 1, high = n - 1, sum = -nums[i], while (low < high)
    4. jod == sum -> push {nums[low], nums[high], nums[i]},
       phir while (low < high && dohra) low++ , same high--   (skip 2, 3), phir low++ high-- ek aur
    5. jod < sum -> low++ ; bada -> high--

IS SUBSEQUENCE
  Maanga:    t me se kuch char hata ke (order wahi) s ban sakta?
  Pehchaan:  do string, order se milao -> do pointer, ek-ek string pe
  Chaabi:
    1. i = 0 (s pe), j = 0 (t pe)
    2. while (i < s.size() && j < t.size())       (koi ek khatam -> ruko)
    3. s[i] == t[j] -> i++, j++ ; warna sirf j++
    4. i >= s.size() -> true, warna false

TRAPPING RAIN WATER (LC-42)
  Maanga:    buildings ke beech KUL kitna paani rukega
  Pehchaan:  har bar pe paani = min(leftMax, rightMax) - height -> 2 pointer O(1)
  Chaabi:
    1. left = 0, right = n - 1, leftMax = 0, rightMax = 0, ans = 0
    2. while (left < right): faisla DEEWAR pe -> height[left] <= height[right]
       (max pe NAHI)
    3. left wali taraf: leftMax > height[left] -> ans += leftMax - height[left], warna leftMax = height[left]; left++
    4. else right wali taraf same, rightMax ke saath; right--
    5. return ans

PALINDROMIC SUBSTRINGS (LC-647)
  Maanga:    kitne substring palindrome (ek char bhi, alag jagah = alag)
  Pehchaan:  har CENTER se dono taraf phailo
  Chaabi:
    1. helper solve(i, j, s, &count): while (i >= 0 && j < n && s[i] == s[j]) -> i--, j++, count++
       (&count, warna bahar badhta nahi; range check PEHLE)
    2. main: count = 0, for (i = 0..n-1)
    3. solve(i, i) odd center + solve(i, i + 1) even center
    4. return count
```

**DHYAAN:**
- 3-sum me duplicate SKIP karo (2-sum me nahi)
- container / trapping / histogram me confuse -> upar NAAM-SE-CONFUSE A

**POORA:** [PATTERN 1](PATTERN_SHEET.md#pattern-1--two-pointer)

---

## 02 SLIDING WINDOW

**KAB:** CONTIGUOUS subarray / substring + "sabse lambi / sabse chhoti / max / count" maanga

**SAWAAL (17):**
```text
LONGEST SUBSTRING NO-REPEAT (LC-3)
  Maanga:    sabse lambi substring jisme koi char repeat na ho -> LENGTH
  Pehchaan:  contiguous + longest -> variable window + freq-map
  Chaabi:
    1. khaali -> 0; ans = INT_MIN, mp, i = 0, j = 0
    2. while (j < n): mp[s[j]]++
    3. while (mp[s[j]] > 1): mp[s[i]]--, i++     (jo abhi aaya wahi repeat; WHILE)
    4. shrink ke BAAD ans = max(ans, j - i + 1), phir j++
    5. return ans == INT_MIN ? 0 : ans

CHAR REPLACEMENT (LONGEST, LC-424)
  Maanga:    max k char badal ke sab SAME, sabse lambi substring -> LENGTH
  Pehchaan:  longest + "kitne badalne" = window - maxFreq
  Chaabi:
    1. mp, maxLen = INT_MIN, maxFreq = INT_MIN, i = 0, j = 0
    2. while (j < n): mp[s[j]]++, maxFreq = max(maxFreq, mp[s[j]])   (update PEHLE)
    3. while (j - i + 1 - maxFreq > k): mp[s[i]]--, 0 hua to erase, i++
       (maxFreq kabhi GHATAO mat)
    4. j - i + 1 - maxFreq <= k -> maxLen = max(...), phir j++
    5. return maxLen

MAX CONSECUTIVE ONES III (LC-1004)
  Maanga:    max k zero flip, sabse lambi lagaatar 1 -> LENGTH
  Pehchaan:  longest + window me zero ginti
  Chaabi:
    1. ans = INT_MIN, zeroCount = 0, i = 0, j = 0
    2. while (j < n): nums[j] == 0 -> zeroCount++
    3. while (zeroCount > k): nums[i] == 0 ho to zeroCount--, PHIR i++
    4. ans = max(ans, j - i + 1) UNCONDITIONAL (if (==k) mat), j++
    5. return ans == INT_MIN ? 0 : ans

FRUIT INTO BASKETS
  Maanga:    2 basket = at most 2 type, sabse lambi lagaatar window -> LENGTH
  Pehchaan:  longest + at most 2 distinct -> map.size()
  Chaabi:
    1. ans = INT_MIN, mp, i = 0, j = 0
    2. while (j < n): mp[fruits[j]]++
    3. while (mp.size() > 2): mp[fruits[i]]--, 0 hua to mp.erase (CRUX), PHIR i++
    4. ans = max(ans, j - i + 1), j++
    5. return ans == INT_MIN ? 0 : ans

LONGEST AT-MOST-K DISTINCT
  Maanga:    sabse lambi substring jisme zyada se zyada k alag char -> LENGTH
  Pehchaan:  fruit ka general version: "2" ki jagah "k"
  Chaabi:
    1. fruit wala hi code (totalFruit(s, k)), bas while (mp.size() > k)
    2. erase-on-0 wahi
    3. longestKDistinct(s, k) = return totalFruit(s, k)

SUBARRAYS WITH K DISTINCT (LC-992)
  Maanga:    kitne subarray jinme EXACTLY k alag -> GINTI
  Pehchaan:  "exactly k" -> atMost(k) - atMost(k - 1)
  Chaabi:
    1. atMostK = fruit ka template, bas count = 0 (INT_MIN nahi)
    2. while (j < n): mp[x]++ ; while (mp.size() > k): --, erase-on-0, i++
    3. max ki jagah count += j - i + 1, j++
    4. main: return atMostK(nums, k) - atMostK(nums, k - 1)   (k=1 pe atMost(0) apne aap 0)

EQUAL SUBSTR WITHIN BUDGET (LC-1208)
  Maanga:    cost = |s[i]-t[i]|, total cost <= maxCost, sabse lamba window -> LENGTH
  Pehchaan:  longest + sum <= limit (string ke bhes me)
  Chaabi:
    1. i = 0, j = 0, cost = 0, ans = INT_MIN      (map nahi, sirf cost)
    2. while (j < n): cost += abs(s[j] - t[j])
    3. while (cost > maxCost): cost -= abs(s[i] - t[i]), i++
    4. ans = max(ans, j - i + 1), j++
    5. return ans

MIN SUBARRAY LEN (>=TARGET, LC-209)
  Maanga:    positive nums, sum >= target wala SABSE CHHOTA subarray, na ho to 0
  Pehchaan:  shortest -> VALID ho tab tak shrink, record ANDAR
  Chaabi:
    1. sum = 0, size = INT_MAX, i = 0, j = 0
    2. while (j < n): sum += nums[j]
    3. while (sum >= target): size = min(size, j - i + 1) PEHLE, phir sum -= nums[i], i++
    4. j++ (shrink loop ke bahar)
    5. return size == INT_MAX ? 0 : size

MAX SUM OF K (FIXED)
  Maanga:    har size-k window ka sum, sabse bada
  Pehchaan:  size fix -> IF (while nahi)
  Chaabi:
    1. sum = 0, maxSum = INT_MIN, i = 0, j = 0
    2. while (j < n): sum += nums[j]
    3. if (j - i + 1 >= k): maxSum = max(maxSum, sum), phir sum -= nums[i], i++
    4. j++ ; return maxSum

SUBARRAY PRODUCT < K
  Maanga:    positive nums, kitne subarray ka product < k -> GINTI
  Pehchaan:  count -> valid window pe count += j - i + 1
  Chaabi:
    1. k == 0 -> return 0 ; count = 0, i = 0, j = 0, prod = 1   (1, 0 nahi)
    2. while (j < n): prod *= nums[j]
    3. while (prod >= k && i <= j): prod /= nums[i], i++   (i <= j : k=1 pe bachav)
    4. count += j - i + 1, j++
    5. return count

MIN WINDOW SUBSTRING (LC-76)
  Maanga:    s ka sabse chhota substring jisme t ke saare char (ginti ke saath), na ho to ""
  Pehchaan:  shortest + need-map + count
  Chaabi:
    1. pehla loop: t ke har char pe mp[c]++ ; i = 0, j = 0, count = t.size(), minLen = INT_MAX, index = 0
    2. while (j < n): mp[s[j]] > 0 ho to count-- ; phir mp[s[j]]-- (IF ke BAHAR)
    3. while (count == 0): len < minLen -> minLen = len, index = i (record PEHLE)
    4. phir mp[s[i]]++, > 0 hua to count++, i++   (aana --, jaana ++ MIRROR)
    5. j++ ; return minLen == INT_MAX ? "" : s.substr(index, minLen)

SUBSTRINGS CONTAINING ALL (LC-1358)
  Maanga:    sirf a b c, kitne substring me teeno kam se kam ek baar -> GINTI
  Pehchaan:  min window ka SAME code, sirf answer ki line alag
  Chaabi:
    1. min window wala need-map + count, ans = 0, t = "abc"
    2. while (j < n): expand same (count--, mp--)
    3. while (count == 0): mp[s[i]]++, > 0 to count++, i++   (record nahi)
    4. inner while ke BAAD ans += i, j++
    5. numberOfSubstrings(s) = minWindow(s, "abc")

FIND ALL ANAGRAMS (LC-438)
  Maanga:    s me p ke saare anagram ki START index
  Pehchaan:  fixed p-size window + isAnagram helper
  Chaabi:
    1. helper isAnagram: size alag -> false ; s pe ++, t pe -- ; koi != 0 -> false
    2. i = 0, j = 0, while (j < n)
    3. while (j - i + 1 > p.size()) i++          (window p-size pe)
    4. j - i + 1 == p.size() aur isAnagram(s.substr(i, j - i + 1), p) -> ans.push_back(i)
    5. j++ ; return ans

PERMUTATION IN STRING (LC-567)
  Maanga:    s2 me s1 ka koi permutation lagaatar hai? true / false
  Pehchaan:  find all anagrams ka exact code
  Chaabi:
    1. find anagrams wala hi loop, push ki jagah return true
    2. loop ke baad return false
    3. checkInclusion(s1, s2) = findAnagrams(s2, s1)   (ulta order)

SLIDING WINDOW MAXIMUM (LC-239)
  Maanga:    har size-k window ka MAX
  Pehchaan:  window ka max / min -> monotonic deque of INDEX
  Chaabi:
    1. ans, deque dq ; for (i = 0..n-1)
    2. EXPIRY: !dq.empty() && dq.front() == i - k -> pop_front
    3. while (!dq.empty() && nums[dq.back()] <= nums[i]) -> pop_back   (chhote bekaar)
    4. dq.push_back(i)                            (index, value nahi)
    5. i >= k - 1 -> ans.push_back(nums[dq.front()])   (kram: expiry -> clean + push -> record)

MAX POINTS FROM CARDS (LC-1423)
  Maanga:    dono sire se kul k card, max sum
  Pehchaan:  ulta socho: jo NAHI liye = beech ka window, size n - k -> Total - min
  Chaabi:
    1. helper minSumK = max sum of k ka MIN version (minSum = INT_MAX, IF window)
    2. pehla loop: Total = saare cards ka sum
    3. n - k == 0 -> return Total                 (saare liye)
    4. minSum = minSumK(nums, n - k) ; return Total - minSum

CONTAINS DUPLICATE II (LC-219)
  Maanga:    nums[i] == nums[j] aur |i - j| <= k? true / false
  Pehchaan:  fixed window size k + hashset
  Chaabi:
    1. k <= 0 -> false ; i = 0, j = 0, unordered_set st
    2. while (j < n): st.count(nums[j]) -> return true   (check PEHLE)
    3. j - i + 1 > k -> st.erase(nums[i]), i++
    4. st.insert(nums[j]) (check ke BAAD), j++
    5. loop ke baad false
```

**DHYAAN:**
- shrink me count 0 ho jaaye to map se ERASE karo, warna `mp.size()` galat bolega
- NEGATIVE numbers ho to SW nahi chalti -> PREFIX + MAP
- "lagaataar" par har server / user ka alag -> upar NAAM-SE-CONFUSE E (map + ginti)

**POORA:** [PATTERN 2](PATTERN_SHEET.md#pattern-2--sliding-window)

---

## 03 HASHING

**KAB:** "pehle dekha?" · "jodi / complement hai?" · "kitni baar aaya?" · grouping (anagram)

**SAWAAL (11):**
```text
ISOMORPHIC STRINGS
  Maanga:    s, t same length. s ka har char ek FIXED char se badal ke t ban sakta (one-to-one)? true / false
  Pehchaan:  dono taraf 1-to-1 mapping -> DO map (mp1: s->t, mp2: t->s)
  Chaabi:
    1. mp1 (char -> char) aur mp2 (char -> char) banao
    2. ek loop i = 0..s.size()-1: pehle CHECK -> mp1 me s[i] hai aur != t[i], YA mp2 me t[i] hai aur != s[i] -> return false
       (count() pehle, seedha mp1[x] khaali key bana deta · || : kisi bhi EK taraf clash kaafi)
    3. else: dono taraf save -> mp1[s[i]] = t[i], mp2[t[i]] = s[i]          (check ke BAAD)
    4. loop poora chala -> return true                                      ("ab"->"aa" sirf ulti taraf pakadta)

WORD PATTERN
  Maanga:    pattern (chars) + s (words). har char ek hi word se, har word ek hi char se bandha? true / false
  Pehchaan:  = ISOMORPHIC, bas char <-> WORD (mp1: char -> string, mp2: string -> char)
  Chaabi:
    1. stringstream ss(s), string word, i = 0 (pattern index), j = 0 (words ki ginti)
    2. while (ss >> word): andar if (i < pattern.size()) -> isomorphic wali EXACT clash line (pattern[i] <-> word)
       clash -> return false, warna dono map me save, phir i++
    3. j++ if ke BAHAR                                                       (har word gino, extra word bhi)
    4. return pattern.size() == j                                            (warna "a","dog cat" galat true)

TWO SUM
  Maanga:    nums[] + target. do INDEX jinka sum = target
  Pehchaan:  "jodi / complement pehle dekha?" -> map {value -> index}
  Chaabi:
    1. unordered_map<int,int> mp
    2. ek loop i = 0..n-1: CHECK pehle -> mp.find(target - nums[i]) mila -> return {mp[target - nums[i]], i}
    3. daalo BAAD -> mp[nums[i]] = i                                        (taaki num khud se match na kare)
    4. loop ke baad return {-1, -1}

VALID ANAGRAM
  Maanga:    s, t. same char, same COUNT (bas order alag)? true / false
  Pehchaan:  har char ki ginti barabar? -> ek map, s se ++ t se --
  Chaabi:
    1. s.size() != t.size() -> turant false
    2. pehla loop s pe: mp[it]++
    3. doosra loop t pe: mp[it]--
    4. teesra loop map pe: koi it.second != 0 -> false                       (!= 0, sirf > 0 nahi, -ve bhi aata)
    5. return true

GROUP ANAGRAMS
  Maanga:    strings ka array. aapas me anagram wale ek group me, saare groups lautao
  Pehchaan:  grouping -> KEY = sorted(word), map {key -> words ki list}
  Chaabi:
    1. unordered_map<string, vector<string>> mp
    2. loop har word pe: string key = it (COPY lo) -> sort(key) -> mp[key].push_back(it)
       (asli word sort kiya to group me galat word jaata)
    3. doosra loop map pe: ans.push_back(it.second)                          (map ki har list = ek group)
    4. return ans

SUBARRAY SUM = K
  Maanga:    nums[] + k. kitne lagaatar subarray jinka sum EXACTLY k -> ginti
  Pehchaan:  sum == K ki GINTI (negative bhi) -> prefix sum + map {prefixSum -> count}
  Chaabi:
    1. mp, sum = 0, count = 0, aur mp[0] = 1                                 (START: jab sum khud == k)
    2. ek loop i = 0..n-1: sum += nums[i]
    3. count += mp[sum - k]                                                  (sum - k, k - sum NAHI)
    4. mp[sum]++                                                             (check ke BAAD daalo)
    5. return count

COUNT DIVISIBLE PAIRS
  Maanga:    nums[] + k. kitne PAIR (i<j) jinka (nums[i] + nums[j]) % k == 0 -> ginti
  Pehchaan:  two-sum jaisa "partner pehle aaya?", par KEY = REMAINDER (pair = 2 alag element, prefix nahi)
  Chaabi:
    1. mp {remainder -> kitne aaye}, count = 0
    2. loop har it pe: rem = ((it % k) + k) % k                              (C++ me minus ka % minus)
    3. count += mp[(k - rem) % k]                                            (rem = 0 -> partner bhi 0, isliye % k)
    4. mp[rem]++                                                             (lookup PEHLE, insert BAAD)
    5. return count

LONGEST CONSECUTIVE SEQ (LC-128)
  Maanga:    unsorted array. sabse lambi CONSECUTIVE chain ki LENGTH, O(n) (sort nahi)
  Pehchaan:  "x+1 hai?" baar-baar -> SET me O(1) lookup, chain sirf START se gino
  Chaabi:
    1. ans = INT_MIN, unordered_set st, pehle loop me saare nums insert
    2. doosra loop i = 0..n-1: num = nums[i]; START tabhi jab st me num-1 NAHI (warna beech ka, skip)
    3. START pe: currNum = num, count = 1 -> while (st.count(currNum + 1)) currNum++, count++
    4. while ke BAAD: ans = max(ans, count)
    5. return ans == INT_MIN ? 0 : ans                                       (khaali array -> 0)

MAJORITY ELEMENT (LC-169, > n/2)
  Maanga:    nums[] me ek element n/2 se ZYADA baar. wahi lautao
  Pehchaan:  O(1) space -> BOYER-MOORE VOTING (do alag element cancel, majority bachta)
  Chaabi:
    1. count = 1, candidate = nums[0]
    2. loop i = 1 se: candidate == nums[i] -> count++
    3. else -> count--, aur count == 0 hua to candidate = nums[i], count = 1   (0 pe RESET, warna -1 me jaata)
    4. return candidate

FIND DISAPPEARED NUMBERS (LC-448)
  Maanga:    size n, values 1..n (repeat bhi). 1..n me jo MISSING, sab lautao (O(1) extra)
  Pehchaan:  "values 1..n + O(1) space" -> array khud hashmap: value v ka ghar = index v-1, NEGATIVE mark
  Chaabi:
    1. pehla loop i = 0..n-1: index = abs(nums[i]) - 1                       (abs: pehle hi -ve ho chuki ho sakti)
    2. nums[index] > 0 ho tabhi nums[index] = -nums[index]                   (duplicate pe dobara minus = wapas plus)
    3. doosra loop i = 0..n-1: nums[i] > 0 -> ans.push_back(i + 1)           (+ve bacha = kabhi mark nahi = missing)
    4. return ans

SERVER ERROR STREAK (03/11)
  Maanga:    logs "<server> <code> <msg>". server ke LAGAATAAR 3 "error" -> alert++ aur ginti 0; "success" -> ginti 0. kul alerts
  Pehchaan:  beech me doosre server ki line -> ek window nahi; "KISKA lagaataar" = key = server -> map {server -> ginti}
  Chaabi:
    1. mp {server -> lagaataar error}, count = 0
    2. loop har line s pe: stringstream ss(s); string server, code, msg; ss >> server >> code >> msg
       (ek LINE do, poora vector nahi)
    3. msg == "error" -> mp[server]++, aur mp[server] >= 3 -> count++, mp[server] = 0   (faisla msg se, code se nahi)
    4. else if msg == "success" -> mp[server] = 0                            (= 0, -- NAHI)
    5. return count
```

**DHYAAN:**
- 1-to-1 mapping (isomorphic / word pattern) -> DO map, ek kaafi nahi
- longest consecutive vs LIS -> upar NAAM-SE-CONFUSE B

**POORA:** [PATTERN 3](PATTERN_SHEET.md#pattern-3--hashing)

---

## 04 PREFIX SUM

**KAB:** range sum baar-baar · "kitne subarray ka sum = K" (negative bhi) · left sum = right sum

**SAWAAL (7):**
```text
PIVOT INDEX
  Maanga:    pehla INDEX jahan left sum == right sum (index khud kisi taraf nahi), na ho to -1
  Pehchaan:  left / right sum baar-baar -> total pehle, right = total - left - khud
  Chaabi:
    1. totalSum = 0, leftSum = 0; pehle loop me poora totalSum
    2. doosra loop i = 0..n-1: rightSum = totalSum - leftSum - nums[i]
    3. leftSum == rightSum -> return i
    4. leftSum += nums[i]                                                    (BAAD me: check ke baad khud ko left me jodo)
    5. loop ke baad return -1

PRODUCT EXCEPT SELF (LC-238)
  Maanga:    ans[i] = nums[i] chhod ke baaki sabka product. division nahi, O(n)
  Pehchaan:  ans[i] = (i se PEHLE ka product) x (i ke BAAD ka product) -> left[] + right[]
  Chaabi:
    1. left(n,1), right(n,1), ans(n,1), prod = 1
    2. loop i = 0..n-1 aage: left[i] = prod (ASSIGN PEHLE), phir prod *= nums[i] (UPDATE BAAD)
    3. prod = 1 RESET, loop i = n-1..0 peeche: right[i] = prod, phir prod *= nums[i]
    4. teesra loop i = 0..n-1: ans[i] = left[i] * right[i]
    5. return ans

HIGHEST ALTITUDE
  Maanga:    gain[i] = agle point tak altitude change, shuru 0 se. sabse UNCHI altitude
  Pehchaan:  running sum + max (simple prefix)
  Chaabi:
    1. sum = 0, maxi = 0                                                     (shuru ki altitude 0 bhi gino)
    2. ek loop: sum += gain[i], maxi = max(maxi, sum)
    3. return maxi

SUBARRAY SUMS DIVISIBLE BY K (LC-974)
  Maanga:    nums[] (+/-) + k. kitne lagaatar subarray jinka sum % k == 0 -> ginti
  Pehchaan:  = SUBARRAY SUM K, bas KEY = remainder (do prefix ka rem same = beech wala divisible)
  Chaabi:
    1. mp, sum = 0, count = 0, mp[0] = 1                                     (SEED: shuru se hi divisible wala)
    2. loop har it pe: sum += it PEHLE
    3. phir rem = (sum % k + k) % k                                          (negative sum ka % negative)
    4. count += mp[rem], phir mp[rem]++
    5. return count

COUNT SUBARRAYS WITH XOR = K
  Maanga:    nums[] + k. kitne lagaatar subarray jinka XOR = k -> ginti
  Pehchaan:  = SUBARRAY SUM K, bas + ki jagah ^ (x ^ x = 0)
  Chaabi:
    1. mp, mp[0] = 1, xorr = 0, count = 0
    2. loop har it pe: xorr ^= it
    3. count += mp[xorr ^ k]                                                 (pichhla XOR jo chahiye)
    4. mp[xorr]++                                                            (lookup ke BAAD)
    5. return count

COUNT SUBARRAYS WITH MAX == K
  Maanga:    arr[] + K. kitne lagaatar subarray jinka MAX bilkul K -> ginti
  Pehchaan:  max FIXED K se compare -> SW / stack / heap nahi; K se bade = DEEWAR, do index track
  Chaabi:
    1. lastWall = -1, lastK = -1, long long count = 0                        (LONG LONG: n^2 ginti overflow)
    2. ek loop i = 0..n-1: arr[i] > K -> lastWall = i
    3. arr[i] == K -> lastK = i                                              (pehle dono update, PHIR count)
    4. lastK > lastWall ho tabhi count += lastK - lastWall                   (guard: warna negative)
    5. return count

DIFFERENCE ARRAY / RANGE UPDATE (LC-1109 Corporate Flight Bookings)
  Maanga:    n flights (1..n), booking [first,last,seats]. har flight ki kul seats
  Pehchaan:  har RANGE pe ADD, bahut range -> sirf do nishaan + ek chalti-sum (sweep line ka bhai)
  Chaabi:
    1. diff(n + 2)                                                           (end+1 sabse bada n+1)
    2. loop har booking pe: start = it[0], end = it[1], count = it[2] -> diff[start] += count, diff[end + 1] -= count
       (+1: last din bhi inclusive)
    3. cummSum = 0, loop i = 1..n (<= n): cummSum += diff[i], ans.push_back(cummSum)   (1-based)
    4. return ans
```

**DHYAAN:**
- `mp[0] = 1` bhoolna nahi ([3], K=3)
- divisible by k: C++ me minus % minus deta -> `((sum % k) + k) % k`
- max sum vs count sum == K -> upar NAAM-SE-CONFUSE C

**POORA:** [PATTERN 4](PATTERN_SHEET.md#pattern-4--prefix-sum)

---

## 05 MATRIX

**KAB:** 2D matrix pe index ka khel: spiral, transpose, set zeroes, row / col ginti

**SAWAAL (4):**
```text
SET MATRIX ZEROES
  Maanga:    m x n matrix. koi cell 0 -> uski POORI row aur col 0, IN-PLACE
  Pehchaan:  pehle mark, phir badlo -> 2 marker array (zeroRow, zeroCol), 2 pass
  Chaabi:
    1. row, col; zeroRow(row, -1), zeroCol(col, -1)                          (-1 = unmarked)
    2. pass-1 do loop (i row, j col): matrix[i][j] == 0 -> zeroRow[i] = 0, zeroCol[j] = 0   (sirf MARK)
    3. pass-2 do loop: zeroRow[i] == 0 || zeroCol[j] == 0 -> matrix[i][j] = 0
       (ek hi pass me 0 kiya to naye 0 bhi mark ho jaate)

SPIRAL MATRIX (LC-54)
  Maanga:    m x n matrix ke saare elements SPIRAL order (bahar se andar, clockwise)
  Pehchaan:  4 boundary andar simat-te: top, bottom, left, right
  Chaabi:
    1. top = 0, bottom = row-1, left = 0, right = col-1                      (inclusive, isliye loop <=)
    2. while (top <= bottom && left <= right), andar 4 loop:
    3. top row i = left..right [top][i] -> top++ ; right col i = top..bottom [i][right] -> right--
    4. if (top <= bottom): bottom row i = right..left ulta [bottom][i]; phir bottom--
    5. if (left <= right): left col i = bottom..top ulta [i][left]; phir left++   (guard sirf 3rd, 4th pe)
    6. return ans

TRANSPOSE
  Maanga:    m x n matrix ka transpose: matrix[i][j] -> res[j][i] (naya n x m)
  Pehchaan:  sirf index ulta, naya matrix col x row size
  Chaabi:
    1. ans(col, vector<int>(row, -1))                                        (col x row: non-square bhi)
    2. outer loop i = 0..col-1, inner loop j = 0..row-1
    3. ans[i][j] = matrix[j][i]
    4. return ans

COUNT SERVERS THAT COMMUNICATE (LC-1267, JP)
  Maanga:    grid, 1 = server. same ROW ya COL me doosra server ho to communicate. aise server ki ginti
  Pehchaan:  = set matrix zeroes ki 2-pass trick, MARK ki jagah COUNT
  Chaabi:
    1. rowCount(m, 0), colCount(n, 0)
    2. pass-1 do loop: grid[i][j] == 1 -> rowCount[i]++, colCount[j]++       (poori ginti pehle)
    3. count = 0; pass-2 do loop: grid[i][j] == 1 && (rowCount[i] > 1 || colCount[j] > 1) -> count++
       (> 1 : khud ke alawa koi aur)
    4. return count
```

**DHYAAN:**
- islands / rotting / max area matrix jaise dikhte hain par GRAPH (BFS / DFS) hain -> 13
- search 2D matrix = sorted -> BINARY SEARCH (07)
- copy-pen pe grid bana ke index chalao

**POORA:** [PATTERN 5](PATTERN_SHEET.md#pattern-5--matrix)

---

## 06 STACK

**KAB:** brackets match · "har element ka agla / pichhla bada ya chhota" · span · histogram

**SAWAAL (12):**
```text
NEXT GREATER ELEMENT
  Maanga:    har element ke DAAYE pehla BADA, koi nahi -> -1
  Pehchaan:  "agla bada" -> MONOTONIC stack, INDEX rakho, < compare
  Chaabi:
    1. ans(n, -1), stack<int> st (INDEX)
    2. loop i = 0..n-1 (left -> right)
    3. andar while (!st.empty() && nums[st.top()] < nums[i]): topp = st.top(), pop, ans[topp] = nums[i]
       (!empty pehle · < : barabar = bada nahi · top pehle bachao phir pop)
    4. while ke BAAD st.push(i)
    5. return ans                                                            (stack me bache = -1 hi)

NEXT SMALLER ELEMENT
  Maanga:    har element ke DAAYE pehla strictly CHHOTA, koi nahi -> -1
  Pehchaan:  = next greater, sirf condition ULTI (>)
  Chaabi:
    1. ans(n, -1), stack<int> st
    2. loop i = 0..n-1
    3. while (!st.empty() && nums[st.top()] > nums[i]): topp = top, pop, ans[topp] = nums[i]   (> strict, >= nahi)
    4. st.push(i), return ans

PREVIOUS SMALLER ELEMENT
  Maanga:    har element ke BAAYE pehla strictly CHHOTA, koi nahi -> -1
  Pehchaan:  = next smaller, sirf LOOP ULTA (condition wahi >)
  Chaabi:
    1. ans(n, -1), stack<int> st
    2. loop i = n-1..0 (right -> left)
    3. while (!st.empty() && nums[st.top()] > nums[i]): topp = top, pop, ans[topp] = nums[i]
    4. st.push(i), return ans

LARGEST RECTANGLE IN HISTOGRAM (LC-84)
  Maanga:    heights[] (width 1). lagaatar bars pe sabse bada rectangle ka AREA
  Pehchaan:  har bar pe khade ho: left me pehla CHHOTA + right me pehla CHHOTA = do stopper (INDEX)
  Chaabi:
    1. nextSmaller(): ans(n, n) DEFAULT n, loop 0..n-1, while top > curr -> ans[tp] = i (INDEX store)
    2. prevSmaller(): ans(n, -1) DEFAULT -1, loop n-1..0 ulta, wahi while, ans[tp] = i
       (> strict dono me: [3,3,3,3] me >= se galat)
    3. main: ns = nextSmaller, ps = prevSmaller, ans = INT_MIN
    4. loop i = 0..n-1: width = ns[i] - ps[i] - 1 -> ans = max(ans, heights[i] * width)
    5. return ans

MAXIMAL RECTANGLE (LC-85, 2D)
  Maanga:    0/1 ki 2D matrix. sirf 1 se bhara sabse bada rectangle ka AREA
  Pehchaan:  = LC-84 ka 2D bhai: HAR ROW = histogram, upar wala largestRectangleArea REUSE
  Chaabi:
    1. heights(col, 0) (COL size), maxi = INT_MIN
    2. outer loop i = 0..row-1, inner loop j = 0..col-1:
       matrix[i][j] == 1 -> heights[j] += 1, warna heights[j] = 0 (column toota -> RESET)
    3. j-loop ke BAHAR (har row ke baad): maxi = max(maxi, largestRectangleArea(heights))
    4. return maxi

DAILY TEMPERATURES
  Maanga:    har din ke liye kitne DIN BAAD pehla garam din, nahi -> 0
  Pehchaan:  = next greater, par ans me VALUE nahi DOORI (i - top)
  Chaabi:
    1. ans(n, 0) (0 init, -1 nahi), stack<int> st
    2. loop i = 0..n-1
    3. while (!st.empty() && temps[st.top()] < temps[i]): tp = top, pop, ans[tp] = i - tp   (< strict)
    4. st.push(i), return ans

NEXT GREATER ELEM II
  Maanga:    nums[] CIRCULAR. har element ka next greater, nahi -> -1
  Pehchaan:  circular -> 2n loop, asli index i % n
  Chaabi:
    1. khaali -> return {}; stack<int> st, ans(n, -1)                        (size n, 2n nahi)
    2. loop i = 0..2n-1
    3. while (!st.empty() && nums[st.top()] < nums[i % n]): tp = top, pop, ans[tp] = nums[i % n]
    4. st.push(i % n)                                                        (i nahi: i >= n pe bahar)
    5. return ans

ONLINE STOCK SPAN (LC-901)
  Maanga:    prices ek-ek aate, next(price). span = aaj samet peeche kitne LAGAATAAR din price <= aaj
  Pehchaan:  monotonic stack, par INDEX nahi PAIR {price, span}, pop wale ka span JODO
  Chaabi:
    1. class StockSpanner, stack<pair<int,int>> st                           ({price, span})
    2. next(price): span = 1                                                 (aaj khud bhi)
    3. while (!st.empty() && st.top().first <= price): span += st.top().second, pop   (<= : barabar bhi span me)
    4. st.push({price, span}), return span

VALID PARENTHESES
  Maanga:    sirf ( ) [ ] { }. har bracket sahi type aur sahi kram me band? true / false
  Pehchaan:  kholo push, band pe top se match
  Chaabi:
    1. stack st
    2. loop har char it pe: opening ( [ { -> push
    3. else (closing): st.empty() -> false; ch = st.top(), pop PEHLE
    4. ch aur it ka jodi match -> continue, warna return false
    5. loop ke baad return st.empty()                                        ("" -> true)

MIN STACK
  Maanga:    push, pop, top, getMin sab O(1)
  Pehchaan:  pop pe purana min wapas chahiye -> stack of PAIR {val, yahan tak ka MIN}
  Chaabi:
    1. stack<pair<int,int>> st
    2. push(val): minVal = st.empty() ? val : min(st.top().second, val) -> st.push({val, minVal})   (empty pehle)
    3. pop() = st.pop() · top() = st.top().first · getMin() = st.top().second
       (single min var nahi: pop pe history chahiye)

MIN ADD MAKE VALID
  Maanga:    sirf '(' ')'. kam se kam kitne bracket jodne padenge taaki valid ho
  Pehchaan:  sirf ek type -> COUNTER, stack nahi
  Chaabi:
    1. khaali -> 0; open_needed = 0, close_needed = 0
    2. loop har char: '(' -> close_needed++
    3. ')' -> close_needed > 0 ho to close_needed-- (match), warna open_needed++ (extra ')')
    4. return open_needed + close_needed

QUEUE USING STACKS (LC-232)
  Maanga:    sirf stack se QUEUE (FIFO): push, pop, peek, empty
  Pehchaan:  do stack: input (push yahan) + output (pop / peek yahan)
  Chaabi:
    1. push(x) -> input.push(x)
    2. pop(): output.empty() HO TABHI while input khaali na ho -> output.push(input.top()), input.pop()
       (khaali ho tabhi palto: warna kram toot jaata)
    3. pop: x = output.top(), output.pop(), return x
    4. peek(): wahi palatne wala if + while, phir return output.top()
    5. empty() -> input.empty() && output.empty()
```

**DHYAAN:**
- stack me INDEX rakho, value nahi (doori chahiye hoti hai)
- histogram vs container / trapping -> upar NAAM-SE-CONFUSE A
- generate parentheses stack nahi, BACKTRACKING (16)

**POORA:** [PATTERN 6](PATTERN_SHEET.md#pattern-6--stack)

---

## 07 BINARY SEARCH

**KAB:** SORTED + ek jagah / threshold dhoondhna · "MIN / MAX value jisme condition sach" (answer pe BS)

**SAWAAL (12):**
```text
BINARY SEARCH (BASIC)
  Maanga:    SORTED nums[] + target. target ka index, na mile -> -1. O(log n)
  Pehchaan:  sorted + ek cheez dhoondhni -> basic BS
  Chaabi:
    1. low = 0, high = n - 1
    2. while (low <= high) loop: mid = low + (high - low) / 2      (<= : akela element bhi check; (low+high)/2 nahi = overflow)
    3. loop ke andar: nums[mid] == target -> return mid · chhota -> low = mid + 1 · bada -> high = mid - 1
    4. loop ke bahar return -1                                    (nahi mila)

SEARCH INSERT POSITION
  Maanga:    SORTED nums[] (distinct) + target. mile to index, na mile to jahan DAALNA ho wo index
  Pehchaan:  sorted + "kahan aayega" -> basic BS, return badla
  Chaabi:
    1. basic BS hubahu: low = 0, high = n - 1, while (low <= high), mid, == pe return mid, mid +- 1
    2. loop ke bahar return low                                   (na mila: low = pehla nums >= target = insert jagah)

FIRST & LAST POSITION (LC-34)
  Maanga:    SORTED (duplicate wale) nums[] + target. pehla aur aakhri index, nahi -> {-1,-1}
  Pehchaan:  sorted + boundary chahiye -> 2 baar BS, MILNE pe RUKO MAT
  Chaabi:
    1. vector<int> ans(2, -1)                                     (size fixed 2, push_back nahi)
    2. PASS 1 (FIRST): basic BS loop, par == pe ans[0] = mid aur high = mid - 1   (aur LEFT dhoondho)
    3. low = 0, high = n - 1 dobara RESET                         (warna pass 2 galat jagah se)
    4. PASS 2 (LAST): wahi loop, == pe ans[1] = mid aur low = mid + 1             (aur RIGHT)
    5. return ans

SEARCH IN ROTATED (LC-33)
  Maanga:    sorted array kahin se ghuma diya (distinct). target ka index, nahi -> -1. O(log n)
  Pehchaan:  rotated sorted + target -> BS, har baar dekho KAUNSA half sorted hai
  Chaabi:
    1. low = 0, high = n - 1, while (low <= high), mid; nums[mid] == target -> return mid
    2. if (nums[low] <= nums[mid]) = LEFT half sorted             (<= : low == mid pe akela bhi sorted)
         target >= nums[low] && target <= nums[mid] -> high = mid - 1, warna low = mid + 1
    3. else = RIGHT half sorted:
         target >= nums[mid] && target <= nums[high] -> low = mid + 1, warna high = mid - 1
    4. loop ke bahar return -1                                    (>= nums[low] zaroori: [5,1,3], 5 miss hota)

FIND MIN IN ROTATED
  Maanga:    rotated sorted (distinct). SABSE CHHOTA element. O(log n)
  Pehchaan:  target nahi, khud MIN -> mid ko HIGH se compare
  Chaabi:
    1. low = 0, high = n - 1
    2. while (low < high) loop                                    (< , <= NAHI: high = mid ke saath infinite loop)
    3. nums[mid] > nums[high] -> low = mid + 1 (min right me) · warna high = mid   (mid-1 NAHI: mid khud min ho sakta)
    4. return nums[low]

KOKO BANANAS (LC-875)
  Maanga:    piles[] kele, h ghante, har ghante ek pile se k kele. sab khane ki MIN speed k
  Pehchaan:  "MIN value jisme condition sach" -> BS on ANSWER (speed pe)
  Chaabi:
    1. solve(piles, h, mid): long long hours = 0; har pile pe loop: hours += ceil((double)piles[i] / mid)
       -> return hours <= h                                       (double cast PEHLE, warna int/int floor)
    2. low = 1 (0 = divide by zero), high = *max_element(piles), ans = -1
    3. while (low <= high), mid: solve(mid) sach -> ans = mid, high = mid - 1 (chhota try) · warna low = mid + 1
    4. return ans

SHIP WITHIN D DAYS (LC-1011)
  Maanga:    weights[] ORDER me D din me bhejna, ek din ka total <= capacity. MIN capacity
  Pehchaan:  MIN capacity jisme D din chale -> BS on ANSWER (koko wala shell)
  Chaabi:
    1. solve(weights, D, mid): days = 1, sum = 0; har w pe loop: sum + w > mid -> days++, sum = 0; phir sum += w
       -> return days <= D                                        (days = 1: aakhri din bhi gino)
    2. low = *max_element (sabse bhaari akela chadhe), high = accumulate = sum, ans = -1   (lo = 0 rakha -> galat 5)
    3. while (low <= high): solve(mid) -> ans = mid, high = mid - 1 · warna low = mid + 1
    4. return ans

SPLIT ARRAY LARGEST SUM (LC-410)
  Maanga:    nums[] ko k LAGATAAR hisson me baanto, sabse bade hisse ka sum MIN karo
  Pehchaan:  "max ka min" -> BS on ANSWER (ship wala hi)
  Chaabi:
    1. solve(nums, k, mid): sum = 0, count = 1; har nums[i] pe loop: sum += nums[i]; sum > mid -> count++, sum = nums[i]
       -> return count <= k                                       (count = 1 se; sum = nums[i], 0 nahi)
    2. low = *max_element, high = accumulate (sum), ans = -1
    3. while (low <= high): solve(mid) -> ans = mid, high = mid - 1 · warna low = mid + 1
    4. return ans

FIND PEAK ELEMENT (LC-162)
  Maanga:    PEAK = dono padosi se bada (bahar -inf). kisi bhi peak ka index. O(log n)
  Pehchaan:  sorted nahi, par ek comparison se aadha phenk sakte -> half-discard BS
  Chaabi:
    1. low = 0, high = n - 1
    2. while (low < high) loop, mid                               (< : mid + 1 hamesha andar)
    3. nums[mid] < nums[mid + 1] -> low = mid + 1 (chadhaai, peak right)
       else if nums[mid] > nums[mid + 1] -> high = mid             (dhalaan: mid khud ho sakta, mid-1 NAHI)
    4. return low

SINGLE ELEMENT (SORTED, LC-540)
  Maanga:    SORTED nums[], har element 2 baar, ek akela. wo akela. O(log n)
  Pehchaan:  sorted + jodiyan + log n -> INDEX-PARITY BS (XOR = O(n), nahi chalega)
  Chaabi:
    1. size == 1 -> return nums[0]; low = 0, high = n - 1
    2. while (low <= high), mid; mid == 0 -> return nums[mid]     (mid - 1 bahar na jaaye)
    3. mid EVEN: nums[mid] == nums[mid - 1] -> high = mid - 1, warna low = mid + 1
       mid ODD:  nums[mid] != nums[mid - 1] -> high = mid - 1, warna low = mid + 1
    4. loop ke bahar return nums[high]                            (HIGH, low nahi)

SEARCH 2D MATRIX
  Maanga:    har row sorted, har row ka pehla > pichhli ka aakhri. target hai? true / false
  Pehchaan:  poora matrix ek sorted line -> 1D maan ke BS
  Chaabi:
    1. row = matrix.size(), col = matrix[0].size()
    2. low = 0, high = row * col - 1
    3. while (low <= high), mid: cell = matrix[mid / col][mid % col]
       == target -> true · chhota -> low = mid + 1 · bada -> high = mid - 1
    4. return false

MEDIAN OF 2 SORTED ARRAYS (LC-4)
  Maanga:    do SORTED A, B. dono ka MEDIAN (odd beech wala, even do ka average). O(log)
  Pehchaan:  merge mat karo -> CUT pe BS (left aadha | right aadha)
  Chaabi:
    1. m > n -> return findMedianSortedArrays(B, A)               (chhote pe BS)
    2. low = 0, high = m; while (low <= high): Px = mid, Py = (m + n + 1) / 2 - Px
    3. x1 = A[Px-1] (Px == 0 -> INT_MIN), x2 = B[Py-1] (Py == 0 -> INT_MIN),
       x3 = A[Px] (Px == m -> INT_MAX), x4 = B[Py] (Py == n -> INT_MAX)
    4. x1 <= x4 && x2 <= x3 -> even: (max(x1,x2) + min(x3,x4)) / 2.0 · odd: max(x1,x2)
    5. warna x1 > x4 -> high = Px - 1, else low = Px + 1; bahar return 0.0
```

**DHYAAN:**
- mid KHUD answer ho sakta -> `high = mid` + `while (low < high)` · nahi -> `high = mid - 1` + `<=`
- `high = mid` ke saath `<=` = infinite loop
- answer pe BS: `lo = max(arr)`, 0 nahi

**POORA:** [PATTERN 7](PATTERN_SHEET.md#pattern-7--binary-search)

---

## 08 LINKED LIST

**KAB:** node / next diya · middle · cycle · ulta karo · nth from end · do list jodo

**SAWAAL (13):**
```text
MIDDLE OF LIST
  Maanga:    head diya. beech ka node; even me DOOSRA beech wala
  Pehchaan:  middle -> fast / slow
  Chaabi:
    1. slow = head, fast = head                                   (dono head se -> even me 2nd middle)
    2. while (fast != NULL && fast->next != NULL) loop            (fast pehle check, warna NULL->next crash)
    3. andar: slow = slow->next, fast = fast->next->next
    4. return slow

DETECT CYCLE
  Maanga:    head diya. list me cycle hai? true / false
  Pehchaan:  cycle -> Floyd fast / slow, mile to cycle
  Chaabi:
    1. slow = head, fast = head
    2. while (fast != NULL && fast->next != NULL): slow 1 kadam, fast 2 kadam
    3. move ke BAAD check: slow == fast -> return true            (pehle check karte to turant true)
    4. loop ke bahar return false

FIND DUPLICATE NUMBER (LC-287)
  Maanga:    n+1 number, values 1..n, ek repeat. wo lautao, O(1) space, array badalna mana
  Pehchaan:  array + duplicate + O(1) + no-modify -> value = POINTER (i -> nums[i]), cycle entry = dup
  Chaabi:
    1. slow = 0, fast = 0; PEHLE ek kadam: slow = nums[slow], fast = nums[nums[fast]]   (warna loop chale hi nahi)
    2. PHASE 1: while (slow != fast): slow = nums[slow], fast = nums[nums[fast]]
    3. slow = 0 wapas (fast meet point pe hi)
    4. PHASE 2: while (slow != fast): dono 1-1 kadam (slow = nums[slow], fast = nums[fast])
    5. return slow                                                (cycle ki entry = duplicate)

LINKED LIST CYCLE II (LC-142)
  Maanga:    head diya. cycle ho to cycle ka START node, nahi to NULL
  Pehchaan:  cycle + kahan shuru -> Floyd + phase 2
  Chaabi:
    1. head == NULL -> NULL; slow = head, fast = head, bool cycle = false
    2. while (fast != NULL && fast->next != NULL): slow 1, fast 2; slow == fast -> cycle = true, break
    3. !cycle -> return NULL                                      (flag se, slow != fast se nahi: 1 node wala case)
    4. slow = head; while (slow != fast): dono 1-1 kadam
    5. return slow

PALINDROME LL (LC-234)
  Maanga:    head diya. aage-peeche same padhi jaaye? true / false
  Pehchaan:  COMBO: middle + 2nd half reverse + compare
  Chaabi:
    1. mid = middleNode(head)                                     (upar wala fast / slow)
    2. rev = reverseList(mid)                                     (neeche wala 3-pointer)
    3. while (head != NULL && rev != NULL): val barabar -> dono aage, warna return false
    4. return true                                                (&& : jo pehle NULL wahin ruko)

REMOVE NTH FROM END
  Maanga:    head + n. END se n-va node hatao, naya head
  Pehchaan:  end se gino -> fast / slow me n ka GAP
  Chaabi:
    1. slow = head, fast = head                                   (dummy nahi)
    2. for (i = 1..n) loop: fast = fast->next                     (fast n kadam aage)
    3. fast == NULL -> return head->next                          (head hi hatana hai)
    4. while (fast->next != NULL): dono 1-1 kadam                  (slow hatane wale ke PEHLE ruke)
    5. loop ke bahar: slow->next = slow->next->next; return head

REMOVE ELEMENTS (LC-203)
  Maanga:    head + val. jitne node == val sab hatao (head bhi), naya head
  Pehchaan:  value se delete -> prev / curr chalao
  Chaabi:
    1. !head -> NULL; prev = NULL, curr = head
    2. while (curr != NULL && head != NULL) loop:
    3.   curr->val == val && prev == NULL -> head = head->next     (head hata)
         curr->val == val && prev != NULL -> prev->next = curr->next   (prev aage NAHI)
         else -> prev = curr                                      (prev SIRF no-match pe aage)
    4.   har case ke end me curr = curr->next
    5. return head

MERGE TWO SORTED (LC-21)
  Maanga:    do SORTED list jod ke ek SORTED list, head lautao
  Pehchaan:  naye node jodte jao -> DUMMY + tail
  Chaabi:
    1. dummy = new Node(0), tail = dummy
    2. while (a != NULL && b != NULL): a->val <= b->val -> tail->next = a, a aage · warna b jodo, b aage; tail aage
    3. while (a != NULL): baaki a jodo, tail aage
    4. while (b != NULL): baaki b jodo, tail aage
    5. return dummy->next

ADD TWO NUMBERS
  Maanga:    do list, har node ek digit, number ULTA (units pehle). jodo, jawab bhi ulta list
  Pehchaan:  naye node + carry -> DUMMY + tail + carry
  Chaabi:
    1. dummy = new Node(0), tail = dummy, carry = 0
    2. while (a != NULL || b != NULL || carry > 0) loop           (|| carry: 999 + 1 wala leading 1)
    3. andar: sum = 0; a hai -> sum += a->val, a aage; b hai -> sum += b->val, b aage; sum += carry
    4. tail->next = new Node(sum % 10), tail aage, carry = sum / 10
    5. return dummy->next

SWAP NODES IN PAIRS
  Maanga:    har do paas wale NODE ki jodi badlo (value nahi), naya head
  Pehchaan:  head bhi badlega -> DUMMY + 3 pointer rewire
  Chaabi:
    1. !head -> nullptr; dummy = new Node(0), prev = dummy, prev->next = head, first = head
    2. while (prev->next != nullptr && first->next != nullptr) loop:
    3.   second = first->next, temp = second->next                (temp PEHLE bachao)
    4.   second->next = first, first->next = temp, prev->next = second
    5.   prev = first, first = temp                               (agli jodi)
    6. return dummy->next

REVERSE LINKED LIST
  Maanga:    head diya. poori list ulti, naya head
  Pehchaan:  reverse -> 3 pointer (prev / curr / nextt)
  Chaabi:
    1. prev = NULL, curr = head, nextt = head
    2. while (curr != NULL) loop:
    3.   nextt = curr->next (PEHLE bachao), curr->next = prev, prev = curr, curr = nextt
    4. return prev                                                (curr NULL pe ruka, prev = naya head)

REVERSE NODES IN K-GROUP (LC-25)
  Maanga:    har k node ka group ulta, aakhir me k se kam bache -> waise hi
  Pehchaan:  groups me reverse -> 3 pointer + count + RECURSION
  Chaabi:
    1. head == NULL -> NULL
    2. pehle GINO: while (currNode && countNode < k) aage; countNode != k -> return head   (kam bache = waise hi)
    3. prev = NULL, curr = head, nextt = NULL; while (curr && count < k): 3 pointer reverse, count++
    4. nextt != NULL -> head->next = reverseKGroup(nextt, k)      (head ab is group ka tail)
    5. return prev                                                (is group ka naya head)

COPY LIST w/ RANDOM POINTER (LC-138)
  Maanga:    har node me val + next + random. DEEP copy, copy ka random copy me hi
  Pehchaan:  random / deep copy -> MAP old -> new + 2 PASS
  Chaabi:
    1. head == NULL -> NULL; unordered_map<Node*, Node*> mp; curr = head, prev = NULL, newHead = NULL
    2. PASS 1 while (curr): temp = new Node(curr->val), mp[curr] = temp;
       newHead NULL -> newHead = temp, prev = newHead · warna prev->next = temp, prev = temp; curr aage
    3. curr = head, currRand = newHead
    4. PASS 2 while (curr): curr->random NULL -> NULL · warna currRand->random = mp[curr->random]; dono aage
    5. return newHead                                             (random ORIGINAL se padho, copy se nahi)
```

**DHYAAN:**
- cycle ka SHURU chahiye -> milne ke baad slow ko head pe, dono 1-1 step
- jodna / hatana -> DUMMY node (head bhi badal sakta)
- har node ka kaam -> `curr != NULL` · aakhri pe rukna -> `curr->next != NULL`

**POORA:** [PATTERN 8](PATTERN_SHEET.md#pattern-8--linked-list)

---

## 09 DESIGN

**KAB:** "class banao": HashMap / HashSet built-in ke bina · LRU cache · get / put O(1)

**SAWAAL (3):**
```text
DESIGN HASHMAP (LLD)
  Maanga:    built-in ke bina HashMap (string -> int): put (hai to update), get (nahi -> -1), remove, size
  Pehchaan:  ARRAY of buckets, har bucket ek CHAIN (linked list) · struct Entry | key | value | next |
  Chaabi:
    1. class: count, cap, vector<Entry*> buckets; constructor -> count = 0, cap = 16, buckets.resize(cap)
    2. put(key, val): index = hash<string>{}(key) % cap, node = new Entry(key, val)
       bucket khaali -> buckets[index] = node, count++
       warna while (curr != NULL): key mili -> value update + TURANT return (warna duplicate node)
       curr->next == nullptr -> curr->next = node, count++; phir curr = curr->next
    3. get(key): index nikaalo, chain pe while -> key mili return value; poori chain ke baad -1
    4. remove(key): curr = head, prev = NULL; while: key mili -> prev != NULL ? prev->next = curr->next
       : buckets[index] = curr->next (HEAD hata), count--; phir prev = curr, curr = curr->next
       (prev = prev->next NAHI: NULL pe crash)
    5. size() -> count

DESIGN HASHSET (LLD)
  Maanga:    built-in ke bina HashSet (string): add (duplicate nahi), contains, remove, size
  Pehchaan:  HashMap ka EXACT code, bas value nahi · struct Entry | key | next |
  Chaabi:
    1. class: sz, cap = 16, buckets.resize(cap)
    2. add(key): bucket khaali -> node lagao + sz++; warna chain pe while: key mili -> return (duplicate skip)
       curr->next == nullptr -> curr->next = node, sz++; curr = curr->next
    3. contains(key): chain pe while, mili -> true; end -> false
    4. remove(key): prev = NULL; mili -> prev == nullptr ? buckets[index] = curr->next : prev->next = curr->next
       sz--, return; phir prev = curr, curr = curr->next
    5. size() -> sz

LRU CACHE (LC-146)
  Maanga:    fixed capacity. get (nahi -> -1) aur put dono O(1). bhar jaaye to sabse purana chhua (LRU) hatao
  Pehchaan:  O(1) find + O(1) order -> hashmap (key -> Node*) + DOUBLY linked list · struct Node | key | val | prev | next |
  Chaabi:
    1. constructor: cap = capacity, head = new Node(-1,-1), tail = new Node(-1,-1), head->next = tail, tail->prev = head
       ("Node *head =" likha = shadowing TRAP)
    2. removeNode(node): node->next->prev = node->prev, node->prev->next = node->next
    3. addFront(node): nxt = head->next PEHLE pakdo -> head->next = node, node->prev = head, node->next = nxt, nxt->prev = node
    4. get(key): mp.count(key) > 0 -> node = mp[key], removeNode + addFront, return node->val; warna -1
    5. put(key, value): hai -> val update, removeNode + addFront (DONO)
       nahi -> mp.size() == cap ho to toBeRemoved = tail->prev, removeNode, mp.erase(toBeRemoved->key) (KEY se)
       phir newNode = new Node(key, value), addFront, mp[key] = newNode
```

**DHYAAN:**
- HashMap: array of buckets, har bucket ek list · `index = hash(key) % size` · same key aaye to update
- LRU me map se bhi hatana mat bhoolna

**POORA:** [PATTERN 9](PATTERN_SHEET.md#pattern-9--design-data-structure)

---

## 10 KADANE

**KAB:** subarray ka MAX sum / product · stock ka ek baar kharido-becho

**SAWAAL (6):**
```text
BUY/SELL STOCK (LC-121)
  Maanga:    prices[i] = din i ka daam. ek din kharido, baad me becho. max profit (na ho to 0)
  Pehchaan:  aaj becho = aaj - ab tak ka SASTA -> running min
  Chaabi:
    1. ans = INT_MIN, minSoFar = INT_MAX
    2. for (i = 0..n-1) loop:
    3.   minSoFar = min(minSoFar, prices[i])                      (min PEHLE)
    4.   ans = max(ans, prices[i] - minSoFar)
    5. return ans < 0 ? 0 : ans

MAX SUBARRAY SUM (LC-53)
  Maanga:    CONTIGUOUS subarray (kam se kam 1) ka sabse bada sum
  Pehchaan:  max sum subarray -> KADANE (negative sum = bojh)
  Chaabi:
    1. maxSum = INT_MIN (all negative bhi pakde), sum = 0
    2. har it pe loop: sum += it
    3. maxSum = max(maxSum, sum)                                  (max PEHLE, reset BAAD)
    4. sum < 0 -> sum = 0
    5. return maxSum

MAX PRODUCT SUBARRAY (LC-152)
  Maanga:    CONTIGUOUS subarray ka sabse bada PRODUCT
  Pehchaan:  product + minus -> MIN bhi saath (neg x neg = bada)
  Chaabi:
    1. mini = 1, maxi = 1, ans = INT_MIN                          (loop 0 se)
    2. for (i = 0..n-1) loop:
    3.   temp = max({nums[i], mini * nums[i], maxi * nums[i]})     (3 candidate, temp me FREEZE)
    4.   mini = min({nums[i], mini * nums[i], maxi * nums[i]})     (purane maxi se)
    5.   maxi = temp, ans = max(ans, maxi)
    6. return ans

MAX ABSOLUTE SUM (LC-1749)
  Maanga:    kisi CONTIGUOUS subarray ka |sum| sabse bada
  Pehchaan:  max product ka skeleton, * ki jagah +, ans me abs
  Chaabi:
    1. n == 1 -> return abs(nums[0])
    2. mini = 0, maxi = 0, ans = 0; for (i = 0..n-1) loop:
    3.   temp = max({nums[i], mini + nums[i], maxi + nums[i]})
    4.   mini = min({nums[i], mini + nums[i], maxi + nums[i]}), maxi = temp
    5.   ans = max(ans, max(abs(mini), abs(maxi)))                (minSum se bhi aa sakta)
    6. return ans

MAX SUM CIRCULAR SUBARRAY (LC-918)
  Maanga:    CIRCULAR nums[], non-empty subarray (wrap bhi) ka max sum
  Pehchaan:  circular -> TRICK: total - MIN subarray
  Chaabi:
    1. maxKadaneK: sum = maxSum = nums[0]; loop 1 se: sum = max(sum + nums[i], nums[i]), maxSum = max(...)
    2. minKadaneK: wahi, min ke saath (sum = min(sum + nums[i], nums[i]), minSum = min(...))
    3. totalSum = poore array ka jod; circularSum = totalSum - minSum
    4. maxSum > 0 -> return max(circularSum, maxSum)
    5. warna return maxSum                                        (sab minus: total - min = 0 = khaali, galat)

2 MAX-SUM NON-OVERLAPPING SUBARRAYS (Google)
  Maanga:    2 non-overlap subarray chuno, dono ke sum ka max jod
  Pehchaan:  har WALL pe left ka best + right ka best -> kadane per index + prefix max
  Chaabi:
    1. maxEndingAtForLeft: ans[0] = nums[0]; loop 1 se: ans[i] = max(ans[i-1] + nums[i], nums[i])
    2. bestLeftPrefix: ans[0] = temp[0]; loop 1 se: ans[i] = max(ans[i-1], temp[i])   (running max)
    3. right ke dono mirror: ans[n-1] se shuru, loop n-2 se 0 tak, ans[i+1] use
    4. wall loop for (i = 0; i < n - 1): ans = max(ans, bestLeft[i] + bestRight[i + 1])   (ans = INT_MIN)
    5. return ans                                                 (i + 1 = non-overlap, i nahi)
```

**DHYAAN:**
- circular: `max(normal, total - minSubarray)`, par SAB negative ho to sirf normal
- max sum vs count sum == K -> upar NAAM-SE-CONFUSE C

**POORA:** [PATTERN 10](PATTERN_SHEET.md#pattern-10--kadane-running-value--max-update)

---

## 11 BIT MANIPULATION

**KAB:** har number 2 baar, ek akela · extra memory nahi

**SAWAAL (1):**
```text
SINGLE NUMBER (LC-136)
  Maanga:    har number DO baar, sirf EK ek baar. wo lautao, O(n) time O(1) space
  Pehchaan:  pair khud kat jaaye -> XOR (a ^ a = 0, a ^ 0 = a)
  Chaabi:
    1. XORR = nums[0]                                                        (nums[0] se, isliye loop 1 se)
    2. loop i = 1..n-1: XORR ^= nums[i]
    3. return XORR
```

**DHYAAN:**
- sorted / 1..n / dohra -> XOR se pehle upar NAAM-SE-CONFUSE D dekh

**POORA:** [PATTERN 11](PATTERN_SHEET.md#pattern-11--bit-manipulation-xor)

---

## 12 TREES

**KAB:** TreeNode diya · depth / height / path / same / mirror · level-wise · BST

**SAWAAL (17):**
```text
3 DFS TRAVERSALS (LC-144 / 94 / 145)
  Maanga:    root diya. preorder, inorder, postorder teeno order me values lautao
  Pehchaan:  tree ka order -> recursion, sirf VISIT ki jagah badalti
  Chaabi:
    1. teeno function (root, res &): base -> if (!root) return
    2. PRE:  res.push_back(root->val) -> left call -> right call     (root pehle)
    3. IN:   left call -> push_back -> right call                     (beech)
    4. POST: left call -> right call -> push_back                     (baad)

MAX DEPTH (LC-104)
  Maanga:    root se sabse door leaf tak kitne node (null -> 0)
  Pehchaan:  gehra / bada -> recurse + COMBINE = max
  Chaabi:
    1. base: if (!root) return 0
    2. left = maxDepth(root->left), right = maxDepth(root->right)
    3. return 1 + max(left, right)                                    (khud = 1 + gehri branch)

INVERT TREE (LC-226)
  Maanga:    har node ke left aur right SWAP (mirror), root lautao
  Pehchaan:  har node pe ek kaam -> recurse dono, phir kaam
  Chaabi:
    1. base: if (!root) return nullptr
    2. invertTree(root->left), invertTree(root->right)               (order free)
    3. swap(root->left, root->right)                                  (POINTER swap, value nahi)
    4. return root

SAME TREE (LC-100)
  Maanga:    do tree p, q. structure + values bilkul same? true / false
  Pehchaan:  dono side match -> COMBINE = &&
  Chaabi:
    1. dono NULL -> true · ek NULL -> false                           (null checks PEHLE, warna null->val crash)
    2. p->val != q->val -> false
    3. return isSameTree(p->left, q->left) && isSameTree(p->right, q->right)

SYMMETRIC TREE (LC-101)
  Maanga:    tree apne center ke around mirror hai? true / false
  Pehchaan:  = SAME TREE, bas CROSS call
  Chaabi:
    1. isSameTree ka wahi base (dono null / ek null / val alag)
    2. return isSameTree(p->left, q->right) && isSameTree(p->right, q->left)   (CROSS = mirror)
    3. isSymmetric: if (!root) true · return isSameTree(root->left, root->right)

DIAMETER (LC-543)
  Maanga:    kinhi do node ke beech sabse lamba path, EDGES me
  Pehchaan:  parent ko height chahiye, answer kuch aur -> height RETURN, answer GLOBAL (&maxDia)
  Chaabi:
    1. height(root, int &maxDia): base if (!root) return 0
    2. left = height(left), right = height(right)                     (recurse PEHLE, post-order)
    3. maxDia = max(maxDia, left + right)                             (is node se guzarti diameter)
    4. return 1 + max(left, right)                                    (parent ko height)
    5. main: maxDia = 0 -> height(root, maxDia) -> return maxDia

MAX PATH SUM (LC-124)
  Maanga:    koi bhi connected path (root zaroori nahi), max node-sum
  Pehchaan:  = DIAMETER ka sum version
  Chaabi:
    1. maxDepth(root, int &maxSum): base if (!root) return 0
    2. left = max(0, call(left)), right = max(0, call(right))         (minus arm -> 0, lo hi mat)
    3. maxSum = max(maxSum, root->val + left + right)                 (junction pe DONO arm + node)
    4. return root->val + max(left, right)                            (upar EK hi arm ja sakti)
    5. main: maxSum = INT_MIN (0 nahi, akela -3 bhi valid) -> call -> return maxSum

BUILD TREE from PREORDER + INORDER (LC-105)
  Maanga:    preorder + inorder diye, original tree bana ke root lautao
  Pehchaan:  pre[rootIndex] = ROOT, inorder me us root pe todo -> left | root | right
  Chaabi:
    1. buildTree: map mp (inorder value -> index) ek loop me, rootIndex = 0
    2. solve(int &rootIndex, pre, in, l, r, mp): base if (l > r) return NULL
    3. pivot = mp[preorder[rootIndex]] -> rootIndex++ -> newNode(inorder[pivot])
    4. newNode->left = solve(l, pivot - 1) PEHLE, phir ->right = solve(pivot + 1, r) -> return newNode
    5. call: solve(rootIndex, ..., 0, n - 1, mp)                      (rootIndex & se: sab calls me shared)

BALANCED TREE (LC-110)
  Maanga:    har node pe left / right height ka farak <= 1? true / false
  Pehchaan:  = MAX DEPTH + -1 sentinel (height nikaalte-nikaalte hi check)
  Chaabi:
    1. maxDepth: base if (!root) return 0
    2. left = call(left) -> -1 hai to turant return -1
    3. right = call(right) -> -1 hai to turant return -1
    4. abs(left - right) > 1 -> return -1 · warna return 1 + max(left, right)
    5. isBalanced: return maxDepth(root) != -1

LCA (LC-236)
  Maanga:    p aur q ka sabse gehra common ancestor (khud bhi ho sakta)
  Pehchaan:  search + bubble up: mila to upar bhejo, DONO taraf se mila = answer
  Chaabi:
    1. if (!root) return NULL · if (root == p || root == q) return root
    2. left = LCA(root->left), right = LCA(root->right)
    3. left NULL -> return right · right NULL -> return left
    4. dono non-null -> return root                                   (yahi split point)

PATH SUM (LC-112)
  Maanga:    root se LEAF tak koi path jiska sum = target? true / false
  Pehchaan:  value NEECHE carry (target ghatao), LEAF pe check, combine = ||
  Chaabi:
    1. if (!root) return false
    2. leaf (!root->left && !root->right) -> return root->val == targetSum
    3. return call(left, targetSum - root->val) || call(right, targetSum - root->val)

LEVEL ORDER (LC-102)
  Maanga:    har level ke node left se right, alag list me (list of lists)
  Pehchaan:  level-wise -> queue + sz snapshot
  Chaabi:
    1. if (!root) return ans · q.push(root)
    2. while (!q.empty()): sz = q.size() PEHLE pakdo, temp khaali
    3. for (i = 0; i < sz): curr = front, pop, temp me val, left / right ho to push
    4. for ke BAHAR ans.push_back(temp)                               (level poora tab)

RIGHT SIDE VIEW (LC-199)
  Maanga:    daaye se har level ka sabse right node
  Pehchaan:  = LEVEL ORDER + 1 line
  Chaabi:
    1. level order wala hi dhaancha (while -> sz -> for i < sz)
    2. for ke andar: if (i == sz - 1) ans.push_back(curr->val)        (aakhri = sabse right; left view = i == 0)
    3. bachche push waise hi

ZIGZAG LEVEL ORDER (LC-103)
  Maanga:    level 0 L->R, level 1 R->L, ulta-palti
  Pehchaan:  = LEVEL ORDER + reverse
  Chaabi:
    1. levelCounter = 0, level order ka dhaancha, temp hamesha normal L->R banao
    2. for ke BAAD: if (levelCounter % 2 != 0) reverse(temp)
    3. phir levelCounter++ -> ans.push_back(temp)                      (++ check ke baad, warna root ulta)

MINIMUM DEPTH (LC-111)
  Maanga:    root se sabse PAAS wale leaf tak kitne node (null -> 0)
  Pehchaan:  = LEVEL ORDER + early stop (DFS 1 + min galat, null child ko 0 maanta)
  Chaabi:
    1. if (!root) return 0 · q.push(root) · depth = 0
    2. while -> sz -> for (i < sz): curr = front, pop
    3. leaf mila (!left && !right) -> turant return depth + 1
    4. bachche push · for ke BAHAR depth++

KTH SMALLEST IN BST (LC-230)
  Maanga:    BST me k-va sabse chhota (1-indexed)
  Pehchaan:  BST + sorted / kth -> INORDER
  Chaabi:
    1. inorder(root, res): left -> push_back(val) -> right
    2. kthSmallest: ans vector me inorder bharo
    3. return ans[k - 1]                                              (k 1-based, vector 0-based)

VALIDATE BST (LC-98)
  Maanga:    valid BST? poora left < node < poora right (strict)
  Pehchaan:  sirf child check kaafi nahi -> har node ki WINDOW (mini, maxi) neeche carry
  Chaabi:
    1. solve(root, long long mini, long long maxi): if NULL -> true
    2. root->val <= mini || root->val >= maxi -> false                (barabar bhi galat)
    3. left = solve(left, mini, root->val), right = solve(right, root->val, maxi)
    4. return left && right
    5. call: solve(root, LLONG_MIN, LLONG_MAX)                        (INT se nahi: node khud INT_MAX ho sakta)
```

**DHYAAN:**
- max path sum: `left = max(0, left)` (minus wali arm mat lo)
- parent ko height chahiye aur answer kuch aur -> height RETURN, answer GLOBAL (diameter)
- BST ka inorder = sorted

**POORA:** [PATTERN 12](PATTERN_SHEET.md#pattern-12--trees-recursion)

---

## 13 GRAPHS

**KAB:** edges / adjacency list / grid diya · "kitne group?" · "A se B pahunch sakte?" · "kam se kam kitne step?"

**SAWAAL (12):**
```text
PATH EXISTS (LC-1971)
  Maanga:    undirected edges, source se destination tak rasta? true / false
  Pehchaan:  ek source se reach -> single BFS, outer loop nahi
  Chaabi:
    1. edges loop: adj[u].push_back(v), adj[v].push_back(u)          (undirected dono taraf)
    2. vis(n, false) · q.push(source) · vis[source] = true
    3. while: node = front, pop · node == destination -> return true   (pop pe check)
    4. adj[node] loop: !vis -> vis = true + push                       (push ke saath mark)
    5. queue khaali -> return false

NUMBER OF ISLANDS (LC-200)
  Maanga:    grid '1' zameen '0' paani, 4-dir jude 1 = ek island. kitne island?
  Pehchaan:  kitne group + grid -> outer loop har cell + DFS sink
  Chaabi:
    1. DFS(grid, i, j, m, n): base ek hi if -> bounds bahar || grid[i][j] == '0' -> return   (bounds PEHLE)
    2. grid[i][j] = '0' (sink = visited) -> 4 call: i+1, i-1, j+1, j-1
    3. numIslands: do loop (i < m, j < n) -> grid[i][j] == '1' -> DFS + count++
    4. return count

CONNECTED COMPONENTS (LC-323)
  Maanga:    n node, undirected edges. kitne alag group?
  Pehchaan:  = ISLANDS, bas grid ki jagah adj list + BFS
  Chaabi:
    1. adj banao (dono taraf) · vis(n, false) · count = 0
    2. OUTER loop i = 0..n-1: if (!vis[i]) -> push(i), vis[i] = true, count++
    3. usi if ke andar while (BFS): node = front, pop -> adj[node] me !vis -> mark + push
    4. return count                                                   (count++ aur BFS dono if ke ANDAR)

MAX AREA OF ISLAND (LC-695)
  Maanga:    sabse bade island ke cells (na ho to 0)
  Pehchaan:  = ISLANDS, par DFS area RETURN kare, count ki jagah max
  Chaabi:
    1. DFS int: bounds bahar || grid == 0 -> return 0
    2. grid[i][j] = 0 -> neeche, upar, right, left = 4 call ka area
    3. return 1 + left + right + upar + neeche
    4. main: ans = 0 · do loop -> grid == 1 -> ans = max(ans, DFS(...)) · return ans

ROTTING ORANGES (LC-994)
  Maanga:    0 khaali 1 fresh 2 rotten, har minute 4 padosi sadte. kitne minute? na ho -1
  Pehchaan:  kitne MINUTE -> level BFS · saare rotten ek saath shuru -> multi source
  Chaabi:
    1. do loop: grid == 2 wale saare q me (level 0) · dirs 4 · mins = 0
    2. while (!q.empty()): sz = q.size() -> while (sz--)
    3. row = q.front().first, col = q.front().second -> PHIR pop     (pehle padho, phir pop)
    4. dirs loop: nR, nC bounds andar && grid == 1 -> grid = 2 + push (push ke saath 2)
    5. while (sz--) ke BAHAR mins++
    6. phir do loop: koi 1 bacha -> -1 · return mins == 0 ? mins : mins - 1   (aakhri level extra)

COURSE SCHEDULE (LC-207)
  Maanga:    [a,b] = a se pehle b. saare course ho sakte? true / false
  Pehchaan:  prereq / kram -> TOPO (indegree + queue)
  Chaabi:
    1. har [a,b] pe: edge b -> a, aur a ka indegree++        (pehla loop: graph banao)
    2. SAARE course pe loop (0..n-1): indegree 0 wale queue me  (kai course ek saath shuru ho sakte)
    3. queue se nikaalo -> ans me daalo -> padosi ka indegree-- -> 0 hua to queue
    4. return ans.size() == numCourses                          (cycle wale kabhi 0 nahi hote)

BUILD ORDER (topo + MIN-HEAP)
  Maanga:    [a->b] = a ke baad b. READY me se sabse CHHOTA pehle, poora kram; loop ho to khaali
  Pehchaan:  dependency -> TOPO · "sabse chhota" -> queue ki jagah min-heap
  Chaabi:
    1. deps loop: u = it[0], v = it[1] -> adj[u].push_back(v), indegree[v]++
    2. pq = min-heap (greater<int>) · loop 0..n-1: indegree 0 wale pq me
    3. ans KHAALI se (ans(n, 0) nahi) · while: node = top, pop, ans me
    4. adj[node] loop: indegree-- -> 0 hua to pq.push
    5. ans.size() == n -> ans, warna {}

CLONE GRAPH (LC-133)
  Maanga:    graph ka deep copy (naye node, naye neighbours), clone lautao
  Pehchaan:  copy + cycle -> BFS + map (purana -> naya)
  Chaabi:
    1. if (!node) nullptr · clone banao, mp[node] = clone, q.push(node)   (seed bhi map me)
    2. while: curr = front, pop -> curr->neighbors loop
    3. mp.find(it) == end -> naya clone, mp[it] = newClone, q.push(it)    (BANANA, sirf naye pe)
    4. if ke BAHAR har nbr pe: mp[curr]->neighbors.push_back(mp[it])       (JODNA)
    5. return mp[node]

IS BIPARTITE (LC-785)
  Maanga:    nodes 2 group me baant sakte ki har edge alag group ke beech ho?
  Pehchaan:  2 team, jude ALAG -> 2-colour BFS + outer loop
  Chaabi:
    1. color(m, -1) · OUTER loop i: color[i] == -1 -> BFS false aaya to return false
    2. BFS(graph, color, node): push(node), color[node] = 0               (start ko rang zaroori)
    3. while: node = front, pop -> graph[node] loop
    4. color == -1 -> color = 1 - color[node] + push · warna same rang -> return false
    5. BFS end true · main end true

DIJKSTRA
  Maanga:    weighted undirected edges, source se har node ka sabse kam total weight
  Pehchaan:  edge pe weight + shortest -> BFS ki jagah min-heap + dist[]
  Chaabi:
    1. adj[u].push_back({v, w}), adj[v].push_back({u, w})
    2. pq min-heap of {dist, node} · dist(n, 1e9), dist[src] = 0, push({0, src})
    3. while: wt = top.first, node = top.second, pop · wt > dist[node] -> continue (stale)
    4. adj[node] loop: wt + it.second < dist[it.first] -> dist update, PHIR push
    5. return dist

DSU (Union-Find)
  Maanga:    n node, edges. Union-Find se kitne alag group
  Pehchaan:  groups / same set, edge ek-ek merge -> parent[] + find + unite
  Chaabi:
    1. class: parent, rnk · constructor: parent[i] = i, rnk 0
    2. find(x): x == parent[x] -> x · warna return parent[x] = find(parent[x])   (path compression)
    3. unite(x, y): u_x = find(x), u_y = find(y), same -> return
    4. chhota rnk bade ke neeche · barabar -> parent[u_y] = u_x, rnk[u_x]++
    5. countComponents: har edge pe unite -> loop i: find(i) == i -> count++

WORD LADDER (LC-127)
  Maanga:    begin se end, har step ek letter badlo, beech ke word list me. kitne word? na ho 0
  Pehchaan:  min steps -> BFS, word = node
  Chaabi:
    1. set st(wordList) (dict + visited dono) · q.push({beginWord, 1}) · st.erase(beginWord)
    2. while: word, steps = front, pop · word == endWord -> return steps
    3. loop i har letter: original = word[i] -> loop ch 'a'..'z': word[i] = ch
    4. st me mila -> st.erase(word) + push({word, steps + 1})          (erase = visited)
    5. ch loop ke baad word[i] = original (restore) · end me return 0
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

**SAWAAL (9):**
```text
LAST STONE WEIGHT (LC-1046)
  Maanga:    stones[]. har turn 2 sabse bhaari ladao: barabar -> dono gaye, warna diff bache. aakhri bacha (warna 0)
  Pehchaan:  har turn "2 sabse bade" + set badalta rehta -> MAX-heap
  Chaabi:
    1. priority_queue<int> pq (MAX); loop se saare stones push
    2. while (pq.size() > 1): x = top, pop · y = top, pop          (smash ko 2 pathar chahiye)
    3. diff = abs(y - x); diff > 0 tabhi wapas push               (barabar = dono khatam)
    4. return pq.empty() ? 0 : pq.top()                            (empty PEHLE check, warna top() crash)

KTH LARGEST ELEMENT (LC-215)
  Maanga:    nums[] + k. k-th SABSE BADA lautao
  Pehchaan:  k-th largest -> MIN-heap size k (top-K ka core)
  Chaabi:
    1. priority_queue<int, vector<int>, greater<int>> pq (MIN)
    2. har num pe loop: push, phir if (pq.size() > k) pop            (sabse chhota bahar, k bade bache)
    3. return pq.top()                                              (k bade me sabse chhota = k-th largest)

K CLOSEST POINTS TO ORIGIN (LC-973)
  Maanga:    points[] [x,y]. origin ke sabse PAAS k points (doori = x*x + y*y), order koi bhi
  Pehchaan:  k sabse CHHOTE -> MAX-heap size k (kth largest ka ulta)
  Chaabi:
    1. priority_queue<pair<int, vector<int>>> maxHeap               (MAX: sabse door top pe)
    2. har point pe loop: push {x*x + y*y, point}                   (dist PEHLE, root NAHI)
    3. usi loop me: if (maxHeap.size() > k) pop                     (sabse door bahar)
    4. while heap khaali na ho: ans me top().second, pop -> return ans

TOP K FREQUENT ELEMENTS (LC-347)
  Maanga:    nums[] + k. k elements jo SABSE ZYADA baar aate (order koi bhi)
  Pehchaan:  top-K par freq pe -> hashmap freq + MIN-heap size k
  Chaabi:
    1. pehla loop: mp[nums[i]]++                                    (freq gino)
    2. MIN-heap of pair<int,int> (greater<pair<int,int>>)
    3. mp pe loop: push {it.second, it.first} (freq PEHLE); size > k -> pop
    4. while heap khaali na ho: top().second (number) ans me, pop -> return ans

MERGE K SORTED LISTS (LC-23)
  Maanga:    k sorted linked lists. sab jod ke EK sorted list
  Pehchaan:  har baar k heads me sabse chhota -> MIN-heap of current heads
  Chaabi:
    1. struct Compare: return a->val > b->val                       (b chhota -> MIN-heap)
    2. lists pe loop: if (l) push                                    (NULL mat daalo, ->val pe crash)
    3. dummy = new ListNode(0), tail = dummy
    4. while heap khaali na ho: curr = top, pop -> tail->next = curr, tail = curr
       -> if (curr->next) push(curr->next)
    5. return dummy->next

SORT NEARLY-SORTED (k-sorted) ARRAY
  Maanga:    har element apni sahi jagah se max k door. poora sorted array lautao
  Pehchaan:  "max k door" -> MIN-heap buffer size k+1
  Chaabi:
    1. MIN-heap (greater<int>), ans vector
    2. arr pe loop: pehle push, phir if (pq.size() > k + 1): top ans me, pop
    3. loop ke baad: while heap khaali na ho -> top ans me, pop       (bache = sorted tail)
    4. return ans

FIND MEDIAN FROM DATA STREAM (LC-295)
  Maanga:    number ek-ek aate (addNum). kabhi bhi findMedian() = ab tak ka beech wala (even -> 2 ka average)
  Pehchaan:  stream + median -> DO heap (chhoti team MAX, badi team MIN)
  Chaabi:
    1. maxHeap = priority_queue<double> (CHHOTI team) · minHeap = MIN of double (BADI team)
    2. addNum PLACEMENT: if (maxHeap.empty() || maxHeap.top() >= num) maxHeap me, warna minHeap me
    3. addNum BALANCE: maxHeap.size() < minHeap.size() -> minHeap ka top maxHeap me
       · else if maxHeap.size() > minHeap.size() + 1 -> maxHeap ka top minHeap me
    4. findMedian: size barabar -> (dono top) / 2.0 · warna maxHeap.top()
       (naam ka TRAP: bada number -> BADI team -> minHeap)

REORGANIZE STRING (LC-767)
  Maanga:    s ke akshar aise lagao ki koi 2 PADOSI same na ho. na bane -> ""
  Pehchaan:  freq + har round sabse zyada wala lagao -> MAX-heap {count, char}, 2-2 nikaalo
  Chaabi:
    1. mp[char]++ freq · priority_queue<pair<int,char>> maxHeap me {count, char}
    2. while (maxHeap.size() > 1): it1 = top, pop · it2 = top, pop -> ans += it1.second, it2.second
    3. dono ka first -= 1; jiska >= 1 bacha wahi wapas push
    4. loop ke baad heap me ek bacha: count > 1 -> return "" · warna uska char ans me
    5. return ans

THROTTLING: MIN UTHAO, PADOSI HATAO (Amazon OA)
  Maanga:    responseTimes[]. har step bache me sabse CHHOTA (tie -> chhota index) jodo, use + i-1, i+1 hatao. total?
  Pehchaan:  hamesha min + beech se hatana -> MIN-heap {val, idx} + vis[] (lazy skip)
  Chaabi:
    1. vis(n, false), long long ans = 0                            (1e9 x lakh -> long long)
    2. MIN-heap of pair<int,int>; loop se push {responseTimes[i], i}   (tie pe chhota idx apne aap)
    3. while heap khaali na ho: index = top().second, pop
    4. if (vis[index] == false): ans += value, vis[index] = true,
       index-1 >= 0 ho to vis[index-1] = true, index+1 < n ho to vis[index+1] = true
    5. return ans                                                   (hata hua pop ho -> chhod do)
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

**SAWAAL (4):**
```text
MERGE INTERVALS (LC-56)
  Maanga:    [start,end] list. overlap wale jodo (touching bhi), merged list
  Pehchaan:  MERGE / combine -> sort by START + merge
  Chaabi:
    1. sort(intervals)                                            (start pe)
    2. ans.push_back(intervals[0])                                (pehla baseline)
    3. for (i = 1..n-1) loop: ans.back()[1] >= intervals[i][0] -> ans.back()[1] = max(ans.back()[1], intervals[i][1])
                              (>= : touching bhi merge; max zaroori: [1,10],[2,3])
    4.   warna ans.push_back(intervals[i])
    5. return ans

MEETING ROOMS (LC-252)
  Maanga:    meetings [start,end]. saari attend ho sakti (koi 2 overlap nahi)? true / false
  Pehchaan:  merge wala template, overlap pe return false
  Chaabi:
    1. empty -> true; sort(intervals); ans.push_back(intervals[0])
    2. for (i = 1..n-1) loop: intervals[i][0] < ans.back()[1] -> return false   (< : touching chalta)
    3.   warna ans.push_back(intervals[i])
    4. return true

INSERT INTERVAL (LC-57)
  Maanga:    sorted + non-overlap list me newInterval daalo (overlap -> merge), sorted list
  Pehchaan:  pehle se sorted -> sort NAHI, 3 phase ek pass
  Chaabi:
    1. ans, count = 0; for (i = 0..n-1) loop:
    2.   intervals[i][1] < newInterval[0] -> ans.push_back            (new se PEHLE khatam)
    3.   else if intervals[i][0] > newInterval[1] -> break             (new ke AAGE)
    4.   else OVERLAP -> newInterval[0] = min(...start), newInterval[1] = max(...end)
    5.   count++ (break wala yahan nahi aata)
    6. loop ke bahar ans.push_back(newInterval); phir for (i = count..n-1) baaki push; return ans

MEETING ROOMS II (LC-253)
  Maanga:    start[], end[]. MIN kitne room (end == start pe wahi room)
  Pehchaan:  "kitne ek saath" -> LINE SWEEP (+1 / -1)
  Chaabi:
    1. map<int,int> mp                                            (ORDERED map, time sorted)
    2. for (i = 0..n-1) loop: mp[start[i]]++, mp[end[i]]--
    3. overlap = 0, maxOverlap = 0; map pe loop: overlap += it.second, maxOverlap = max
    4. return maxOverlap
```

**DHYAAN:**
- MERGE -> start pe sort · MAX pick / MIN hatao -> END pe sort (greedy) · kitne ek saath -> line sweep
- touching (2 aur 2) overlap hai ya nahi -> `<` ya `<=` sawaal se tay karo

**POORA:** [PATTERN 15](PATTERN_SHEET.md#pattern-15--intervals)

---

## 16 BACKTRACKING

**KAB:** "SAARE" subsets / permutations / combinations · grid me word dhoondo · N-queens

**SAWAAL (7):**
```text
SUBSETS (LC-78)
  Maanga:    nums[] (unique). SAARE subsets lautao
  Pehchaan:  "saare subsets" -> har element pe lo / chhodo (backtracking)
  Chaabi:
    1. solve(nums, ans, index, temp): if (index >= nums.size()) -> ans me temp, return
    2. INCLUDE: temp.push_back(nums[index]) -> solve(index + 1)
    3. temp.pop_back()                                              (EXCLUDE se PEHLE undo)
    4. EXCLUDE: solve(index + 1)
    5. caller: ans, temp BAHAR banao, & se bhejo -> solve(nums, ans, 0, temp)
       (ALT: har node pe ans me temp, phir for (i = index..) push -> solve(i+1) -> pop)

PERMUTATIONS (LC-46)
  Maanga:    nums[] (unique). SAARE order (permutations) lautao
  Pehchaan:  order matter -> used[] + loop 0 se, base = full length
  Chaabi:
    1. base: temp.size() == nums.size() -> ans me temp, return
    2. for (i = 0; i < n) loop, 0 SE (start nahi): used[i] ho to continue
    3. CHOOSE: temp.push_back(nums[i]), used[i] = true -> solve(...)
    4. UN-CHOOSE dono: temp.pop_back(), used[i] = false
    5. caller: temp + used(n, false) BAHAR bana ke & se

COMBINATIONS (LC-77)
  Maanga:    1..n me se k number ke SAARE combinations (order matter nahi)
  Pehchaan:  subsets ka start-loop, bas base size == k
  Chaabi:
    1. base: temp.size() == k -> ans me temp, return
    2. for (i = index; i <= n; i++) loop                            (<= n: n khud bhi number)
    3. temp.push_back(i) -> solve(n, k, ans, i + 1, temp) -> temp.pop_back()
    4. caller: solve(n, k, ans, 1, temp)                            (1 se shuru, 0 nahi)

COMBINATION SUM (LC-39)
  Maanga:    candidates[] + target. saare combos jinka SUM == target, ek number kitni bhi baar
  Pehchaan:  reuse allowed + target -> include me index SAME
  Chaabi:
    1. if (target < 0) return                                      (overshoot, prune)
    2. if (index >= nums.size()): target == 0 ho to ans me temp; return
    3. INCLUDE: push(nums[index]) -> solve(index SAME, target - nums[index])   (dobara le sakte)
    4. pop_back -> EXCLUDE: solve(index + 1, target)
       (ALT for-loop: target == 0 pe record; for (i = index..) push -> solve(i, target - c[i]) -> pop)

WORD SEARCH (LC-79)
  Maanga:    char grid + word. bagal-bagal cells se word banta, har cell ek baar? true / false
  Pehchaan:  grid pe raasta + cell dobara nahi -> DFS + mark / un-mark (backtracking)
  Chaabi:
    1. solve ka pehla if: bounds bahar || vis[i][j] || board[i][j] != word[index] -> false   (bounds PEHLE)
    2. vis[i][j] = true, index++
    3. chaaron disha ki solve || se jodo -> koi true to return true
    4. vis[i][j] = false (UN-MARK) -> return index == word.size()
    5. exist: vis BAHAR bana ke &, do loop (i, j) har cell pe: board[i][j] == word[0] ho to solve;
       true -> return true · loop ke BAAD return false        (pehla fail pe false MAT, saare start try)

GENERATE PARENTHESES (LC-22)
  Maanga:    n jode brackets. saare VALID strings lautao
  Pehchaan:  2 rule se hi banao (open / close count), brute nahi
  Chaabi:
    1. solve(open, close, path, ans, n): open == n && close == n -> ans me path, return
    2. if (open < n) solve(open + 1, close, path + '(')
    3. if (close < open) solve(open, close + 1, path + ')')       (ALAG if, else if NAHI, return NAHI)
    4. path BY VALUE (path + '(' naya copy -> pop nahi) · caller: solve(0, 0, "", ans, n)

LETTER COMBINATIONS OF A PHONE NUMBER (LC-17)
  Maanga:    digits (2-9). keypad ke aksharon se saare shabd, khaali -> []
  Pehchaan:  har level = ek ank, uske saare akshar try -> for loop backtracking
  Chaabi:
    1. base: index >= digits.size() -> ans me temp, return
    2. str = mp[digits[index]]; for (i = 0; i < str.size()) loop
    3. temp.push_back(str[i]) -> solve(index + 1) -> temp.pop_back()
    4. caller: digits khaali -> return {} (pehle hi) · mp['2'] = "abc" ... '9' = "wxyz"
       · ans aur temp function ke ANDAR banao (global nahi) -> solve(0, temp, mp, ans, digits)
```

**DHYAAN:**
- beech me `return solve(...)` doosri branch maar deta hai
- `else if` mat lagao, dono branch alag `if`
- grid: visit mark -> explore -> UNMARK

**POORA:** [PATTERN 16](PATTERN_SHEET.md#pattern-16--backtracking-choose--explore--un-choose)

---

## 17 DP

**KAB:** "kitne tareeke" · "min / max cost" · "ho sakta hai?" + har step pe choice + same sawaal baar-baar

**SAWAAL (16):**
```text
CLIMBING STAIRS (LC-70)
  Maanga:    n steps, ek baar me 1 ya 2. top tak kitne tareeke?
  Pehchaan:  kitne tareeke + pichhle 2 se -> Fibonacci DP
  Chaabi:
    1. base: n == 0 || n == 1 -> return 1
    2. memo: dp[n] != -1 -> return dp[n]
    3. return dp[n] = solve(n - 1) + solve(n - 2)
    4. caller: dp(n + 1, -1) -> solve(n, dp)

DECODE WAYS (LC-91)
  Maanga:    digit string, 1=A .. 26=Z. kitne tareeke se decode? (akela '0' kuch nahi)
  Pehchaan:  tukdon me kaato + har tukda rule pass + ginti -> index DP (climbing ka bhai)
  Chaabi:
    1. base kram: i == n -> 1 (PEHLE) · s[i] == '0' -> 0 · dp[i] != -1 -> dp[i]
    2. count = solve(i + 1)                                         (1 ank hamesha valid)
    3. if (i + 1 < n) aur (s[i] == '1' || (s[i] == '2' && s[i+1] <= '6')) -> count += solve(i + 2)
    4. return dp[i] = count · caller: dp(n + 1, -1) -> solve(0, s, n, dp)

HOUSE ROBBER (LC-198)
  Maanga:    nums[] = ghar ka paisa. 2 padosi nahi loot sakte. MAX loot
  Pehchaan:  har ghar pe loot / chhodo + max -> take / skip DP
  Chaabi:
    1. solve(A, i, dp): i < 0 -> 0 · dp[i] != -1 -> dp[i]
    2. take = A[i] + solve(i - 2)  · notTake = 0 + solve(i - 1)
    3. return dp[i] = max(take, notTake)
    4. caller: dp(n + 1, -1) -> solve(A, n - 1, dp)                (LAST index se peeche)

HOUSE ROBBER II (LC-213)
  Maanga:    house robber par ghar GOL me (pehla aur aakhri padosi). MAX loot
  Pehchaan:  gol todo: ek ghar hatao -> robber I do baar
  Chaabi:
    1. solve = robber I (i < 0 -> 0, i == 0 -> nums[0], memo, notTake / Take, max)
    2. rob1(nums): dp(n + 1, -1) ANDAR naya -> solve(nums, n - 1, dp)
    3. rob: n == 1 -> nums[0] (pehle hi)
    4. ek loop: i != 0 -> temp1 me · i != n-1 -> temp2 me
    5. return max(rob1(temp1), rob1(temp2))

COIN CHANGE (LC-322)
  Maanga:    coins[] (unlimited) + amount. MIN coins, na bane -> -1
  Pehchaan:  min + har coin pe lo / chhodo + reuse -> DP (INT_MAX = ban nahi sakta)
  Chaabi:
    1. FORM A solve(coins, amount, i, dp): amount < 0 -> INT_MAX · i < 0 -> amount == 0 ? 0 : INT_MAX
       · dp[i][amount] != -1 -> wahi
    2. notTake = solve(amount, i - 1) · take = INT_MAX
    3. if (solve(amount - coins[i], i) != INT_MAX) take = 1 + wahi     (i SAME = reuse, overflow guard)
    4. return dp[i][amount] = min(take, notTake)
    5. caller: dp(n + 1, vector<int>(amount + 1, -1)) -> ans = solve(n - 1) -> INT_MAX ho to -1
       (FORM B: 1D dp[amount], amount == 0 -> 0, for loop har coin pe Take = min(Take, 1 + solve))

LONGEST INCREASING SUBSEQUENCE (LC-300)
  Maanga:    nums[]. sabse lambi STRICTLY badhti subsequence ki LENGTH
  Pehchaan:  take / skip + pichhla yaad -> state (index, prevIndex)
  Chaabi:
    1. base: index == n -> 0 · memo dp[index][prevIndex + 1]          (+1: prev -1 se shuru)
    2. notTake = solve(index + 1, prevIndex)
    3. Take = 0; if (prevIndex == -1 || nums[index] > nums[prevIndex]) Take = 1 + solve(index + 1, index)
    4. return dp[index][prevIndex + 1] = max(Take, notTake)
    5. caller: dp(n + 1, vector<int>(n + 1, -1)) -> solve(0, -1, nums, dp)

UNIQUE PATHS (LC-62)
  Maanga:    m x n grid, (0,0) se (m-1,n-1), sirf RIGHT / DOWN. kitne raaste?
  Pehchaan:  grid + kitne raaste -> 2D DP (up + left)
  Chaabi:
    1. solve(i, j): i < 0 || j < 0 -> 0 · i == 0 && j == 0 -> 1 · dp[i][j] memo
    2. up = solve(i - 1, j) · left = solve(i, j - 1)
    3. return dp[i][j] = up + left
    4. caller: dp(m + 1, vector<int>(n + 1, -1)) -> solve(m - 1, n - 1, dp)

MIN PATH SUM (LC-64)
  Maanga:    grid me har cell ka cost. (0,0) se (m-1,n-1), RIGHT / DOWN. sabse KAM sum
  Pehchaan:  unique paths jaisa, par MIN + cost
  Chaabi:
    1. i < 0 || j < 0 -> INT_MAX (INVALID) · i == 0 && j == 0 -> grid[0][0] · memo
    2. up = solve(i - 1, j) · left = solve(i, j - 1)
    3. return dp[i][j] = grid[i][j] + min(up, left)
    4. caller: m, n -> dp(m + 1, n + 1, -1) -> solve(m - 1, n - 1, dp, grid)

LONGEST COMMON SUBSEQUENCE (LC-1143)
  Maanga:    s1, s2. dono me common subsequence ki sabse lambi LENGTH
  Pehchaan:  2 string -> dp[i][j], peeche se
  Chaabi:
    1. i < 0 || j < 0 -> 0 · memo dp[i][j]
    2. s1[i] == s2[j] -> dp[i][j] = 1 + solve(i - 1, j - 1)          (MATCH: dono peeche)
    3. warna dp[i][j] = max(solve(i - 1, j), solve(i, j - 1))
    4. caller: dp(m + 1, vector<int>(n + 1, -1)) -> solve(s1, s2, m - 1, n - 1, dp)

EDIT DISTANCE (LC-72)
  Maanga:    word1 ko word2 banane ke MIN ops (insert / delete / replace)
  Pehchaan:  2 string + min ops -> LCS jaisa dp[i][j], no-match pe 3 choice
  Chaabi:
    1. base: i < 0 -> j + 1 (INSERT) · j < 0 -> i + 1 (DELETE) · memo
    2. s1[i] == s2[j] -> 0 + solve(i - 1, j - 1)
    3. warna 1 + min({ solve(i - 1, j) DELETE, solve(i, j - 1) INSERT, solve(i - 1, j - 1) REPLACE })
    4. caller (LCS wala reuse): dp(m + 1, n + 1, -1) -> solve(m - 1, n - 1, s1, s2, dp)

0/1 KNAPSACK
  Maanga:    n item (wt, val), capacity W, har item ek baar. MAX value
  Pehchaan:  lo / chhodo + item ONCE -> take me i - 1
  Chaabi:
    1. i < 0 -> 0 · memo dp[i][W]
    2. notTake = solve(W, i - 1) · take = 0
    3. if (wt[i] <= W) take = val[i] + solve(W - wt[i], i - 1)        (fit tabhi, warna index crash)
    4. return dp[i][W] = max(take, notTake) · caller: dp(n + 1, W + 1, -1) -> solve(..., n - 1, dp)

SUBSET SUM (boolean)
  Maanga:    nums[] (positive) + target. koi subset exactly target jodta? true / false
  Pehchaan:  knapsack ka true / false bhai
  Chaabi:
    1. base KRAM: target == 0 -> true (SABSE UPAR) · index < 0 -> false · index == 0 -> nums[0] == target
    2. memo dp[index][target] (-1 = nahi nikala)
    3. notTake = solve(target, index - 1) · take = false; nums[index] <= target ho to take = solve(target - nums[index], index - 1)
    4. return dp = take || notTake · caller: dp(n + 1, target + 1, -1) -> solve(nums, target, n - 1, dp)

PARTITION EQUAL SUBSET SUM (LC-416)
  Maanga:    nums[]. do hisse me baant sakte jinka sum barabar? true / false
  Pehchaan:  ek subset S/2 bana -> baaki apne aap S/2 -> subset sum
  Chaabi:
    1. loop se total S nikaalo
    2. dp(n + 1, vector<int>(S / 2 + 1, -1))
    3. S % 2 != 0 -> false
    4. return subsetSum(nums, S / 2, n, dp) -> andar solve(nums, target, n - 1, dp) (subset sum wala hubahu)

VALID PARENTHESIS STRING with '*' (LC-678)
  Maanga:    sirf '(' ')' '*', '*' = '(' ya ')' ya khaali. valid ban sakti? true / false
  Pehchaan:  har char pe choice + kitne khule -> state (index, open)
  Chaabi:
    1. index == s.size() -> open == 0 · memo dp[index][open]
    2. '(' -> solve(index + 1, open + 1)
    3. ')' -> open > 0 ho to solve(index + 1, open - 1), warna false
    4. '*' -> teen: open + 1 · (open > 0 ho to) open - 1 warna false · open wahi -> teeno ka ||
    5. caller: dp(n + 1, vector<int>(n + 1, -1)) -> solve(s, 0, 0, dp)

MATRIX CHAIN MULTIPLICATION (MCM)
  Maanga:    p[] dims (A_i = p[i-1] x p[i]). multiply order chuno ki total cost MIN
  Pehchaan:  range (i, j) + beech me deewar k -> INTERVAL DP
  Chaabi:
    1. i == j -> 0 · memo dp[i][j] · ans = INT_MAX
    2. for (k = i; k <= j - 1; k++) loop                            (k = deewar, j-1 TAK)
    3. ans = min(ans, solve(i, k) + solve(k + 1, j) + p[i-1] * p[k] * p[j])
    4. return dp[i][j] = ans · caller: dp(n + 1, n + 1, -1) -> solve(1, n - 1, p, dp)   (1 se: p[i-1])

BURST BALLOONS (LC-312)
  Maanga:    balloons[]. phodne pe coins = left * khud * right padosi (kinaare = 1). MAX coins
  Pehchaan:  MCM jaisa, k = range me AAKHRI phootne wala
  Chaabi:
    1. i > j -> 0 (khaali range) · memo dp[i][j] · ans = INT_MIN
    2. for (k = i; k <= j; k++) loop                                (j TAK, MCM me j-1)
    3. ans = max(ans, solve(i, k - 1) + solve(k + 1, j) + p[i-1] * p[k] * p[j+1])
    4. caller: n = size PEHLE, dp(n + 1, n + 1, -1), phir dono taraf 1 pad (insert begin + push_back)
       -> solve(1, n, p, dp)
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

**SAWAAL (5):**
```text
ASSIGN COOKIES (LC-455)
  Maanga:    g[] = greed, s[] = cookie size. s[j] >= g[i] to khush. MAX khush bachhe
  Pehchaan:  dono SORT + 2 pointer, chhoti cookie chhote greed ko
  Chaabi:
    1. i = 0 (bachha), j = 0 (cookie), count = 0; s khaali -> 0
    2. sort(g), sort(s)
    3. while (i < n && j < m) loop: s[j] >= g[i] -> i++, j++, count++ · warna j++ (badi cookie dhoondho)
    4. return count

JUMP GAME (LC-55)
  Maanga:    nums[i] = index i se max kood. 0 se last tak pahunch sakte? true / false
  Pehchaan:  pahunch sakte? -> greedy farthest reach
  Chaabi:
    1. maxReach = 0
    2. for (i = 0..n-1) loop: i > maxReach -> return false        (yahan tak pahunche hi nahi)
    3.   maxReach = max(maxReach, i + nums[i])                    (i jodo: absolute index)
    4. return maxReach >= n - 1

NON-OVERLAPPING INTERVALS (LC-435)
  Maanga:    MIN kitne hatao ki overlap na bache (touching chalta)
  Pehchaan:  "kam se kam hatao" -> greedy, chhota END wala rakho
  Chaabi:
    1. sort(intervals) (start pe); start = 0 (rakha hua), end = 1 (candidate), count = 0
    2. while (end < n) loop:
    3.   intervals[start][1] <= intervals[end][0] -> no overlap: start = end, end++   (<= : chhoona overlap nahi)
    4.   warna OVERLAP: count++; intervals[start][1] > intervals[end][1] -> start = end; end++
    5. return count                                               (overlap pe blindly start rakha -> galat)

GAS STATION (LC-134)
  Maanga:    gol me N pump, gas[i] mile, cost[i] kharcha. kis index se poora chakkar? na ho -> -1
  Pehchaan:  circular + start dhoondho -> running tank + reset
  Chaabi:
    1. index = 0, meriGaddiKaTank = 0, kyeNegativeHua = 0
    2. for (i = 0..n-1) loop: dono me += gas[i] - cost[i]          (tank reset hota, total kabhi nahi)
    3.   meriGaddiKaTank < 0 -> tank = 0, index = i + 1             (agle pump se naya start)
    4. return kyeNegativeHua < 0 ? -1 : index

TASK SCHEDULER (LC-621)
  Maanga:    tasks (letters), same task ke beech n gap. MIN total time (idle bhi)
  Pehchaan:  cooldown + min time -> sabse zyada wala task dhaancha banata
  Chaabi:
    1. vector<int> mp(26); har task pe mp[t - 'A']++; sort(mp)     (maxFreq = mp[25])
    2. gadde = maxFreq - 1, idleSlotes = n * gadde                (gap sirf BEECH me)
    3. for (i = 24 se 0 tak) loop: idleSlotes -= min(mp[i], gadde)
    4. idleSlotes > 0 -> return tasks.size() + idleSlotes · warna return tasks.size()
```

**DHYAAN:**
- greedy tabhi jab abhi ka best = aage ka best, warna DP
- intervals me "kam se kam hatao" -> END pe sort
- gas station: tank minus hua -> agla station naya start

**POORA:** [PATTERN 18](PATTERN_SHEET.md#pattern-18--greedy)
