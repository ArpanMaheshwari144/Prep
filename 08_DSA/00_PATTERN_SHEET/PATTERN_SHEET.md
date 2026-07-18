# DSA PATTERN SHEET — trick per problem (code NAHI, sirf pattern)

> Arpan ka revision-tool (12-Jul se banna shuru). REVISE isi se hoga -> code nahi kholenge.
> "similar Q aaye -> pattern dekho, trick recall karo, laga do."
> ★ AB FAMILY-WISE: same-trick problems ek DAABE (family-box) me saath. family header = trick summary. families ke beech separator.

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

 ┌── FAMILY: slow/fast OVERWRITE ─ (fast scan · good-element ko slow pe likho · slow++) ──
 │
 │ • Move Zeroes
 │     fast scan kare, non-zero mile to swap -> slow++. [swap(nums[slow], nums[fast]);]
 │
 │ • Remove Dup (sorted)
 │     slow=last-unique index, fast=scan. nums[slow]!=nums[fast] (naya unique)
 │     -> slow++ (slow ab ek DUPLICATE pe), phir nums[slow]=nums[fast] (dupe OVERWRITE). fast har baar++. count = slow+1.
 │
 │ • Remove Element
 │     saare val hata, non-val ki count k lauta. nums[fast]!=val -> nums[slow]=nums[fast], slow++. ==val -> sirf fast++ (skip).
 │     end me slow = k (bache shuru ke k me).
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: left/right COMPARE-CONVERGE ─ (dono chhor se andar aao, char/condition compare) ──
 │
 │ • Valid Palindrome (simple)
 │     left/right dono taraf se, compare karte andar aao. (sirf lowercase-letters wala aasan version.)
 │
 │ • Valid Palindrome (FULL, LC-125)
 │     ★ mixed chars (letter/digit/space/punct, UPPER+lower). SIRF alnum consider · case IGNORE. helpers: isalnum, tolower (<cctype>).
 │     ORDER: 1. tolower(l)==tolower(r) -> match -> l++ r--.  2. else !isalnum(l) -> l++ (skip).
 │            3. else !isalnum(r) -> r-- (skip).  4. else (dono alnum par alag) -> FALSE. bina false cross -> TRUE.
 │
 │ • Valid Palindrome II (ek delete allowed)
 │     = palindrome YA ek char DELETE karke palindrome. MISMATCH pe -> LEFT-delete YA RIGHT-delete try.
 │     v2 (CLEAN/YEHI): helper isPalindrome(s,l,r). mismatch pe -> return isPalindrome(l+1,r) || isPalindrome(l,r-1);
 │         (|| = ek se bhi bane -> true). koi mismatch na aaye -> already palindrome -> true.
 │     v1 (BEKAAR/verbose): 2 pass, left-skip-flag phir right-skip-flag (pass-1 me doosra mismatch pe BREAK, false nahi).
 │
 │ • Reverse Vowels
 │     sirf vowels aapas me reverse. helper isVowel(ch). dono vowel -> swap + l++ r--. left vowel nahi -> l++. warna r--.
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: left/right by-VALUE decision ─ (sum/area value dekh ke konsa pointer chalana) ──
 │
 │ • Container Most Water
 │     area = min(h)*width; CHHOTI height wala pointer andar (bottleneck).
 │
 │ • Two Sum II (SORTED)
 │     sorted -> hashmap ki zaroorat nahi. sum==target -> mila (1-based). sum<target -> left++ (bada). sum>target -> right-- (chhota). O(1) space.
 │
 │ • Sum of Square Numbers
 │     a²+b²==c? (LC 633). RECOGNITION: 2 NUMBER (pair) -> TWO-POINTER (BS nahi). high=sqrt(c). ans=l²+h²: ==c true · <c l++ · >c h--.
 │     ★★ c=0 case: low=0 (not 1) + while low<=high (single-point low==high==0 bhi check ho). ★ OVERFLOW -> ans LONG LONG.
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: fill-from-END ─ (result PEECHE se bharo, bada element end me) ──
 │
 │ • Squares of Sorted Arr
 │     negatives ka square order ULTA -> 2 pointer; dono ke square compare -> BADA result ke END me (k=n-1, ghatta); us pointer andar. O(n) no sort.
 │
 │ • Merge Sorted Array (in-place)
 │     PEECHE se bharo. 3 ptr: i=m-1, j=n-1, k=m+n-1 (NA m*n-1). while(i>=0 && j>=0): BADA nums1[k] me daalo.
 │     ★ && (|| nahi -> crash). leftover nums2 (j) neeche tail-while se bhar do. (pattern: main-loop && + tail-loops)
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: PARTITION / swap-in-place ─ (elements ko category me baanto) ──
 │
 │ • Sort Colors (0,1,2) — Dutch flag
 │     3 pointer (low/mid/high). nums[mid]==0 -> swap(low,mid), low++ AUR mid++. ==1 -> sirf mid++.
 │     ==2 -> swap(mid,high), high-- (★ mid++ NAHI -> high se aaya element unknown, dobara check).
 │
 │ • Sort Array By Parity
 │     evens pehle, odds baad. left ODD && right EVEN (dono galat) -> swap + l++ r--. left EVEN -> l++. warna r--.
 │     ★ TRAP: left even && right odd pe swap MAT karo -> dono already sahi jagah.
 └────────────────────────────────────────────────────────────

 ┌── (STANDALONE) ──
 │
 │ • 3Sum
 │     ★ pehle SORT -> ek number fix -> baaki 2 pointer target=-fixed; TRIPLETS; duplicates skip.
 │
 │ • Is Subsequence
 │     s, t ki subsequence? 2 pointer i(s)/j(t). s[i]==t[j] -> i++ AUR j++. nahi -> sirf j++ (t scan). ANT: i>=s.size() -> TRUE.
 └────────────────────────────────────────────────────────────
