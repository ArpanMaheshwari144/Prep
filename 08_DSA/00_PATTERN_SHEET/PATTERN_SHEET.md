# DSA PATTERN SHEET — trick per problem (code NAHI, sirf pattern)

> Arpan ka revision-tool (12-Jul se banna shuru). REVISE isi se hoga -> code nahi kholenge.
> "similar Q aaye -> pattern dekho, trick recall karo, laga do."
> broad/high-leverage tricks pehchano -> ek trick, kai problem.
> ★ FAMILY-WISE: har pattern ke andar same-trick problems ek DAABE (family) me. family ke upar "KYUN saath" likha. content waisa ka waisa.

---

## ★★ BROAD-TRICKS INDEX (sabse high-leverage — ek trick, kai problem)
```
   FAST/SLOW pointer      -> middle · detect-cycle · palindrome-LL · remove-nth-from-end (GAP: fast ko n aage bhejo)
   REVERSE (3-pointer)    -> reverse-LL · palindrome-LL
   DUMMY node             -> merge-two-sorted (+ LL insert/build)
   MONOTONIC stack        -> next-greater · daily-temperatures · next/prev-smaller
   NEAREST-SMALLER/GREATER BOUNDARY (monotonic) -> har element ka prev+next smaller/greater se boundary -> uska CONTRIBUTION (width/count/span):
                            largest-rectangle-histogram (width) · sum-of-subarray-minimums (count) · maximal-rectangle-2D (per-row histogram) · stock-span (prev-greater). [ek weapon, kai problem]
   PREFIX-SUM + map[sum-k] -> subarray-sum=K
   num-1 START-check       -> longest-consecutive-sequence (O(n))
   BS on ANSWER           -> koko (min speed) · ship (min capacity) · split-array (min largest-sum) [teeno same shape: low/high range + solve(mid) count-check]
   sorted-half-check      -> search in rotated array
   2D-index-map           -> search 2D matrix (row=i/col, col=i%col)
   VARIABLE window (shrink-till-valid, ans unconditional) -> longest-no-repeat · max-consec-ones · char-replace · min-len
   "assign PEHLE, update BAAD" -> pivot-index · product-except-self
   fill-from-END (result peeche se) -> squares-of-sorted-array · merge-sorted-array (bada element end me)
   count += window-size (subarray COUNT) -> subarray-product<k (j pe end hone wale saare valid)
   -> ye broad-tricks yaad -> zyadatar focus-8 problems recognize + solve.
```

---

## ★ NAAM-SE-CONFUSE MAT HO (Container vs Trapping vs Histogram — teeno ALAG!)
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

   ★ 1-line: Container = 2-deewar-BEECH-max-paani · Trapping = har-gaddha-SUM · Histogram = SOLID-block-MAX (paani nahi).
   ★ bada farak: Container/Trapping = PAANI · Histogram = SOLID.   Trapping = SUM · baaki dono = MAX.
```

---

```
═══════════════════ PATTERN 1 — TWO POINTER ═══════════════════
 BROAD IDEA: do pointer (dono taraf se, ya slow-fast) -> ek saath move karke O(n).

┌── FAMILY: slow/fast OVERWRITE ────────────────────────────────
│ KYUN SAATH: fast pura array scan karta; jo element "rakhna" hai use slow pe likh do + slow++; baaki apne aap peeche reh jaate. teeno same skeleton (move-zeroes cousins).
└───────────────────────────────────────────────────────────────
 ▸ MOVE ZEROES ─────────────────────────────────────────────────
     slow/fast; fast scan kare, non-zero mile to swap -> slow++. [swap(nums[slow], nums[fast]);]

 ▸ REMOVE DUP (SORTED) ─────────────────────────────────────────
     ★ move-zeroes cousin. slow=last-unique index, fast=scan. nums[slow]!=nums[fast] (naya unique)
     -> slow++ (slow ab ek DUPLICATE pe, khali nahi), phir nums[slow]=nums[fast] (dupe ko OVERWRITE).
     fast har baar++. unique count = slow+1 (0-based +1).

 ▸ REMOVE ELEMENT ──────────────────────────────────────────────
     ★ saare val hata, non-val ki count k lauta (move-zeroes cousin). slow/fast.
     nums[fast]!=val -> nums[slow]=nums[fast], slow++.  ==val -> sirf fast++ (skip).
     end me slow = k (bache shuru ke k me).

┌── FAMILY: left/right COMPARE-CONVERGE ────────────────────────
│ KYUN SAATH: do pointer dono chhor se andar aate; har step CHAR/condition compare karke decide. (palindrome-type + vowel-swap.)
└───────────────────────────────────────────────────────────────
 ▸ VALID PALINDROME (SIMPLE) ───────────────────────────────────
     left/right dono taraf se, compare karte andar aao. (sirf lowercase-letters wala aasan version.)

 ▸ VALID PALINDROME (FULL, LC-125) ─────────────────────────────
     ★ mixed chars (letter/digit/space/punct, UPPER+lower). SIRF alnum consider · case IGNORE. helpers: isalnum, tolower (<cctype>).
     2 pointer left/right, ORDER:
       1. tolower(s[left])==tolower(s[right]) -> match -> left++ right--.
       2. else !isalnum(s[left])  -> left non-alnum -> skip -> left++.
       3. else !isalnum(s[right]) -> right non-alnum -> skip -> right--.
       4. else (dono alnum PAR alag) -> return FALSE.
     bina false cross -> TRUE. (koi alnum na ho / khaali -> true.)

 ▸ VALID PALINDROME II ─────────────────────────────────────────
     ★ = palindrome YA ek char DELETE karke palindrome ban jaaye.
     IDEA: two-pointer; MISMATCH pe -> ya LEFT char delete ya RIGHT char delete. koi ek bane -> true, dono na bane -> false.
     v1 (BEKAAR/verbose): 2 poore pass. pass-1 = LEFT skip (flag, 1 baar); doosra mismatch -> BREAK
         (false NAHI -> right-skip try baaki -> pass-2). pass-1 ke baad palindrome nikla ("aba") -> return true.
         pass-2 = RESET, RIGHT skip; doosra mismatch -> return FALSE (dono option exhaust).
     v2 (CLEAN/YEHI likhna): helper isPalindrome(s,l,r). main loop; mismatch pe seedha ->
         return isPalindrome(s,l+1,r) || isPalindrome(s,l,r-1);   (left-delete YA right-delete; || = ek se bhi bane -> true)
         koi mismatch na aaye -> already palindrome -> true.

 ▸ REVERSE VOWELS ──────────────────────────────────────────────
     ★ sirf vowels (a/e/i/o/u, lower+upper) aapas me reverse, baaki char apni jagah.
     chhota helper isVowel(ch). left/right pointer:
     dono vowel -> swap + left++ right--.  left vowel nahi -> left++.  warna right--.

