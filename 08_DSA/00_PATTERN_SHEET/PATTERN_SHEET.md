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
   MONOTONIC stack        -> next-greater · daily-temperatures
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

```
═══════════════════ PATTERN 1 — TWO POINTER ═══════════════════
 BROAD IDEA: do pointer (dono taraf se, ya slow-fast) -> ek saath move karke O(n).

┌── FAMILY: slow/fast OVERWRITE ────────────────────────────────
│ KYUN SAATH: fast pura array scan karta; jo element "rakhna" hai use slow pe likh do + slow++; baaki apne aap peeche reh jaate. teeno same skeleton (move-zeroes cousins).
└───────────────────────────────────────────────────────────────
 • Move Zeroes
     slow/fast; fast scan kare, non-zero mile to swap -> slow++. [swap(nums[slow], nums[fast]);]
 ───────────────────────────────────────────────────────────────
 • Remove Dup (sorted)
     ★ move-zeroes cousin. slow=last-unique index, fast=scan. nums[slow]!=nums[fast] (naya unique)
     -> slow++ (slow ab ek DUPLICATE pe, khali nahi), phir nums[slow]=nums[fast] (dupe ko OVERWRITE).
     fast har baar++. unique count = slow+1 (0-based +1).
 ───────────────────────────────────────────────────────────────
 • Remove Element
     ★ saare val hata, non-val ki count k lauta (move-zeroes cousin). slow/fast.
     nums[fast]!=val -> nums[slow]=nums[fast], slow++.  ==val -> sirf fast++ (skip).
     end me slow = k (bache shuru ke k me).

┌── FAMILY: left/right COMPARE-CONVERGE ────────────────────────
│ KYUN SAATH: do pointer dono chhor se andar aate; har step CHAR/condition compare karke decide. (palindrome-type + vowel-swap.)
└───────────────────────────────────────────────────────────────
 • Valid Palindrome (simple)
     left/right dono taraf se, compare karte andar aao. (sirf lowercase-letters wala aasan version.)
 ───────────────────────────────────────────────────────────────
 • Valid Palindrome (FULL, LC-125)
     ★ mixed chars (letter/digit/space/punct, UPPER+lower). SIRF alnum consider · case IGNORE. helpers: isalnum, tolower (<cctype>).
     2 pointer left/right, ORDER:
       1. tolower(s[left])==tolower(s[right]) -> match -> left++ right--.
       2. else !isalnum(s[left])  -> left non-alnum -> skip -> left++.
       3. else !isalnum(s[right]) -> right non-alnum -> skip -> right--.
       4. else (dono alnum PAR alag) -> return FALSE.
     bina false cross -> TRUE. (koi alnum na ho / khaali -> true.)
 ───────────────────────────────────────────────────────────────
 • Valid Palindrome II
     ★ = palindrome YA ek char DELETE karke palindrome ban jaaye.
     IDEA: two-pointer; MISMATCH pe -> ya LEFT char delete ya RIGHT char delete. koi ek bane -> true, dono na bane -> false.
     v1 (BEKAAR/verbose): 2 poore pass. pass-1 = LEFT skip (flag, 1 baar); doosra mismatch -> BREAK
         (false NAHI -> right-skip try baaki -> pass-2). pass-1 ke baad palindrome nikla ("aba") -> return true.
         pass-2 = RESET, RIGHT skip; doosra mismatch -> return FALSE (dono option exhaust).
     v2 (CLEAN/YEHI likhna): helper isPalindrome(s,l,r). main loop; mismatch pe seedha ->
         return isPalindrome(s,l+1,r) || isPalindrome(s,l,r-1);   (left-delete YA right-delete; || = ek se bhi bane -> true)
         koi mismatch na aaye -> already palindrome -> true.
 ───────────────────────────────────────────────────────────────
 • Reverse Vowels
     ★ sirf vowels (a/e/i/o/u, lower+upper) aapas me reverse, baaki char apni jagah.
     chhota helper isVowel(ch). left/right pointer:
     dono vowel -> swap + left++ right--.  left vowel nahi -> left++.  warna right--.

┌── FAMILY: left/right by-VALUE decision ───────────────────────
│ KYUN SAATH: sorted-ish / value-based; sum ya area ki VALUE dekh ke decide karo konsa pointer chalana. (koi target-jodi dhoondhna.)
└───────────────────────────────────────────────────────────────
 • Container Most Water
     left/right; area = min(h)*width; CHHOTI height wala pointer andar (bottleneck).
 ───────────────────────────────────────────────────────────────
 • Two Sum II (SORTED)
     ★ sorted -> hashmap ki zaroorat nahi -> 2 pointer (left,right); sum==target -> mila (return 1-based).
     sum<target -> left++ (bada chahiye); sum>target -> right-- (chhota). O(1) space.
     (regular two-sum UNSORTED = hashmap; SORTED = 2-pointer.)
 ───────────────────────────────────────────────────────────────
 • Sum of Square Numbers
     ★ a²+b²==c possible? (LC 633, Google). ★ RECOGNITION: 2 NUMBER (pair) chahiye -> TWO-POINTER.
     (BS nahi -- BS=single/direction dhundhna; JODI chahiye to two-pointer, dono taraf se converge.)
     high=sqrt(c). ans=low²+high²: ==c->true · <c->low++ · >c->high--. low>high->false.
     ★★ c=0 case dono cheez pakadta (high=sqrt(0)=0): (1) low=0 (not 1, warna loop skip -> 0+0 miss) (2) while low<=high (NOT low<high -- low==high==0 wala single-point bhi check ho).
     ★ OVERFLOW: ans ko LONG LONG rakho (c up to 2^31-1).

┌── FAMILY: fill-from-END ──────────────────────────────────────
│ KYUN SAATH: result ko PEECHE se bharo (aakhir me jagah khali). bada element end me daalte hue 2/3 pointer.
└───────────────────────────────────────────────────────────────
 • Squares of Sorted Arr
     ★ negatives ka square order ULTA -> 2 pointer (left,right); dono ke square compare
     -> BADA wala result ke END me daalo (k=n-1, ghatta); us pointer andar. O(n), no sort.
 ───────────────────────────────────────────────────────────────
 • Merge Sorted Array
     ★ in-place, PEECHE se bharo (aakhir me zeros = jagah). 3 ptr: i=m-1, j=n-1, k=m+n-1 (NA m*n-1).
     while(i>=0 && j>=0): BADA nums1[k] me daalo, us ptr+k ghatao. ★ && (|| nahi -> ek khatam = out-of-bounds crash).
     leftover nums2 (j) neeche while-loop se bhar do. (pattern: main-loop && + tail-loops)

┌── FAMILY: PARTITION / swap-in-place ──────────────────────────
│ KYUN SAATH: elements ko category (0/1/2 ya even/odd) me baanto; galat-jagah pe swap. left/mid/high ya left/right pointer.
└───────────────────────────────────────────────────────────────
 • Sort Colors (0,1,2)
     ★ Dutch flag, 3 pointer (low/mid/high):
     nums[mid]==0 -> swap(low,mid), low++ AUR mid++ (dono).
     nums[mid]==1 -> sirf mid++.
     nums[mid]==2 -> swap(mid,high), high-- (★ mid++ NAHI -> high se aaya element unknown, dobara check).
 ───────────────────────────────────────────────────────────────
 • Sort Array By Parity
     ★ evens pehle, odds baad (aapas order koi bhi). left/right pointer.
     left ODD && right EVEN (dono galat jagah) -> swap + left++ right--.
     left EVEN (sahi) -> left++.  warna -> right--.
     ★ TRAP: left even && right odd pe swap MAT karo -> dono already sahi jagah.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────
 • 3Sum
     ★ pehle SORT [sort(begin(nums),end(nums));] -> ek number fix -> baaki 2 pointer target=-fixed; TRIPLETS; duplicates skip.
 ───────────────────────────────────────────────────────────────
 • Is Subsequence
     ★ s, t ki subsequence hai kya (order same, beech ke char skip allowed). 2 pointer: i for s, j for t.
     s[i]==t[j] -> i++ AUR j++ (char mila).  nahi mila -> sirf j++ (t aage scan).
     ANT: i >= s.size() -> saare s-char order me mil gaye -> TRUE. warna FALSE.
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
 • Longest Substring No-Repeat
     track: char freq-map; INVALID = repeat (freq>1) -> left shrink; ans = MAX length.
 ───────────────────────────────────────────────────────────────
 • Char Replacement (longest)
     track: freq + maxFreq; INVALID = (windowLen - maxFreq) > k -> left shrink; ans = MAX length.
 ───────────────────────────────────────────────────────────────
 • Max Consecutive Ones III
     track: zerosCount; INVALID = zeros > k -> left shrink; ans = MAX length (UNCONDITIONAL).
 ───────────────────────────────────────────────────────────────
 • Fruit Into Baskets
     ★ = "Longest Subarray with AT MOST 2 DISTINCT elements" (fruit/basket sirf kahani). track: map<type,count>;
     INVALID = mp.size()>2 -> shrink (count--, ★ count==0 -> map.ERASE warna size galat); ans = MAX length.
 ───────────────────────────────────────────────────────────────
 • Longest At-Most-K Distinct
     ★ FRUIT-INTO-BASKETS ka GENERAL version. wahi variable window + map<char,count>,
     distinct (mp.size()) > k ho -> left se shrink (erase-on-0). ans = max length.
     farak: fruit me "2" hardcoded tha, yahan wahi jagah "k" (mp.size() > k).

┌── FAMILY: variable-SHORTEST ──────────────────────────────────
│ KYUN SAATH: ULTA — jab VALID ho tab shrink (WHILE) karke chhota karo; ans = MIN length.
└───────────────────────────────────────────────────────────────
 • Min Subarray Len (>=target)
     ★ SHORTEST type (ULTA): track sum; VALID (sum>=target) hote hi shrink-WHILE-valid -> ans = MIN length.

┌── FAMILY: FIXED window ───────────────────────────────────────
│ KYUN SAATH: window ka size k pehle se DIYA hai; slide karte jao.
└───────────────────────────────────────────────────────────────
 • Max Sum of K (FIXED)
     i/j pointer (baaki SW jaisa): sum += nums[j]. ★ IF (j-i+1) >= k (WHILE nahi -- fixed size, ek hi remove) ->
     maxSum = max(maxSum, sum); phir left hatao (sum -= nums[i], i++). window k-size pe slide. ans = maxSum.

┌── FAMILY: COUNT (length nahi) ────────────────────────────────
│ KYUN SAATH: subarray GINTI chahiye (length nahi); valid window pe count += (j-i+1).
└───────────────────────────────────────────────────────────────
 • Subarray Product < K
     ★ COUNT (length nahi). track: prod; INVALID = prod>=k -> prod/=nums[i], i++.
     ★★ COUNT TRICK: valid -> count += (j-i+1) = window size (j pe end hone wale saare valid). (bahut count-Q me)

┌── FAMILY: ANAGRAM-window ─────────────────────────────────────
│ KYUN SAATH: fixed p-length window + har position pe anagram-check (isAnagram helper reuse).
└───────────────────────────────────────────────────────────────
 • Find All Anagrams
     ★ s me p ke SAARE anagram ki start-index. ANAGRAM-check + FIXED window ka combo.
     helper isAnagram (valid-anagram reuse): same length + har char ka count barabar.
     i/j se p ki length ka window; size == p.size() pe -> substr(i,len) p se anagram? -> ans.push(i).
     size > p.size() -> i++. j har baar++.
 ───────────────────────────────────────────────────────────────
 • Permutation in String
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
 • Isomorphic Strings
     ★ 2 MAP trick (dono taraf): mp1=s->t, mp2=t->s. (ek map kaafi nahi -- "ab"->"aa" clash sirf ULTI-taraf se pakadta.)
     ★ MAP-CHECK idiom (har map-Q me): mp.count(key)==1 (key hai?) && mp[key]!=val (par value alag?) -> CLASH -> false.
     ★ || KYUN: mp1-side clash YA mp2-side clash -> koi ek bhi -> false (isliye OR, && nahi).
     loop s/t saath; koi bhi taraf clash -> false; warna dono map me jodo (mp1[s]=t, mp2[t]=s). end -> true.
 ───────────────────────────────────────────────────────────────
 • Word Pattern
     ★ ISOMORPHIC ka bhai -- wahi 2-map bijection, bas char<->STRING (word). s ko stringstream(s)+while(ss>>word) se todo.
     pattern pe for-loop NAHI (words drive) -> counter i: if(i<pattern.size()){ map + i++ }. mapping = char<->word clash-check dono taraf (|| , same isomorphic).
     ★ EDGE: words ki ginti (j) == pattern.size() honi chahiye (warna "a","dog cat" galat true). end me pattern.size()==j -> true.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────
 • Two Sum
     map me {value: index}; har num pe (target-num) map me hai? -> mila.
 ───────────────────────────────────────────────────────────────
 • Valid Anagram
     count-array (26) ya map; ek me ++, doosre me --; sab zero -> anagram.
 ───────────────────────────────────────────────────────────────
 • Group Anagrams
     har word ko SORT karo -> wahi key; map<sortedKey, list> me group.
 ───────────────────────────────────────────────────────────────
 • Subarray Sum = K
     ★ prefix-sum + map[sum]; map{0:1} se START. [count += mp[sum-k];  // = prefix[j]-prefix[i-1]=k]
 ───────────────────────────────────────────────────────────────
 • Longest Consecutive Seq
     ★ set me sab; count START tabhi jab (num-1) set me NAHI (sequence start) -> O(n).
```