```

---

```
═══════════════════ PATTERN 2 — SLIDING WINDOW ═══════════════════
 ★ KAB: CONTIGUOUS subarray/substring + "longest/shortest/max/min/COUNT" -> SW.
 ★ TEMPLATE (variable): for j: window me arr[j] add -> while(INVALID) left[i] hatao,i++ -> ans update.
 ★ KYA TRACK: sum · freq-map (distinct=map.size) · zeros-count · product · maxFreq.
 ★ KEY: shrink ke BAAD window HAMESHA valid -> ans UNCONDITIONAL (koi if(==k) nahi).
 ★ LENGTH -> max(ans, j-i+1) · COUNT -> count += (j-i+1) (window size).

 ┌── FAMILY: variable-LONGEST ─ (shrink jab INVALID · ans = MAX length) ──
 │
 │ • Longest Substring No-Repeat
 │     track: char freq-map; INVALID = repeat (freq>1) -> left shrink; ans = MAX.
 │
 │ • Char Replacement (longest)
 │     track: freq + maxFreq; INVALID = (windowLen - maxFreq) > k -> shrink; ans = MAX. (maxFreq kabhi ghatate nahi)
 │
 │ • Max Consecutive Ones III
 │     track: zerosCount; INVALID = zeros > k -> shrink; ans = MAX (UNCONDITIONAL). (k=0 apne aap handle)
 │
 │ • Fruit Into Baskets
 │     = "AT MOST 2 DISTINCT". track: map<type,count>; INVALID = mp.size()>2 -> shrink (count--, ★ count==0 -> map.ERASE); ans = MAX.
 │
 │ • Longest At-Most-K Distinct
 │     ★ FRUIT ka GENERAL version. wahi window + map; mp.size() > k -> shrink (erase-on-0). farak: "2" ki jagah "k".
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: variable-SHORTEST ─ (shrink jab VALID · ans = MIN length) ──
 │
 │ • Min Subarray Len (>=target)
 │     ★ ULTA: track sum; VALID (sum>=target) hote hi shrink-WHILE-valid -> ans = MIN length.
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: FIXED window ─ (size k DIYA hai) ──
 │
 │ • Max Sum of K
 │     sum += nums[j]. jab (j-i+1) >= k -> maxSum update, phir left hatao (sum -= nums[i], i++). window k-size pe slide.
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: COUNT (length nahi) ──
 │
 │ • Subarray Product < K
 │     track: prod; INVALID = prod>=k -> prod/=nums[i], i++.
 │     ★★ COUNT TRICK: valid -> count += (j-i+1) = window size (j pe end hone wale saare valid).
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: ANAGRAM-window ─ (fixed p-len window + anagram check) ──
 │
 │ • Find All Anagrams
 │     s me p ke SAARE anagram ki start-index. helper isAnagram (same length + har char count barabar).
 │     size == p.size() -> substr(i,len) anagram? -> ans.push(i). size > p.size() -> i++.
 │
 │ • Permutation in String
 │     = FIND-ALL-ANAGRAMS wahi. farak: index push ki jagah -> ek match -> seedha TRUE. checkInclusion(s1,s2)=findAnagrams(s2,s1).
 └────────────────────────────────────────────────────────────

 ★★ LONGEST vs SHORTEST: LONGEST -> shrink jab INVALID, ans=MAX. SHORTEST -> shrink jab VALID, ans=MIN.