┌── FAMILY: left/right by-VALUE decision ───────────────────────
│ KYUN SAATH: sorted-ish / value-based; sum ya area ki VALUE dekh ke decide karo konsa pointer chalana. (koi target-jodi dhoondhna.)
└───────────────────────────────────────────────────────────────
 ▸ CONTAINER MOST WATER ────────────────────────────────────────
     left/right; area = min(h)*width; CHHOTI height wala pointer andar (bottleneck).

 ▸ TWO SUM II (SORTED) ─────────────────────────────────────────
     ★ sorted -> hashmap ki zaroorat nahi -> 2 pointer (left,right); sum==target -> mila (return 1-based).
     sum<target -> left++ (bada chahiye); sum>target -> right-- (chhota). O(1) space.
     (regular two-sum UNSORTED = hashmap; SORTED = 2-pointer.)

 ▸ SUM OF SQUARE NUMBERS ───────────────────────────────────────
     ★ a²+b²==c possible? (LC 633, Google). ★ RECOGNITION: 2 NUMBER (pair) chahiye -> TWO-POINTER.
     (BS nahi -- BS=single/direction dhundhna; JODI chahiye to two-pointer, dono taraf se converge.)
     high=sqrt(c). ans=low²+high²: ==c->true · <c->low++ · >c->high--. low>high->false.
     ★★ c=0 case dono cheez pakadta (high=sqrt(0)=0): (1) low=0 (not 1, warna loop skip -> 0+0 miss) (2) while low<=high (NOT low<high -- low==high==0 wala single-point bhi check ho).
     ★ OVERFLOW: ans ko LONG LONG rakho (c up to 2^31-1).

┌── FAMILY: fill-from-END ──────────────────────────────────────
│ KYUN SAATH: result ko PEECHE se bharo (aakhir me jagah khali). bada element end me daalte hue 2/3 pointer.
└───────────────────────────────────────────────────────────────
 ▸ SQUARES OF SORTED ARR ───────────────────────────────────────
     ★ negatives ka square order ULTA -> 2 pointer (left,right); dono ke square compare
     -> BADA wala result ke END me daalo (k=n-1, ghatta); us pointer andar. O(n), no sort.

 ▸ MERGE SORTED ARRAY ──────────────────────────────────────────
     ★ in-place, PEECHE se bharo (aakhir me zeros = jagah). 3 ptr: i=m-1, j=n-1, k=m+n-1 (NA m*n-1).
     while(i>=0 && j>=0): BADA nums1[k] me daalo, us ptr+k ghatao. ★ && (|| nahi -> ek khatam = out-of-bounds crash).
     leftover nums2 (j) neeche while-loop se bhar do. (pattern: main-loop && + tail-loops)

┌── FAMILY: PARTITION / swap-in-place ──────────────────────────
│ KYUN SAATH: elements ko category (0/1/2 ya even/odd) me baanto; galat-jagah pe swap. left/mid/high ya left/right pointer.
└───────────────────────────────────────────────────────────────
 ▸ SORT COLORS (0,1,2) ─────────────────────────────────────────
     ★ Dutch flag, 3 pointer (low/mid/high):
     nums[mid]==0 -> swap(low,mid), low++ AUR mid++ (dono).
     nums[mid]==1 -> sirf mid++.
     nums[mid]==2 -> swap(mid,high), high-- (★ mid++ NAHI -> high se aaya element unknown, dobara check).

 ▸ SORT ARRAY BY PARITY ────────────────────────────────────────
     ★ evens pehle, odds baad (aapas order koi bhi). left/right pointer.
     left ODD && right EVEN (dono galat jagah) -> swap + left++ right--.
     left EVEN (sahi) -> left++.  warna -> right--.
     ★ TRAP: left even && right odd pe swap MAT karo -> dono already sahi jagah.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────
 ▸ 3SUM ────────────────────────────────────────────────────────
     ★ pehle SORT [sort(begin(nums),end(nums));] -> ek number fix -> baaki 2 pointer target=-fixed; TRIPLETS; duplicates skip.

 ▸ IS SUBSEQUENCE ──────────────────────────────────────────────
     ★ s, t ki subsequence hai kya (order same, beech ke char skip allowed). 2 pointer: i for s, j for t.
     s[i]==t[j] -> i++ AUR j++ (char mila).  nahi mila -> sirf j++ (t aage scan).
     ANT: i >= s.size() -> saare s-char order me mil gaye -> TRUE. warna FALSE.

 ▸ TRAPPING RAIN WATER (LC-42) ─────────────────────────────────
     ★ ek bar `i` ke upar paani = min(leftMax[i], rightMax[i]) - height[i]. (min kyun: chhoti side se bah jaata.)
     [A] O(n) space: leftMax[] pass-aage · rightMax[] pass-peeche · ans += min(leftMax[i],rightMax[i]) - height[i].
     [B] O(1) two-pointer: left/right + 2 var leftMax/rightMax. if(leftMax<=rightMax) -> REFRESH leftMax pehle,
         phir ans += leftMax-height[left], left++ (else side ulta). ★ refresh-PEHLE-add-baad (warna naya-bada-bar pe negative).
         ★ kyun safe: chhoti-max side ka paani sirf usi side se limit (doosri side oonchi wall already).
     edge: empty -> 0.
