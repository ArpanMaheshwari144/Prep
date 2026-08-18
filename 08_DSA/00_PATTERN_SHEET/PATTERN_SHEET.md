<h1 align="center">DSA PATTERN SHEET</h1>

## ★★★ DECISION — kaunsa DS/technique KAB aur KYUN (11-Aug, Arpan-diagnosed; sabse-upar)

> Neeche wali sheet PATTERNS deti hai (kya hote hain). Ye section DERIVATION deta hai — *"naya problem dekh ke kaise pata karun kaunsa lagega."*
> Arpan-diagnosis: abhi tak "WHAT hashmap hai" pata tha, "WHEN/WHY lagta hai" kabhi nahi. Ye wahi bharta. Recognition pe depend NAHI — REASON karo.

**THE METHOD (har NAYE problem pe 4 step):**
1. **BRUTE FORCE likho** — hamesha hota hai (partial answer + waqt).
2. **Poocho:** *"main baar-baar SAME kya compute kar raha?"* → **REDUNDANCY** dhoondo.
3. Us redundancy ko **CACHE/STORE** karo → recompute ki jagah **LOOK UP**.
4. **Cache-ka-DS hi PATTERN hai** (guess nahi — DERIVE).
> + "STORY hatao — structure kya? pairs? range? next-bigger/smaller? min/max? subproblems?"
> + FREEZE = threat-response (na capability). Calm → naya twist sirf hard PUZZLE. Brute+reason+bolo.

**STEP-1: BRUTE kaise likhein — NOUN → kitne loop** (enumerate + check):

| Problem me NOUN        | Loops | Shell                          |
|------------------------|-------|--------------------------------|
| har **element**        | 1     | `for i`                        |
| **pair** (i != j)      | 2     | `for i:  for j=i+1`            |
| **subarray** (contig.) | 2 (+inner) | `for l:  for r=l`  {andar sum/compute} |
| **triplet**            | 3     | `for i:  for j=i+1:  for k=j+1`|
| **subset** (saare)     | rec.  | choose / not-choose            |

> Loops = NOUN. Andar = CHECK (condition) + answer update (best/count/return).
> Poora naksha: **noun→loop (brute) → "kya dohra raha" (redundancy) → DS (neeche table).**

> **TEMPLATE (har brute):** ans=INIT → loops(noun) → if(CONDITION) ans=UPDATE → return.
> ★ subarray-SUM = 3 loop: for l, for r (subarray chuno) + inner sum. (ya running-sum se inner hatao.)

**TYPE-B — DATA-STRUCTURE derive karo** (ye kabhi practice nahi kiya tha; koi template NAHI):

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

**TYPE-A — TEMPLATE wale** (pehchano + skeleton; tera STRONG):