```

---

```
═══════════════════ PATTERN 3 — HASHING ═══════════════════
 BROAD IDEA: hashmap/hashset = O(1) lookup. "pehle dekha?" / "complement?" / count -> O(n).

 ┌── FAMILY: 2-MAP BIJECTION ─ (dono taraf mapping · clash-check dono side · || ) ──
 │
 │ • Isomorphic Strings
 │     mp1=s->t, mp2=t->s. (ek map kaafi nahi -- "ab"->"aa" clash sirf ULTI-taraf se pakadta.)
 │     ★ MAP-CHECK idiom: mp.count(key)==1 (key hai?) && mp[key]!=val (value alag?) -> CLASH.
 │     ★ || KYUN: mp1-side clash YA mp2-side clash -> koi ek bhi -> false (OR, && nahi). warna dono map me jodo. end -> true.
 │
 │ • Word Pattern
 │     ISOMORPHIC ka bhai -- 2-map bijection, bas char<->STRING (word). s ko stringstream+while(ss>>word) se todo.
 │     pattern pe for-loop NAHI (words drive) -> counter i: if(i<pattern.size()){ map + i++ }. clash-check dono taraf (||).
 │     ★ EDGE: words ginti (j) == pattern.size() honi chahiye (warna "a","dog cat" galat true). end me pattern.size()==j.
 └────────────────────────────────────────────────────────────

 ┌── (STANDALONE) ──
 │
 │ • Two Sum
 │     map {value:index}; har num pe (target-num) map me hai? -> mila.
 │
 │ • Valid Anagram
 │     count-array (26) ya map; ek me ++, doosre me --; sab zero -> anagram.
 │
 │ • Group Anagrams
 │     har word ko SORT karo -> wahi key; map<sortedKey, list> me group.
 │
 │ • Subarray Sum = K
 │     ★ prefix-sum + map[sum]; map{0:1} se START. count += mp[sum-k]; (= prefix[j]-prefix[i-1]=k)
 │
 │ • Longest Consecutive Seq
 │     ★ set me sab; count START tabhi jab (num-1) set me NAHI (sequence start) -> O(n).
 └────────────────────────────────────────────────────────────
```

---

```
═══════════════════ PATTERN 4 — PREFIX SUM ═══════════════════
 BROAD IDEA: running sum pehle se -> range/left-right O(1).

 ┌── FAMILY: "assign PEHLE, update BAAD" ──
 │
 │ • Pivot Index
 │     total nikaalo; leftSum chalao. ★ order: rightSum=total-leftSum-nums[i]; if(left==right)return i; PHIR leftSum+=nums[i]. (leftSum BAAD me)
 │
 │ • Product Except Self
 │     2 pass. ★ pehle left[i]=prod, PHIR prod*=nums[i] (assign-before-update). left-pass + right-pass -> ans[i]=left×right. (bina division)
 └────────────────────────────────────────────────────────────

 ┌── (STANDALONE) ──
 │
 │ • Highest Altitude
 │     running sum chalao, max track. (simple prefix)
 └────────────────────────────────────────────────────────────
```

---

```
═══════════════════ PATTERN 5 — MATRIX ═══════════════════
 BROAD IDEA: 2D grid pe index-navigation (boundary/direction careful; copy-pen pe grid banao).

 ┌── (STANDALONE) ──
 │
 │ • Set Matrix Zeroes
 │     2 pass: pehle zeroRow[]/zeroCol[] mark; phir jahan row ya col marked -> 0.
 │
 │ • Spiral Matrix
 │     4 boundary (top/bottom/left/right); L->R, T->B, R->L, B->T; har layer ke baad shrink.
 │     ★ guard: BOTTOM-row se pehle if(top<=bottom); LEFT-col se pehle if(left<=right).
 │
 │ • Transpose
 │     ans[i][j] = matrix[j][i]; naya matrix col×row size ka.
 └────────────────────────────────────────────────────────────