```

---

```
═══════════════════ PATTERN 2 — SLIDING WINDOW ═══════════════════
 ★ KAB PEHCHANO: CONTIGUOUS subarray/substring + "longest / shortest / max / min / COUNT" maanga -> SW socho.
 ★ 2 TYPE:
   (a) FIXED window   -> size k DIYA hai. i/j pointer: sum += nums[j]; jab (j-i+1) >= k -> track + left hatao (sum -= nums[i], i++).
   (b) VARIABLE window-> size condition se. TEMPLATE:
          for j in arr:  window me arr[j] add (update tracker)
                         while (INVALID): left[i] hatao (tracker update), i++
                         ans update  (length = j-i+1, ya count += j-i+1)
 ★ KYA TRACK: sum · freq-map (distinct=map.size / char-count) · zeros-count · product · maxFreq.
 ★ KEY: shrink ke BAAD window HAMESHA valid -> ans UNCONDITIONAL (koi if(==k) nahi).
 ★ LENGTH ya COUNT: length -> max(ans, j-i+1) · subarray COUNT -> count += (j-i+1) (window size).
 ★★ SHRINK construct: FIXED window -> if (ek add, ek remove -- size fix). VARIABLE window -> while (jab tak INVALID, ek se zyada bhi nikal sakta).
 [track] · [INVALID kab -> shrink] · [ans]

┌── FAMILY: variable-LONGEST ───────────────────────────────────
│ KYUN SAATH: window expand; jab INVALID ho tab left se shrink (WHILE); ans = MAX length. (kya track alag: freq/maxFreq/zeros/distinct.)
└───────────────────────────────────────────────────────────────
 ▸ LONGEST SUBSTRING NO-REPEAT ─────────────────────────────────
     track: char freq-map; INVALID = repeat (freq>1) -> left shrink; ans = MAX length.

 ▸ CHAR REPLACEMENT (LONGEST) ──────────────────────────────────
     track: freq + maxFreq; INVALID = (windowLen - maxFreq) > k -> left shrink; ans = MAX length.

 ▸ MAX CONSECUTIVE ONES III ────────────────────────────────────
     track: zerosCount; INVALID = zeros > k -> left shrink; ans = MAX length (UNCONDITIONAL).

 ▸ FRUIT INTO BASKETS ──────────────────────────────────────────
     ★ = "Longest Subarray with AT MOST 2 DISTINCT elements" (fruit/basket sirf kahani). track: map<type,count>;
     INVALID = mp.size()>2 -> shrink (count--, ★ count==0 -> map.ERASE warna size galat); ans = MAX length.

 ▸ LONGEST AT-MOST-K DISTINCT ──────────────────────────────────
     ★ FRUIT-INTO-BASKETS ka GENERAL version. wahi variable window + map<char,count>,
     distinct (mp.size()) > k ho -> left se shrink (erase-on-0). ans = max length.
     farak: fruit me "2" hardcoded tha, yahan wahi jagah "k" (mp.size() > k).

┌── FAMILY: variable-SHORTEST ──────────────────────────────────
│ KYUN SAATH: ULTA — jab VALID ho tab shrink (WHILE) karke chhota karo; ans = MIN length.
└───────────────────────────────────────────────────────────────
 ▸ MIN SUBARRAY LEN (>=TARGET) ─────────────────────────────────
     ★ SHORTEST type (ULTA): track sum; VALID (sum>=target) hote hi shrink-WHILE-valid -> ans = MIN length.

┌── FAMILY: FIXED window ───────────────────────────────────────
│ KYUN SAATH: window ka size k pehle se DIYA hai; slide karte jao.
└───────────────────────────────────────────────────────────────
 ▸ MAX SUM OF K (FIXED) ────────────────────────────────────────
     i/j pointer (baaki SW jaisa): sum += nums[j]. ★ IF (j-i+1) >= k (WHILE nahi -- fixed size, ek hi remove) ->
     maxSum = max(maxSum, sum); phir left hatao (sum -= nums[i], i++). window k-size pe slide. ans = maxSum.

┌── FAMILY: COUNT (length nahi) ────────────────────────────────
│ KYUN SAATH: subarray GINTI chahiye (length nahi); valid window pe count += (j-i+1).
└───────────────────────────────────────────────────────────────
 ▸ SUBARRAY PRODUCT < K ────────────────────────────────────────
     ★ COUNT (length nahi). track: prod; INVALID = prod>=k -> prod/=nums[i], i++.
     ★★ COUNT TRICK: valid -> count += (j-i+1) = window size (j pe end hone wale saare valid). (bahut count-Q me)

