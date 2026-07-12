# DSA PATTERN SHEET — trick per problem (code NAHI, sirf pattern)

> Arpan ka revision-tool (12-Jul se banna shuru). REVISE isi se hoga -> code nahi kholenge.
> "similar Q aaye -> pattern dekho, trick recall karo, laga do."
> broad/high-leverage tricks pehchano -> ek trick, kai problem.

---

## ★★ BROAD-TRICKS INDEX (sabse high-leverage — ek trick, kai problem)
```
   FAST/SLOW pointer     -> middle · detect-cycle · palindrome-LL · remove-nth-from-end (GAP: fast ko n aage bhejo)
   REVERSE (3-pointer)   -> reverse-LL · palindrome-LL
   DUMMY node            -> merge-two-sorted (+ LL insert/build)
   MONOTONIC stack       -> next-greater · daily-temperatures
   PREFIX-SUM + map[sum-k]-> subarray-sum=K
   num-1 START-check      -> longest-consecutive-sequence (O(n))
   BS on ANSWER          -> koko (speed pe search, not array)
   sorted-half-check     -> search in rotated array
   2D-index-map          -> search 2D matrix (row=i/col, col=i%col)
   VARIABLE window (shrink-till-valid, ans unconditional) -> longest-no-repeat · max-consec-ones · char-replace · min-len
   "assign PEHLE, update BAAD" -> pivot-index · product-except-self
   fill-from-END (result peeche se) -> squares-of-sorted-array (bada square end me)
   -> ye 11 broad-tricks yaad -> zyadatar focus-8 problems recognize + solve.
```

---

## PATTERN 1 — TWO POINTER
```
   BROAD IDEA: do pointer (dono taraf se, ya slow-fast) -> ek saath move karke O(n).

   Move Zeroes           -> slow/fast; fast scan kare, non-zero mile to swap -> slow++. [swap(nums[slow], nums[fast]);]
   Valid Palindrome      -> left/right dono taraf se, compare karte andar aao.
   Container Most Water   -> left/right; area = min(h)*width; CHHOTI height wala pointer andar (bottleneck).
   3Sum                  -> ★ pehle SORT [sort(begin(nums),end(nums));] -> ek number fix -> baaki 2 pointer target=-fixed; TRIPLETS; duplicates skip.
   Sort Colors (0,1,2)    -> 3 pointer (low/mid/high): 0->low-swap, 2->high-swap, 1->mid++. (Dutch flag)
   Squares of Sorted Arr  -> ★ negatives ka square order ULTA -> 2 pointer (left,right); dono ke square compare
                             -> BADA wala result ke END me daalo (k=n-1, ghatta); us pointer andar. O(n), no sort.
   Two Sum II (SORTED)    -> ★ sorted -> hashmap ki zaroorat nahi -> 2 pointer (left,right); sum==target -> mila (return 1-based).
                             sum<target -> left++ (bada chahiye); sum>target -> right-- (chhota). O(1) space.
                             (regular two-sum UNSORTED = hashmap; SORTED = 2-pointer.)
```

---

## PATTERN 2 — SLIDING WINDOW
```
   BROAD IDEA: right se EXPAND -> CONDITION ke hisab se left se SHRINK -> ans STORE.
   ★ KEY: ans store HAMESHA condition ke saath NAHI hota. jab shrink guarantee kare window valid -> ans UNCONDITIONAL
          (jaise max-consec-ones). condition sirf SHRINK pe lagti, store pe zaroori nahi.

   Max Sum of K (FIXED window) -> fixed size k window slide; naya add, purana(left) minus -> max track.
   Min Subarray Len (>=target) -> expand jab tak sum>=target -> shrink karke chhoti length -> min track.
   Longest Substring No-Repeat -> set/map me char; repeat aaye to left shrink jab tak repeat gaya -> max length.
   Char Replacement (longest)  -> window me (windowLen - maxFreqChar) <= k valid; warna left shrink.
   Max Consecutive Ones III    -> window me zeros count <= k; zeros>k to left shrink -> ans unconditional.

   ★ 2 type: FIXED window (max-sum-k) · VARIABLE window (baaki).
```

---

## PATTERN 3 — HASHING
```
   BROAD IDEA: hashmap/hashset = O(1) lookup. "pehle dekha?" / "complement hai?" / count -> O(n).

   Two Sum                 -> map me {value: index}; har num pe (target-num) map me hai? -> mila.
   Valid Anagram           -> count-array (26) ya map; ek me ++, doosre me --; sab zero -> anagram.
   Group Anagrams          -> har word ko SORT karo -> wahi key; map<sortedKey, list> me group.
   Subarray Sum = K        -> ★ prefix-sum + map[sum]; map{0:1} se START. [count += mp[sum-k];  // = prefix[j]-prefix[i-1]=k]
   Longest Consecutive Seq -> ★ set me sab; count START tabhi jab (num-1) set me NAHI (sequence start) -> O(n).
```

---

## PATTERN 4 — PREFIX SUM
```
   BROAD IDEA: running sum pehle se rakho -> range/left-right sum O(1) (baar-baar loop nahi).

   Pivot Index          -> total nikaalo; leftSum chalao. ★ order: rightSum=total-leftSum-nums[i]; if(left==right)return i; PHIR leftSum+=nums[i]. (leftSum BAAD me)
   Highest Altitude     -> running sum chalao, max track. (simple prefix)
   Product Except Self  -> 2 pass. ★ ORDER trick: pehle left[i]=prod, PHIR prod*=nums[i] (assign-before-update).
                           left-pass phir right-pass -> ans[i] = left × right. (bina division)

   ★ common trick: "assign PEHLE, update BAAD me" (pivot + product dono me).
```