```

---

```
═══════════════════ PATTERN 6 — STACK ═══════════════════
 BROAD IDEA: stack = LIFO. "kholo push, band karo pop-match" · ya MONOTONIC.

 ┌── FAMILY: MONOTONIC stack ─ (index rakho · curr > top -> pop + ans set) ──
 │
 │ • Next Greater Element
 │     jab curr > stack-top -> pop + ans[top]=curr (greater VALUE).
 │
 │ • Daily Temperatures
 │     same monotonic; ans[top] = i - top (VALUE nahi, DISTANCE/din).
 │
 │ • Next Greater Elem II (circular)
 │     same monotonic, array CIRCULAR. trick: loop 2 BAAR (i:0..2n-1), access i % n (mod). wrap se aakhri ko shuru ka greater. na mile -> -1.
 └────────────────────────────────────────────────────────────

 ┌── (STANDALONE) ──
 │
 │ • Valid Parentheses
 │     opening push; closing -> top se match+pop; end me stack KHALI = valid. (closing pe empty -> false)
 │     ★ match: (topp=='('&&it==')') || ('['&&']') || ('{'&&'}')
 │
 │ • Min Stack
 │     ★ pair{val, minSoFar} push -> getMin = top().second (O(1), koi scan nahi).
 │
 │ • Min Add Make Valid
 │     ★ COUNTER, stack nahi. close_needed = unmatched '(' · open_needed = unmatched ')'.
 │     '(' -> close_needed++. else ')' -> close_needed>0 ? close_needed-- : open_needed++. ans = open+close.
 │     ★ trap: match hua ')' open_needed me MAT gino (nested "(())" = 0).
 └────────────────────────────────────────────────────────────
```

---

```
═══════════════════ PATTERN 7 — BINARY SEARCH ═══════════════════
 BROAD IDEA: sorted space -> mid -> aadha kaato -> O(log n). (array ho ya ANSWER-range).
 ★★ BS ko POORI SORTED chahiye HI NAHI -- bas ek comparison se "LEFT ya RIGHT half phenkna" DECIDE ho.
    find-peak (unsorted) me bhi mid vs mid+1 se half discard -> BS lagta. asli requirement = "half safely discard rule".

 ┌── FAMILY: BASIC / BOUNDARY ─ (target ko mid se compare · low<=high · mid±1) ──
 │
 │ • Binary Search (basic)
 │     mid; target<mid -> left half, warna right. (low<=high, mid±1)
 │
 │ • Search Insert Position
 │     basic BS; na mile to low return (wahi insert jagah).
 │
 │ • First & Last Position
 │     ★ 2 BS. mila to RUKO MAT: LEFT-most -> ans=mid; high=mid-1. RIGHT-most -> ans=mid; low=mid+1.
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: ROTATED ─ (kaunsa half sorted / mid-vs-high se pivot) ──
 │
 │ • Search in Rotated
 │     mid==target return. nums[low]<=nums[mid] -> LEFT sorted: target [low..mid] me? high=mid-1 : low=mid+1.
 │     else RIGHT sorted: target [mid..high] me? low=mid+1 : high=mid-1.
 │
 │ • Find Min in Rotated
 │     mid ko HIGH se compare (koi target nahi). while(low<high): nums[mid]>nums[high] -> low=mid+1; else high=MID. return nums[low].
 │     ★★ high=MID (NOT mid-1): else me mid KHUD min ho sakta (CANDIDATE). [BUG-CATCH: [3,1,2] high=mid-1 se galat 3.]
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: BS-on-ANSWER ─ (low/high = answer-range · solve(mid) count-check · feasible->high=mid-1) ──
 │
 │ • Koko Bananas
 │     speed k pe search. hours=sum(ceil(pile/k))<=h? -> chhoti k try.
 │     ★ ceil: (piles[i]+mid-1)/mid ya double-cast. TRAP: ceil(int/int)=floor, bekaar.
 │
 │ • Ship Within D Days
 │     answer=CAPACITY. low=max(w), high=sum(w). solve: sum+w>mid -> naya din (day++,sum=0); sum+=w HAMESHA. day<=D? feasible.
 │
 │ • Split Array Largest Sum
 │     answer=largest-part-sum. low=max, high=sum. solve: sum>mid -> count++, sum=nums[i]. ★ count=1 se START. count<=k? feasible.
 │
 │     ★ teeno IDENTICAL shape: koko=min-speed · ship=min-capacity · split=min-largest-sum. feasible->ans=mid,high=mid-1; warna low=mid+1.
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: HALF-DISCARD (unsorted / half-property) ──
 │
 │ • Find Peak Element
 │     array SORTED nahi. mid ko sirf mid+1 se compare: nums[mid]<nums[mid+1] -> chadhaai -> peak RIGHT -> low=mid+1.
 │     else -> dhalaan -> peak mid-khud/left -> high = mid (NOT mid-1). while(low<high), return low. edges auto (-inf).
 │
 │ • Single Element (sorted)
 │     har element 2x, sirf EK akela. O(log n) via INDEX-PARITY. single se PEHLE (even,odd); BAAD (odd,even) SHIFT.
 │     while(low<=high); nums[mid] ko nums[mid-1] se compare: mid EVEN: ==prev -> single PEECHE -> high=mid-1 · else low=mid+1.
 │        mid ODD: !=prev -> high=mid-1 · else low=mid+1. return nums[HIGH]. ★ edge mid==0 -> return nums[0].
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: 2D-index-map ──
 │
 │ • Search 2D Matrix
 │     ★ poore matrix ko 1D maano (low=0, high=row*col-1). access: matrix[mid/col][mid%col].
 └────────────────────────────────────────────────────────────

 ★★ high=mid vs high=mid-1 (kab konsa -- ek sawaal: "mid KHUD answer ho sakta abhi bhi?"):
      TARGET-REJECT (mid ko target/condition se compare -> mid pakka galat, e.g. basic-BS/rotated-search) -> high = mid-1  (+ while low<=high)
      KHUD-ANSWER-DHOONDH (koi target nahi, mid KHUD answer ho sakta -- find-PEAK · find-MIN-rotated) -> high = MID  (+ while low<high)
      ★ BUG-CATCH (find-min, 18-Jul): [3,1,2] pe high=mid-1 karte to mid(=1, khud min) HAT jaata -> galat 3. high=mid se bacha.
      ★ trap: high=mid ke saath while(low<=high) = INFINITE LOOP -> high=mid hamesha low<high ke saath.
      ★ KYUN: mid = low+(high-low)/2 NEECHE round karta. low==high==mid pe: high=mid-1 -> shrink -> exit (safe low<=high);
          high=mid -> high nahi hilta -> low<=high hua to ATAK (infinite) -> isliye high=mid ko low<high chahiye.