┌── FAMILY: ANAGRAM-window ─────────────────────────────────────
│ KYUN SAATH: fixed p-length window + har position pe anagram-check (isAnagram helper reuse).
└───────────────────────────────────────────────────────────────
 ▸ FIND ALL ANAGRAMS ───────────────────────────────────────────
     ★ s me p ke SAARE anagram ki start-index. ANAGRAM-check + FIXED window ka combo.
     helper isAnagram (valid-anagram reuse): same length + har char ka count barabar.
     i/j se p ki length ka window; size == p.size() pe -> substr(i,len) p se anagram? -> ans.push(i).
     size > p.size() -> i++. j har baar++.

 ▸ PERMUTATION IN STRING ───────────────────────────────────────
     ★ s2 me s1 ka koi permutation (anagram) substring hai? = FIND-ALL-ANAGRAMS wahi.
     farak: index push karne ki jagah -> ek match mile to seedha TRUE (koi na mile -> FALSE).
     checkInclusion(s1,s2) = findAnagrams(s2, s1) [return true on first match].

 ★★ LONGEST vs SHORTEST (yaad rakh): LONGEST -> shrink jab INVALID, ans=MAX. SHORTEST (min-len) -> shrink jab VALID, ans=MIN.
```

---

```
═══════════════════ PATTERN 3 — HASHING ═══════════════════
 BROAD IDEA: hashmap/hashset = O(1) lookup. "pehle dekha?" / "complement hai?" / count -> O(n).