---

## PATTERN 5 — MATRIX
```
   BROAD IDEA: 2D grid pe index-navigation (boundary/direction careful; copy-pen pe grid banao).

   Set Matrix Zeroes  -> 2 pass: pehle zeroRow[]/zeroCol[] mark; phir jahan row ya col marked -> 0.
   Spiral Matrix      -> 4 boundary (top/bottom/left/right); L->R, T->B, R->L, B->T; har layer ke baad boundary shrink.
                         ★ guard: BOTTOM-row chalane se pehle if(top<=bottom); LEFT-col chalane se pehle if(left<=right).
   Transpose          -> [ans[i][j] = matrix[j][i];] naya matrix col×row size ka.
```

---

## PATTERN 6 — STACK
```
   BROAD IDEA: stack = LIFO. "kholo push, band karo pop-match" · ya MONOTONIC (next-greater/smaller).

   Valid Parentheses    -> opening push; closing aaye to top se match+pop; end me stack KHALI = valid. (closing pe stack empty -> false)
                           ★ match: (topp=='('&&it==')') || (topp=='['&&it==']') || (topp=='{'&&it=='}')
   Min Stack            -> ★ pair{val, minSoFar} push -> getMin = top().second (O(1), koi scan nahi).
   Next Greater Element  -> ★ MONOTONIC stack (index rakho); jab curr > stack-top -> pop + ans[top]=curr.
   Daily Temperatures   -> same monotonic; ans[top] = i - top (VALUE nahi, DISTANCE/din).

   ★ broad-trick: MONOTONIC stack (next-greater + daily-temp = same, bas ans me kya bharo alag).
```

---

## PATTERN 7 — BINARY SEARCH
```
   BROAD IDEA: sorted space -> mid -> aadha kaato -> O(log n). (space array ho ya ANSWER-range).

   Binary Search (basic)   -> mid; target<mid -> left half, warna right. (low<=high, mid±1)
   Search Insert Position  -> basic BS; na mile to low return (wahi insert jagah).
   First & Last Position   -> ★ 2 BS. mila to RUKO MAT -> boundary tak continue:
                              LEFT-most ke liye  -> ans=mid; high=mid-1 (aur left dhoondo).
                              RIGHT-most ke liye -> ans=mid; low=mid+1 (aur right dhoondo).
   Search in Rotated       -> ★ mid; kaunsa half SORTED -> target us sorted half ki RANGE me hai? -> udhar jao.
   Find Min in Rotated     -> ★ mid ko HIGH se compare. [if(nums[mid]>nums[high]) low=mid+1; else high=mid;] -> low pe min.
   Koko Bananas            -> ★ BS on ANSWER (speed k); hours(k)=sum(ceil(pile/k))<=h? -> chhoti k try.
                              ★ ceil: hours += ceil((double)piles[i]/mid);  ya integer: (piles[i]+mid-1)/mid.
                              ★ TRAP: ceil(int/int) BEKAAR (int-div pehle floor). double-cast ya integer-formula use karo.
   Search 2D Matrix        -> ★ poore matrix ko 1D maano (low=0, high=row*col-1). access: matrix[mid/col][mid%col]. (row=mid/col, col=mid%col)

   ★ 3 broad-trick: BS-on-ANSWER (Koko) · sorted-half-check (Rotated) · 2D-index-map (matrix).
```

---

## PATTERN 8 — LINKED LIST
```
   BROAD IDEA: pointer manipulation. 3 broad-trick: FAST/SLOW · REVERSE (3-pointer) · DUMMY node.

   Reverse Linked List  -> ★ 3-pointer: nextt=curr->next; curr->next=prev; prev=curr; curr=nextt. return prev.
   Middle of List       -> ★ FAST/SLOW: slow 1 step, fast 2 step; fast end pe -> slow MIDDLE pe.
   Detect Cycle         -> ★ FAST/SLOW (Floyd): cycle ho to tez-dheema MILENGE (slow==fast) -> true.
   Palindrome LL        -> ★ COMBO: middle nikaalo -> 2nd half REVERSE -> dono half compare. (mid pe list TOOT jaati)
   Merge Two Sorted     -> ★ DUMMY node + tail; chhota wala tail->next; ek khatam -> baaki jod do. ans=dummy->next.
   Remove Nth From End  -> ★ FAST/SLOW GAP: fast ko PEHLE aage bhejo (loop i=1; i<=n). ab fast & slow me n ka gap.
                           ★ agar fast==NULL -> head hi hatana hai -> return head->next.
                           warna dono SAATH chalao (gap constant) jab tak fast->next NULL -> slow "hatane wale ke PEHLE" baith jaata.
                           slow->next us node ko point kar raha -> slow->next = slow->next->next. done.

   ★ 3 high-leverage: fast/slow (middle+cycle+palindrome+remove-nth) · reverse (reverse+palindrome) · dummy-node (merge).
```

---
> REVISE isi sheet se. code nahi kholना. "similar Q -> pattern pehchano -> trick recall -> lagao."