```

---

```
═══════════════════ PATTERN 8 — LINKED LIST ═══════════════════
 BROAD IDEA: pointer manipulation. 3 broad-trick: FAST/SLOW · REVERSE (3-pointer) · DUMMY node.

 ┌── FAMILY: FAST/SLOW ─ (slow 1 step, fast 2 step) ──
 │
 │ • Middle of List
 │     fast/slow; fast end pe -> slow MIDDLE pe.
 │
 │ • Detect Cycle (Floyd)
 │     cycle ho to tez-dheema MILENGE (slow==fast) -> true.
 │
 │ • Palindrome LL
 │     COMBO: middle nikaalo -> 2nd half REVERSE -> dono half compare. (mid pe list TOOT jaati)
 │
 │ • Remove Nth From End (GAP variant)
 │     fast ko PEHLE n aage bhejo (i=1;i<=n). ★ fast==NULL -> head hatana -> return head->next.
 │     warna dono SAATH chalao jab tak fast->next NULL -> slow "hatane wale ke PEHLE". slow->next = slow->next->next.
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: DUMMY node ─ (fake head + tail · rewire) ──
 │
 │ • Merge Two Sorted
 │     dummy + tail; chhota wala tail->next; ek khatam -> baaki jod do. ans=dummy->next.
 │
 │ • Add Two Numbers
 │     dummy+tail + CARRY. while(a || b || carry): sum = a?+ b?+ carry. digit=sum%10 (naya node), carry=sum/10.
 │     ★ '|| carry' zaroori (999+1=1000 -> end me carry -> naya node).
 │
 │ • Swap Nodes in Pairs
 │     har do adjacent NODE swap (pointer rewire). prev=dummy, first=head. loop jab tak (prev->next && first->next):
 │       second=first->next · temp=second->next (BACHA lo). second->next=first · first->next=temp · prev->next=second.
 │       advance: prev=first AUR first=temp. ★ dummy kyu: pehli jodi se pehle koi node nahi. ★ har iter = SIRF EK jodi.
 └────────────────────────────────────────────────────────────

 ┌── FAMILY: REVERSE (3-pointer) ──
 │
 │ • Reverse Linked List
 │     nextt=curr->next; curr->next=prev; prev=curr; curr=nextt. return prev.
 └────────────────────────────────────────────────────────────
```

---
> REVISE isi sheet se. code nahi kholना. "similar Q -> pattern pehchano -> trick recall -> lagao."