┌── FAMILY: 2-MAP BIJECTION ────────────────────────────────────
│ KYUN SAATH: dono taraf ki mapping (mp1 + mp2); clash dono side check; koi ek clash -> false (||). (one map kaafi nahi.)
└───────────────────────────────────────────────────────────────
 ▸ ISOMORPHIC STRINGS ──────────────────────────────────────────
     ★ 2 MAP trick (dono taraf): mp1=s->t, mp2=t->s. (ek map kaafi nahi -- "ab"->"aa" clash sirf ULTI-taraf se pakadta.)
     ★ MAP-CHECK idiom (har map-Q me): mp.count(key)==1 (key hai?) && mp[key]!=val (par value alag?) -> CLASH -> false.
     ★ || KYUN: mp1-side clash YA mp2-side clash -> koi ek bhi -> false (isliye OR, && nahi).
     loop s/t saath; koi bhi taraf clash -> false; warna dono map me jodo (mp1[s]=t, mp2[t]=s). end -> true.

 ▸ WORD PATTERN ────────────────────────────────────────────────
     ★ ISOMORPHIC ka bhai -- wahi 2-map bijection, bas char<->STRING (word). s ko stringstream(s)+while(ss>>word) se todo.
     pattern pe for-loop NAHI (words drive) -> counter i: if(i<pattern.size()){ map + i++ }. mapping = char<->word clash-check dono taraf (|| , same isomorphic).
     ★ EDGE: words ki ginti (j) == pattern.size() honi chahiye (warna "a","dog cat" galat true). end me pattern.size()==j -> true.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────
 ▸ TWO SUM ─────────────────────────────────────────────────────
     map me {value: index}; har num pe (target-num) map me hai? -> mila.

 ▸ VALID ANAGRAM ───────────────────────────────────────────────
     count-array (26) ya map; ek me ++, doosre me --; sab zero -> anagram.

 ▸ GROUP ANAGRAMS ──────────────────────────────────────────────
     har word ko SORT karo -> wahi key; map<sortedKey, list> me group.

 ▸ SUBARRAY SUM = K ────────────────────────────────────────────
     ★ prefix-sum + map[sum]; map{0:1} se START. [count += mp[sum-k];  // = prefix[j]-prefix[i-1]=k]

 ▸ LONGEST CONSECUTIVE SEQ ─────────────────────────────────────
     ★ set me sab; count START tabhi jab (num-1) set me NAHI (sequence start) -> O(n).
```

---

```
═══════════════════ PATTERN 4 — PREFIX SUM ═══════════════════
 BROAD IDEA: running sum pehle se rakho -> range/left-right sum O(1) (baar-baar loop nahi).

┌── FAMILY: "assign PEHLE, update BAAD" ────────────────────────
│ KYUN SAATH: current index ke liye pehle result store karo (left/right sum), PHIR running value update. order galat = bug.
└───────────────────────────────────────────────────────────────
 ▸ PIVOT INDEX ─────────────────────────────────────────────────
     total nikaalo; leftSum chalao. ★ order: rightSum=total-leftSum-nums[i]; if(left==right)return i; PHIR leftSum+=nums[i]. (leftSum BAAD me)

 ▸ PRODUCT EXCEPT SELF ─────────────────────────────────────────
     2 pass. ★ ORDER trick: pehle left[i]=prod, PHIR prod*=nums[i] (assign-before-update).
     left-pass phir right-pass -> ans[i] = left × right. (bina division)

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────
 ▸ HIGHEST ALTITUDE ────────────────────────────────────────────
     running sum chalao, max track. (simple prefix)

 ★ common trick: "assign PEHLE, update BAAD me" (pivot + product dono me).
```

---

```
═══════════════════ PATTERN 5 — MATRIX ═══════════════════
 BROAD IDEA: 2D grid pe index-navigation (boundary/direction careful; copy-pen pe grid banao).

┌── (STANDALONE — har ek apni index-navigation) ────────────────
└───────────────────────────────────────────────────────────────
 ▸ SET MATRIX ZEROES ───────────────────────────────────────────
     2 pass: pehle zeroRow[]/zeroCol[] mark; phir jahan row ya col marked -> 0.

 ▸ SPIRAL MATRIX ───────────────────────────────────────────────
     4 boundary (top/bottom/left/right); L->R, T->B, R->L, B->T; har layer ke baad boundary shrink.
     ★ guard: BOTTOM-row chalane se pehle if(top<=bottom); LEFT-col chalane se pehle if(left<=right).

 ▸ TRANSPOSE ───────────────────────────────────────────────────
     [ans[i][j] = matrix[j][i];] naya matrix col×row size ka.
```

---

```
═══════════════════ PATTERN 6 — STACK ═══════════════════
 BROAD IDEA: stack = LIFO. "kholo push, band karo pop-match" · ya MONOTONIC (next-greater/smaller).

┌── FAMILY: MONOTONIC stack ────────────────────────────────────
│ KYUN SAATH: index stack rakho; curr vs stack-top compare -> pop + us index ka ans set. (bas alag: comparison > ya < · ans me value/distance · circular.)
└───────────────────────────────────────────────────────────────
 ▸ NEXT GREATER ELEMENT ────────────────────────────────────────
     ★ MONOTONIC stack (index rakho); jab curr > stack-top -> pop + ans[top]=curr.

 ▸ NEXT SMALLER ELEMENT ────────────────────────────────────────
     ★ Next-Greater ka SAME code, bas condition ULTA: while nums[st.top()] > nums[i] -> pop + ans[top]=nums[i]. (greater me < tha, smaller me >.)
     ★ strictly smaller (> , >= nahi) -> [2,2,2] -> saare -1. baaki (index-stack, push, init -1) sab wahi.

 ▸ PREVIOUS SMALLER ELEMENT ────────────────────────────────────
     ★ Next-Smaller ka MIRROR: SAME code, bas for-loop ULTA (right->left, i=n-1..0). (array peeche se dekho -> "right ka next-smaller" = "left ka previous-smaller".)
     while nums[st.top()] > nums[i] -> pop + ans[top]=nums[i] (curr i popped ke LEFT me -> uska prev-smaller). push(i). left me kuch nahi -> -1.
     ★ prev-smaller + next-smaller = Largest-Rectangle-in-Histogram ke building block.

 ▸ LARGEST RECTANGLE IN HISTOGRAM (LC-84) ──────────────────────
     ★★ MENTAL MODEL: HAR height pe KHADE ho -> dono taraf NEAREST-SMALLER (prev+next) -> width=next-prev-1 -> area=height*width -> MAX. bas.
     ★ prev-smaller + next-smaller ka COMBO. har bar apni OWN height pe rectangle banata -> left prev-smaller tak, right next-smaller tak (dono usse chhote = boundary). sabka area -> MAX.
     width = nS[i] - pS[i] - 1 · area = heights[i]*width · ans = max(ans, area).
     ★ TRICK 1: prev/next-smaller me VALUE nahi, INDEX store karo (width ke liye).
     ★ TRICK 2: next-smaller "koi chhota nahi" -> sentinel = n (right-edge, bar END tak failta), NA -1. (-1 se width negative -> [2,4]: 2-(-1)-1=-1 galat; n=2 se 2-(-1)-1=2 -> area 4 sahi.) prev-smaller "none" = -1 sahi (left boundary).
     ★ "-1" rule: boundary bar rectangle me NAHI (smaller) -> -1 lagta. include hoti -> nahi.
     ★ width=NS-PS-1 KYUN (derive): boundary rectangle me nahi -> bars PS+1 se NS-1 tak. count=last-first+1=(NS-1)-(PS+1)+1=NS-PS-1. (-1 SIRF EK baar, do nahi -- building6: 4-2-1=1 sahi, 4-2-1-1=0 galat.)
     ★ next-GREATER se NAHI: rectangle taller bar ko include karta, shorter pe rukta -> smaller boundary chahiye. (greater tab jab bada element boundary ho.)

 ▸ MAXIMAL RECTANGLE (LC-85, 2D) ───────────────────────────────
     ★ Largest-Rectangle-Histogram ka 2D bhai. HAR ROW ko histogram maano -> us row ke histogram pe largest-rectangle -> saare rows ka MAX.
     heights[] (size = COL): cell 1 -> heights[j]+=1 (build up) · cell 0 -> heights[j]=0 (reset). har row ke baad maxi=max(maxi, largestRectangleArea(heights)).
     ★ heights COL-size ka (har column neeche jaate height accumulate). inner loop j=col, outer i=row.

 ▸ DAILY TEMPERATURES ──────────────────────────────────────────
     same monotonic; ans[top] = i - top (VALUE nahi, DISTANCE/din).

 ▸ NEXT GREATER ELEM II ────────────────────────────────────────
     ★ same monotonic, bas array CIRCULAR. trick: loop 2 BAAR (i: 0..2n-1), access i % n (mod).
     doosre chakkar me aakhri elements ko SHURU ka greater mil jaata (wrap). baaki wahi; na mile -> -1.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────
 ▸ VALID PARENTHESES ───────────────────────────────────────────
     opening push; closing aaye to top se match+pop; end me stack KHALI = valid. (closing pe stack empty -> false)
     ★ match: (topp=='('&&it==')') || (topp=='['&&it==']') || (topp=='{'&&it=='}')

 ▸ MIN STACK ───────────────────────────────────────────────────
     ★ pair{val, minSoFar} push -> getMin = top().second (O(1), koi scan nahi).

 ▸ MIN ADD MAKE VALID ──────────────────────────────────────────
     ★ COUNTER, stack nahi (sirf () hain). 2 counter:
     close_needed = kitne '(' UNMATCHED bache (inke liye ')' chahiye)  ·  open_needed = kitne ')' UNMATCHED aaye (inke liye '(' chahiye).
     '(' aaye -> close_needed++.   else (')' aaye) -> close_needed>0 ? close_needed-- (match) : open_needed++.
     ans = open_needed + close_needed.   ★ trap: match hua ')' open_needed me MAT ginо (nested "(())" = 0).

 ★ broad-trick: MONOTONIC stack (next-greater + daily-temp = same, bas ans me kya bharo alag).
```

---

```
═══════════════════ PATTERN 7 — BINARY SEARCH ═══════════════════
 BROAD IDEA: sorted space -> mid -> aadha kaato -> O(log n). (space array ho ya ANSWER-range).
 ★★ BS ko POORI SORTED array chahiye HI NAHI -- bas ek comparison se "LEFT ya RIGHT (konsa half phenkna)" DECIDE kar paana chahiye.
    sorted-array us decision ka ek common case hai; find-peak (unsorted) me bhi mid vs mid+1 se half discard hota -> BS lagta.
    => asli requirement = "half safely discard karne ka rule", na ki full sorting.

┌── FAMILY: BASIC / BOUNDARY ───────────────────────────────────
│ KYUN SAATH: normal sorted-array BS; target ko mid se compare (low<=high, mid±1). boundary = mila to ruko-mat, ek taraf aur dhoondo.
└───────────────────────────────────────────────────────────────
 ▸ BINARY SEARCH (BASIC) ───────────────────────────────────────
     mid; target<mid -> left half, warna right. (low<=high, mid±1)

 ▸ SEARCH INSERT POSITION ──────────────────────────────────────
     basic BS; na mile to low return (wahi insert jagah).

 ▸ FIRST & LAST POSITION ───────────────────────────────────────
     ★ 2 BS. mila to RUKO MAT -> boundary tak continue:
     LEFT-most ke liye  -> ans=mid; high=mid-1 (aur left dhoondo).
     RIGHT-most ke liye -> ans=mid; low=mid+1 (aur right dhoondo).

┌── FAMILY: ROTATED ────────────────────────────────────────────
│ KYUN SAATH: array rotate hua; ek comparison se pata karo kaunsa half sorted / pivot kis taraf, phir wahi half rakho.
└───────────────────────────────────────────────────────────────
 ▸ SEARCH IN ROTATED ───────────────────────────────────────────
     mid==target return. ★ kaunsa half sorted? nums[low]<=nums[mid] -> LEFT sorted:
     target [nums[low]..nums[mid]] me? -> high=mid-1, warna low=mid+1.
     else RIGHT sorted: target [nums[mid]..nums[high]] me? -> low=mid+1, warna high=mid-1.

 ▸ FIND MIN IN ROTATED ─────────────────────────────────────────
     ★ mid ko HIGH se compare (koi target nahi, khud min dhoondna). while(low<high): if(nums[mid]>nums[high]) low=mid+1; else high=MID; -> return nums[low].
     ★★ high=MID (NOT mid-1): else me mid KHUD min ho sakta (CANDIDATE) -> discard mat karo. isliye while(low<high) (warna infinite-loop). [BUG-CATCH: [3,1,2] high=mid-1 se galat 3 deta tha.]
     (ye wahi high=mid vs high=mid-1 rule -- find-peak jaisa: khud-answer-dhoondh -> high=mid+low<high; target-reject -> high=mid-1+low<=high.)

┌── FAMILY: BS-on-ANSWER ───────────────────────────────────────
│ KYUN SAATH: array pe nahi, ANSWER-range pe BS (low..high = possible answers). solve(mid) feasible? -> feasible to chhota try (high=mid-1), warna bada (low=mid+1). TEENO ka code SAME-TO-SAME.
└───────────────────────────────────────────────────────────────
 ▸ KOKO BANANAS ────────────────────────────────────────────────
     ★ BS on ANSWER (speed k); hours(k)=sum(ceil(pile/k))<=h? -> chhoti k try.
     ★ ceil: hours += ceil((double)piles[i]/mid);  ya integer: (piles[i]+mid-1)/mid.
     ★ TRAP: ceil(int/int) BEKAAR (int-div pehle floor). double-cast ya integer-formula use karo.

 ▸ SHIP WITHIN D DAYS ──────────────────────────────────────────
     ★ BS on ANSWER = CAPACITY (Koko cousin). low=max(weights), high=sum(weights).
     solve(mid): days ginо -> if(sum+w>mid) -> naya din (day++, sum=0); phir sum+=w HAMESHA (dono case me -- current weight naye din me add hota, drop nahi). day<=D? feasible.
     feasible -> ans=mid, high=mid-1 (choti try); warna low=mid+1. (Koko: ceil per pile; yahan: running-load "exceed to naya din" kyunki order fixed)

 ▸ SPLIT ARRAY LARGEST SUM ─────────────────────────────────────
     ★ BS on ANSWER = KOKO/SHIP ka SAME-TO-SAME. "k parts me baanto -> LARGEST part-sum MINIMIZE".
     range: low=max(arr) (bada element akela aayega, tod nahi sakte) · high=sum(arr) (k=1 -> pura ek tukda).
     solve(mid): mid=cap. sum+=nums[i]; sum>mid -> count++, sum=nums[i] (naye tukde me carry). ★ count=1 se START (warna aakhri tukda chhoot jaye). feasible = count<=k.
     feasible -> ans=mid, high=mid-1 (chhota try); warna low=mid+1. while low<=high, return ans.
     ★ sibling: koko (min speed) · ship (min capacity) · split (min largest-sum) -> teeno IDENTICAL shape.

┌── FAMILY: HALF-DISCARD (unsorted / half-property) ────────────
│ KYUN SAATH: array pura sorted nahi, phir bhi ek comparison se aadha safe discard ho jaata (peak: mid vs mid+1 · single: index-parity).
└───────────────────────────────────────────────────────────────
 ▸ FIND PEAK ELEMENT ───────────────────────────────────────────
     ★ NAYA: array SORTED nahi, phir bhi BS (ek comparison se half discard -- full-sorted nahi chahiye).
     mid ko sirf mid+1 se compare: nums[mid] < nums[mid+1] -> chadhaai -> peak RIGHT -> low=mid+1.
     else (nums[mid] > nums[mid+1]) -> dhalaan -> peak mid-khud/left -> high = mid (NOT mid-1, mid khud peak ho sakta).
     while(low<high), return low. edges auto (bahar -inf -> kinaara khud peak).
     ★ 2 valid version, DONO SAME core (answer HAMESHA return-low se): V1=short (upar, interview-BEST) · V2=explicit peak-check+return-mid+high=mid-1 (extra early-return, optional).
     ★ interview=V1 short: no explicit-check, no edge-sentinel (low<high -> mid+1 hamesha in-bounds). sentinel dena ho to INT_MIN (nums[0] nahi).

 ▸ SINGLE ELEMENT (SORTED) ─────────────────────────────────────
     ★ har element 2x, sirf EK akela. O(log n) BS via INDEX-PARITY (XOR bhi solve karta par O(n) -> yahan log-n chahiye).
     pairs: single se PEHLE (even,odd) index pe shuru; single ke BAAD (odd,even) me SHIFT. single hi ye shift karta.
     while(low<=high); mid nikalo + parity check, nums[mid] ko nums[mid-1] se compare:
       mid EVEN: ==nums[mid-1] -> single PEECHE -> high=mid-1 · else -> low=mid+1
       mid ODD:  !=nums[mid-1] -> single PEECHE -> high=mid-1 · else -> low=mid+1
     return nums[HIGH] (NOT low) -- dono branch mid ko REJECT karte (high=mid-1 / low=mid+1) -> isliye while low<=high; high khud single pe aa ke rukta.
     ★ edge mid==0 -> return nums[0] (baaki sab pair eliminate).

┌── FAMILY: 2D-index-map ───────────────────────────────────────
│ KYUN SAATH: 2D matrix ko 1D sorted array maan ke normal BS; index ko (row,col) me convert.
└───────────────────────────────────────────────────────────────
 ▸ SEARCH 2D MATRIX ────────────────────────────────────────────
     ★ poore matrix ko 1D maano (low=0, high=row*col-1). access: matrix[mid/col][mid%col]. (row=mid/col, col=mid%col)

 ★ 3 broad-trick: BS-on-ANSWER (Koko) · sorted-half-check (Rotated) · 2D-index-map (matrix).
 ★★ high=mid vs high=mid-1 (kab konsa -- ek sawaal: "mid KHUD answer ho sakta abhi bhi?"):
      TARGET-REJECT (mid ko target/condition se compare -> mid pakka galat, e.g. basic-BS/rotated-search: target!=mid) -> high = mid-1  (+ while low<=high)
      KHUD-ANSWER-DHOONDH (koi target nahi, mid KHUD answer ho sakta -- find-PEAK · find-MIN-rotated) -> high = MID  (+ while low<high)
      ★ BUG-CATCH (find-min, 18-Jul): [3,1,2] pe high=mid-1 karte to mid(=1, khud min) HAT jaata -> galat 3 aata. high=mid se bacha.
      ★ trap: high=mid ke saath while(low<=high) = INFINITE LOOP -> high=mid hamesha low<high ke saath.
      ★ KYUN (nuance): mid = low+(high-low)/2 NEECHE (low ki taraf) round karta. jab low==high==mid ho:
          high=mid-1 -> range SHRINK (mid-1) -> low>high -> exit. safe with low<=high.
          high=mid   -> high nahi hilta (mid=low) -> range shrink NAHI -> low<=high hua to ATAK (infinite).
          isliye high=mid ko low<high chahiye (low==high pe TURANT exit, atakne se pehle).
          e.g. [2,1] peak-logic + while(low<=high): mid=0 -> high=mid=0 -> low=high=0 kabhi khatam nahi (loop).
```

---

```
═══════════════════ PATTERN 8 — LINKED LIST ═══════════════════
 BROAD IDEA: pointer manipulation. 3 broad-trick: FAST/SLOW · REVERSE (3-pointer) · DUMMY node.

┌── FAMILY: FAST/SLOW ──────────────────────────────────────────
│ KYUN SAATH: slow 1 step, fast 2 step (ya gap). middle/cycle/palindrome/nth-from-end sab isi 2-speed pe.
└───────────────────────────────────────────────────────────────
 ▸ MIDDLE OF LIST ──────────────────────────────────────────────
     ★ FAST/SLOW: slow 1 step, fast 2 step; fast end pe -> slow MIDDLE pe.

 ▸ DETECT CYCLE ────────────────────────────────────────────────
     ★ FAST/SLOW (Floyd): cycle ho to tez-dheema MILENGE (slow==fast) -> true.

 ▸ PALINDROME LL ───────────────────────────────────────────────
     ★ COMBO: middle nikaalo -> 2nd half REVERSE -> dono half compare. (mid pe list TOOT jaati)

 ▸ REMOVE NTH FROM END ─────────────────────────────────────────
     ★ FAST/SLOW GAP: fast ko PEHLE aage bhejo (loop i=1; i<=n). ab fast & slow me n ka gap.
     ★ agar fast==NULL -> head hi hatana hai -> return head->next.
     warna dono SAATH chalao (gap constant) jab tak fast->next NULL -> slow "hatane wale ke PEHLE" baith jaata.
     slow->next us node ko point kar raha -> slow->next = slow->next->next. done.
 ───────────────────────────────────────────────────────────────
 ▸ REMOVE ELEMENTS (delete by val, LC-203) ─────────────────────
     ★ val wale SAARE node hatao. HEAD bhi hat sakta -> prev/curr traverse + head special-case.
       3 case (curr har baar aage):
         match & HEAD (prev null)  -> head aage (head = head->next).
         match & beech (prev hai)  -> prev->next = curr->next  (curr skip).
         no-match                  -> prev = curr.
     ★★ TRICK: prev sirf NO-MATCH pe aage badhta (removal pe prev=curr MAT -- curr to hata diya). warna consecutive [1,6,6,2] fail.
     (guard: head null -> return null. dummy-node se head-special-case avoid ho sakta.)

┌── FAMILY: DUMMY node ─────────────────────────────────────────
│ KYUN SAATH: fake head (dummy) + tail; naye node rewire/jodte jao; ans = dummy->next. head-edge apne aap handle.
└───────────────────────────────────────────────────────────────
 ▸ MERGE TWO SORTED ────────────────────────────────────────────
     ★ DUMMY node + tail; chhota wala tail->next; ek khatam -> baaki jod do. ans=dummy->next.

 ▸ ADD TWO NUMBERS ─────────────────────────────────────────────
     ★ MERGE jaisa (dummy + tail) + CARRY ka khel. digits ULTA store (units-digit pehle) -> seedha jodo.
     while(a || b || carry):   <- koi list bachi HAI ya carry bacha -> chalo
        sum = 0; a hai to sum += a->val, a=a->next; b hai to sum += b->val, b=b->next; phir sum += carry.
        ★ digit = sum % 10 -> naya node, tail se jodo, tail aage.   ★ carry = sum / 10 -> agle ke liye.
     return dummy->next.   ★ '|| carry' zaroori (999+1=1000 -> end me carry bacha to naya node).

 ▸ SWAP NODES IN PAIRS ─────────────────────────────────────────
     ★ har do adjacent NODE ki jodi swap (values nahi, NODES -- pointer rewire). DUMMY-node trick.
     prev=dummy (head se pehle), first=head. loop jab tak jodi (2 node) bache (prev->next && first->next):
       second=first->next · temp=second->next (BACHA lo warna gum).
       second->next=first (2->1) · first->next=temp (1->baaki) · prev->next=second (dummy->2 = naya head).
       advance: prev=first AUR first=temp. end -> return dummy->next.
     ★ dummy kyu: jodi ko reconnect karne ko "jodi se pehle wala" node chahiye; pehli jodi se pehle kuch nahi -> dummy.
     ★ har iteration = SIRF EK jodi (aage ki jodi ka kaam mat karo).

┌── FAMILY: REVERSE (3-pointer) ────────────────────────────────
│ KYUN SAATH: prev/curr/next se link ulti karo. (reverse khud + palindrome ka 2nd-half.)
└───────────────────────────────────────────────────────────────
 ▸ REVERSE LINKED LIST ─────────────────────────────────────────
     ★ 3-pointer: nextt=curr->next; curr->next=prev; prev=curr; curr=nextt. return prev.

 ★ 3 high-leverage: fast/slow (middle+cycle+palindrome+remove-nth) · reverse (reverse+palindrome) · dummy-node (merge).
```

---

```
═══════════════════ PATTERN 9 — DESIGN (data-structure) ═══════════════════
 BROAD IDEA: data-structure ko khud banao (built-in ke bina) -> array + pointers/nodes se.

 ▸ DESIGN HASHMAP (LLD) ────────────────────────────────────────
     ★★ MENTAL MODEL: ek ARRAY (buckets); har slot me ek LINKED-LIST (chain). buckets[index] = us chain ka HEAD.
        index = hash(key) % cap.  same index pe 2 key (collision) -> chain me jode (next se).
     fields: buckets (Entry* ka array) · cap (slots = 16) · sz (kitne pairs).

     put(key,val):  index nikalo -> chain me key DHUNDO.
                       mil gayi -> value UPDATE karo, return.
                       na mili  -> naya node chain me LAGAO + sz++.
     get(key):      chain traverse -> key match -> value laut.  na mile / null -> -1.
     remove(key):   prev + curr se traverse -> key match pe:
                       HEAD (prev == null) -> buckets[i] = curr->next.
                       beech/end           -> prev->next = curr->next.
                       phir sz--.

     ★ COMBO (yahi naya/tricky tha): buckets[index] khud ek POINTER hai (chain ka head)
       -> isliye array-assign (buckets[i]=X) aur node-link (X->next) MIX ho jaate = array + linked-list.

     VISUAL:                                      Entry = | key | value | next |
        buckets
        ┌────┐
      0 │ ●──┼──> NULL
        ├────┤
      1 │ ●──┼──> | apple | 5 | ●─|──> NULL
        ├────┤
      3 │ ●──┼──> | cat | 9 | ●─|──> | dog | 4 | ●─|──> NULL     <- COLLISION (chain)
        └────┘
        buckets[i] = us chain ka HEAD (ya NULL).

     REMOVE head (prev==null):            REMOVE beech (prev exist):
        buckets[i]->[A]->[B]->NULL           [A]->[B]->[C]->NULL   (remove B)
                    ^curr                          ^prev ^curr
        buckets[i] = curr->next              prev->next = curr->next
        => buckets[i]->[B]->NULL             => [A]->[C]->NULL
     (redo ke liye poora code: file 08_DSA/09_DESIGN/01_hashmap.cpp)

 ▸ DESIGN HASHSET (LLD) ────────────────────────────────────────
     ★ HashMap ka CHHOTA bhai: same bucket-array + chaining, bas VALUE/update nahi. Entry = |key|next|.
     add:      key chain me hai? -> kuch mat karo (duplicate nahi). na ho -> node lagao + sz++.
     contains: chain traverse -> key match -> true, else false. (= HashMap get, bas bool.)
     remove:   prev+curr traverse. HEAD(prev null) -> buckets[i]=curr->next · beech -> prev->next=curr->next. sz--.
     ★★ remove BUG-CATCH: prev ko "prev = curr" karo (NA prev=prev->next -- prev NULL se non-head remove pe CRASH).
     (file 08_DSA/09_DESIGN/02_hashset.cpp)
```

---
> REVISE isi sheet se. code nahi kholna. "similar Q -> pattern pehchano -> trick recall -> lagao."