| Jab ye dikhe | → PATTERN | core |
|---|---|---|
| choices/subproblems + **overlap** | **DP** | state + recurrence + memo |
| saare **combinations/arrangements/subsets** | **BACKTRACKING** | choose→recurse→un-choose |
| **grid/graph** spread / connected / islands | **DFS/BFS** | visit+mark+neighbors |
| level-by-level / **shortest (unweighted)** | **BFS** | queue, level-order |
| prerequisites / cycle / ordering | **TOPO SORT (Kahn's)** | indegree + queue |
| **"minimum X jisme condition sach"** / sorted-search | **BINARY SEARCH (on answer)** | lo/hi, feasible(mid)? |

**DRILL (roz 10 min):** bare problem → solve MAT karo → sirf: brute soch → "kya dohra raha" → is table se DS DERIVE karo → galat to signal wapas padho. Type-A = pehchan (strong). Type-B = DERIVE (banana hai). Dono = poora naksha.

---

## ESSENCE — DSA ka ASLI tareeka (31-Jul, Arpan-derived)

> **DSA = pattern reflexively THOPNA nahi. Pehle PROBLEM samjho -> phir pattern.**
>
> 1. **Problem SAMJHO** — kya bol raha? kyun? kya maang raha? kaise? (shaant baitho, jump NAHI)
> 2. **PHIR** approach/pattern socho (ab template lage)
> 3. **PHIR** code
>
> direct-jump galat -- bhale hi answer SW/DP/graph ho, seedha pattern pe mat kudo. **samajho -> approach -> code.**
> source ka LABEL blindly mat lo ("reel ne SW bola" != SW). khud examine karo -- "sach me aisa hai?"
> **problem-solver** = ye (understand-first + question-the-label), na ki pattern-ratta. dimaag freeze nahi -> pehla-move = "problem samjho".

---

## CODE-LIKHTE-WAQT: INIT-DEFAULT ka RULE (min/max silly-bug)

> min le raha hai  -> "invalid / nahi-ho-sakta" ka default = INT_MAX  (badi value, min me kabhi galat na jeete).
> max le raha hai  -> default = INT_MIN.
> 0 sirf tab jab 0 sach-much VALID answer ho.
>
> KYUN: coin-change (min problem) me take = 0 daala -> 0 HAMESHA min jeet gaya -> jhootha answer.
>       INT_MAX daalo -> na-ho-sakne wala branch min me HAAR jaata (sahi).
>
> PAKADNE ka NET: ye bug aankh se code padhne pe nahi dikhta.
>   -> sabse CHHOTA dry-run chalao (n=1, ya jaha coin fit hi na ho) -> 2 line me galat output turant dikhta.
>   -> init likhte waqt haath ruke: "min le raha -> default MAX? max le raha -> default MIN?"

---

## NAAM-SE-CONFUSE MAT HO (Container vs Trapping vs Histogram — teeno ALAG!)
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

---

## PATTERN 1 — TWO POINTER

```

 BROAD IDEA: do pointer (dono taraf se, ya slow-fast) -> ek saath move karke O(n).

┌── FAMILY: slow/fast OVERWRITE ────────────────────────────────
│ KYUN SAATH: fast pura array scan karta; jo element "rakhna" hai use slow pe likh do + slow++; baaki apne aap peeche reh jaate. teeno same skeleton (move-zeroes cousins).
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MOVE ZEROES
 └──────────────────────────────────────────────────────────────
     slow = agli jagah jahan non-zero rakhna hai; fast SCAN karta.
     fast pe non-zero mila -> pehle swap(nums[slow], nums[fast]) -> PHIR slow++.
     ORDER: swap PEHLE, slow++ BAAD (slow tabhi aage jab ek non-zero apni jagah lag gaya).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ REMOVE DUP (SORTED)
 └──────────────────────────────────────────────────────────────
     move-zeroes cousin. slow = last-UNIQUE ka index, fast SCAN.
     naya unique (nums[slow] != nums[fast]) -> pehle slow++ (slow ab duplicate/khali slot pe) -> PHIR nums[slow] = nums[fast] (overwrite).
     ORDER: yahan slow++ PEHLE, overwrite BAAD (move-zeroes se ULTA). fast har baar++. unique count = slow+1.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ REMOVE ELEMENT
 └──────────────────────────────────────────────────────────────
     saare 'val' hata, non-val ki count k lauta (move-zeroes cousin). slow = agli keep-jagah, fast scan.
     nums[fast] != val -> pehle nums[slow] = nums[fast] -> PHIR slow++.   nums[fast] == val -> skip (sirf fast++).
     ORDER: write PEHLE, slow++ BAAD (move-zeroes jaisa). end me slow = k (bache shuru ke k me).

┌── FAMILY: left/right COMPARE-CONVERGE ────────────────────────
│ KYUN SAATH: do pointer dono chhor se andar aate; har step CHAR/condition compare karke decide. (palindrome-type + vowel-swap.)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ VALID PALINDROME (SIMPLE)
 └──────────────────────────────────────────────────────────────
     left/right dono taraf se, compare karte andar aao. (sirf lowercase-letters wala aasan version.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ VALID PALINDROME (FULL, LC-125)
 └──────────────────────────────────────────────────────────────
     mixed chars (letter/digit/space/punct, UPPER+lower). SIRF alnum consider · case IGNORE. helpers: isalnum, tolower (<cctype>).
     2 pointer left/right, ORDER:
       1. tolower(s[left])==tolower(s[right]) -> match -> left++ right--.
       2. else !isalnum(s[left])  -> left non-alnum -> skip -> left++.
       3. else !isalnum(s[right]) -> right non-alnum -> skip -> right--.
       4. else (dono alnum PAR alag) -> return FALSE.
     bina false cross -> TRUE. (koi alnum na ho / khaali -> true.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ VALID PALINDROME II
 └──────────────────────────────────────────────────────────────
     = palindrome YA ek char DELETE karke palindrome ban jaaye.
     IDEA: two-pointer; MISMATCH pe -> ya LEFT char delete ya RIGHT char delete. koi ek bane -> true, dono na bane -> false.
     TEMPLATE (CLEAN): helper isPalindrome(s,l,r). main loop; mismatch pe seedha ->
         return isPalindrome(s,l+1,r) || isPalindrome(s,l,r-1);   (left-delete YA right-delete; || = ek se bhi bane -> true)
         koi mismatch na aaye -> already palindrome -> true.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ REVERSE VOWELS
 └──────────────────────────────────────────────────────────────
     sirf vowels (a/e/i/o/u, lower+upper) aapas me reverse, baaki char apni jagah.
     chhota helper isVowel(ch). left/right pointer:
     dono vowel -> swap + left++ right--.  left vowel nahi -> left++.  warna right--.

┌── FAMILY: left/right by-VALUE decision ───────────────────────
│ KYUN SAATH: sorted-ish / value-based; sum ya area ki VALUE dekh ke decide karo konsa pointer chalana. (koi target-jodi dhoondhna.)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ CONTAINER MOST WATER
 └──────────────────────────────────────────────────────────────
     left/right; area = min(h)*width; CHHOTI height wala pointer andar (bottleneck).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ TWO SUM II (SORTED)
 └──────────────────────────────────────────────────────────────
     sorted -> 2 pointer (left,right); sum==target -> mila (return 1-based).
     sum<target -> left++ (bada chahiye); sum>target -> right-- (chhota). O(1) space.
     (regular two-sum UNSORTED = hashmap; SORTED = 2-pointer.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SUM OF SQUARE NUMBERS
 └──────────────────────────────────────────────────────────────
     a²+b²==c possible? (LC 633). RECOGNITION: 2 NUMBER (pair) chahiye -> TWO-POINTER.
     low=0, high=sqrt(c). ans=low²+high²: ==c->true · <c->low++ · >c->high--. low<=high tak, warna false.
     ★ low<=high (NOT low<high) -- c=0 ka 0+0 single-point bhi check ho jaaye. OVERFLOW: ans = long long.

┌── FAMILY: fill-from-END ──────────────────────────────────────
│ KYUN SAATH: result ko PEECHE se bharo (aakhir me jagah khali). bada element end me daalte hue 2/3 pointer.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SQUARES OF SORTED ARR
 └──────────────────────────────────────────────────────────────
     SAAR: sorted arr me negatives ka square order ULTA -> sabse BADA square dono CHHOR pe (beech me nahi). 2-ptr, ans PEECHE se bharo. O(n), no sort.
     TEMPLATE: left=0, right=n-1, k=n-1.
        while(left<=right):
           if(left² <= right²) -> ans[k--]=right², right--
           else                -> ans[k--]=left²,  left++
     FAMILY: fill-from-END (result peeche se bharo).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MERGE SORTED ARRAY
 └──────────────────────────────────────────────────────────────
     SAAR: 2 sorted arr; nums1 ke END me n khaali. in-place -> PEECHE se bharo (aage se karo to zinda element overwrite). O(m+n).  ★ k=m+n-1 NA m*n-1
     TEMPLATE: i=m-1, j=n-1, k=m+n-1.
        while(i>=0 && j>=0):                       // ★ && (|| nahi -> -1 index crash)
           if(nums1[i] < nums2[j]) -> nums1[k--]=nums2[j], j--
           else                    -> nums1[k--]=nums1[i], i--
        while(i>=0): nums1[k--]=nums1[i], i--
        while(j>=0): nums1[k--]=nums2[j], j--      // nums2[j] NA nums1[j]

┌── FAMILY: PARTITION / swap-in-place ──────────────────────────
│ KYUN SAATH: elements ko category (0/1/2 ya even/odd) me baanto; galat-jagah pe swap. left/mid/high ya left/right pointer.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SORT COLORS (0,1,2)
 └──────────────────────────────────────────────────────────────
     Dutch flag, 3 pointer (low/mid/high):
     nums[mid]==0 -> swap(low,mid), low++ AUR mid++ (dono).
     nums[mid]==1 -> sirf mid++.
     nums[mid]==2 -> swap(mid,high), high-- (mid++ NAHI -> high se aaya element unknown, dobara check).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SORT ARRAY BY PARITY
 └──────────────────────────────────────────────────────────────
     evens pehle, odds baad (aapas order koi bhi). left/right pointer.
     left ODD && right EVEN (dono galat jagah) -> swap + left++ right--.
     left EVEN (sahi) -> left++.  warna -> right--.
     TRAP: left even && right odd pe swap MAT karo -> dono already sahi jagah.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ 3SUM (LC-15)  = SORT + ek fix + baaki 2-pointer (+ duplicate skip)
 └──────────────────────────────────────────────────────────────
     SAAR : saare UNIQUE triplets jinka sum==0. SORT -> ek number FIX -> baaki 2 ke liye 2-pointer (sum = -fixed).
     TEMPLATE:
         sort(begin(nums), end(nums));
         for(int i=0; i<nums.size(); i++){
             if(i>0 && nums[i]==nums[i-1]) continue;              // duplicate FIXED skip
             int low=i+1, high=nums.size()-1, sum=-nums[i];
             while(low<high){
                 if(nums[low]+nums[high]==sum){
                     ans.push_back({nums[i],nums[low],nums[high]});
                     while(low<high && nums[low]==nums[low+1]) low++;   // duplicate LOW skip
                     while(low<high && nums[high]==nums[high-1]) high--; // duplicate HIGH skip
                     low++; high--;
                 }
                 else if(nums[low]+nums[high] < sum) low++;       // chhota -> low aage (bada chahiye)
                 else high--;                                     // bada  -> high peeche
             }
         }
     CRUX = DUPLICATE skip 3 JAGAH: (1) fixed i (i>0 && nums[i]==nums[i-1]) (2) match ke baad LOW (3) match ke baad HIGH.
     sum = -nums[i] (fixed ko doosri taraf) -> sorted pe 2-pointer (= 2SUM-II).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ IS SUBSEQUENCE
 └──────────────────────────────────────────────────────────────
     s, t ki subsequence hai kya (order same, beech ke char skip allowed). 2 pointer: i for s, j for t.
     s[i]==t[j] -> i++ AUR j++ (char mila).  nahi mila -> sirf j++ (t aage scan).
     ANT: i >= s.size() -> saare s-char order me mil gaye -> TRUE. warna FALSE.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ TRAPPING RAIN WATER (LC-42)
 └──────────────────────────────────────────────────────────────
     CORE: bar `i` ke upar paani = min(leftMax[i], rightMax[i]) - height[i].  (min kyun: CHHOTI side se paani bah jaata.)

     [A] O(n) space: leftMax[] ek pass AAGE se · rightMax[] ek pass PEECHE se · ans += min(leftMax[i], rightMax[i]) - height[i].

     [B] O(1) two-pointer (arrays hata, bas 2 var):
           left=0, right=n-1;   leftMax=height[0], rightMax=height[n-1];
           while (left < right):
               if (leftMax <= rightMax):  leftMax  = max(leftMax,  height[left]);   ans += leftMax  - height[left];   left++;
               else:                      rightMax = max(rightMax, height[right]);  ans += rightMax - height[right];  right--;
         REFRESH pehle, ADD baad (warna naya-bada-bar pe ans NEGATIVE de deta).
         kyun SAFE: chhoti-max side ka paani sirf usi side se limit (doosri side oonchi wall already khadi).
     edge: empty -> 0.
```

---

## PATTERN 2 — SLIDING WINDOW

```

 KAB PEHCHANO: CONTIGUOUS subarray/substring + "longest / shortest / max / min / COUNT" maanga -> SW socho.

 2 TYPE:
   (a) FIXED window   -> size k DIYA hai. i/j pointer: sum += nums[j]; jab (j-i+1) >= k -> track + left hatao (sum -= nums[i], i++).
   (b) VARIABLE window-> size condition se. TEMPLATE:
          for j in arr:  window me arr[j] add (update tracker)
                         while (INVALID): left[i] hatao (tracker update), i++
                         ans update  (length = j-i+1, ya count += j-i+1)

 KYA TRACK: sum · freq-map (distinct=map.size / char-count) · zeros-count · product · maxFreq.

 KEY: shrink ke BAAD window HAMESHA valid -> ans UNCONDITIONAL (koi if(==k) nahi).

 LENGTH ya COUNT: length -> max(ans, j-i+1) · subarray COUNT -> count += (j-i+1) (window size).

 IF vs WHILE (ye confusion CLEAR kar) -> SAWAAL: "window ka SIZE fix hai, ya condition pe depend?"
     · SIZE fix (koi number diya: k-elements ya p.length()) -> IF (ek add, ek remove, size wahi rehta).   [Max-Sum-of-K · Anagram]
     · SIZE condition pe (sum/distinct/repeat/product) -> WHILE (jab tak invalid, ek se zyada nikal sakte).  [baaki sab]
     TRAP -- `k` ka matlab dekh: Max-Sum-size-K me k = SIZE (fix -> IF); Subarray-Product<K me k = product LIMIT (size NAHI -> WHILE). same letter `k`, alag matlab.
 [track] · [INVALID kab -> shrink] · [ans]

 ONE SKELETON (fixed + variable DONO isi pe -- ye poora khel):
    i = 0, j = 0
    while (j < n):                     // EXPAND (j HAMESHA aage). [for(j..) == while(j<n){..;j++} -- SAME]
        arr[j] window me ADD           (tracker update: sum / map / zeros / product)
        while / if  (WINDOW BAD):      // SHRINK (i aage)
            arr[i] REMOVE (tracker update);  i++
        ans UPDATE
        j++
    -> sirf 2 cheez badalti: (1) inner IF (fixed, ek remove) ya WHILE (variable, jab tak invalid)
                             (2) "WINDOW BAD" kya + kya track. baaki dhaancha har baar SAME.

┌── FAMILY: variable-LONGEST ───────────────────────────────────
│ KYUN SAATH: window expand; jab INVALID ho tab left se shrink (WHILE); ans = MAX length. (kya track alag: freq/maxFreq/zeros/distinct.)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LONGEST SUBSTRING NO-REPEAT (LC-3)  = variable window + freq-map
 └──────────────────────────────────────────────────────────────
     SAAR : longest substring jisme koi char REPEAT na ho. variable window + freq-map(mp).
     TEMPLATE:
         unordered_map<char,int> mp;  int i=0, j=0, minLen=INT_MIN;
         while(j < s.size()){
             mp[s[j]]++;                          // window me add
             while(mp[s[j]] > 1){                 // INVALID: current char (s[j]) repeat
                 mp[s[i]]--; i++;                 // left shrink jab tak repeat khatam
             }
             minLen = max(minLen, j - i + 1);     // valid window length
             j++;
         }
         return minLen==INT_MIN ? 0 : minLen;     // khaali string -> INT_MIN reh jaata -> 0
     INVALID = mp[s[j]]>1 (jo abhi add hua wahi repeat). empty-string edge -> INT_MIN -> 0.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ CHAR REPLACEMENT (LONGEST, LC-424)  = variable window + freq-map (k badal sakte)
 └──────────────────────────────────────────────────────────────
     SAAR : longest window jisme <=k char badal ke sabko SAME bana sako. variable window + freq-map.
     KEY  : windowLen - maxFreq = kitne char badalne padenge. <=k -> valid.
     j pe : mp[s[j]]++, maxFreq = max(maxFreq, mp[s[j]]).
     VALID   (windowLen - maxFreq <= k)  -> maxLen = max(maxLen, windowLen).
     INVALID (windowLen - maxFreq > k)   -> left shrink (mp[s[i]]--, i++).
     CORE TRICK (non-obvious): maxFreq kabhi GHATAO mat (shrink pe recompute nahi). window best-length
        se chhota hota hi nahi (sirf badhta/slide) -> stale/purana maxFreq bhi answer kharab nahi karta.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAX CONSECUTIVE ONES III (LC-1004)  = variable window + zeros-count (k flips)
 └──────────────────────────────────────────────────────────────
     SAAR : at most k zeros ko FLIP -> longest all-1 window. sirf zeroCount track (freq-map nahi).
     TEMPLATE:
         int ans=INT_MIN, zeroCount=0, i=0, j=0;
         while(j < nums.size()){
             if(nums[j]==0) zeroCount++;          // window me zero add
             while(zeroCount > k){                // INVALID: k se zyada zero (flip nahi kar sakte)
                 if(nums[i]==0) zeroCount--;      // left ka zero nikla to count kam
                 i++;
             }
             ans = max(ans, j - i + 1);           // UNCONDITIONAL (koi if nahi)
             j++;
         }
         return ans==INT_MIN ? 0 : ans;
     CORE LEARNING: ans UNCONDITIONAL update -> if(zeroCount==k) MAT lagao.
        kyun: upar while(zeroCount>k) shrink ke BAAD window HAMESHA valid (<=k) -> yahan pahunchte hi valid -> condition faltu.
        (galti thi: if(zeroCount==k) -> sirf exactly-k ginta -> k=0 / k>zeros case toot jaate.)
     shrink pe nums[i]==0 hi zeroCount-- (1 nikalne pe count na badle).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ FRUIT INTO BASKETS
 └──────────────────────────────────────────────────────────────
     SAAR : "longest subarray with AT MOST 2 DISTINCT" (fruit/basket sirf kahani). SW skeleton + map<type,count>.
     DELTA (SW skeleton pe -- sirf ye badalta):
         mp[nums[j]]++;                              // window me add
         while(mp.size() > 2){                       // INVALID: 2 se zyada distinct
             mp[nums[i]]--;
             if(mp[nums[i]]==0) mp.erase(nums[i]);   // count 0 -> ERASE
             i++;
         }
         ans = max(ans, j - i + 1);
     CRUX = erase-on-0: count 0 hone pe key map me PADI reh jaati -> mp.size() galat distinct dega -> condition toot. isliye ERASE.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LONGEST AT-MOST-K DISTINCT
 └──────────────────────────────────────────────────────────────
     FRUIT-INTO-BASKETS ka GENERAL version. wahi variable window + map<char,count>,
     distinct (mp.size()) > k ho -> left se shrink (erase-on-0). ans = max length.
     farak: fruit me "2" hardcoded tha, yahan wahi jagah "k" (mp.size() > k).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ EQUAL SUBSTR WITHIN BUDGET (LC-1208)  = longest-subarray-sum <= k (string ke bhes me)
 └──────────────────────────────────────────────────────────────
     SAAR : s ko t banane me har char ka cost = |s[i]-t[i]|. longest window jiska cost-SUM <= maxCost.
     KEY  : cost-array bana ke "longest subarray with sum <= k" -- bas string ke bhes me chhupa.
     j pe : cost += abs(s[j]-t[j]).
     INVALID (cost > maxCost) -> WHILE shrink: cost -= abs(s[i]-t[i]), i++.
     ans  : max(ans, j-i+1).  track sirf cost (koi map nahi).

┌── FAMILY: variable-SHORTEST ──────────────────────────────────
│ KYUN SAATH: ULTA — jab VALID ho tab shrink (WHILE) karke chhota karo; ans = MIN length.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MIN SUBARRAY LEN (>=TARGET, LC-209)  = SHORTEST window (longest se ULTA)
 └──────────────────────────────────────────────────────────────
     SAAR : positive nums, sum >= target wala SABSE CHHOTA subarray ki length (nahi mile -> 0).
     TEMPLATE:
         int sum=0, minLen=INT_MAX, i=0, j=0;
         while(j < nums.size()){
             sum += nums[j];                       // window add (expand)
             while(sum >= target){                 // VALID hote hi -> record + SHRINK (aur chhota dhoondo)
                 minLen = min(minLen, j - i + 1);   //   record ANDAR (shrink loop ke andar)
                 sum -= nums[i]; i++;               //   left shrink
             }
             j++;
         }
         return minLen==INT_MAX ? 0 : minLen;      // koi valid window nahi -> 0
     LONGEST vs SHORTEST (yahi confusion ki JAD -- window BADA chahiye ya CHHOTA):
        LONGEST  (jaise no-repeat): window BADA chahiye -> shrink SIRF jab INVALID ho (validity wapas laane ko) ->
                 record BAAHAR (shrink ke BAAD, valid window ki length).
        SHORTEST (ye / min-window):  window CHHOTA chahiye -> VALID hote hi record + shrink (chhota karte jao jab tak valid) ->
                 record ANDAR (shrink loop ke andar, har step).   <- YE wahi.
        1-line: LONGEST = shrink-jab-MAJBOORI(invalid), record-baad  ·  SHORTEST = shrink-jab-tak-VALID, record-andar.
     char-matching wala SHORTEST = MIN WINDOW (LC-76) -> neeche "need-map + COUNT" family me (LC-1358 ke saath).

┌── FAMILY: FIXED window ───────────────────────────────────────
│ KYUN SAATH: window ka size k pehle se DIYA hai; slide karte jao.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAX SUM OF K (FIXED)  = fixed window size-k + running sum
 └──────────────────────────────────────────────────────────────
     SAAR : k-size ke har window ka sum -> unme se MAX. fixed window (size hamesha k).
     KEY  : variable nahi -- size fix k, isliye IF (ek hi remove).
     j pe : sum += nums[j].
     FULL (j-i+1 >= k) -> maxSum = max(maxSum, sum); phir 1 remove (sum -= nums[i], i++).
     WHY IF-not-WHILE : fixed size -> har step 1 add + 1 remove -> slide. variable hota to WHILE (kai remove).
     ans  : maxSum. window k-size pe slide.

┌── FAMILY: COUNT (length nahi) ────────────────────────────────
│ KYUN SAATH: subarray GINTI chahiye (length nahi); valid window pe count += (j-i+1).
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SUBARRAY PRODUCT < K  = variable window + COUNT (running product)
 └──────────────────────────────────────────────────────────────
     SAAR : kitne subarray jinka product < k. variable window, COUNT (length nahi).
     j pe : prod *= nums[j].
     INVALID (prod >= k) -> shrink: prod /= nums[i], i++.
     COUNT TRICK: valid -> count += (j-i+1) = window size (j pe end hone wale saare valid subarray). (bahut count-Q me)

┌── FAMILY: need-map + COUNT (t ke SAARE char chahiye -- --/++ MIRROR) ─────
│ LC-76 + LC-1358 ka EXPAND+SHRINK bilkul SAME. FARAK = answer KAHAN (LC-76 loop-ANDAR · LC-1358 loop-BAAD). t="abc" -> dono ek.
└───────────────────────────────────────────────────────────────

 SHARED SKELETON (expand + shrink -- dono me same):
      need-map(t);  count = t.size();                 // count==0 => window me t ke SAARE char (VALID)
      while (j < n) {                                  // OUTER: j (right) aage
          if (mp[s[j]] > 0) count--;                  // t-char mila -> ek zaroorat puri
          mp[s[j]]--;                                 // IF ke BAHAR: non-t char -ve -> count me "invisible"
                                                      //   (andar rakha to D-ASYMMETRY BUG: shrink me count++ galat)
          while (count == 0) {                         // INNER: VALID -> shrink (i++)
              mp[s[i]]++;  if (mp[s[i]] > 0) count++;  i++;   // MIRROR: aana = -- , jaana = ++
          }
          j++;
      }

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MIN WINDOW SUBSTRING (LC-76, Hard) -- answer = MIN-track, loop ke ANDAR
 └──────────────────────────────────────────────────────────────
     s ka sabse CHHOTA window jisme t ke saare char.  count = t.size().
     while (j < n) {                          // OUTER: j (right) aage
         if(mp[s[j]]>0) count--;  mp[s[j]]--; // EXPAND
         while (count == 0) {                 // INNER: VALID -> shrink + SAVE (answer andar)
             len=j-i+1; if(len<minLen){minLen=len; index=i;}
             mp[s[i]]++; if>0 count++; i++;
         }
         j++;
     }
     return minLen==INT_MAX ? "" : s.substr(index, minLen).
     DERIVE: chhote example HAATH-trace (ADOBECODEBANC -> BANC) se map+count nikla.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SUBSTRINGS CONTAINING ALL (LC-1358) -- answer = COUNT, loop ke BAAD
 └──────────────────────────────────────────────────────────────
     = LC-76 ka SAME minWindow() -- literally REUSE: numberOfSubstrings(s) = minWindow(s, "abc").
       farak SIRF answer-line:
           LC-76   : while ke ANDAR -> if(len<minLen){minLen; index}   (MIN track)
           LC-1358 : while ke BAAD  -> ans += i                        (COUNT)
       WHY ans += i: shrink INVALID tak -> i = kitne valid left-starts. hand-trace "abcabc": i = 0,0,1,2,3,4 -> sum 10.
     t = "abc"  (count = 3 = t.size()).
     while (j < n) {                                 // OUTER: j (right) aage
         if(mp[s[j]]>0) count--;  mp[s[j]]--;        // EXPAND (same)
         while (count == 0) { mp[s[i]]++; if>0 count++; i++; }   // INNER: shrink to INVALID
         ans += i;                                   // outer ke ANDAR, inner-while ke BAAD -> har j pe
         j++;
     }
     WHY ans += i: shrink window INVALID tak le jaata -> ab [0..i-1] SAARE valid left-starts -> ginti = i.
     CODE: numberOfSubstrings(s) = minWindow(s, "abc")  -- wahi SW function reuse, bas t="abc" pass.

┌── FAMILY: ANAGRAM-window ─────────────────────────────────────
│ KYUN SAATH: fixed p-length window + har position pe anagram-check (isAnagram helper reuse).
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ FIND ALL ANAGRAMS (LC-438)  = ANAGRAM-check + FIXED window (PARENT)
 └──────────────────────────────────────────────────────────────
     SAAR : s me p ke SAARE anagram ki START-INDEX. fixed window (p ki length) + har window pe anagram-check.
     TEMPLATE:
         bool isAnagram(a,b): size same + har char count barabar (map: a se ++, b se --, sab 0 -> true).
         int i=0, j=0;
         while(j < s.size()){
             while(j-i+1 > p.size()) i++;                          // window ko p-size pe rakho (FIXED)
             if(j-i+1 == p.size())
                 if(isAnagram(s.substr(i, j-i+1), p)) ans.push_back(i);  // anagram -> START-index push
             j++;
         }
     valid-anagram helper reuse. fixed-window slide + har full-window pe check.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ PERMUTATION IN STRING (LC-567)  = FIND-ALL-ANAGRAMS ka EXACT same code
 └──────────────────────────────────────────────────────────────
     SAAR : s2 me s1 ka koi permutation (=anagram) substring hai? (bool)
     = FIND-ALL-ANAGRAMS ka SAME code -- REUSE: checkInclusion(s1,s2) = findAnagrams(s2, s1).
       farak SIRF answer-line:
           LC-438 : isAnagram mila -> ans.push_back(i)  ... loop-baad: return ans    (SAARE indices)
           LC-567 : isAnagram mila -> return true        ... loop-baad: return false  (pehla match = bool)
     CODE (findAnagrams; skeleton LC-438 wala, sirf answer-handle badla):
         while(j < s.size()){
             while(j-i+1 > p.size()) i++;                  // window ko p-size pe rakho (FIXED)
             if(j-i+1 == p.size())
                 if(isAnagram(s.substr(i,j-i+1), p))
                     return true;                          // <- DELTA: push ki jagah return true
             j++;
         }
         return false;                                     // loop-baad: koi na mila
     checkInclusion(s1,s2) = findAnagrams(s2, s1).

 LONGEST vs SHORTEST (yaad rakh): LONGEST -> shrink jab INVALID, ans=MAX. SHORTEST (min-len) -> shrink jab VALID, ans=MIN.

┌── FAMILY: MONOTONIC DEQUE (window max/min) ───────────────────
│ KYUN ALAG: expand/shrink+tracker NAHI. har window ka MAX/MIN O(n) -> deque me INDICES (values decreasing), front = answer.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SLIDING WINDOW MAXIMUM (LC-239)  = monotonic DEQUE of INDICES
 └──────────────────────────────────────────────────────────────
     SAAR : har size-k window ka MAX, O(n). deque me INDICES rakho (values DECREASING) -> front = window ka max.
     CONNECT : back-pop (nums[back]<=nums[i]) = MONOTONIC banana -- ye UNIVERSAL (stack ho ya deque, wahi cheez; isi se monotonic).
               DEQUE lene ki ASLI (aur AKELI) wajah = WINDOW-EXPIRY: front se purana index hatana (dq.front()==i-k -> pop_front).
               = monotonic STACK + bas wahi 1 window-front-expiry delta. (front-pop hamesha EXISTING element hota, naya nahi.) order dono me matter.
     CORE : nums[i] aaya -> usse CHHOTE back-wale BEKAAR (jab tak nums[i] zinda, wo max ban hi nahi sakte) -> pop_back.
     ★ ORDER MATTERS (warna galat ans): (1) front-expiry -> (2) monotonic-clean + push -> (3) record. record hamesha LAST.
     TEMPLATE:
         deque<int> dq;  vector<int> ans;                              // dq = INDICES, front = MAX
         for(int i=0; i<nums.size(); i++){
             if(!dq.empty() && dq.front()==i-k) dq.pop_front();        // 1) front-index window se bahar (==i-k) -> hata
             while(!dq.empty() && nums[dq.back()]<=nums[i]) dq.pop_back(); // 2) chhote bekaar -> pop
             dq.push_back(i);
             if(i>=k-1) ans.push_back(nums[dq.front()]);               // 3) window poora -> front=max
         }
     TRAP: deque me INDICES (values nahi -- expiry ke liye index chahiye) · front-expiry ==i-k kaafi (har step check) · record sirf i>=k-1.
     FAMILY: MIN chahiye -> pop_back condition ULTA (nums[back] >= nums[i]).
```

---

## PATTERN 3 — HASHING

```

 BROAD IDEA: hashmap/hashset = O(1) lookup. "pehle dekha?" / "complement hai?" / count -> O(n).

 COMMON SKELETON (seen / complement / count family -- ek pass):
    map/set banao   (kabhi PRELOAD: two-sum khaali · subarray-sum-k -> mp{0:1})
    for x in arr:
        CHECK pehle   (map me complement/seen/count hai? -> ans use/count)   // UPDATE se PEHLE
        UPDATE baad   (map me x daalo / count++)
    ORDER KYUN (check-before-update): warna element KHUD se match kar leta (two-sum: x apne aap ko complement maan leta). isliye pehle dekho, phir daalo.
    (NOTE: har hashing Q one-pass nahi -- group-anagrams = pehle poora map bharo phir output · longest-consec = set-membership. ye skeleton "seen/complement/count" family ke liye.)

┌── FAMILY: 2-MAP BIJECTION ────────────────────────────────────
│ KYUN SAATH: dono taraf ki mapping (mp1 + mp2); clash dono side check; koi ek clash -> false (||). (one map kaafi nahi.)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ ISOMORPHIC STRINGS
 └──────────────────────────────────────────────────────────────
     SAAR : s->t har char ka 1-to-1 mapping (bijection)? "egg"->"add" true; "foo"->"bar", "ab"->"aa" false.
     KEY  : 2 MAP dono taraf -- mp1=s->t, mp2=t->s. ek map kaafi nahi ("ab"->"aa" clash sirf ULTI-taraf pakadta).
     CLASH: mp.count(key)==1 && mp[key]!=val. dono taraf check, || (kisi ek taraf bhi clash -> false).
     TEMPLATE:
         for(i=0..n-1){
             if( (mp1.count(s[i]) && mp1[s[i]]!=t[i]) || (mp2.count(t[i]) && mp2[t[i]]!=s[i]) )
                 return false;
             mp1[s[i]]=t[i];  mp2[t[i]]=s[i];
         }
         return true;

 ┌──────────────────────────────────────────────────────────────
 │ ▸ WORD PATTERN
 └──────────────────────────────────────────────────────────────
     = ISOMORPHIC ka SAME 2-map clash-check. REUSE: wahi mp1/mp2 || check, bas char <-> WORD.
       farak SIRF 3 delta:
           (1) INPUT: char[] ki jagah word-stream -> stringstream ss(s); while(ss>>word).
           (2) KEY  : pattern[i] <-> word  (char<->char nahi).
           (3) GUARD: end me pattern.size()==j (word-count) -> warna false ("a","dog cat" galat-true se bache).
     TEMPLATE:
         stringstream ss(s); string word; int i=0, j=0;
         while(ss>>word){
             if(i<pattern.size()){
                 if( (mp1.count(pattern[i]) && mp1[pattern[i]]!=word) || (mp2.count(word) && mp2[word]!=pattern[i]) )
                     return false;                       // <- ISOMORPHIC ki EXACT clash-line, bas char->word
                 mp1[pattern[i]]=word; mp2[word]=pattern[i]; i++;
             }
             j++;
         }
         return pattern.size()==j;
     EDGE: words ki ginti (j) == pattern.size() honi chahiye (warna "a","dog cat" galat true). end me pattern.size()==j -> true.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ TWO SUM
 └──────────────────────────────────────────────────────────────
     map me {value -> index}. har num pe dekho: (target - num) map me HAI? -> haan to jodi mil gayi (dono index).
     ek pass: pehle CHECK phir daalo (taaki num khud se na match kare).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ VALID ANAGRAM
 └──────────────────────────────────────────────────────────────
     dono me har char ka count barabar? count-array[26] (ya map): string-1 se ++, string-2 se -- -> sab ZERO -> anagram.
     (length alag -> turant false.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ GROUP ANAGRAMS
 └──────────────────────────────────────────────────────────────
     anagrams ka SORTED-roop same hota -> har word SORT karo = KEY -> map<sortedKey, list<word>> me daalo -> lists hi groups.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SUBARRAY SUM = K
 └──────────────────────────────────────────────────────────────
     prefix-sum + hashmap {prefixSum -> count}. running sum chalao; count += mp[sum - k], phir mp[sum]++.
     mp[0]=1 se START zaroori (jab sum khud == k ho, tab prefix-0 chahiye). idea: pichla prefix jahan se ab tak k bana.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ COUNT DIVISIBLE PAIRS  = two-sum/hashmap-complement, par KEY = REMAINDER
 └──────────────────────────────────────────────────────────────
     count pairs (i<j): (A[i]+A[j])%k==0.
     DERIVE: brute O(n²) har pair -> "partner pehle aaya?" baar-baar -> partner ko hashmap me CACHE -> O(n).
     ★ A[i] KYUN, prefix NAHI: pair = 2 ALAG element (contiguous range nahi) -> har element ka apna remainder.
     sum = ((A[i]%k)+k)%k;   count += mp[(k-sum)%k];   mp[sum]++;
        partner = (k-sum)%k · lookup PEHLE phir insert (warna khud-se-pair).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LONGEST CONSECUTIVE SEQ (LC-128)
 └──────────────────────────────────────────────────────────────
     saare num ek SET me daalo. har num pe: agar (num-1) set me NAHI -> ye sequence ka START ->
        curr = num, count = 1;  while (set me curr+1) { count++; curr++; }  -> ans = max(ans, count).
     (num-1)-check kyun: sirf START-num se ginti chalao -> har element ek hi baar visit -> O(n) (warna O(n^2)).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAJORITY ELEMENT (LC-169, >n/2)
 └──────────────────────────────────────────────────────────────
     map se: mp[x]++; count > n/2 wala return. O(n) time, O(n) space. (seedha.)
     BOYER-MOORE VOTING (O(1) space): majority n/2 se zyada -> baaki sab milkar bhi kam -> do ALAG element CANCEL karo, majority bachega.
       candidate + count. ==candidate -> count++ · !=candidate -> count-- · count==0 -> candidate=nums[i], count=1 (0 chhoda to -1 me chala jaayega, reset kabhi nahi -> BUG). end: candidate = ans.
```

---

## PATTERN 4 — PREFIX SUM

```

 BROAD IDEA: running sum pehle se rakho -> range/left-right sum O(1) (baar-baar loop nahi).

┌── FAMILY: "assign PEHLE, update BAAD" ────────────────────────
│ KYUN SAATH: current index ke liye pehle result store karo (left/right sum), PHIR running value update. order galat = bug.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ PIVOT INDEX
 └──────────────────────────────────────────────────────────────
     total nikaalo; leftSum chalao. order: rightSum=total-leftSum-nums[i]; if(left==right)return i; PHIR leftSum+=nums[i]. (leftSum BAAD me)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ PRODUCT EXCEPT SELF (LC-238)
 └──────────────────────────────────────────────────────────────
     ans[i] = (i se PEHLE sabka product) × (i ke BAAD sabka product).  no division, O(n).
     running `prod` se 2 pass (assign-BEFORE-update dono me):
        left-pass  (aage):    left[i]  = prod;  prod *= nums[i];
        right-pass (peeche):  prod=1;  right[i] = prod;  prod *= nums[i];
        combine:              ans[i] = left[i] * right[i].
     ORDER TRAP: pehle ASSIGN (left[i]=prod), PHIR UPDATE (prod*=nums[i]) -> warna khud ka element bhi product me ghus jaata.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ HIGHEST ALTITUDE
 └──────────────────────────────────────────────────────────────
     running sum chalao, max track. (simple prefix)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SUBARRAY SUMS DIVISIBLE BY K (LC-974)  = SUBARRAY-SUM-K + REMAINDER-key
 └──────────────────────────────────────────────────────────────
     count subarrays: sum%k==0.
     DERIVE: brute har subarray ka sum baar-baar -> redundancy -> running PREFIX sum -> do prefix ka remainder same = beech ka subarray %k==0.
     ★ PREFIX KYUN, single A[i] NAHI: subarray = CONTIGUOUS range -> poore range ka sum chahiye -> do prefix ka farak = wahi subarray.
     rem = ((sum%k)+k)%k;   count += mp[rem];   mp[rem]++;
        mp[0]=1 SEED (khaali-prefix) · = subarray-sum-k, bas KEY = remainder (sum ki jagah).

 common trick: "assign PEHLE, update BAAD me" (pivot + product dono me).
```

---

## PATTERN 5 — MATRIX

```

 BROAD IDEA: 2D grid pe index-navigation (boundary/direction careful; copy-pen pe grid banao).

┌── (STANDALONE — har ek apni index-navigation) ────────────────
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SET MATRIX ZEROES
 └──────────────────────────────────────────────────────────────
     TRICK: 2 marker-array -> zeroRow[row], zeroCol[col]. pass-1: cell==0 pe row+col mark; pass-2: marked ho to 0. (in-place, O(m+n) space)
       pass-1:  for i,j:  if(matrix[i][j]==0){ zeroRow[i]=1; zeroCol[j]=1; }
       pass-2:  for i,j:  if(zeroRow[i]==1 || zeroCol[j]==1)  matrix[i][j]=0;

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SPIRAL MATRIX (LC-54)  = 4 boundary andar simat-te (clockwise)
 └──────────────────────────────────────────────────────────────
     SAAR : top/bottom/left/right 4 boundary rakho -> clockwise push -> boundaries andar simat-te.
     TEMPLATE:
         int top=0, bottom=row-1, left=0, right=col-1;
         while(top<=bottom && left<=right){
             for(int i=left; i<=right; i++)  ans.push_back(matrix[top][i]);     top++;   // top row L->R
             for(int i=top; i<=bottom; i++)  ans.push_back(matrix[i][right]);   right--; // right col T->B
             if(top<=bottom){                                                   // GUARD
                 for(int i=right; i>=left; i--) ans.push_back(matrix[bottom][i]); bottom--; // bottom row R->L
             }
             if(left<=right){                                                   // GUARD
                 for(int i=bottom; i>=top; i--) ans.push_back(matrix[i][left]);   left++;   // left col B->T
             }
         }
     GUARD kyun: last 2 loops (bottom-row / left-col) se PEHLE check -- warna single row/col bache to DUPLICATE push.
     index yaad: top-row [top][i] · right-col [i][right] · bottom-row [bottom][i] · left-col [i][left].

 ┌──────────────────────────────────────────────────────────────
 │ ▸ TRANSPOSE
 └──────────────────────────────────────────────────────────────
     [ans[i][j] = matrix[j][i];] naya matrix col×row size ka.
```

---

## PATTERN 6 — STACK

```

 BROAD IDEA: stack = LIFO. "kholo push, band karo pop-match" · ya MONOTONIC (next-greater/smaller).

┌── FAMILY: MONOTONIC stack ────────────────────────────────────
│ KYUN SAATH: index stack rakho; curr vs stack-top compare -> pop + us index ka ans set. (bas alag: comparison > ya < · ans me value/distance · circular.)
└───────────────────────────────────────────────────────────────

 ONE SKELETON (next/prev + greater/smaller + daily-temp + histogram DONO isi pe):
    ans[] init (na-mile default: -1 ya n) ; stack<int> st  (INDEX rakho, value nahi)
    for i in arr:                         // prev-wale ke liye loop ULTA (n-1..0)
        while (!st.empty() && cmp(nums[st.top()], nums[i])):   // top ka jawab mil gaya
            ans[st.top()] = <nums[i] / (i - st.top()) / width>;  st.pop()
        st.push(i)
    -> sirf 3 cheez badalti: (1) cmp = `>` (smaller dhoondh rahe) ya `<` (greater) [ULTA lagta -- top ko jab curr HARA de]
                             (2) ans me kya: VALUE (next-greater/smaller) · DISTANCE i-top (daily-temp) · WIDTH (histogram)
                             (3) direction: next = aage(0..n) · prev = peeche(n-1..0) · circular = 2n loop, i%n.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ NEXT GREATER ELEMENT
 └──────────────────────────────────────────────────────────────
     MONOTONIC stack (index rakho); jab curr > stack-top -> pop + ans[top]=curr.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ NEXT SMALLER ELEMENT
 └──────────────────────────────────────────────────────────────
     Next-Greater ka SAME code, bas condition ULTA: while nums[st.top()] > nums[i] -> pop + ans[top]=nums[i]. (greater me < tha, smaller me >.)
     strictly smaller (> , >= nahi) -> [2,2,2] -> saare -1. baaki (index-stack, push, init -1) sab wahi.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ PREVIOUS SMALLER ELEMENT
 └──────────────────────────────────────────────────────────────
     Next-Smaller ka MIRROR: SAME code, bas for-loop ULTA (right->left, i=n-1..0). (array peeche se dekho -> "right ka next-smaller" = "left ka previous-smaller".)
     while nums[st.top()] > nums[i] -> pop + ans[top]=nums[i] (curr i popped ke LEFT me -> uska prev-smaller). push(i). left me kuch nahi -> -1.
     prev-smaller + next-smaller = Largest-Rectangle-in-Histogram ke building block.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LARGEST RECTANGLE IN HISTOGRAM (LC-84)
 └──────────────────────────────────────────────────────────────
     MENTAL MODEL: HAR height pe KHADE ho -> dono taraf NEAREST-SMALLER (prev+next) -> width=next-prev-1 -> area=height*width -> MAX. bas.
     prev-smaller + next-smaller ka COMBO. har bar apni OWN height pe rectangle banata -> left prev-smaller tak, right next-smaller tak (dono usse chhote = boundary). sabka area -> MAX.
     width = nS[i] - pS[i] - 1 · area = heights[i]*width · ans = max(ans, area).

     TRICK 1: prev/next-smaller me VALUE nahi, INDEX store karo (width ke liye).
     TRICK 2: next-smaller "koi chhota nahi" -> sentinel = n (right-edge, bar END tak failta), NA -1. (-1 se width negative -> [2,4]: 2-(-1)-1=-1 galat; n=2 se 2-(-1)-1=2 -> area 4 sahi.) prev-smaller "none" = -1 sahi (left boundary).

     "-1" rule: boundary bar rectangle me NAHI (smaller) -> -1 lagta. include hoti -> nahi.
     width=NS-PS-1 KYUN (derive): boundary rectangle me nahi -> bars PS+1 se NS-1 tak. count=last-first+1=(NS-1)-(PS+1)+1=NS-PS-1. (-1 SIRF EK baar, do nahi -- building6: 4-2-1=1 sahi, 4-2-1-1=0 galat.)

     VISUAL (aankhon se, [2,1,5,6,2,3]):  idx: 0 1 2 3 4 5 · h: 2 1 5 6 2 3 · PS: -1 -1 1 2 1 4 · NS: 1 6 4 4 6 6 · width(NS-PS-1): 1 6 2 1 4 1 · area: 2 6 [10] 6 8 3 -> MAX 10.
       bar5(idx2): PS=1(h1),NS=4(h2) -> rectangle DONO stoppers ke BEECH (c2,c3, dono>=5) = 2 chauda. first=PS+1=2, last=NS-1=3 -> 3-2+1=2. stoppers khud andar nahi.

     next-GREATER se NAHI: rectangle taller bar ko include karta, shorter pe rukta -> smaller boundary chahiye. (greater tab jab bada element boundary ho.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAXIMAL RECTANGLE (LC-85, 2D)
 └──────────────────────────────────────────────────────────────
     = Largest-Rectangle-Histogram (LC-84) ka 2D bhai. HAR ROW ko histogram maano -> us pe LC-84 laga -> saare rows ka MAX.
     heights[] (size = COL, har column ki running height -- upar se neeche):
        cell == 1 -> heights[j] += 1;    // upar wale 1s ke saath build-up
        cell == 0 -> heights[j] = 0;     // column toot gaya -> RESET
     har row ke baad:  maxi = max(maxi, largestRectangleArea(heights));   // LC-84 code REUSE (prev/next-smaller + area)
     inner loop j (col), outer i (row).  heights COL-size ka (row nahi).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ DAILY TEMPERATURES
 └──────────────────────────────────────────────────────────────
     same monotonic; ans[top] = i - top (VALUE nahi, DISTANCE/din).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ NEXT GREATER ELEM II
 └──────────────────────────────────────────────────────────────
     same monotonic, bas array CIRCULAR. trick: loop 2 BAAR (i: 0..2n-1), access i % n (mod).
     doosre chakkar me aakhri elements ko SHURU ka greater mil jaata (wrap). baaki wahi; na mile -> -1.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ VALID PARENTHESES
 └──────────────────────────────────────────────────────────────
     opening push; closing aaye to top se match+pop; end me stack KHALI = valid. (closing pe stack empty -> false)
     match: (topp=='('&&it==')') || (topp=='['&&it==']') || (topp=='{'&&it=='}')

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MIN STACK
 └──────────────────────────────────────────────────────────────
     DESIGN: push/pop/top/getMin SAB O(1). trick = min hamesha "ready" rakho, bina scan kiye.
     har element ke SAATH uska "yahaan tak ka MIN" store karo -> stack<pair<val, minSoFar>>.
        push:   newMin = empty ? val : min(val, top().second);  push {val, newMin}.
        getMin: top().second (O(1), koi loop nahi).   top: top().first.   pop: normal.
     KYUN pair (ek single min-var kyun NAHI): pop pe min WAPAS badal sakta -> purana min laut aata;
        single var us history ko recover nahi kar paata. har element apna min saath -> pop pe neeche wala min apne-aap sahi.
        (e.g. push 5,3,7 -> min 3 · pop 7 -> min still 3 · pop 3 -> min 5 WAPAS aa gaya.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MIN ADD MAKE VALID
 └──────────────────────────────────────────────────────────────
     COUNTER (stack nahi -- sirf '(' ')'). 2 counter (naam tere code se):
        close_needed = kitne '(' abhi UNMATCHED khade (inhe aage ')' chahiye).
        open_needed  = kitne ')' UNMATCHED aaye (inhe pehle '(' chahiye tha).
     '(' aaye  -> close_needed++                                          (ek open, iska close pending).
     ')' aaye  -> close_needed > 0 ? close_needed-- (pending open se MATCH) : open_needed++ (koi open nahi -> ye ')' extra).
     ans = open_needed + close_needed                                     (dono bache = utne insert karne).
     TRAP: ')' match ho gaya to open_needed me MAT gino -- match = 0 insertion (nested "(())" -> 0).

 broad-trick: MONOTONIC stack (next-greater + daily-temp = same, bas ans me kya bharo alag).
```

---

## PATTERN 7 — BINARY SEARCH

```

 BROAD IDEA: sorted space -> mid -> aadha kaato -> O(log n). (space array ho ya ANSWER-range).

 BS ko POORI SORTED array chahiye HI NAHI -- bas ek comparison se "LEFT ya RIGHT (konsa half phenkna)" DECIDE kar paana chahiye.
    sorted-array us decision ka ek common case hai; find-peak (unsorted) me bhi mid vs mid+1 se half discard hota -> BS lagta.
    => asli requirement = "half safely discard karne ka rule", na ki full sorting.

 BOUNDARY RULE (har BS me lagta): jis comparison-boundary pe element KHUD valid answer ho sakta -> equality INCLUDE karo (>= ya <=).
    jahaan boundary already-handle ho chuki (jaise mid==target upar return ho gaya) -> < ya <= farak NAHI.
    galat >/< = off-by-one edge miss -> chhota DRY-RUN se pakdo. (eg SEARCH-ROTATED: nums[low] pe >= zaruri warna target==nums[low] fail.)

┌── FAMILY: BASIC / BOUNDARY ───────────────────────────────────
│ KYUN SAATH: normal sorted-array BS; target ko mid se compare (low<=high, mid±1). boundary = mila to ruko-mat, ek taraf aur dhoondo.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ BINARY SEARCH (BASIC)
 └──────────────────────────────────────────────────────────────
     mid; target<mid -> left half, warna right. (low<=high, mid±1)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SEARCH INSERT POSITION
 └──────────────────────────────────────────────────────────────
     basic BS; na mile to low return (wahi insert jagah).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ FIRST & LAST POSITION (LC-34)
 └──────────────────────────────────────────────────────────────
     sorted + duplicates. target ka {first, last} index. 2 BS, ans(2,-1). mila nahi -> {-1,-1}.
     CORE: mid==target pe RUKO MAT -> boundary tak ek side aur dhoondo.
     1st pass (FIRST): nums[mid]==target -> ans[0]=mid; high=mid-1   (aur LEFT dhoondo)
     2nd pass (LAST):  nums[mid]==target -> ans[1]=mid; low=mid+1    (aur RIGHT dhoondo)
        baaki dono pass me normal: nums[mid] < target -> low=mid+1 · else -> high=mid-1.

┌── FAMILY: ROTATED ────────────────────────────────────────────
│ KYUN SAATH: array rotate hua; ek comparison se pata karo kaunsa half sorted / pivot kis taraf, phir wahi half rakho.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SEARCH IN ROTATED (LC-33)  = rotated sorted array me target, O(log n)
 └──────────────────────────────────────────────────────────────
     SAAR : sorted array pivot pe rotate ([4,5,6,7,0,1,2]). target ka INDEX, nahi mile -> -1.
     CORE : mid==target? return. warna -> ek half HAMESHA sorted -> wahi se decide:
         nums[low] <= nums[mid]  ->  LEFT half sorted:
              nums[low] <= target <= nums[mid]  ? -> high = mid-1   :  low = mid+1
         else                    ->  RIGHT half sorted:
              nums[mid] <= target <= nums[high] ? -> low = mid+1    :  high = mid-1
         har baar aadha kaat -> O(log n). loop khatam -> -1.
     BOUNDARY RULE (dry-run se pakda): jahaan boundary KHUD valid answer ho sakti (nums[low]/nums[high]) -> >= / <= ZARURI (equality include).
              jahaan mid hai (already returned) -> < ya <= farak NAHI.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ FIND MIN IN ROTATED
 └──────────────────────────────────────────────────────────────
     mid ko HIGH se compare (koi target nahi, khud min dhoondna). while(low<high): if(nums[mid]>nums[high]) low=mid+1; else high=MID; -> return nums[low].
     high=MID (NOT mid-1): else me mid KHUD min ho sakta (CANDIDATE) -> discard mat karo. isliye while(low<high) (warna infinite-loop). [BUG-CATCH: [3,1,2] high=mid-1 se galat 3 deta tha.]
     (ye wahi high=mid vs high=mid-1 rule -- find-peak jaisa: khud-answer-dhoondh -> high=mid+low<high; target-reject -> high=mid-1+low<=high.)

┌── FAMILY: BS-on-ANSWER (koko · ship · split -- SHELL same, sirf RANGE + solve(mid) alag) ─────
│ array pe nahi, ANSWER-range pe BS. solve(mid) feasible? -> feasible=chhota try (high=mid-1), warna bada (low=mid+1).
└───────────────────────────────────────────────────────────────

 SHARED SHELL (teeno bilkul same):
      low, high = <RANGE>;   ans = -1;
      while (low <= high) {
          mid = low + (high - low) / 2;
          if (solve(mid)) { ans = mid; high = mid - 1; }   // feasible -> chhota try
          else              low = mid + 1;                  // bada chahiye
      }
      return ans;
   FARAK sirf 2 cheez: (a) RANGE (low, high)   (b) solve(mid) ka FORMULA.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ KOKO BANANAS (LC-875) -- min eating-speed k
 └──────────────────────────────────────────────────────────────
     RANGE : low=1, high=max(piles).
     solve : hours = Σ ceil(piles[i] / mid);   return hours <= h.
     ceil TRAP: ceil(int/int) BEKAAR (int-div pehle FLOOR kar deta). use (piles[i]+mid-1)/mid YA ceil((double)piles[i]/mid). hours = long (overflow).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SHIP WITHIN D DAYS (LC-1011) -- min capacity
 └──────────────────────────────────────────────────────────────
     RANGE : low=max(weights), high=sum(weights).
     solve : day=1, sum=0;   for weight: if (sum+weight > mid) { day++; sum=0; }   sum += weight;   return day <= D.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SPLIT ARRAY LARGEST SUM (LC-410) -- min largest part-sum
 └──────────────────────────────────────────────────────────────
     "k parts me baanto -> LARGEST part-sum ko MINIMIZE."
     RANGE : low=max(arr) (bada element akela aayega, tod nahi sakte) · high=sum(arr) (k=1 -> pura ek tukda).
     solve : sum=0, count=1;   for x: sum += x; if (sum > mid) { count++; sum = x; }   return count <= k.
     count = 1 se START (warna aakhri tukda ginti me chhoot jaaye).

 sibling: koko (min speed) · ship (min capacity) · split (min largest-sum) -> teeno IDENTICAL shell.

┌── FAMILY: HALF-DISCARD (unsorted / half-property) ────────────
│ KYUN SAATH: array pura sorted nahi, phir bhi ek comparison se aadha safe discard ho jaata (peak: mid vs mid+1 · single: index-parity).
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ FIND PEAK ELEMENT (LC-162)
 └──────────────────────────────────────────────────────────────
     array SORTED nahi, phir bhi BS -- ek comparison (mid vs mid+1) se aadha discard. (peak = neighbours se bada; edges bahar -inf.)
     low=0, high=n-1;   while (low < high):   mid = low+(high-low)/2;
        nums[mid] < nums[mid+1]        -> CHADHAAI -> peak RIGHT me       -> low = mid+1;
        else (nums[mid] > nums[mid+1]) -> DHALAAN  -> peak mid-KHUD ya LEFT -> high = mid;   // mid (NOT mid-1)
     return low;   (low==high pe converge = peak. edges auto.)
     GOTCHA: dhalaan me high = mid (mid-1 NAHI) -- mid khud peak ho sakta. isliye while low<high (mid+1 hamesha in-bounds, no sentinel).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SINGLE ELEMENT (SORTED, LC-540)  = INDEX-PARITY binary search
 └──────────────────────────────────────────────────────────────
     SAAR : har element 2x, sirf EK akela. O(log n) BS via INDEX-PARITY. (XOR = O(n); yahan log-n chahiye.)
        idea: single se PEHLE (left-half) pairs (even,odd) pe -> single ke BAAD (odd,even) SHIFT. mid ko mid-1 (LEFT) se compare.
     TEMPLATE:
         if(nums.size()==1) return nums[0];            // BASE
         int low=0, high=n-1;
         while(low<=high){
             int mid=low+(high-low)/2;
             if(mid==0) return nums[mid];              // edge
             if(mid%2==0){                             // EVEN mid
                 if(nums[mid]==nums[mid-1]) high=mid-1; //   single PEECHE
                 else low=mid+1;                        //   single AAGE
             } else {                                  // ODD mid
                 if(nums[mid]!=nums[mid-1]) high=mid-1; //   single PEECHE
                 else low=mid+1;                        //   single AAGE
             }
         }
         return nums[high];
     ── DRY-RUN -- ISKO paper pe dekh, prose bhool (single = 2 @ idx 2) ──
        idx:  0  1  2  3  4  5  6  7  8
        arr:  1  1  2  3  3  4  4  8  8
        low0 high8 -> mid=4 (EVEN) : nums[4]=3 == nums[3]=3 -> high=4-1=3
        low0 high3 -> mid=1 (ODD)  : nums[1]=1 == nums[0]=1 -> low=1+1=2
        low2 high3 -> mid=2 (EVEN) : nums[2]=2 != nums[1]=1 -> low=2+1=3
        low3 high3 -> mid=3 (ODD)  : nums[3]=3 != nums[2]=2 -> high=3-1=2
        low3 > high2 -> STOP -> return nums[high]=nums[2]= 2  (sahi)
     return nums[HIGH] (not low): dono branch mid REJECT karte -> high khud single pe aa ke rukta.

┌── FAMILY: 2D-index-map ───────────────────────────────────────
│ KYUN SAATH: 2D matrix ko 1D sorted array maan ke normal BS; index ko (row,col) me convert.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SEARCH 2D MATRIX
 └──────────────────────────────────────────────────────────────
     poore matrix ko 1D maano (low=0, high=row*col-1). access: matrix[mid/col][mid%col]. (row=mid/col, col=mid%col)

 3 broad-trick: BS-on-ANSWER (Koko) · sorted-half-check (Rotated) · 2D-index-map (matrix).

 high=mid vs high=mid-1 (kab konsa -- ek sawaal: "mid KHUD answer ho sakta abhi bhi?"):
      TARGET-REJECT (mid ko target/condition se compare -> mid pakka galat, e.g. basic-BS/rotated-search: target!=mid) -> high = mid-1  (+ while low<=high)
      KHUD-ANSWER-DHOONDH (koi target nahi, mid KHUD answer ho sakta -- find-PEAK · find-MIN-rotated) -> high = MID  (+ while low<high)
      BUG-CATCH (find-min, 18-Jul): [3,1,2] pe high=mid-1 karte to mid(=1, khud min) HAT jaata -> galat 3 aata. high=mid se bacha.
      trap: high=mid ke saath while(low<=high) = INFINITE LOOP -> high=mid hamesha low<high ke saath.
      KYUN (nuance): mid = low+(high-low)/2 NEECHE (low ki taraf) round karta. jab low==high==mid ho:
          high=mid-1 -> range SHRINK (mid-1) -> low>high -> exit. safe with low<=high.
          high=mid   -> high nahi hilta (mid=low) -> range shrink NAHI -> low<=high hua to ATAK (infinite).
          isliye high=mid ko low<high chahiye (low==high pe TURANT exit, atakne se pehle).
          e.g. [2,1] peak-logic + while(low<=high): mid=0 -> high=mid=0 -> low=high=0 kabhi khatam nahi (loop).
```

---

## PATTERN 8 — LINKED LIST

```

 BROAD IDEA: pointer manipulation. 3 broad-trick: FAST/SLOW · REVERSE (3-pointer) · DUMMY node.

 LOOP-CONDITION TRICK (while ka kya likhun -- CP-log ise DEKHTE hi jaante, dry-run nahi karte):
    EK sawaal: "aakhri node ka bhi kaam hai, ya sirf uske NEXT ko peek karna hai?"
      - har node ka apna kaam (key check / process / remove / end pe append) -> `while (curr != NULL)`   [ye ZYADATAR: get, remove, count, print]
      - curr ke NEXT ko dekhna/use, ya aakhri node PE rukna (tail pakadke aage jodna) -> `while (curr->next != NULL)`   [khaas case, kam]
    (proof: `A->B->NULL` pe dono trace -> `curr!=NULL` B tak jaake B ka kaam karta; `curr->next!=NULL` B se pehle ruk jaata, B ka kaam nahi. => op ka naam nahi, ye sawaal decide karta.)

┌── FAMILY: FAST/SLOW ──────────────────────────────────────────
│ KYUN SAATH: slow 1 step, fast 2 step (ya gap). middle/cycle/palindrome/nth-from-end sab isi 2-speed pe.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MIDDLE OF LIST
 └──────────────────────────────────────────────────────────────
     FAST/SLOW: slow 1 step, fast 2 step; fast end pe -> slow MIDDLE pe.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ DETECT CYCLE
 └──────────────────────────────────────────────────────────────
     FAST/SLOW (Floyd): cycle ho to tez-dheema MILENGE (slow==fast) -> true.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ PALINDROME LL (LC-234)
 └──────────────────────────────────────────────────────────────
     COMBO (3 tool jodo): middle -> 2nd half REVERSE -> head(front) & rev(back) saath chala ke compare.
        Node* mid = middleNode(head);     // slow/fast
        Node* rev = reverseList(mid);     // 2nd half ulta (rev = naya head)
        while (head && rev) { if (head->val != rev->val) return false; head=head->next; rev=rev->next; }
        return true;   (khali list -> true)
     list TOOT-ti NAHI: pehle-half node ka ->next kabhi nahi badla -> odd me middle node SHARED (head-se aur rev-se dono beech pe aa ke NULL milte -> while(head&&rev) bina cut ke odd+even dono handle).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ REMOVE NTH FROM END
 └──────────────────────────────────────────────────────────────
     FAST/SLOW GAP: fast ko PEHLE aage bhejo (loop i=1; i<=n). ab fast & slow me n ka gap.
     agar fast==NULL -> head hi hatana hai -> return head->next.
     warna dono SAATH chalao (gap constant) jab tak fast->next NULL -> slow "hatane wale ke PEHLE" baith jaata.
     slow->next us node ko point kar raha -> slow->next = slow->next->next. done.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ REMOVE ELEMENTS (delete by val, LC-203)
 └──────────────────────────────────────────────────────────────
     val wale SAARE node hatao.  if (!head) return null;   prev = null, curr = head;
     while (curr != null && head != null):
        curr->val==val && prev==null   -> head = head->next          (HEAD / ya sab-node-same)
        curr->val==val && prev!=null   -> prev->next = curr->next     (beech)
        else (no-match)                -> prev = curr
        curr = curr->next              (har case ke end me)
     return head;
     TRICK: prev SIRF no-match pe aage (removal pe prev=curr NAHI -- curr hata) -> warna consecutive [1,6,6,2] fail.

┌── FAMILY: DUMMY node ─────────────────────────────────────────
│ KYUN SAATH: fake head (dummy) + tail; naye node rewire/jodte jao; ans = dummy->next. head-edge apne aap handle.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MERGE TWO SORTED (LC-21)
 └──────────────────────────────────────────────────────────────
     DUMMY node + tail (dummy = head ka special-case avoid).
        while (a && b): (a->val <= b->val) ? { tail->next=a; a=a->next; } : { tail->next=b; b=b->next; }   tail=tail->next;
        while (a) { tail->next=a; a=a->next; tail=tail->next; }    // baaki a jod do (already sorted)
        while (b) { tail->next=b; b=b->next; tail=tail->next; }    // baaki b jod do
        return dummy->next;   (dummy skip -> asli head)
     ek list khatam -> doosri ki BAAKI seedha jod do (dono already sorted).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ ADD TWO NUMBERS
 └──────────────────────────────────────────────────────────────
     dummy + tail (MERGE jaisa) + CARRY. digits ULTA store (units-digit pehle) -> seedha aage-aage jodo.
     while(a || b || carry):              <- koi list bachi HAI ya carry bacha -> chalte raho
        sum = carry;                        (pichle ka carry se shuru)
        a hai to sum += a->val (a aage);  b hai to sum += b->val (b aage).
        naya node = sum % 10 -> tail se jodo, tail aage.   carry = sum / 10 (agle ke liye).
     return dummy->next.
     '|| carry' zaroori: 999 + 1 = 1000 -> dono list khatam par carry bacha -> ek aur node chahiye (leading '1').

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SWAP NODES IN PAIRS  = DUMMY + 3-player rewire (order + temp-save)
 └──────────────────────────────────────────────────────────────
     SAAR : har do adjacent NODE ki jodi swap (values nahi, NODES = pointer rewire). DUMMY-node trick.
     TEMPLATE:
         if(!head) return nullptr;
         Node* dummy=new Node(0); dummy->next=head;
         Node* prev=dummy, *first=head;
         while(prev->next && first->next){        // jodi (2 node) bache tab tak
             Node* second=first->next;
             Node* temp=second->next;             // BACHA lo (warna next line me gum)
             second->next=first;                  // 2 -> 1
             first->next=temp;                    // 1 -> baaki list
             prev->next=second;                   // prev -> 2 (= is jodi ka naya head)
             prev=first;  first=temp;             // ADVANCE (prev=purana first, first=agli jodi ka pehla)
         }
         return dummy->next;
     dummy kyu: jodi reconnect karne ko "jodi se pehle wala" node chahiye; pehli jodi ke pehle kuch nahi -> dummy.
     rewire ORDER + temp-save = crux. har iteration = SIRF EK jodi (aage ki jodi ka kaam mat karo).

┌── FAMILY: REVERSE (3-pointer) ────────────────────────────────
│ KYUN SAATH: prev/curr/next se link ulti karo. (reverse khud + palindrome ka 2nd-half.)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ REVERSE LINKED LIST
 └──────────────────────────────────────────────────────────────
     3-pointer: nextt=curr->next; curr->next=prev; prev=curr; curr=nextt. return prev.

 3 high-leverage: fast/slow (middle+cycle+palindrome+remove-nth) · reverse (reverse+palindrome) · dummy-node (merge).
```

---

## PATTERN 9 — DESIGN (data-structure)

```

 BROAD IDEA: data-structure ko khud banao (built-in ke bina) -> array + pointers/nodes se.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ DESIGN HASHMAP (LLD)
 └──────────────────────────────────────────────────────────────
     MENTAL MODEL: ek ARRAY (buckets); har slot me ek LINKED-LIST (chain). buckets[index] = us chain ka HEAD.
        index = hash<string>{}(key) % cap.  (exact C++ syntax: hash<string>{}(key) -- string hash.)  same index pe 2 key (collision) -> chain me jode (next se).
     fields: buckets (Entry* ka array) · cap (slots = 16) · sz (kitne pairs).

     TRICK: index = hash<string>{}(key)%cap -> us bucket ki chain me traverse -> key match pe kaam; put me end tak na-mili -> naya node append. (collision = same index -> chain)
       put(key,val):  int index = hash<string>{}(key) % cap;   Entry* node = new Entry(key,val);
                      if(buckets[index] == nullptr){ buckets[index] = node; sz++; }
                      else{
                         Entry* head = buckets[index];
                         while(head != NULL){
                            if(head->key == key){ head->value = val; return; }
                            else if(head->next == nullptr){ head->next = node; sz++; }
                            head = head->next;
                         }
                      }
       get(key):      Entry* head = buckets[index];
                      while(head != NULL){ if(head->key == key) return head->value;  head = head->next; }
                      return -1;
       remove(key):   Entry* head = buckets[index], *prev = NULL;
                      while(head != NULL){
                         if(head->key == key){ if(prev) prev->next = head->next;  else buckets[index] = head->next;  sz--; }
                         prev = head;  head = head->next;
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

 ┌──────────────────────────────────────────────────────────────
 │ ▸ DESIGN HASHSET (LLD)
 └──────────────────────────────────────────────────────────────
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

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LRU CACHE (LC146) — COMBO: hashmap + doubly-linked-list
 └──────────────────────────────────────────────────────────────
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
     ctor:   dummy head,tail bana -> head<->tail link · cap=capacity.   (TRAP-3)

     CRUX = 2 DLL helper (yehi asli mechanic, get/put inko call karte):
        removeNode(n): n->prev->next=n->next;  n->next->prev=n->prev;      // beech se nikal, O(1)
        addFront(n):   nxt=head->next;  head->next=n; n->prev=head;        // nxt PEHLE pakdo (TRAP-2)
                       n->next=nxt; nxt->prev=n;                           // n = MRU

     get(key):  mp.count(key)==0 -> -1.  warna: Node* node = mp[key]; removeNode(node); addFront(node); return node->val;
     put(k,v):  hai  -> node->val=v  -> removeNode + addFront.
                naya -> FULL? tail->prev(=LRU) removeNode + mp.erase(uski key); phir new node addFront + mp[k]=node.

     TRAP: (1) mp[key] missing pe CHUP-CHAAP entry INSERT -> check ke liye mp.count()/find(). (C++ jaal)
           (2) DLL rewire: overwrite se PEHLE temp me pakdo (nxt).  (3) ctor me type-dobara = shadowing (naya local).
```

---

## PATTERN 10 — KADANE (running value + max update)

```

 BROAD IDEA: ek value AAGE carry karo (min/sum), har step MAX update. single pass, O(1) space.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ BUY/SELL STOCK (LC-121)
 └──────────────────────────────────────────────────────────────
     TRICK: aaj becho = aaj ka daam MINUS ab-tak-ka-sasta. bas 2 cheez track: mini + maxProfit.
       mini = min(mini, prices[i]);   maxProfit = max(maxProfit, prices[i] - mini);
     (sell buy-ke-BAAD auto -- mini hamesha current se pehle ka.)
     BRUTE->OPT (aise pahunche): brute = saare pair (i<j): prices[j]-prices[i], max. O(n^2).
       REDUNDANCY = har sell-din pe peeche ka "sabse sasta buy" baar-baar scan.
       us min ko ek VARIABLE me CACHE -> inner loop gayab -> O(n).  (redundancy->cache = running-min)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAX SUBARRAY SUM (LC-53)
 └──────────────────────────────────────────────────────────────
     TRICK: pichhla running sum NEGATIVE = aage bojh -> PHENK do (sum=0), fresh. bas yahi.
       sum += nums[i];   max_sum = max(max_sum, sum);   if(sum < 0) sum = 0;
     TRAP: max_sum update RESET se PEHLE + max_sum=INT_MIN start (warna all-negative [-1] miss).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAX PRODUCT SUBARRAY (LC-152)
 └──────────────────────────────────────────────────────────────
     TRICK: neg*neg = BADA -> sirf max se nahi banega, MIN bhi track karo (chhota kal sabse bada ban sakta).
       temp    = max({ num, num*max, num*min });     // 3 candidate ka max
       min     = min({ num, num*max, num*min });     // FREEZE: min purane max/min se
       max     = temp;  ans = max(ans, max);         // max min-ke-BAAD, ans me purana bhi

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAX ABSOLUTE SUM (LC-1749)
 └──────────────────────────────────────────────────────────────
     = MAX PRODUCT SUBARRAY (152) ka HUBAHU same skeleton (temp/min/max freeze, upar dekh), sirf 2 TWEAK:
        (1) GUNA (*) -> PLUS (+)                    (num*max -> num+max, num*min -> num+min)
        (2) ans me ABS dono -> ans = max({ ans, |max|, |min| })   (answer minSum se bhi aa sakta -> isliye abs)
     dry-run [2,-5,1,-4,3,-2] -> 8: subarray [-5,1,-4]=-8 minSum me -> |-8|=8.

   FAMILY: 53=sum(1 value) · 152=product(flip -> max+min) · 1749=abs-sum(max+min).
   MECHANIC yaad rakh: "flip ho to MIN bhi track + 3 candidate + old FREEZE (temp)".


 ┌──────────────────────────────────────────────────────────────
 │ ▸ 2 MAX-SUM NON-OVERLAPPING SUBARRAYS (Google)   = do array (bestLeft + bestRight) + WALL
 └──────────────────────────────────────────────────────────────
     ESSENCE: har wall pe -> [0..i] ka best + [i+1..n-1] ka best -> in sab ka MAX. bas yehi.
       val       : [  1   2  -7   8   6  -4 ]
       bestLeft  : [  1   3   3  (8) 14  14 ]   (0..i)          <- aage
       bestRight : [ 14  14  14 (14) 6  -4 ]   (i+1..n-1)      <- peeche
       overlap dekh: index 3 pe bestLeft ka 8 AUR bestRight ka 14 -- dono me index 3 COMMON = do baar count (jhootha).
       isliye: left = [0..i], right = [i+1..n-1] -> ek AAGE shift -> index 3 dono me nahi. (ya left [0..i-1], right [i..n-1])

     CONNECT: ye KADANE + PREFIX/SUFFIX ka COMBO hai (do pattern jud rahe).
        05 (kadane per-index) -> maxEndingAt array -> uska PREFIX-max (aage) = bestLeft,
        SUFFIX-max (peeche) = bestRight -> phir WALL se jodo.  (var = code jaisa: ans/temp)
        KYUN kadane+prefix: har index tak ka running SUM = prefix-sum; uska MAX (neg aaye to reset) = kadane.
        eg [2,1,-99,3,3]=9 -> wall index-1 pe: LEFT best=3 ([2,1]) + RIGHT best=6 ([3,3]) = 9.

       maxEndingAtForLeft   (base ans[0]=nums[0];        loop i=1..n-1  AAGE)
           ans[i] = max( nums[i] , ans[i-1] + nums[i] )                   // Kadane per-index (=05, prefix-sum+reset)
              |__ ye jo array RETURN hua  -->  neeche bestLeftPrefix me 'temp' ban jaata

       bestLeftPrefix       (temp = UPAR wale ka RETURN;  base ans[0]=temp[0];   loop i=1..n-1  AAGE)
           ans[i] = max( ans[i-1] , temp[i] )                            // PREFIX-max (=06)

       maxEndingAtForRight  (base ans[n-1]=nums[n-1];    loop i=n-2..0  PEECHE)
           ans[i] = max( nums[i] , ans[i+1] + nums[i] )                  // 05 ka MIRROR
              |__ ye jo array RETURN hua  -->  neeche bestRightPrefix me 'temp' ban jaata

       bestRightPrefix      (temp = UPAR wale ka RETURN;  base ans[n-1]=temp[n-1];  loop i=n-2..0  PEECHE)
           ans[i] = max( ans[i+1] , temp[i] )                           // SUFFIX-max (=06 ulta)

       maxTwoNonOverlap     (bestLeft=bestLeftPrefix, bestRight=bestRightPrefix;  ans=INT_MIN)
           for i = 0..n-2 :  ans = max( ans , bestLeft[i] + bestRight[i+1] )    // WALL, i+1 = non-overlap

     VISUAL [1,2,-7,8,6,-4]:
       val       : [ 1    2   -7    8    6   -4 ]
       maxEndAt  : [ 1    3   -4    8   14   10 ]   (05)
       bestLeft  : [ 1    3    3    8   14   14 ]   (06)
       bestRight : [ 14   14   14   14   6   -4 ]   (06 ka mirror)
       i=1: bestLeft[1]=3 + bestRight[2]=14 = 17

     i+1 KYUN (non-overlap ki JAAN): right ko i se lo (bestRight[i]) -> shared index DONO subarray me -> DO BAAR count -> OVERLAP.
       e.g. bestLeft[3]=8 + bestRight[3]=14 = 22, index 3 DONO me = jhootha. i+1 (ya i-1, loop i=1->n-1) se dono ALAG.

     loop i<n-1: aakhri split (right = akela last element) MISS mat karo. [-5,10,10] -> 20.

```

---

## PATTERN 11 — BIT MANIPULATION (XOR)

```

 BROAD IDEA: XOR ki 2 property -> a^a=0 (pair cancel), a^0=a (single bacha). pairs khud gayab, extra memory nahi.

┌── FAMILY: XOR-cancel ─────────────────────────────────────────
│ KYUN SAATH: pair-wise cheezein XOR se KHUD cancel (a^a=0); jo "akela / missing" bacha wahi answer. no map, O(1) space.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SINGLE NUMBER (LC-136)
 └──────────────────────────────────────────────────────────────
     TRICK: har num 2-baar, ek 1-baar -> saare XOR karo -> pairs cut -> jo bacha = answer.
       XORR = nums[0];  for i=1..n: XORR ^= nums[i];  return XORR.   (O(n)/O(1), koi map nahi)
     FAMILY (aage isi daabe me aayenge): Missing-Number · Single-Number-II bhi XOR-based.
```

---

## PATTERN 12 — TREES (recursion)

```

 BROAD IDEA: har tree-Q = RECURSION on TreeNode(val, left, right). 3 cheez -> BASE (null pe return) · dono taraf RECURSE · COMBINE.

 ORDER: kaam recursion-RESULT pe depend kare -> recurse-PEHLE (post-order); independent -> free.

 3 DFS TRAVERSALS (LC-144/94/145): teeno SAME recursion, sirf VISIT [res.push_back] ki JAGAH badalti ->
      PRE = VISIT->left->right (root pehle) · IN = left->VISIT->right (beech) · POST = left->right->VISIT (baad).

 ──────────────────────────────────────────────────────────────
 DECISION-GUIDE (Arpan-derived -- 4 simple sawaal, koi tree-Q likhne se pehle):
   Q1  "root->left" likhu ya "left"?
        root->left / root->right = ASLI bachche (structure)  -> inme RECURSE · LEAF-check (!root->left && !root->right) · SWAP (invert)
        left / right             = recursion ka RETURN (jawab) -> COMBINE me use
   Q1b RETURN TYPE = problem kya maangta -> variable ya DIRECT (asli code se):

       bool (hai kya/match?)  -- value 1 baar -> DIRECT call, koi variable NAHI:
           return hasPathSum(root->left, t-val) || hasPathSum(root->right, t-val);   // path-sum
           return isSame(p->left, q->left) && isSame(p->right, q->right);            // same-tree

       TreeNode* (wo node do) -- LCA (variable, dono side check karne):
           TreeNode* left  = lca(root->left, p, q);
           TreeNode* right = lca(root->right, p, q);
           if(!left) return right;  if(!right) return left;  return root;

       int (kitna/gehrai)     -- value DOBARA chahiye -> variable ZAROORI:
           int left  = height(root->left);
           int right = height(root->right);
           maxDia = max(maxDia, left + right);   // <- yahan left, right chahiye
           return 1 + max(left, right);          // <- aur yahan phir -> isliye variable
   Q2  COMBINE me kaunsa operator? (question se pata):
        "gehra/bada" -> max  ·  "dono side match" -> &&  ·  "koi ek path" -> ||  ·  "wo node dhundo" -> node return
        kuch Q me base ke ALAWA ek special CONDITION: path-sum -> LEAF pe check · LCA -> root==p||q. (jahan zaroorat, wahan lagao.)
   Q3  answer RETURN karu ya GLOBAL me rakhu?
        answer seedha ban raha -> RETURN.   answer alag (parent ko height chahiye) -> height RETURN + answer GLOBAL (diameter)
   Q4  value UPAR le jau ya NEECHE?
        answer subtree se banti (height/sum) -> UPAR return.   target/condition path pe -> value NEECHE carry (path-sum)

 ──────────────────────────────────────────────────────────────
 CORE -- GENERIC tree-recursion (har tree-Q bas yehi 4 line; sirf BASE + COMBINE badalte):
       solve(node):
         if (node == NULL) return BASE;      // (1) base  -> null pe ruko
         left  = solve(node->left);          // (2) left bachche ka jawab MAANGO
         right = solve(node->right);         // (3) right bachche ka jawab MAANGO
         return COMBINE(left, right);        // (4) apna jawab banao -> parent ko DO

   2 shabd jo poori recursion kholte:
        node->left  =  NEECHE bhejo   (child ko solve karne bhej diya -- CALL)
        left        =  jo VALUE wapas aayi   (us child ka jawab -> ab COMBINE me use)

 ──────────────────────────────────────────────────────────────
 FLOW -- calls NEECHE jaati, JAWAB upar BUBBLE hota:
        [A]
       /   \
     [B]   [C]
     /  \
   [D]  [E]

   solve(A)
     line2:  left  = solve(B) ──► neeche gaya
                  solve(B):  left=solve(D), right=solve(E) ─► DONO ka COMBINE ═► B ka jawab ─► A ki 'left'
     line3:  right = solve(C) ──► neeche gaya
                  solve(C):  ... ─► COMBINE ═► C ka jawab ─► A ki 'right'
     line4:  return COMBINE(left, right) ═════════════════► FINAL ANSWER

   -> har node SIRF apne 2 bachchon ka jawab leta -> apna banata -> upar deta. poora tree ek saath NAHI.
   -> null pe BASE -> phir jawab neeche se upar chadhta -> root pe final.

 ──────────────────────────────────────────────────────────────
   RECURSION DEBUG (trace POORA mat karo -- 3 tool, copy-pen deep-recursion ke liye bana hi nahi):

     1. TRUST ONE LEVEL: maano left/right ka jawab MIL gaya -> bas "is node pe kya karu?" socho. deep me mat jhaanko.

     2. CHHOTA-CASE verify: bug pakadne ko sirf BASE + 2-3 node wala SMALLEST tree haath se check (deep case nahi).
        base sahi + 1-level sahi = poori recursion sahi.

     3. PRINT > debugger -- crash-safe placement (KAHAN/KAB daalo):

          int solve(TreeNode* node) {
              if (!node) return BASE;                        // null-check PEHLE
              cout << "enter " << node->val << endl;         // enter-print: null-check ke BAAD (warna null->val = CRASH)
              int left  = solve(node->left);
              int right = solve(node->right);
              int result = COMBINE(left, right);             // result ko VARIABLE me lo
              cout << node->val << " : left=" << left << " right=" << right << " -> " << result << endl;  // return-print
              return result;
          }

        -> output LINEAR LOG (enter=neeche jaana, return=upar bubble) -> flow PADH lo, stepping/confusion nahi.
        enter-print HAMESHA null-check ke NEECHE (sabse common crash-trap).  result variable me -> print + return dono ho jaayen.

┌── FAMILY: recurse + COMBINE (answer = return-value) ──────────
│ base=null; left+right recurse -> jo COMBINE karo WAHI return. answer seedha return me aata.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAX DEPTH (LC-104)
 └──────────────────────────────────────────────────────────────
     base : null -> 0.
     comb : return 1 + max(leftDepth, rightDepth).   (khud=1 + gehri branch)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ INVERT TREE (LC-226)
 └──────────────────────────────────────────────────────────────
     base : null -> null.
     comb : recurse dono (left, right) -> phir children SWAP -> return root.
     TRAP : swap POINTERS (root->left <-> root->right), NA values.
            pointer-swap = poori BRANCH side badalti · value-swap = sirf 2 number + leaf pe null->val CRASH. order FREE.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SAME TREE (LC-100)
 └──────────────────────────────────────────────────────────────
     base : dono null->true · ek null->false · val alag->false.
     comb : isSame(p->left, q->left) && isSame(p->right, q->right).
     COMBINE = && (dono side match ho tabhi true).   (clean base: if(!p||!q) return p==q;)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SYMMETRIC TREE (LC-101)  = SAME-TREE + CROSS
 └──────────────────────────────────────────────────────────────
     base : dono null->true · ek null->false · val alag->false.
     comb : CROSS -> compare(p->left, q->right) && compare(p->right, q->left).   CROSS = mirror (yehi ek change).
     call : isSymmetric(root) = root null?true : compare(root->left, root->right).

┌── FAMILY: return-ONE + track-ANOTHER (post-order + global) ───
│ parent ko ek cheez (HEIGHT) chahiye -> wo RETURN; asli ANSWER global/ref me TRACK. recurse-PEHLE zaroori.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ DIAMETER (LC-543)  = MAX-DEPTH + 1 line
 └──────────────────────────────────────────────────────────────
     = max-depth ka height() helper (maxDia by-REFERENCE), BAS 1 line EXTRA:
        int height(root, int& maxDia):
            if (!root) return 0;
            left = height(root->left, maxDia);   right = height(root->right, maxDia);
            maxDia = max(maxDia, left + right);   // EXTRA line: is node se guzarne wali diameter = leftH + rightH
            return 1 + max(left, right);          // HEIGHT return (parent ko yeh chahiye)
     diameterOfBinaryTree: maxDia=0; height(root, maxDia); return maxDia.
     RETURN = height (parent ko chahiye); diameter = BYPRODUCT -> global maxDia (return NAHI).
     maxDia = MAX (overwrite NAHI -- test-pass != code-sahi trap).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ BALANCED TREE (LC-110)  = MAX-DEPTH + (-1 SENTINEL)
 └──────────────────────────────────────────────────────────────
     Q: HAR node pe abs(leftH - rightH) <= 1?  true/false.
     trick: alag "balanced?" pass NAHI -> height nikaalte-nikaalte HI check -> O(n) single-pass. maxDepth return = HEIGHT ya -1 (imbalance-flag).
        int maxDepth(root):
            if (!root) return 0;
            left  = maxDepth(root->left);    if (left == -1)  return -1;   // neeche imbalance -> UPAR propagate
            right = maxDepth(root->right);   if (right == -1) return -1;
            if (abs(left - right) > 1) return -1;                          // IS node pe imbalance
            return 1 + max(left, right);                                   // balanced -> asli HEIGHT
        isBalanced = (maxDepth(root) != -1).
     -1 = SENTINEL: imbalance neeche mila -> UPAR propagate -> ek node bhi galat = poora false.
     TRAP: check HAR node pe (sirf root nahi) -- root balanced par ANDAR imbalanced ho sakta [test-pass != code-sahi].

┌── FAMILY: search + BUBBLE-UP (found-node upar bhejo) ─────────
│ jise dhundh rahe wo mila -> UPAR return karo; jahan DONO taraf se kuch mila = answer.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LCA -- Lowest Common Ancestor (LC-236)
 └──────────────────────────────────────────────────────────────
     idea : wo node jahan p, q ALAG side split ho jaate (ek left, ek right).
        TreeNode* lca(root, p, q):
            if (!root) return NULL;
            if (root == p || root == q) return root;    // mil gaya -> UPAR bhejo
            left  = lca(root->left, p, q);
            right = lca(root->right, p, q);
            if (!left)  return right;                    // ek side khali -> doosri return
            if (!right) return left;
            return root;                                 // DONO non-null -> yehi split-point = LCA
     mechanic: found-node UPAR bubble; jahan DONO taraf se kuch mila = LCA. (if(!left) return right dono case sambhaalta: left-khali+right-mila, aur dono-khali->NULL.)

┌── FAMILY: carry-value DOWN + root-to-LEAF ────────────────────
│ value NEECHE carry karo (target ghatao / running-sum add); LEAF pe check. combine = OR (koi ek path).
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ PATH SUM (LC-112)
 └──────────────────────────────────────────────────────────────
     idea : root-se-LEAF sum == target? value NEECHE carry (target me se ghatao).
     base : null -> false.
     leaf : (!root->left && !root->right) -> return target == root->val.
     comb : hasPathSum(left, t-val) || hasPathSum(right, t-val).
     TRAP: LEAF = !root->left && !root->right (ACTUAL bachche), NA "!left && !right" (recursion-result) -> warna non-leaf pe galat match. [test-pass != code-sahi]

┌── FAMILY: BFS / LEVEL-ORDER (QUEUE -- DFS recursion se ALAG) ──
│ level-by-level chahiye (depth nahi) -> QUEUE (FIFO). koi recursion NAHI. root push -> while queue: ek baar = ek LEVEL.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LEVEL ORDER (LC-102)
 └──────────────────────────────────────────────────────────────
     idea : har level ki alag list -> list-of-lists. queue = FIFO (pehle-aaya-pehle-process) -> level-order + L-to-R apne aap.
     SKELETON (saare BFS-level Q bas isi pe):
         if (!root) return ans;
         q.push(root);
         while (!q.empty()) {
             int sz = q.size();               // SNAPSHOT -- is level ke node (for se PEHLE pakdo)
             vector<int> level;
             for (int i = 0; i < sz; i++) {
                 node = q.front();  q.pop();
                 level.push_back(node->val);
                 if (node->left)  q.push(node->left);    // bachche -> AGLE level me jaayenge
                 if (node->right) q.push(node->right);
             }
             ans.push_back(level);
         }
     TRAP: sz ko for se PEHLE pakdo. loop me bachche push -> q.size() badalta -> warna agle level ke node is level me ghus jaate.
     TEMPLATE: right-side-view (level ka LAST) · level-average · zigzag -- sab isi skeleton pe, bas "level me kya karna" badalta.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ RIGHT SIDE VIEW (LC-199)  = LEVEL-ORDER + 1 line
 └──────────────────────────────────────────────────────────────
     idea : daaye khade -> har level ka SIRF right-most node dikhta (baaki peeche chhup jaate).
     LEVEL-ORDER template bilkul SAME (while + sz-snapshot + for + push L/R). FARAK sirf level ke andar 1 line:
         level-order :  har node ka kaam / sab collect
         right-view  :  if(i == sz-1) ans.push_back(curr->val);     // level ka LAST hi
     KYUN i==sz-1: nodes L-to-R (FIFO) nikalte -> level ka AAKHRI = sabse RIGHT.  (left-side-view -> i==0 le lo.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ ZIGZAG LEVEL ORDER (LC-103)  = LEVEL-ORDER + reverse
 └──────────────────────────────────────────────────────────────
     idea : level-order BFS jaisa, par direction ALTERNATE (0:L->R · 1:R->L · 2:L->R...).
     = LEVEL-ORDER ka SAME skeleton (queue + sz-SNAPSHOT + inner-loop -> level) + ek levelCounter.
     DELTA (naya = bas ye, inner-loop ke BAAD -- level HAMESHA normal L->R banao):
         if (levelCounter % 2 != 0) reverse(level);   // ODD level -> ULTA (R->L)
         levelCounter++;  ans.push_back(level);
     even (0,2..) = seedha · odd (1,3..) = reversed.  (alt: deque/front-insert, par reverse simplest.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MINIMUM DEPTH (LC-111)   = LEVEL-ORDER + EARLY-STOP
 └──────────────────────────────────────────────────────────────
     idea : root se sabse PAAS wale LEAF tak depth. BFS neeche -> PEHLA leaf mila = min depth -> RUK jao (first = shortest).
     TEMPLATE (level-order skeleton + EARLY-STOP):
         if(!root) return 0;
         queue<TreeNode*> q; q.push(root);  int depth=0;
         while(!q.empty()){
             int sz=q.size();
             for(int i=0;i<sz;i++){
                 auto curr=q.front(); q.pop();
                 if(!curr->left && !curr->right) return depth+1;  // PEHLA leaf -> turant return (aage mat dekho)
                 if(curr->left)  q.push(curr->left);
                 if(curr->right) q.push(curr->right);
             }
             depth++;                                             // level POORA -> depth++ (for-loop ke BAAHAR)
         }
     depth = ab tak POORE hue level; leaf is level me mila -> return depth+1.
     DFS-TRAP (isliye BFS): "1 + min(leftD, rightD)" GALAT -- NULL-child ko depth-0 maan ke skew-tree (2->3->4) pe galat deta. BFS me ye dikkat nahi.

┌── FAMILY: BST (Binary Search Tree -- left < node < right) ────
│ BST ka INORDER (left->node->right) = SORTED (ascending). yehi BST ki asli TRICK -- "sorted/kth chahiye? -> INORDER".
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ KTH SMALLEST IN BST (LC-230)
 └──────────────────────────────────────────────────────────────
     idea : BST inorder = SORTED -> kth smallest = inorder ka k-va element.
     steps: inorder(root) -> sorted vector 'ans' -> return ans[k-1].
     k-1 TRAP: k 1-based (1st smallest = k=1), vector 0-based -> index = k-1.
     optimize: k-va pe EARLY-STOP (counter, k-va node pe ruko) -- collect-all bhi gate-theek.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ VALIDATE BST (LC-98)  = RANGE carry-DOWN
 └──────────────────────────────────────────────────────────────
     TRAP : local check (sirf turant child) KAAFI NAHI -- node ko SAARE ancestors ka rule maanna.
     IDEA : har node ki ek WINDOW (mini, maxi) -> node us range me ho. neeche jaate window TIGHT hoti.
     TEMPLATE:
         solve(root, mini, maxi):
            null -> true
            (val <= mini || val >= maxi) -> false
            left  = solve(L, mini, val)      // LEFT  jao -> maxi = val (chhota hona)
            right = solve(R, val, maxi)      // RIGHT jao -> mini = val (bada hona)
            return left && right
     CALL : solve(root, INT_MIN, INT_MAX).
     (alt: inorder traversal = strictly increasing? -> BST.)
     (2nd tarika: inorder = strictly increasing? -> kth-smallest ka near-transfer.)
```

---

## PATTERN 13 — GRAPHS (BFS/DFS + visited)

```

 BROAD IDEA: graph = nodes + edges. TREE bhi ek graph hai (special) -> Graph = Tree + 2 cheez:
   (1) CYCLE ho sakti -> VISITED chahiye (warna A->B->C->A infinite loop)   (2) pointers ki jagah ADJACENCY LIST.

 TREE se transfer: BFS = queue (level-order jaisa) · DFS = recursion (left/right ki jagah adj[node]). bas 'visited' add.

 ADJACENCY LIST (har graph-Q ka same FIRST-step) -- edges se banao:
      unordered_map<int, vector<int>> adj;    // ya  vector<int> adj[n];
      undirected -> adj[u].push_back(v);  adj[v].push_back(u);   // DONO taraf
      directed   -> sirf adj[u].push_back(v);                    // ek taraf

 ──────────────────────────────────────────────────────────────
 DECISION-GUIDE (koi graph-Q se pehle -- Trees-guide jaisa; "question dekh ke pehchano"):
   Q1  OUTER-LOOP bahar chahiye? (SABSE bada -- yehi trick)
        kitne group / poora graph / disconnected -> HAAN:
             for (i : all nodes)  if (!vis[i]) { count++; BFS(i); }         (CC, Islands)
        ek source -> target / ek hi component -> NAHI:
             sirf source se EK traversal (koi outer-loop nahi)              (Path-Exists)

 ──────────────────────────────
   Q2  DFS ya BFS? (connectivity ke liye dono same)
        BFS = shortest-path / level (unweighted)   ·   DFS = recursion, code chhota

        BFS (queue):                          DFS (recursion):
            vis[start] = true;                    void dfs(int node) {
            q.push(start);                            vis[node] = true;
            while (!q.empty()) {                      for (int nbr : adj[node])
                int node = q.front();                     if (!vis[nbr]) dfs(nbr);
                q.pop();                              }
                for (int nbr : adj[node])
                    if (!vis[nbr]) {
                        vis[nbr] = true;
                        q.push(nbr);
                    }
            }
        GRID pe: adj[node] ki jagah 4-direction (Q3 dirs-trick); visited = cell ko SINK ('0'/'2').

 ──────────────────────────────
   Q3  neighbours kaise? (graph ka roop)
        edges diye -> adjacency-list -> adj[node]
        grid diya  -> 2 TRICK (ratta -- islands/rotting/flood-fill sab me; ye apne-aap nahi aate):
          (a) DIRS-vector:  vector<vector<int>> dirs = {{1,0},{-1,0},{0,1},{0,-1}};
                            for (auto &d : dirs) { int nr=r+d[0], nc=c+d[1]; ... }
          (b) BOUNDS+valid if:  if (nr>=0 && nr<m && nc>=0 && nc<n && grid[nr][nc]==?) { ... }
              HAR grid-Q me lagti (YEHI trick); '==?' per-Q badalta (islands/rotting =='1').
   visited HAMESHA (cycle rok) -> push/enter ke waqt mark.

┌── FAMILY: single-source traversal (reachability) ────────────
│ KYUN SAATH: ek source se EK BFS/DFS -> jo REACHABLE wo visit; "target mila? / connected?". koi OUTER-loop nahi (ek source).
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ PATH EXISTS (LC-1971)  = single BFS (reachability)
 └──────────────────────────────────────────────────────────────
     = tree LEVEL-ORDER (BFS) ka SAME skeleton (queue + pop + push), graph ke liye 3 ADD:
        (1) NEIGHBOURS : tree me node->left/right built-in · graph me adj[node] (edges se banao).
        (2) VISITED    : tree me cycle nahi · graph me vis[] chahiye (cycle-rok, dobara na ghoomo).
        (3) LEVEL-loop NAHI : path-exists level-by-level nahi -> sirf reachability (target mil?).

     ADJ build (undirected -- NAYA):
         for(auto& it : edges){
             adj[it[0]].push_back(it[1]);
             adj[it[1]].push_back(it[0]);     // undirected -> dono taraf
         }

     BFS flow:
         q.push(src);  vis[src] = true;
         while(!q.empty()):
             node = pop
             if(node == dst) return true
             for(nbr : adj[node])
                 if(!vis[nbr]) { vis[nbr]=true; q.push(nbr); }
         return false;                        // queue khali -> koi path nahi

┌── FAMILY: COUNT components (outer-loop + FLOOD each) ─────────
│ KYUN SAATH: har node/cell pe jao; UNVISITED mila -> naya group -> count++ + flood (poora group visit-mark). dono same skeleton.
│ (Islands = grid · Connected-Components = adj-list; sirf "neighbour kaise" badalta.)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ NUMBER OF ISLANDS (LC-200)  = Connected-Components on a GRID
 └──────────────────────────────────────────────────────────────
     outer: for(i,j) if(grid[i][j]=='1') { DFS(i,j); count++; }   // har naya '1' = naya island
     DFS(i,j) — code order (upar se neeche), grid ke 3 farak (adj-list nahi):
        1. BASE (ek hi if, sab || se juda):
              if( i<0 || i>=m || j<0 || j>=n || grid[i][j]=='0' )  return;
        2. VISITED   : grid[i][j]='0' (SINK) -> dobara na gino.
        3. NEIGHBOURS: 4-dir DFS -> (i+1,j)(i-1,j)(i,j+1)(i,j-1).
     '0'-check base me (bounds ke SAATH) -- warna paani/visited pe nahi rukta, poora grid kha jaata.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ CONNECTED COMPONENTS (LC-323)  = ISLANDS par ADJ-LIST graph (grid nahi) + BFS
 └──────────────────────────────────────────────────────────────
     SAAR : ISLANDS ka SAME skeleton (outer-loop + count), bas 2 farak:
            (a) graph = ADJ-LIST (grid nahi)   (b) BFS use kiya (islands ne DFS kiya tha).
     input = n nodes + edges[] -> pehle ADJ banao (undirected -> DONO taraf):   -- NAYA
         unordered_map<int, vector<int>> adj;
         for(auto &it : edges){ adj[it[0]].push_back(it[1]); adj[it[1]].push_back(it[0]); }
     TEMPLATE:
         vector<bool> vis(n,false);   int count=0;
         for(i=0..n-1)                                // outer-loop = har node scan -> DISCONNECTED groups bhi cover
             if(!vis[i]){ count++; bfs(i,adj,vis); }  // har UNVISITED node = naya component
         return count;
         bfs(i): q.push i + vis[i]=true;
                 pop node -> for(auto &it : adj[node]) if(!vis[it]){ vis[it]=true; q.push it; }   // = PATH-EXISTS ka body
     count++ AUR bfs DONO if(!vis[i]) ke ANDAR -- warna har node alag gine (=n galat).
     neighbours = adj[node] (islands me 4-direction tha) -- yahi ek farak, baaki skeleton same.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MAX AREA OF ISLAND (LC-695)  = Islands, par MAX AREA (count nahi)
 └──────────────────────────────────────────────────────────────
     = ISLANDS ka SAME code (base + sink SAME), bas 2 DELTA:
        (1) DFS void nahi -> AREA return:  return 1 + 4-dir DFS ka sum;   // khud(1) + chaaron ka area
        (2) outer count++ nahi -> ans = max(ans, DFS(i,j));              // har island ka area -> MAX
     base+sink = islands JAISA (bas DFS ab int -> base return 0).
     ans = 0 se start (area hamesha >=0; koi island nahi -> ans 0 hi -> return 0).

┌── FAMILY: MULTI-SOURCE / level-BFS ──────────────────────────
│ KYUN SAATH: saare sources EK saath queue (level-0); per-level sz-snapshot -> ek level = ek step/minute. (level-order ka cousin.)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ ROTTING ORANGES (LC-994)  = grid MULTI-SOURCE BFS + levels
 └──────────────────────────────────────────────────────────────
     = tree LEVEL-ORDER (sz-snapshot) grid pe, MULTI-SOURCE.  har level = 1 minute.

     SETUP:
         for(i,j): if(grid[i][j]==2) q.push({i,j});   // MULTI-SOURCE = level-0 (saare rotten PEHLE)
         dirs = {{1,0},{-1,0},{0,1},{0,-1}};          // 4-dir neighbour-trick

     BFS (har level = 1 minute):
         while(!q.empty()){
             sz = q.size();
             while(sz--){
                 pop(r,c);
                 for(d : dirs){
                     nr=r+d[0], nc=c+d[1];
                     if(nr>=0 && nr<m && nc>=0 && nc<n && grid[nr][nc]==1){
                         grid[nr][nc]=2; push({nr,nc});
                     }
                 }
             }
             mins++;
         }

     END:
         for(i,j): if(grid[i][j]==1) return -1;       // fresh bacha -> impossible
         return mins-1;                               // last level extra -> -1

┌── FAMILY: TOPO-SORT / cycle-detect (DIRECTED graph) ─────────
│ KYUN SAATH: "prerequisites/dependencies + can-finish? / valid-order?" -> TOPO-SORT (Kahn's = BFS + indegree). cycle -> impossible.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ COURSE SCHEDULE (LC-207)  = TOPO-SORT (Kahn's), cycle-detect
 └──────────────────────────────────────────────────────────────
     idea : "prereq + can-finish-ALL?" = koi CYCLE to nahi? cycle -> false.
     question deta -> MATRIX:  prerequisites = [[1,0],[2,1],[3,2]]   // har [a,b] = a lene ke liye b pehle (edge b se... u->v)
     TEMPLATE (Kahn's = BFS + indegree):
         usme se KHUD adj-list + indegree banao:
         for(it : prerequisites){ u=it[0], v=it[1];  adj[u].push_back(v); indegree[v]++; }   // u->v; v ka prereq++
         for(i=0;i<n;i++) if(indegree[i]==0) q.push(i);        // jo bhi indegree-0 (koi prereq nahi) -> queue me
         while(q){ node=pop; topo.push(node);
                   for(nbr:adj[node]){ indegree[nbr]--; if(indegree[nbr]==0) q.push(nbr); } }  // prereq pura -> ghatao; 0 hua -> push
         return topo.size() == numCourses;                     // sab process = no cycle
     indegree = "kitne prereq BACHE"; 0 = ready-to-take. cycle-wale nodes ka indegree KABHI 0 nahi -> queue me aate hi nahi -> topo.size() < n -> false.

┌── FAMILY: CLONE / deep-copy (hashmap old->new) ────────────────
│ KYUN SAATH: "graph ka DEEP COPY / naya-graph banao" -> traversal (BFS/DFS) + map(purana->naya). map = cycle-rok + clone-dhundo.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ CLONE GRAPH (LC-133)  = BFS/DFS + map(old->new)
 └──────────────────────────────────────────────────────────────
     SAAR : "mere GRAPH ke saare node map me clone bana (A->A', B->B'), phir clones ko CONNECT kar (A'--B')." bas.

     idea : map me har node ka clone BANAO (A->A', B->B') -> phir un clones ko CONNECT karo (sab map ke andar).
            2 kaam:  (1) clone map me daalo   (2) neighbours ke clone se JODO (push_back).
            map ki VALUE hi clone -> mp[node] = us node ka clone; map = "clone ho chuka?" + "uska clone kaha".

     VISUAL (A-B graph):
         original:  A --- B
         (1) clone map me daalo:  mp = { A->A' ,  B->B' }     // naye node, abhi LOOSE (jude nahi)
         (2) clones ko JODO:      A' --- B'                    // mp[A]->neighbors += mp[B]
         => naya deep-copy graph  A'-B'  taiyaar

     TEMPLATE (BFS = normal traversal; map<old,new> + 2 special move):
         mp[node] = clone(node);  q.push(node);          // seed: start ka clone map me + queue
         BFS(q): curr nikalo -> for(it : curr->neighbors):
             (a) it clone nahi hua? -> mp[it]=clone(it); q.push(it);   // naya clone banao + push
             (b) mp[curr]->neighbors.push_back(mp[it]);                // mp[curr]=A', mp[it]=B' -> JODO (crux)
         return mp[node];
         (clone(x) = new Node(); ->val = x->val)

     2 alag kaam: clone BANANA (exist) vs JODNA (push_back = edge). bina jodna -> loose clones -> adhoora.
     DFS: recursion + same map -> node pe: map me hai? return; nahi -> clone+map -> har nbr recurse+jodo -> return.

┌── FAMILY: GRAPH-COLOURING / bipartite (2-colour) ─────────────
│ KYUN SAATH: "graph ko 2 group me baant sakte? / odd-cycle?" -> 2-COLOUR BFS. adjacent same-rang = false.
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ IS BIPARTITE (LC-785)  = 2-colour BFS + outer-loop (CC jaisa)
 └──────────────────────────────────────────────────────────────
     SAAR : har node 2 rang (0/1); koi 2 ADJACENT same-rang na ho -> bipartite. odd-cycle -> false.
     idea : input KHUD adj-list. disconnected -> outer-loop (CC jaisa).
     TRICK (NAYA): 2-COLOUR
        color[] : -1 = uncolored,  0/1 = do rang.
        START ko 0 do -> har neighbour OPPOSITE:  color[it] = !color[curr]   (0<->1 flip).
        neighbour ka rang curr ke SAME nikla -> CONFLICT -> return false.
     TEMPLATE:
         color[] = -1 (sab uncolored);
         outer loop har node i: uncolored mila ->
         phir BFS(i) us current node se shuru:
             color[i]=0;  q.push(i);        // START ko color ZAROORI -- warna !color[curr] galat
             curr nikalo -> for(it : graph[curr]):
                 uncolored(-1) -> color[it] = !color[curr];  q.push(it);
                 else color[it]==color[curr]? -> return false;

┌── FAMILY: WEIGHTED shortest-path ──────────────────────────────
│ KYUN SAATH: "edges pe WEIGHT/cost + shortest path?" -> Dijkstra. (kam edges != kam weight -> BFS kaafi nahi)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ DIJKSTRA  = BFS template + 2 swap + relaxation
 └──────────────────────────────────────────────────────────────
     REAL: Google Maps/GPS (shortest route), network routing, delivery/flights -- roz use hota.

     SAAR : BFS ka WEIGHTED bhai. wahi template, 2 swap + relaxation:
              queue     -> MIN-HEAP (chhoti dist pehle)
              visited[] -> dist[] (init INF, src=0)
              + naya    = RELAXATION

     NO visited[] kyun:
        dist[] + "wt > dist[node] continue" (STALE-skip) = visited-ka-kaam (node dobara finalize nahi).
        strict-relax (naya STRICTLY chhota -> tabhi push) -> undirected me bhi infinite-loop nahi.
        negative weight -> Dijkstra FAIL (dist ghatta rehta) -> Bellman-Ford.

     TRICK (RELAXATION = "dheela karo"):
        d = node tak ki dist,  w = us EDGE ka weight (it.second, NA dist[node]).
        naya rasta (d + w) purane dist[nbr] se kam? -> dist[nbr] = d+w; push.

     idea : min-heap kyun -> "kam edges != kam weight" -> greedily sabse-sasti-abhi-tak node uthao.

     PAIR-in-HEAP trick:
        pq me {dist, node} PAIR -> heap FIRST element (dist) se sort (tie -> second).
        min-heap -> sabse chhoti dist TOP pe. (top-K / value+index saath me bhi: HAMESHA {jispe-sort, saath-wala}.)

     TEMPLATE:
         adj: for(it:edges){ u=it[0],v=it[1],w=it[2]; adj[u]+={v,w}; adj[v]+={u,w}; }  // undirected DONO taraf
         vector<int> dist(n, INF);  dist[src]=0;   pq.push({0, src});
         while(!pq.empty()){
             wt = pq.top().first;  node = pq.top().second;  pq.pop();
             if(wt > dist[node]) continue;                     // STALE skip
             for(it : adj[node]){              // it.first = nbr node,  it.second = edge weight
                 if(wt + it.second < dist[it.first]){          // RELAX (naya rasta chhota?)
                     dist[it.first] = wt + it.second;
                     pq.push({wt + it.second, it.first});
                 }
             }
         }
         return dist;
     min-heap (pura syntax): priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;  // {dist,node}

┌── FAMILY: DSU / Union-Find ────────────────────────────────────
│ KYUN SAATH: "groups / connected / same-set?" + edges ek-ek MERGE -> DSU. (BFS/DFS=pura traverse; DSU=incremental)
└───────────────────────────────────────────────────────────────

 ┌──────────────────────────────────────────────────────────────
 │ ▸ DSU (Union-Find)  = parent[] + find + unite
 └──────────────────────────────────────────────────────────────
     REAL: dost-circle manager ("A-B same circle?"), network/accounts merge.

     SAAR : parent[] = har node ka leader. edge -> 2 node UNITE. end me alag ROOT (parent[i]==i) = components.
     KAB  : groups/connected/same-set? · UNDIRECTED cycle-detect · provinces · redundant-conn · MST-Kruskal.

     TEMPLATE (class):
         DSU(n): parent[i]=i;  rnk[i]=0;

         find(x):
            if(x==parent[x]) return x;
            return parent[x] = find(parent[x]);          // PATH COMPRESSION

         unite(x,y):
            ux=find(x), uy=find(y);
            if(ux==uy) return;                            // already same set
            if(rnk[ux] < rnk[uy]) parent[ux]=uy;          // UNION BY RANK
            else if(rnk[uy] < rnk[ux]) parent[uy]=ux;     // chhota bade ke neeche
            else { parent[uy]=ux; rnk[ux]++; }

     same group? -> find(x) == find(y)
     components  -> count(i where find(i)==i)

     2 opt: path-compression (tree flat) + union-by-rank (chhota bade ke neeche) -> find/unite ~O(1).
```

---

## PATTERN 14 — HEAP (priority_queue: min/max top pe)

```
 BROAD IDEA: baar-baar MIN ya MAX chahiye + data BADALTA rehta -> HEAP (sort mahenga). top=O(1), push/pop=O(log n).
   C++ syntax:  priority_queue<int> = MAX-heap  ·  priority_queue<int, vector<int>, greater<int>> = MIN-heap.
   ops: pq.push(x) · pq.top() · pq.pop() · pq.size() · pq.empty().
   REAL: emergency-room (severity top pe). KAB: top-K · k-th largest/smallest · merge-k-lists · Dijkstra · median-stream.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LAST STONE WEIGHT (LC-1046, Easy)  = MAX-HEAP smash
 └──────────────────────────────────────────────────────────────
     SAAR : har turn "2 SABSE BHAARI" pathar smash -> MAX-HEAP. pop 2, diff wapas push, till <=1 bache.
     TEMPLATE:
         priority_queue<int> pq;  for(s:stones) pq.push(s);        // saare -> max-heap
         while(pq.size() > 1){
             x=pq.top(); pq.pop();   y=pq.top(); pq.pop();         // 2 sabse bhaari nikaalo
             diff = abs(x-y);  if(diff>0) pq.push(diff);           // barabar -> 0 (dono khatam)
         }
         return pq.empty() ? 0 : pq.top();
     heap kyun (sort nahi): smash ke baad set BADALTA -> baar-baar "2 max" chahiye -> pop O(log n), top O(1).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ KTH LARGEST ELEMENT (LC-215, Medium)  = MIN-HEAP of size k
 └──────────────────────────────────────────────────────────────
     SAAR : k SABSE BADE min-heap me rakho -> unme se sabse chhota (heap TOP) = k-th largest.
     TRICK (top-K ka core): MIN-heap of size k.  (kyun MIN: size>k pe sabse-chhota EVICT karna hai.)
     TEMPLATE:
         priority_queue<int, vector<int>, greater<int>> pq;   // MIN-heap
         for(num : nums){ pq.push(num);  if(pq.size() > k) pq.pop(); }   // k se zyada -> chhota nikaal
         return pq.top();                                      // k bade me se chhota = kth largest
     O(n log k) (sort O(n log n) se behtar jab sirf k-th chahiye).  yehi pattern = "top-K" saare problems.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MERGE K SORTED LISTS (LC-23, Hard)  = MIN-HEAP of current heads
 └──────────────────────────────────────────────────────────────
     SAAR : min-heap me har list ka current HEAD. pop chhota -> result me jodo -> uska NEXT push. repeat.
     TEMPLATE:
         struct Compare { bool operator()(ListNode*a, ListNode*b){ return a->val > b->val; } };  // MIN-heap
         priority_queue<ListNode*, vector<ListNode*>, Compare> pq;
         for(l:lists) if(l) pq.push(l);                            // saare heads (NULL skip)
         dummy + tail;                                             // merge-2-lists jaisa build
         while(!pq.empty()){
             curr = pq.top(); pq.pop();  tail->next = curr; tail = curr;   // chhota -> jodo
             if(curr->next) pq.push(curr->next);                  // uska next pq me
         }
         return dummy->next;
     heap me max k nodes (1 per list) -> O(N log k).  ListNode* min-heap = custom Compare (a->val > b->val).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SORT NEARLY-SORTED (k-sorted) ARRAY  = MIN-HEAP size k+1 (sliding buffer)
 └──────────────────────────────────────────────────────────────
     SAAR : har element sahi position se max k DOOR -> O(n log k) sort (n log n se behtar).
     DERIVE (WHY heap): index-0 pe MIN aayega, wo max index k tak -> "pehle k+1 me se MIN kaun?"
        -> baar-baar MIN + data badalta -> DECISION-table = HEAP. min chahiye -> MIN-heap.
     TEMPLATE: min-heap. har element push -> size k+1 se bada -> top pop (ans me). end: heap khali karo (pop -> ans).
     = KTH-LARGEST ("heap size k") ka COUSIN: wahan k bade RAKHE, yahan k+1 ka SLIDING buffer.  O(n log k).
```

---

## PATTERN 15 — INTERVALS (sort-by-start + overlap)

```
 BROAD IDEA: interval = [start,end]. dikha -> PEHLA reflex = SORT by start. phir left-to-right overlap handle.
   OVERLAP (sorted me): curr.start <= prev.end  ->  merge [start, max(prev.end, curr.end)].
   REAL: calendar/meeting-scheduling, booking, resource-alloc.  KAB: merge/insert/count-overlaps, meeting-rooms.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MERGE INTERVALS (LC-56)  = sort-by-start + merge
 └──────────────────────────────────────────────────────────────
     SAAR : sort by start -> ans me first daalo -> har next OVERLAP? merge : push.
     TEMPLATE:
         sort(intervals.begin(), intervals.end());     // default FIRST-element (start) se sort (pair jaisa)
         ans.push_back(intervals[0]);                   // pehla baseline
         for(i=1..n){
             if(intervals[i][0] <= ans.back()[1])       // OVERLAP: curr.start <= last.END
                 ans.back()[1] = max(ans.back()[1], intervals[i][1]);   // merge -> bada end
             else ans.push_back(intervals[i]);          // no overlap -> naya
         }
     DRY-RUN  [[1,3],[2,6],[8,10],[15,18]]  (sorted):
         ans=[[1,3]]
         [2,6]  : 2 <= 3  overlap -> end=max(3,6)=6 -> ans=[[1,6]]
         [8,10] : 8 <= 6? NO      -> push          -> ans=[[1,6],[8,10]]
         [15,18]: 15 <=10? NO     -> push          -> ans=[[1,6],[8,10],[15,18]]
     ans.back() = last interval; [0]=start [1]=END.  overlap = curr.start last.END ko chhoo/cross kare. sort-by-start HAMESHA pehla step.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MEETING ROOMS (LC-252, Easy)  = Merge Intervals ka SAME template, bas action alag
 └──────────────────────────────────────────────────────────────
     SAAR : saare meeting attend ho sakte? = koi 2 OVERLAP na karein. (Merge ka SAME skeleton reuse.)
     SAME as MERGE (sort + ans.push(first) + loop overlap-check) -- bas overlap pe KYA karo:
          MERGE:   overlap -> ans.back()[1]=max(...)  (merge)  |  end: return ANS (list)
          MEETING: overlap -> return FALSE           (ruko)    |  end: return TRUE
     TEMPLATE (merge-jaisa hi):
         sort(intervals);  ans.push_back(intervals[0]);
         for(i=1..n){
             if(intervals[i][0] < ans.back()[1]) return false;   // overlap -> attend nahi
             else ans.push_back(intervals[i]);
         }
         return true;
     boundary FARAK (crux): Merge touching-MERGE -> `<=`  ·  Meeting touching-OK -> `<`.
     ek pattern (sort+overlap) -> bas (a) overlap pe action (merge vs stop) + (b) boundary (< vs <=) badalta. PROBLEM SAMJHO -> decide.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ INSERT INTERVAL (LC-57)  = already-SORTED -> 3 phase (no sort)
 └──────────────────────────────────────────────────────────────
     SAAR : intervals pehle se sorted+non-overlap. new insert karo (overlap->merge). SORT nahi -> ek pass, 3 phase.
     KEY : new = "GROWING BOX" -- har overlap use STRETCH karta; sab overlap khatam pe ek baar push.
     TEMPLATE:
         vector<vector<int>> ans;  int count=0;
         for(i=0..n){
             if(intervals[i][1] < new[0])       ans.push_back(intervals[i]);   // 1. new se PEHLE khatam -> push
             else if(intervals[i][0] > new[1])  break;                          // 3. new ke AAGE -> BREAK (sorted, aage sab door)
             else { new[0]=min(new[0],intervals[i][0]);                         // 2. OVERLAP -> new ko GROW
                    new[1]=max(new[1],intervals[i][1]); }
             count++;                                                           // kitne process hue (break se pehle ruk)
         }
         ans.push_back(new);                                                   // 4. bada-hua new daalo
         for(i=count..n) ans.push_back(intervals[i]);                          //    phir count se aage baaki
         return ans;
     break-optimization: interval.start > new.end -> aage sab aur door (sorted) -> ruk. 'count'=kahan ruke -> doosra loop wahin se.
     Merge/Meeting me SORT + ans.push(first);  yahan sorted-diya -> SORT nahi, bas 3-phase (before / grow / after).
```

## PATTERN 16 — BACKTRACKING (choose / explore / un-choose)

```
 BROAD IDEA: saari possibilities explore -> solution EK-EK step build; galat lage -> PEECHE aao (backtrack) -> agla raasta.
   MAZE anchor: raasta pakdo -> dead-end -> WAPAS last-junction -> doosra try. har element pe CHOICE(s).
   TEMPLATE (3 step): CHOOSE (element lo/path me daalo) -> EXPLORE (aage recurse) -> UN-CHOOSE (pop/undo -> agli choice).
   path by-REFERENCE (bahar bana) = ek SHARED path; push/pop usi pe -> backtrack kaam kare. (helper-andar naya = toot jaata.)
   DECISION TREE: har node = choice-point; har leaf tak ka raasta = ek possible answer.
   KAB: "saare subsets/permutations/combinations", "N-queens", "har-combination try karo".

 ──────────────────────────────────────────────────────────────
 2 FORMS = SAME cheez, alag LIKHAWAT (dono recursion + undo, same output):
    A) INCLUDE/EXCLUDE : 2 explicit call (element liya / nahi-liya). base pe record.
         solve(i): if(i==n){ record; return; }
                   push -> solve(i+1) -> pop;    // liya
                   solve(i+1);                    // nahi-liya
    B) FOR-LOOP (start-loop): loop se har element pe branch. HAR node pe record.
         solve(start): record;
                       for(i=start..n){ push(i) -> solve(i+1) -> pop; }
 dono me core SAME: CHOOSE -> EXPLORE (recursion) -> UN-CHOOSE (undo/pop). bas shakal alag.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ SUBSETS (LC-78)  = har element pe 2 choice (INCLUDE / EXCLUDE)
 └──────────────────────────────────────────────────────────────
     SAAR : har element ke saamne 2 raaste -> LE lo ya CHHOD do -> 2 recursive call. index==size pe temp = ek subset.
     TEMPLATE (index-based, include/exclude):
         void solve(nums, ans, index, temp):
             if(index >= nums.size()){ ans.push_back(temp); return; }   // poora subset ban gaya
             // INCLUDE:
             temp.push_back(nums[index]);
             solve(nums, ans, index+1, temp);
             temp.pop_back();                     // UN-CHOOSE (backtrack)
             // EXCLUDE:
             solve(nums, ans, index+1, temp);     // element chhod ke aage
         // caller: temp BAHAR bana ke by-ref pass; solve(nums, ans, 0, temp).
     temp by-REFERENCE (shared path); push/pop usi pe -> backtrack. helper-andar banate -> har call naya -> toot jaata.
     2^n subsets (har element in/out = power set).

     ── ALT FORM: START-LOOP (for-loop) ── same cheez, alag likhawat (dono yaad rakho):
         void solve(nums, ans, index, temp):
             ans.push_back(temp);                 // HAR node = ek subset (alag base-case NAHI)
             for(i = index; i < nums.size(); i++){
                 temp.push_back(nums[i]);          // CHOOSE
                 solve(nums, ans, i+1, temp);      // EXPLORE (i+1 -> agla aage se, peeche dobara nahi)
                 temp.pop_back();                  // UN-CHOOSE (backtrack)
             }
     start-loop me EXPLICIT base-case ki zaroorat NAHI: index==n -> loop chalega hi nahi -> khud ruk jaata.
       (include/exclude form me 'if(index==size) return' likhna padta; loop form khud terminate.)
     FARAK: include/exclude = base pe subset add + 2 branch  |  start-loop = HAR node pe add + loop. dono 2^n, same choose/explore/un-choose.

     ── RECURSION TREE (nums=[1,2,3]) ── har edge 2 BAAR: ↓neeche=PUSH(choose) · ↑wapas=POP(backtrack)
                                  [ ]                       <- solve(start=0)
                 ┌─────────────────┼─────────────────┐
             ↓+1 ↑pop1        ↓+2 ↑pop2        ↓+3 ↑pop3
                 │                 │                 │
                [1]               [2]               [3]      (har node = subset -> RECORD)
            ┌────┴────┐            │
        ↓+2 ↑pop2  ↓+3 ↑pop3   ↓+3 ↑pop3
            │         │            │
          [1,2]     [1,3]        [2,3]
            │
        ↓+3 ↑pop3
            │
         [1,2,3]  END (i=3, loop khatam)

     har edge pe DONO arrow: neeche jaate waqt ↓PUSH (choose), wapas aate waqt ↑POP (backtrack). ek edge = 2 baar cross.
     POP KYU? branch khatam -> jo element DAALA tha usko NIKAALO (undo) -> taaki WAHI jagah AGLE sibling ko mile.
        (undo na karo -> path ganda -> galat subset.)  e.g. [1,2,3]→↑pop3→[1,2]→↑pop2→[1]→ab loop aage ↓+3→[1,3].
     right taraf tree PATLI: i+1/start-aage -> node sirf apne AAGE wale pe branch ([2]→+3, [3]→kuch nahi).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ PERMUTATIONS (LC-46)  = used[] + FULL-LENGTH base (har position pe koi bhi bacha)
 └──────────────────────────────────────────────────────────────
     SAAR : saare elements ka har ORDER (arrangement). har position pe koi bhi UNUSED element -> used[] track.
     SUBSETS se FARAK (crux): subsets = 'start' index (sirf AAGE) -> [1,2]==[2,1] repeat nahi.
                                 permutation = ORDER matter -> loop 0 SE + used[] se skip (start nahi).
     TEMPLATE:
         void solve(nums, ans, temp, used):
             if(temp.size() == nums.size()){ ans.push_back(temp); return; }   // poora arrangement
             for(i = 0; i < nums.size(); i++){                                 // 0 se (start nahi)
                 if(used[i]) continue;                                         // already-liya -> skip
                 used[i]=true;  temp.push_back(nums[i]);      // CHOOSE
                 solve(nums, ans, temp, used);                // EXPLORE (i+1 nahi -> phir 0 se, used skip)
                 temp.pop_back();  used[i]=false;             // UN-CHOOSE (dono undo)
             }
         // caller: temp + used(n,false) BAHAR bana ke by-ref pass.
     n! permutations.  base = size==n (full length), subsets me har-node/index-end tha.
     INDEX/start KYU NAHI PASS kiya (permutation me): order MATTER karta -> har position pe koi bhi UNUSED element (peeche wala bhi) chahiye -> loop HAMESHA 0-se -> "peeche mat jao" wali baat hi nahi -> isliye 'start' ki zaroorat nahi, uski jagah used[].
        (subsets/combinations me start = "peeche mat jao" (duplicate-rok, order-agnostic). permutation me wo nahi chahiye -> used[] = "same element dobara mat lo".)
     1-line yaad: SUBSETS='start-aage' (kaunse) · PERMUTATION='used[]+loop-0-se, NO start' (kis order).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ COMBINATIONS (LC-77)  = SUBSETS-code REUSE, bas base size==k (pattern-reuse!)
 └──────────────────────────────────────────────────────────────
     SAAR : 1..n me se k numbers ke saare combos (order nahi). = subsets ka start-loop, record SIRF size==k pe.
     TEMPLATE:
         void solve(n, k, ans, start, temp):
             if(temp.size() == k){ ans.push_back(temp); return; }   // fixed-size combo poora
             for(i = start; i <= n; i++){                            // 1..n numbers, start se aage
                 temp.push_back(i);                // CHOOSE
                 solve(n, k, ans, i+1, temp);      // EXPLORE (i+1 -> order nahi, aage hi)
                 temp.pop_back();                  // UN-CHOOSE
             }
         // caller: solve(n, k, ans, 1, temp)   (start = 1).
     subsets se FARAK = SIRF base: subsets har-node add · combinations size==k pe add. (baaki code SAME -> reuse.)
     family 1-line: SUBSETS='start-aage, har-node' · COMBINATIONS='start-aage, size==k' · PERMUTATION='used[]+loop-0, size==n'.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ COMBINATION SUM (LC-39)  = COMBINATIONS-code REUSE, 2 tweak: REUSE-'i' + TARGET-base
 └──────────────────────────────────────────────────────────────
     SAAR : candidates me se (REUSE allowed) sum==target wale saare combos. = combinations code, bas base target + recurse 'i'.
     TEMPLATE:
         void solve(cand, target, ans, start, temp):
             if(target < 0) return;                          // prune (aage koi faida nahi)
             if(target == 0){ ans.push_back(temp); return; } // valid combo
             for(i = start; i < cand.size(); i++){
                 temp.push_back(cand[i]);                     // CHOOSE
                 solve(cand, target - cand[i], ans, i, temp); // EXPLORE ('i' -> REUSE allowed, i+1 nahi)
                 temp.pop_back();                             // UN-CHOOSE
             }
     combinations se FARAK: (1) recurse 'i' na i+1 (same element dobara) (2) base = target (0=record, <0=prune), size==k nahi.
     start=i -> same element repeat OK par PEECHE wale nahi (duplicate combo rok). candidates positive -> target ghatta -> infinite nahi.

     ── ALT FORM: INCLUDE/EXCLUDE (2-branch) ── same output, alag likhawat (subsets ki tarah dono):
         void solve(cand, target, ans, index, temp):
             if(target < 0) return;
             if(index >= cand.size()){ if(target==0) ans.push_back(temp); return; }   // end pe check
             // INCLUDE: cand[index] liya (index SAME -> reuse)
             temp.push_back(cand[index]);
             solve(cand, target - cand[index], ans, index, temp);
             temp.pop_back();
             // EXCLUDE: cand[index] chhoda -> aage (index+1)
             solve(cand, target, ans, index+1, temp);
     for-loop form: base target==0 pe turant record.  include/exclude form: base index==size pe (target==0? record).
     FAMILY (5) 1-line: SUBSETS=start-aage,har-node · COMBINATIONS=start-aage,size==k · COMB-SUM=start-'i'(reuse),target-base · PERMUTATION=used[]+loop-0,size==n.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ WORD SEARCH (LC-79)  = grid-DFS + BACKTRACKING (subsets-family se ALAG: grid pe PATH, na element choose/skip)
 └──────────────────────────────────────────────────────────────
     SAAR : grid me `word` adjacent (4-dir) cells se banta hai? har cell EK-BAAR. har cell se DFS, mark→recurse→un-mark.
     TEMPLATE:
         bool dfs(m,n, i,j, board,word,index, vis):
             if(i>=m||i<0||j>=n||j<0 || vis[i][j] || board[i][j]!=word[index]) return false;  // ★ bounds+VISITED+mismatch = EK check (top pe)
             vis[i][j]=true; index++;
             if(dfs(i+1,j..) || dfs(i-1,j..) || dfs(i,j-1..) || dfs(i,j+1..)) return true;  // 4-dir || SHORT-CIRCUIT (ek true→baaki call NAHI)
             vis[i][j]=false;                                     // BACKTRACK (un-mark)
             return index==word.size() ? true : false;           // base: poora word mila
         exist(): har cell jaha board[i][j]==word[0] se dfs; koi true -> return true.
     ★ CRUX (yehi atkaata tha): (1) 4 DFS ko || me JODO — SHORT-CIRCUIT (ek direction true → baaki call hi nahi) (2) visited-check EK-hi if me (bounds+mismatch ke saath) (3) base = index==word.size().
     islands-DFS se FARAK: islands = bas fill/count. word-search = mark + recurse + UN-mark (path dobara use na ho) + return-bubble. yehi backtracking-twist.
```

## PATTERN 17 — DP (dynamic programming = recursion + memo)

```
 ESSENCE: "DP kuch ALAG nahi -- RECURSION ko SMART way me karna. plain recursion
      sabko BAAR-BAAR calculate karta (waste); DP bolti = jo ek baar nikala use ARRAY me STORE karlo, dobara maango
      to wahan se uthao. bas. DP = recursion ka smart tareeka." (kaam TABHI karta jab OVERLAP ho -- same subproblem repeat.)
 ──────────────────────────────────────────────────────────────
 ESSENCE-2 (DP me flow aane ke baad): "DP = FULL-TEMPLATE based, sabse template-able topic. har DP me wahi 3-part
      SKELETON: base + TRANSITION (solve(i-1)/solve(j-1)/... = choices) + memo. SIRF base+transition badalti, skeleton SAME.
      aur ye transition wahi RECURSION hai jo TREES (left/right=2) / DFS-grid (4-dir) / grid-DP (up/left=2) me thi -- bas
      BRANCH-ginti alag. DP = purani recursion + MEMO chipka di. 'naya' nahi laga kyunki tha hi nahi." => naya DP = bas "transition kya?" socho.
 ──────────────────────────────────────────────────────────────
 BASE-CASE = asli FIDDLY part: RELATION (take/not-take, min/max) lagbhag SAME rehta, aasaan.
      MUSHKIL = BASE + invalid-handling, jo har problem me ALAG. climbing/robber = base SIMPLE (i==0 -> value) -> jaldi ho gaye;
      coin-change = base AJEEB (i<0 vs i==0, INVALID=INT_MAX, -1) -> wahi phasaya. => relation easy likho, BASE pe RUK ke socho.

 ──────────────────────────────────────────────────────────────
 BROAD IDEA: same SUBPROBLEM baar-baar aaye (OVERLAPPING) -> dobara compute mat karo -> ek baar karke YAAD rakho.
   DP = RECURSION + memo. (recursion tu jaanta -- DP usi pe "cache" add karta.)
   ARC (har DP isi tarah crack karo): 1) plain RECURSION likho (relation + base) -> 2) OVERLAP dekho (dry-run: same call 2x)
      -> 3) MEMOIZE (dp[] cache) -> [optional] 4) BOTTOM-UP (loop se table bharo).
   2 STYLE:
      TOP-DOWN (memoization) = recursion + dp[] cache. sochne me aasaan (relation seedha).
      BOTTOM-UP (tabulation) = base bharo -> loop chhote-se-bade dp[i] bharo. no recursion-stack.
   MEMO ka pattern (har DP me SAME 3 cheez):
      1. dp[] ko -1 (ya kisi "nahi-nikala" marker) se init.
      2. function start: if(dp[state]!=-1) return dp[state];   // cache HIT
      3. compute + STORE: return dp[state] = <relation>;
   KAB DP: "kitne WAYS" · "min/max cost" · "kya possible" + har step CHOICE + subproblems OVERLAP.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ CLIMBING STAIRS (LC-70)  = Fibonacci-shape (ways = ways(n-1)+ways(n-2))
 └──────────────────────────────────────────────────────────────
     SAAR : n steps, ek baar 1 ya 2 step -> top tak kitne tareeke. step n pe -> (n-1) se 1-step YA (n-2) se 2-step.
        => solve(n) = solve(n-1) + solve(n-2).  base: n==0 || n==1 -> 1.  (= Fibonacci)
     TEMPLATE (top-down memo):
         int solve(int n, vector<int>& dp){
             if(n==0 || n==1) return 1;              // base
             if(dp[n]!=-1) return dp[n];             // cache HIT
             return dp[n] = solve(n-1,dp) + solve(n-2,dp);   // compute + STORE
         }
         // caller: vector<int> dp(n+1, -1); return solve(n, dp);
     plain recursion = solve(n-2) BAAR-BAAR (overlap) -> exponential. memo se O(n).
     BOTTOM-UP bhi: dp[0]=dp[1]=1; for(i=2..n) dp[i]=dp[i-1]+dp[i-2]; return dp[n].

 ┌──────────────────────────────────────────────────────────────
 │ ▸ HOUSE ROBBER (LC-198)  = har step pe TAKE/SKIP choice + max
 └──────────────────────────────────────────────────────────────
     SAAR : adjacent ghar loot nahi sakte -> max paisa. har ghar i pe 2 CHOICE:
        LOOT i : nums[i] + solve(i-2)   (i-1 skip -- adjacent nahi)  ·  SKIP i : solve(i-1)
        => solve(i) = max(LOOT, SKIP).  base: i<0 -> 0 · i==0 -> nums[0].
     TEMPLATE (top-down memo, LAST index se):
         int solve(nums, i, dp){
             if(i<0) return 0;
             if(i==0) return nums[0];
             if(dp[i]!=-1) return dp[i];                          // cache HIT
             int LOOT = nums[i] + solve(nums, i-2, dp);
             int SKIP =           solve(nums, i-1, dp);
             return dp[i] = max(LOOT, SKIP);                      // compute + STORE
         }
         // caller: dp(n,-1); return solve(nums, n-1, dp);        // n-1 = array ka LAST index (0..n-1)
     climbing-stairs se FARAK: wahan sirf ADD (ways(n-1)+ways(n-2)); yahan har step CHOICE -> max(take, skip).
     n-1 kyu: ye TOP-DOWN hi hai (bottom-up nahi). nums[] ke index 0..n-1 -> LAST ghar = n-1 -> "last tak max" = solve(n-1).
        (climbing me 'n' = step-count 1..n tha, array nahi; yahan index hai -> n-1. sirf naming, direction nahi.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ COIN CHANGE (LC-322)  = MIN coins + REUSE (dono form, backtracking jaisa)
 └──────────────────────────────────────────────────────────────
     SAAR : amount banane ko MIN coins (har coin UNLIMITED = reuse). na ban sake -> -1.
     amount==0 vs amount<0 (THEEK utna, na kam na zyada): amount==0 -> THEEK bana (11 = theek 11) -> SUCCESS -> 0 coins ·
        amount<0 -> 0 ke NEECHE (coin bada le liya, overshoot) -> INVALID. (ban-na = theek 0 pe RUKNA, cross nahi.)
     3 TRAP (yahi phasaate): (1) INT_MAX = "ban nahi sakta" signal -> min me apne-aap HAAR jaata.
        (2) OVERFLOW: 1+solve(...) SIRF jab solve != INT_MAX (warna 1+INT_MAX -> negative -> min galat).
        (3) caller: answer INT_MAX -> return -1.

     ── FORM A: TAKE / NOT-TAKE (index i, 2D dp[i][amount]) ──
         int solve(coins, amount, i, dp):
             if(amount < 0) return INT_MAX;                       // INVALID
             if(i < 0) return amount==0 ? 0 : INT_MAX;            // koi coin nahi bacha (i<0, NA i==0 -- coin[0] usable)
             if(dp[i][amount]!=-1) return dp[i][amount];
             int notTake = solve(coins, amount, i-1, dp);         // coin i chhoda -> agla
             int Take = INT_MAX;
             if(solve(coins, amount-coins[i], i, dp) != INT_MAX)  // overflow guard
                 Take = 1 + solve(coins, amount-coins[i], i, dp); // coin i liya (i SAME = reuse)
             return dp[i][amount] = min(notTake, Take);
         // caller: dp(n+1, vec(amount+1,-1)); ans=solve(..,n-1,..); return ans==INT_MAX?-1:ans;

     ── FORM B: FOR-LOOP (saare coins, 1D dp[amount]) ──  SAME idea, likhawat alag
         int solve(coins, amount, dp):
             if(amount < 0) return INT_MAX;
             if(amount == 0) return 0;
             if(dp[amount]!=-1) return dp[amount];
             int Take = INT_MAX;
             for(int i=0; i<coins.size(); i++)                              // har coin TRY (choose khud loop me)
                 if(solve(coins, amount-coins[i], dp) != INT_MAX)           // overflow guard
                     Take = min(Take, 1 + solve(coins, amount-coins[i], dp)); // sabme se MIN
             return dp[amount] = Take;
     dono form SAME (backtracking jaisa): TAKE/NOT-TAKE = index pe 2 branch · FOR-LOOP = SIRF take, NOT-TAKE apne-aap
        (loop me "kaunsa coin" khud choose). for-loop me 'i' index nahi -> 1D dp; 2D me state = (i, amount).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LONGEST INCREASING SUBSEQUENCE (LC-300)  = TAKE/NOT-TAKE + prev-track
 └──────────────────────────────────────────────────────────────
     SAAR : sabse lambi STRICTLY-increasing subsequence ki LENGTH. coin-change family (take/not-take), ek twist = prev.
     TEMPLATE:
         int solve(index, prevIndex, nums, dp):
             if(index == n) return 0;                                   // base (saaf -- no INT_MAX/overflow/-1)
             if(dp[index][prevIndex+1] != -1) return dp[index][prevIndex+1];
             int notTake = solve(index+1, prevIndex, nums, dp);         // chhoda -> prev wahi
             int Take = 0;
             if(prevIndex==-1 || nums[index] > nums[prevIndex])         // increasing rahe TABHI take
                 Take = 1 + solve(index+1, index, nums, dp);            // liya -> ab prev = index
             return dp[index][prevIndex+1] = max(Take, notTake);
         // caller: dp(n+1, vec(n+1,-1)); return solve(0, -1, nums, dp);
     TWIST vs coin-change: (1) state me PREV bhi (kya-liya, taaki increasing check ho) (2) take pe CONDITION (nums[i]>nums[prev]).
     MEMO offset: prevIndex -1 se shuru -> dp[index][prevIndex + 1] (+1 se index 0..n valid).
     LIS EASY kyu (Arpan): base sirf index==n -> 0 (coin-change ka i<0/INT_MAX/overflow/-1 ganda tha; yahan saaf).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ UNIQUE PATHS (LC-62)  = 2D DP intro (climbing-stairs ka 2D cousin)
 └──────────────────────────────────────────────────────────────
     SAAR : m x n grid, robot (0,0) -> (m-1,n-1), sirf RIGHT/DOWN. kitne raaste? cell pe = UPAR se + LEFT se.
     TEMPLATE:
         int solve(i, j, dp):
             if(i<0 || j<0) return 0;                       // grid ke bahar -> koi raasta nahi
             if(i==0 && j==0) return 1;                     // start cell
             if(dp[i][j] != -1) return dp[i][j];
             int up   = solve(i-1, j, dp);
             int left = solve(i, j-1, dp);
             return dp[i][j] = up + left;
         // caller: dp(m, vec(n,-1)); return solve(m-1, n-1, dp);
     climbing-stairs se CONNECT: 1D -> 2D. ways(n-1)+ways(n-2) -> ways(i-1,j)+ways(i,j-1). dp[n] -> dp[i][j].
     2D DP ka pehla: state = (i,j) do coordinate. base do (out-of-grid=0, start=1). memo 2D array.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MIN PATH SUM (LC-64)  = UNIQUE-PATHS reuse, bas MIN + cost
 └──────────────────────────────────────────────────────────────
     SAAR : grid me har cell cost. (0,0)->(m-1,n-1), right/down. MIN-sum path. = unique-paths code, 2 tweak.

     unique-paths se CONNECT (SAME template, sirf 3 line badli):
        return 0          ->  return INT_MAX             // base out-of-grid: count me 0 tha, ab INVALID -> min ignore karega
        return 1          ->  return grid[0][0]          // base start: count nahi -> start ki COST
        return up + left  ->  grid[i][j] + min(up,left)  // count-JOD -> apni cost + CHHOTA rasta
     KYUN: count problem -> cost-MIN problem. baaki structure (memo + 2 base + 2-dir) bilkul SAME.
     INT_MAX = coin-change INVALID-signal (min me haar jaata; real cell pe >=1 dir valid -> overflow nahi).

     TEMPLATE:
         int solve(i, j, grid, dp):
             if(i<0 || j<0) return INT_MAX;                 // INVALID (grid ke bahar) -> min ignore karega
             if(i==0 && j==0) return grid[0][0];            // start ki cost
             if(dp[i][j] != -1) return dp[i][j];
             int up = solve(i-1, j, ...), left = solve(i, j-1, ...);
             return dp[i][j] = grid[i][j] + min(up, left);  // apni cost + upar/left me CHHOTA

 ┌──────────────────────────────────────────────────────────────
 │ ▸ LONGEST COMMON SUBSEQUENCE (LC-1143)  = 2D DP on 2 STRINGS (match/no-match)
 └──────────────────────────────────────────────────────────────
     SAAR : do string me sabse lambi COMMON subsequence ki length. state = (i,j) = s1 index-i-tak, s2 index-j-tak.
     TEMPLATE:
         int solve(i, j, s1, s2, dp):
             if(i<0 || j<0) return 0;                              // koi string khatam
             if(dp[i][j] != -1) return dp[i][j];
             if(s1[i] == s2[j])                                    // char MATCH
                 return dp[i][j] = 1 + solve(i-1, j-1, ...);       //   liya -> DONO se peeche (diagonal)
             return dp[i][j] = max(solve(i-1, j, ...), solve(i, j-1, ...));  // no-match -> ek char chhodo (2 side)
         // caller: dp(m, vec(n,-1)); return solve(m-1, n-1, s1, s2, dp);
     MATCH -> diagonal (i-1,j-1) +1 · NO-MATCH -> max of 2 side (i-1,j)/(i,j-1). base i<0||j<0 -> 0.
     grid nahi (2 string) par same 2D soch; ye "2-string DP" ka base (edit-distance, etc. isi pe).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ EDIT DISTANCE (LC-72, Hard)  = LCS reuse -- no-match 3-choice + base 2-sided
 └──────────────────────────────────────────────────────────────
     SAAR : w1 -> w2 min ops (insert/delete/replace). = LCS code, transition badli. (HARD par template = easy)
     TEMPLATE:
         int solve(i, j, w1, w2, dp):
             if(i<0) return j+1;                          // w1 khatam -> bache w2 ke j+1 char INSERT
             if(j<0) return i+1;                          // w2 khatam -> w1 ke i+1 char DELETE
             if(dp[i][j] != -1) return dp[i][j];
             if(w1[i] == w2[j]) return dp[i][j] = solve(i-1, j-1, ...);   // MATCH -> koi op nahi
             return dp[i][j] = 1 + min({ solve(i-1, j-1, ...),   // REPLACE
                                         solve(i-1, j,   ...),   // DELETE (w1 peeche)
                                         solve(i,   j-1, ...) }); // INSERT (w2 peeche)
     LCS se FARAK: (1) no-match = 1 + MIN-of-3 (LCS me max-of-2 tha) (2) base = j+1/i+1 (LCS me 0).
     3-choice matlab: replace=diagonal · delete=i-1 · insert=j-1. base 2-sided (dono string ke bache char count).

     VISUAL (pointer PEECHE se: i=w1 last, j=w2 last, X != Y -> 1 op lagega):
        SETUP:   w1: . . X          w2: . . Y
                         ^i                  ^j

        REPLACE -- X ko Y bana do (dono match ho jaate):
           w1: . . X   ->   w1: . . Y        w2: . . Y
                   ^i               ^                ^j
           ab dono ka last = Y (MATCH) -> DONO peeche
           => solve(i-1, j-1)     [i--, j--]

        DELETE -- X hata do (w1 me ye extra tha):
           w1: . . X   ->   w1: . .          w2: . . Y   (chhua nahi)
                   ^i             ^i-1                ^j
           w1 ka X nikaala -> i peeche; w2 wahi ka wahi
           => solve(i-1, j)       [i--, j same]

        INSERT -- Y ko w1 me DAALO (w2 ka Y match karne ko):
           w1: . . X   ->   w1: . . X [Y]    w2: . . Y
                   ^i               ^i(wahi)          ^j-1
           naya [Y] daala -> w2 ka Y NIPAT gaya -> j peeche
           par w1 ka apna X abhi bacha (naya daala, purana nahi chhua) -> i WAHI
           => solve(i, j-1)       [j--, i same]

        Ek nazar me:
           REPLACE : X badla->Y          -> dono nipte -> i--, j--
           DELETE  : X NIKAALA (w1)      -> w1 nipta   -> i--, j same
           INSERT  : Y DAALA (w2 ke liye) -> w2 nipta  -> j--, i same
     CRUX: DELETE = w1 ka char NIKAALA (i ghata) · INSERT = w2 ke liye naya char DAALA (j ghata). ek nikaal ek daal -> ulta index.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ 0/1 KNAPSACK (classic)  = PURE take/not-take (item ONCE)
 └──────────────────────────────────────────────────────────────
     SAAR : items (wt,val) + bag capacity W. har item ek baar (0/1). MAX value, weight<=W. (chor: bag me best chura)
     TEMPLATE:
         int solve(wt, val, W, i, dp):
             if(i<0) return 0;                                       // koi item nahi
             if(dp[i][W] != -1) return dp[i][W];
             int notTake = solve(wt, val, W, i-1, dp);               // item chhoda
             int take = 0;
             if(wt[i] <= W)                                          // fit hota hai tabhi
                 take = val[i] + solve(wt, val, W - wt[i], i-1, dp); // liya -> value+ , weight ghata, i-1 (ONCE)
             return dp[i][W] = max(take, notTake);
     coin-change se FARAK: item ONCE -> take me i-1 (coin me REUSE -> i same tha). yehi 0/1 ka matlab.
     BASE i<0 vs i==0 (dono valid, same jawab): i<0 -> index-0 NORMAL item (recursion + i<0-base khud sambhaale, CLEANER);
        i==0 -> item-0 explicit-handle (recursion 1 level pehle rukti, zyada code). i<0 = "sach me khatam", index-0 = normal.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ VALID PAREN with '*' (LC-678)  = choice-per-char, 2D dp[index][open]
 └──────────────────────────────────────────────────────────────
     SAAR : '*' = '(' YA ')' YA khaali. valid ban SAKTI hai? har '*' pe 3-choice -> try-all -> state repeat -> memo.
     TRACK : ek counter open = ab tak kitne '(' UNMATCHED. state = (index, open). '(' 1 raasta, ')' 1, '*' 3 (choice).
     TEMPLATE:
         bool solve(s, index, open, dp):
             if(index == s.size()) return open == 0;                     // base: sab '(' match ho gaye?
             if(dp[index][open] != -1) return dp[index][open];
             if(s[index]=='(') return dp[index][open] = solve(s, index+1, open+1, dp);
             if(s[index]==')') return dp[index][open] = (open>0) ? solve(s, index+1, open-1, dp) : false;
             bool a = solve(s, index+1, open+1, dp);                      // '*' as '('
             bool b = (open>0) ? solve(s, index+1, open-1, dp) : false;   // '*' as ')'  (GUARD)
             bool c = solve(s, index+1, open, dp);                        // '*' khaali
             return dp[index][open] = a || b || c;
         // caller: dp(n+1, vec(n+1,-1)); return solve(s, 0, 0, dp);   -- open 0..n (worst = saare '(')
     ★ GUARD : jahan bhi open-1 -> pehle open>0 (warna open<0 -> dp[-1] crash + galat). '*' pe direct return-false MAT (baaki 2 option zinda).
     = COIN-CHANGE ka cousin (choice + memo), DELTA: state me 'open' (balance) + '*' pe 3-branch OR (true/false, min nahi).

 ┌──────────────────────────────────────────────────────────────
 │ ▸ MATRIX CHAIN MULT / MCM  = INTERVAL-DP  = two_max WALL + loop + recurse
 └──────────────────────────────────────────────────────────────
     SAAR : matrices chain multiply -> result SAME, COST order-pe badalti -> MIN scalar-mult nikaalo.

     INPUT p[]: chain ki dims. consecutive PAIR = ek matrix ka ROW x COL. adjacent matrices ek number SHARE karti.
        p[] :   10  --  30  --  5  --  60
        idx :   p0      p1     p2     p3
                 \__A1__/\__A2_/\_A3__/
        A1 = p0 x p1 = 10 x 30
        A2 = p1 x p2 = 30 x 5     <- p1=30 SHARED (A1 ka COL = A2 ka ROW -> isliye multiply hoti)
        A3 = p2 x p3 = 5  x 60
        => matrix A_i = p[i-1] x p[i]   (i = 1..n ;  n = p.size()-1 = 3 matrices)

     = two_max (Google) ka WALL hi. FARAK: two_max me left/right PRECOMPUTED (O(1)); yahan ranges OVERLAP
       -> har wall LOOP + har side RECURSE + memo (interval-DP, state = RANGE (i,j) -> 2D dp).
     TEMPLATE:
         solve(i, j):                              // A_i..A_j ko 1 matrix banane ka MIN cost
             if i == j: return 0                   // base: ek hi matrix bacha -> multiply ko kuch nahi -> 0  (2+ matrix hon tabhi mult)
             if dp[i][j] != -1: return dp[i][j]
             ans = INT_MAX

             // k = i se j-1 KYUN: k = WALL, chain ko (i..k)|(k+1..j) me todti; har side >=1 matrix chahiye.
             //   k=j pe RIGHT block khaali = invalid -> isliye max k = j-1.
             //   eg {10,30,5,60}: k=j-1 pe right single, recursion sambhaal leta.
             for k = i .. j-1:
                 cost = solve(i,k) + solve(k+1,j) + p[i-1]*p[k]*p[j]
                 ans = min(ans, cost)
             return dp[i][j] = ans
         // caller: solve(1, n) [n=p.size()-1].   i=0 MAT karo (p[i-1]=p[-1] crash).
     COMBINE (a*b*c): block A_i..A_k -> size p[i-1] x p[k]. wall pe LEFT=p[i-1]xp[k], RIGHT=p[k]xp[j] -> mult = p[i-1]*p[k]*p[j].
              number-line: p[i-1]=poore-left baayaan chhor · p[k]=WALL · p[j]=poore-right dayaan chhor.

     DRY-RUN (combine = p[i-1]*p[k]*p[j]):   p=[10,30,5,60], solve(1,3), wall k=1
             val:    10    30    5    60
             idx:    p0    p1    p2   p3
                     |LEFT|    |__RIGHT__|
                     (A1)       (A2..A3)
         LEFT  (A1):    10x30                          -> 1 matrix:  10 x 30   (= p0 x p1)
         RIGHT (A2,A3): 30x5 , 5x60   --recursion-->      1 matrix:  30 x 60   (= p1 x p3)
         COMBINE do result-matrix: (10x30) x (30x60) -> 10 * 30 * 60
                     = p0 * p1 * p3  =  p[i-1] * p[k] * p[j]   (i=1, k=1, j=3)
         (3 matrices -> ek side single, ek block. 4 hote -> dono block. formula wahi.)

     real: DB JOIN-order optimization · ML/graphics compute. interval-DP FAMILY: burst-balloons, stone-merge, optimal-BST.


 ┌──────────────────────────────────────────────────────────────
 │ ▸ BURST BALLOONS (LC 312)  = INTERVAL-DP  = MCM WALL + loop + recurse
 └──────────────────────────────────────────────────────────────
     SAAR : balloons ek-ek phodo. phodne pe coins = left * khud * right (padosi = us waqt ZINDA) -> saare phod ke MAX total.
     Q: balloons row, har pe number. ek-ek phodo; balloon i pe coins = nums[i-1]*nums[i]*nums[i+1]
        (padosi = us waqt zinda; array-bahar = 1). saare phod ke MAX total coins?

     INPUT p[]: balloons row; dono taraf 1 PAD (kinaare ka gaayab padosi = 1).
        pad = 1 KYUN (na 0): 1 = MULTIPLY ki identity -> kinaare wala phoote, gaayab-padosi ko 1 se guna = coin waisa ka waisa
           (edge saaf handle). 0 se pad karte -> coin = x*0 = 0 -> poora coin gayab (galat).

            .-.    .-.    .-.    .-.    .-.    .-.
           ( 1 )  ( 3 )  ( 1 )  ( 5 )  ( 8 )  ( 1 )
            `-'    `-'    `-'    `-'    `-'    `-'
           p0     p1     p2     p3     p4     p5        <- indexing (p0 & p[n+1] = pad)
           pad    \_________ asli _________/    pad

        PHOD  k=2 (the (1)):   padosi (3) & (5)
             .-.    .-.    .-.
            ( 3 )  ( 1 )  ( 5 )          coins = 3 * 1 * 5 = 15
             `-'   *POP*   `-'
             .-.           .-.
            ( 3 )         ( 5 )     ->   (1) gaya, ab (3) aur (5) adjacent
             `-'           `-'

        => phodne pe coin = (left) * (khud) * (right) ;  caller solve(1, n)  (n = asli count)

     Q (yahan): "k ko phodun to coins kitne milenge?"  ->  question ke hisaab se: coin = (left-padosi) * k * (right-padosi)
     FORMULA  p[i-1]*p[k]*p[j+1]  kaise banti: k phoda -> uske padosi = range ke BAAHAR wale 2 balloons:
              .-.                    .-.                    .-.
           (p[i-1])               ( p[k] )              (p[j+1])
              `-'                    `-'                    `-'
            baahar-L               k phoota               baahar-R
                       coin = p[i-1] * p[k] * p[j+1]

     = MCM ka WALL hi. FARAK: MCM me k = WALL (block ka part, MIN); yahan k = jo LAST phoota
       (REMOVED, dono side se BAHAR) -> solve(i,k-1)+solve(k+1,j), MAX, + PAD.
     TEMPLATE:
         solve(i, j):                              // A_i..A_j sab phodne ka MAX coins
             if i > j: return 0                    // base: khaali range (single balloon base NAHI -> wo phoodta)
             if dp[i][j] != -1: return dp[i][j]
             ans = INT_MIN

             // k = i se j KYUN: har balloon ko baari-baari phod-ke try karo -> unme se MAX = answer.
             //   eg {3,1,5,8}: 3 phodo, ya 1, ya 5, ya 8 -> har ek try -> MAX nikaalo.
             //   MCM se ULTA: wahan matrix ke PAIRS the (wall pairs) -> j-1; yahan pair nahi -> k poora i..j.
             for k = i .. j:
                 coins = solve(i,k-1) + solve(k+1,j) + p[i-1]*p[k]*p[j+1]
                 ans = max(ans, coins)
             return dp[i][j] = ans
         // caller: solve(1, n) [n = asli balloon count, pad ke baad].

     DRY-RUN (coin = p[i-1]*p[k]*p[j+1]):   p=[1,3,2,5,1], solve(p1,p3), k=p2 phoda
             val:    1     3     2     5     1
             idx:    p0    p1    p2    p3    p4
                          |LEFT| (k) |RIGHT|
                          (p1)       (p3)
         k=p2 phoda -> range TOOTI:  LEFT = solve(i,k-1) = solve(p1,p1)  |  k = p2  |  RIGHT = solve(k+1,j) = solve(p3,p3)
         phir teeno ka coin JODO:
         LEFT  solve(p1,p1):   1*3*2 = 6     (= p0*p1*p2)   -> p1(3) phoota, bacha:  1 _ 2 5 1
         RIGHT solve(p3,p3):   2*5*1 = 10    (= p2*p3*p4)   -> p3(5) phoota, bacha:  1 _ 2 _ 1
         k=p2  phoota:         1*2*1 = 2     (= p0*p2*p4 = p[i-1]*p[k]*p[j+1])   -> p2(2) phoota, bacha:  1 _ _ _ 1
         uske baad JODO: 6 + 10 + 2 = 18    (MCM me result MULTIPLY hote; yahan coins ADD)

     real: coins/DP interview-classic. interval-DP FAMILY: MCM, burst-balloons, stone-merge, optimal-BST.
```

---

## PATTERN 18 — GREEDY

```

 BROAD IDEA: har step pe ABHI-KA-BEST choice pakdo (local best), aage ki poori planning nahi.
   Kaam karta SIRF jab abhi-ka-best = aage-bhi-best (greedy-choice property); warna DP (jo sab explore kare).
   ★ SIGNATURE: aksar pehle SORT (kisi key pe -- size/finish-time) -> phir ek LINEAR pass me pick/skip.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ ASSIGN COOKIES (LC-455)  = greedy intro (sort + 2-pointer)
 └──────────────────────────────────────────────────────────────
     g[] = bachhe ki GREED, s[] = cookie SIZE. cookie j bachhe i ko khush karti: s[j] >= g[i]. max kitne bachhe khush?
     GREEDY soch: chhoti-greed wale bachhe ko jitni-ZAROORAT-utni (chhoti-se-chhoti jo kaam kare) cookie do
                  -> badi cookies bade-greed walon ke liye bachi rahein.
     dono SORT -> 2-pointer (i=bachha, j=cookie):
        if (s[j] >= g[i])   i++, j++, count++;     // khush -> dono aage
        else                j++;                    // cookie choti -> badi dhoondho
     count = ans.  (s khali -> 0 return)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ JUMP GAME (LC-55)  = greedy REACHABILITY (farthest-reach track)
 └──────────────────────────────────────────────────────────────
     nums[i] = us index se max kitni door kood sakte. index0 se LAST tak pahunch sakte ho? true/false.
     GREEDY soch: har index pe track karo "ab tak sabse DOOR kahan tak pahunch sakta hoon" (maxReach).
        agar koi index i KHUD maxReach se aage nikal gaya -> wahan pahunch hi nahi sakte -> FALSE.
     for i in 0..n-1:
        if (i > maxReach)  return false;            // is index tak reach hi nahi
        maxReach = max(maxReach, i + nums[i]);       // apni pahunch badhao
     return maxReach >= n-1.    O(n), koi DP nahi.
     ★ cookies se FARAK: wahan sort+2-pointer; yahan sort nahi -> ek scan me "farthest reachable" track. greedy ke 2 alag rang.

 ┌──────────────────────────────────────────────────────────────
 │ ▸ NON-OVERLAPPING INTERVALS (LC-435)  = greedy interval-scheduling (keep SMALLER-END)
 └──────────────────────────────────────────────────────────────
     intervals me se MIN kitne HATAO taaki koi overlap na bache? (= max non-overlapping rakho.)
     GREEDY: default SORT. 2-pointer -> start=rakha-hua, end=candidate.
        NO-overlap (start.end <= end.start):  start = end; end++;       // candidate safe, rakho
        OVERLAP (else):  count++;                                        // ek to hatana hi
                         if (start.end > end.end)  start = end;          // chhota-END wale ko rakho
                         end++;
     count = removals.  WHY chhota-end rakho: aage zyada jagah -> zyada fit.  O(n log n).
     ★ EDGE (dry-run se pakda): overlap pe BLINDLY start rakhna GALAT -> [[1,100],[2,3],[3,4]] pe 2 aata (sahi 1).
       end-compare zaroori. (alt: END pe sort karo -> pehla-waala hamesha chhota-end -> ye if-check gayab.)

 ┌──────────────────────────────────────────────────────────────
 │ ▸ GAS STATION (LC-134)  = greedy running-tank + reset-start
 └──────────────────────────────────────────────────────────────
     N pump CIRCLE me. gas[i]=milta, cost[i]=agle pump tak kharcha. kaha se shuru karu
     ki poora ghoom ke wapas wahin aa jau? koi start possible nahi -> -1.
     har pump ka NET = gas[i] - cost[i]. Ek pass, DO accumulator:
        kyaNegativeHua  += gas[i]-cost[i];   // KABHI reset nahi -> ant me <0 -> -1 (petrol hi kam)
        meriGaddiKaTank += gas[i]-cost[i];   // running; jaha doobta wahan se aage nahi ja sakte
        if (meriGaddiKaTank < 0) { meriGaddiKaTank = 0; index = i+1; }   // start ko AGLE pump pe khiska do
     return  kyaNegativeHua < 0 ? -1 : index;
     ★ KEY: kyaNegativeHua>=0 ho to jo LAST reset-index bacha wahi ANSWER (unique). kyun: jis pump pe
       tank dooba, us-se-pehle kisi bhi start se wahan pahucha ja HI nahi sakta -> start hamesha aage hi.
     ★ cookies/jump/interval se FARAK: yahan sort NAHI. circular + 2-accumulator (feasibility + start-track).
```