---

```
═══════════════════ PATTERN 4 — PREFIX SUM ═══════════════════
 BROAD IDEA: running sum pehle se rakho -> range/left-right sum O(1) (baar-baar loop nahi).

┌── FAMILY: "assign PEHLE, update BAAD" ────────────────────────
│ KYUN SAATH: current index ke liye pehle result store karo (left/right sum), PHIR running value update. order galat = bug.
└───────────────────────────────────────────────────────────────
 • Pivot Index
     total nikaalo; leftSum chalao. ★ order: rightSum=total-leftSum-nums[i]; if(left==right)return i; PHIR leftSum+=nums[i]. (leftSum BAAD me)
 ───────────────────────────────────────────────────────────────
 • Product Except Self
     2 pass. ★ ORDER trick: pehle left[i]=prod, PHIR prod*=nums[i] (assign-before-update).
     left-pass phir right-pass -> ans[i] = left × right. (bina division)

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────
 • Highest Altitude
     running sum chalao, max track. (simple prefix)

 ★ common trick: "assign PEHLE, update BAAD me" (pivot + product dono me).
```

---

```
═══════════════════ PATTERN 5 — MATRIX ═══════════════════
 BROAD IDEA: 2D grid pe index-navigation (boundary/direction careful; copy-pen pe grid banao).

┌── (STANDALONE — har ek apni index-navigation) ────────────────
└───────────────────────────────────────────────────────────────
 • Set Matrix Zeroes
     2 pass: pehle zeroRow[]/zeroCol[] mark; phir jahan row ya col marked -> 0.
 ───────────────────────────────────────────────────────────────
 • Spiral Matrix
     4 boundary (top/bottom/left/right); L->R, T->B, R->L, B->T; har layer ke baad boundary shrink.
     ★ guard: BOTTOM-row chalane se pehle if(top<=bottom); LEFT-col chalane se pehle if(left<=right).
 ───────────────────────────────────────────────────────────────
 • Transpose
     [ans[i][j] = matrix[j][i];] naya matrix col×row size ka.
```

---

```
═══════════════════ PATTERN 6 — STACK ═══════════════════
 BROAD IDEA: stack = LIFO. "kholo push, band karo pop-match" · ya MONOTONIC (next-greater/smaller).

┌── FAMILY: MONOTONIC stack ────────────────────────────────────
│ KYUN SAATH: index stack rakho; jab curr > stack-top -> pop + us index ka ans set. (bas ans me kya bharo alag: value / distance / circular.)
└───────────────────────────────────────────────────────────────
 • Next Greater Element
     ★ MONOTONIC stack (index rakho); jab curr > stack-top -> pop + ans[top]=curr.
 ───────────────────────────────────────────────────────────────
 • Daily Temperatures
     same monotonic; ans[top] = i - top (VALUE nahi, DISTANCE/din).
 ───────────────────────────────────────────────────────────────
 • Next Greater Elem II
     ★ same monotonic, bas array CIRCULAR. trick: loop 2 BAAR (i: 0..2n-1), access i % n (mod).
     doosre chakkar me aakhri elements ko SHURU ka greater mil jaata (wrap). baaki wahi; na mile -> -1.

┌── (STANDALONE — apni alag trick) ─────────────────────────────
└───────────────────────────────────────────────────────────────
 • Valid Parentheses
     opening push; closing aaye to top se match+pop; end me stack KHALI = valid. (closing pe stack empty -> false)
     ★ match: (topp=='('&&it==')') || (topp=='['&&it==']') || (topp=='{'&&it=='}')
 ───────────────────────────────────────────────────────────────
 • Min Stack
     ★ pair{val, minSoFar} push -> getMin = top().second (O(1), koi scan nahi).
 ───────────────────────────────────────────────────────────────
 • Min Add Make Valid
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
 • Binary Search (basic)
     mid; target<mid -> left half, warna right. (low<=high, mid±1)
 ───────────────────────────────────────────────────────────────
 • Search Insert Position
     basic BS; na mile to low return (wahi insert jagah).
 ───────────────────────────────────────────────────────────────
 • First & Last Position
     ★ 2 BS. mila to RUKO MAT -> boundary tak continue:
     LEFT-most ke liye  -> ans=mid; high=mid-1 (aur left dhoondo).
     RIGHT-most ke liye -> ans=mid; low=mid+1 (aur right dhoondo).

┌── FAMILY: ROTATED ────────────────────────────────────────────
│ KYUN SAATH: array rotate hua; ek comparison se pata karo kaunsa half sorted / pivot kis taraf, phir wahi half rakho.
└───────────────────────────────────────────────────────────────
 • Search in Rotated
     mid==target return. ★ kaunsa half sorted? nums[low]<=nums[mid] -> LEFT sorted:
     target [nums[low]..nums[mid]] me? -> high=mid-1, warna low=mid+1.
     else RIGHT sorted: target [nums[mid]..nums[high]] me? -> low=mid+1, warna high=mid-1.
 ───────────────────────────────────────────────────────────────
 • Find Min in Rotated
     ★ mid ko HIGH se compare (koi target nahi, khud min dhoondna). while(low<high): if(nums[mid]>nums[high]) low=mid+1; else high=MID; -> return nums[low].
     ★★ high=MID (NOT mid-1): else me mid KHUD min ho sakta (CANDIDATE) -> discard mat karo. isliye while(low<high) (warna infinite-loop). [BUG-CATCH: [3,1,2] high=mid-1 se galat 3 deta tha.]
     (ye wahi high=mid vs high=mid-1 rule -- find-peak jaisa: khud-answer-dhoondh -> high=mid+low<high; target-reject -> high=mid-1+low<=high.)

┌── FAMILY: BS-on-ANSWER ───────────────────────────────────────
│ KYUN SAATH: array pe nahi, ANSWER-range pe BS (low..high = possible answers). solve(mid) feasible? -> feasible to chhota try (high=mid-1), warna bada (low=mid+1). TEENO ka code SAME-TO-SAME.
└───────────────────────────────────────────────────────────────
 • Koko Bananas
     ★ BS on ANSWER (speed k); hours(k)=sum(ceil(pile/k))<=h? -> chhoti k try.
     ★ ceil: hours += ceil((double)piles[i]/mid);  ya integer: (piles[i]+mid-1)/mid.
     ★ TRAP: ceil(int/int) BEKAAR (int-div pehle floor). double-cast ya integer-formula use karo.
 ───────────────────────────────────────────────────────────────
 • Ship Within D Days
     ★ BS on ANSWER = CAPACITY (Koko cousin). low=max(weights), high=sum(weights).
     solve(mid): days ginо -> if(sum+w>mid) -> naya din (day++, sum=0); phir sum+=w HAMESHA (dono case me -- current weight naye din me add hota, drop nahi). day<=D? feasible.
     feasible -> ans=mid, high=mid-1 (choti try); warna low=mid+1. (Koko: ceil per pile; yahan: running-load "exceed to naya din" kyunki order fixed)
 ───────────────────────────────────────────────────────────────
 • Split Array Largest Sum
     ★ BS on ANSWER = KOKO/SHIP ka SAME-TO-SAME. "k parts me baanto -> LARGEST part-sum MINIMIZE".
     range: low=max(arr) (bada element akela aayega, tod nahi sakte) · high=sum(arr) (k=1 -> pura ek tukda).
     solve(mid): mid=cap. sum+=nums[i]; sum>mid -> count++, sum=nums[i] (naye tukde me carry). ★ count=1 se START (warna aakhri tukda chhoot jaye). feasible = count<=k.
     feasible -> ans=mid, high=mid-1 (chhota try); warna low=mid+1. while low<=high, return ans.
     ★ sibling: koko (min speed) · ship (min capacity) · split (min largest-sum) -> teeno IDENTICAL shape.

┌── FAMILY: HALF-DISCARD (unsorted / half-property) ────────────
│ KYUN SAATH: array pura sorted nahi, phir bhi ek comparison se aadha safe discard ho jaata (peak: mid vs mid+1 · single: index-parity).
└───────────────────────────────────────────────────────────────
 • Find Peak Element
     ★ NAYA: array SORTED nahi, phir bhi BS (ek comparison se half discard -- full-sorted nahi chahiye).
     mid ko sirf mid+1 se compare: nums[mid] < nums[mid+1] -> chadhaai -> peak RIGHT -> low=mid+1.
     else (nums[mid] > nums[mid+1]) -> dhalaan -> peak mid-khud/left -> high = mid (NOT mid-1, mid khud peak ho sakta).
     while(low<high), return low. edges auto (bahar -inf -> kinaara khud peak).
     ★ 2 valid version, DONO SAME core (answer HAMESHA return-low se): V1=short (upar, interview-BEST) · V2=explicit peak-check+return-mid+high=mid-1 (extra early-return, optional).
     ★ interview=V1 short: no explicit-check, no edge-sentinel (low<high -> mid+1 hamesha in-bounds). sentinel dena ho to INT_MIN (nums[0] nahi).
 ───────────────────────────────────────────────────────────────
 • Single Element (sorted)
     ★ har element 2x, sirf EK akela. O(log n) BS via INDEX-PARITY (XOR bhi solve karta par O(n) -> yahan log-n chahiye).
     pairs: single se PEHLE (even,odd) index pe shuru; single ke BAAD (odd,even) me SHIFT. single hi ye shift karता.
     while(low<=high); mid nikalo + parity check, nums[mid] ko nums[mid-1] se compare:
       mid EVEN: ==nums[mid-1] -> single PEECHE -> high=mid-1 · else -> low=mid+1
       mid ODD:  !=nums[mid-1] -> single PEECHE -> high=mid-1 · else -> low=mid+1
     return nums[HIGH] (NOT low) -- dono branch mid ko REJECT karte (high=mid-1 / low=mid+1) -> isliye while low<=high; high khud single pe aa ke rukता.
     ★ edge mid==0 -> return nums[0] (baaki sab pair eliminate).

┌── FAMILY: 2D-index-map ───────────────────────────────────────
│ KYUN SAATH: 2D matrix ko 1D sorted array maan ke normal BS; index ko (row,col) me convert.
└───────────────────────────────────────────────────────────────
 • Search 2D Matrix
     ★ poore matrix ko 1D maano (low=0, high=row*col-1). access: matrix[mid/col][mid%col]. (row=mid/col, col=mid%col)

 ★ 3 broad-trick: BS-on-ANSWER (Koko) · sorted-half-check (Rotated) · 2D-index-map (matrix).
 ★★ high=mid vs high=mid-1 (kab konsa -- ek sawaal: "mid KHUD answer ho sakta abhi bhi?"):
      TARGET-REJECT (mid ko target/condition se compare -> mid pakka galat, e.g. basic-BS/rotated-search: target!=mid) -> high = mid-1  (+ while low<=high)
      KHUD-ANSWER-DHOONDH (koi target nahi, mid KHUD answer ho sakta -- find-PEAK · find-MIN-rotated) -> high = MID  (+ while low<high)
      ★ BUG-CATCH (find-min, 18-Jul): [3,1,2] pe high=mid-1 karte to mid(=1, khud min) HAT jaata -> galat 3 aata. high=mid se bacha.
      ★ trap: high=mid ke saath while(low<=high) = INFINITE LOOP -> high=mid hamesha low<high ke saath.
      ★ KYUN (nuance): mid = low+(high-low)/2 NEECHE (low ki taraf) round karta. jab low==high==mid ho:
          high=mid-1 -> range SHRINK (mid-1) -> low>high -> exit. safe with low<=high.
          high=mid   -> high nahi hilता (mid=low) -> range shrink NAHI -> low<=high hua to ATAK (infinite).
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
 • Middle of List
     ★ FAST/SLOW: slow 1 step, fast 2 step; fast end pe -> slow MIDDLE pe.
 ───────────────────────────────────────────────────────────────
 • Detect Cycle
     ★ FAST/SLOW (Floyd): cycle ho to tez-dheema MILENGE (slow==fast) -> true.
 ───────────────────────────────────────────────────────────────
 • Palindrome LL
     ★ COMBO: middle nikaalo -> 2nd half REVERSE -> dono half compare. (mid pe list TOOT jaati)
 ───────────────────────────────────────────────────────────────
 • Remove Nth From End
     ★ FAST/SLOW GAP: fast ko PEHLE aage bhejo (loop i=1; i<=n). ab fast & slow me n ka gap.
     ★ agar fast==NULL -> head hi hatana hai -> return head->next.
     warna dono SAATH chalao (gap constant) jab tak fast->next NULL -> slow "hatane wale ke PEHLE" baith jaata.
     slow->next us node ko point kar raha -> slow->next = slow->next->next. done.

┌── FAMILY: DUMMY node ─────────────────────────────────────────
│ KYUN SAATH: fake head (dummy) + tail; naye node rewire/jodte jao; ans = dummy->next. head-edge apne aap handle.
└───────────────────────────────────────────────────────────────
 • Merge Two Sorted
     ★ DUMMY node + tail; chhota wala tail->next; ek khatam -> baaki jod do. ans=dummy->next.
 ───────────────────────────────────────────────────────────────
 • Add Two Numbers
     ★ MERGE jaisa (dummy + tail) + CARRY ka khel. digits ULTA store (units-digit pehle) -> seedha jodo.
     while(a || b || carry):   <- koi list bachi HAI ya carry bacha -> chalo
        sum = 0; a hai to sum += a->val, a=a->next; b hai to sum += b->val, b=b->next; phir sum += carry.
        ★ digit = sum % 10 -> naya node, tail se jodo, tail aage.   ★ carry = sum / 10 -> agle ke liye.
     return dummy->next.   ★ '|| carry' zaroori (999+1=1000 -> end me carry bacha to naya node).
 ───────────────────────────────────────────────────────────────
 • Swap Nodes in Pairs
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
 • Reverse Linked List
     ★ 3-pointer: nextt=curr->next; curr->next=prev; prev=curr; curr=nextt. return prev.

 ★ 3 high-leverage: fast/slow (middle+cycle+palindrome+remove-nth) · reverse (reverse+palindrome) · dummy-node (merge).
```

---
> REVISE isi sheet se. code nahi kholना. "similar Q -> pattern pehchano -> trick recall -> lagao."
