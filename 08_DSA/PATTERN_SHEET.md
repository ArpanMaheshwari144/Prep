# DSA PATTERN SHEET — trick per problem (code NAHI, sirf pattern)

> Arpan ka revision-tool (12-Jul se banna shuru). REVISE isi se hoga -> code nahi kholenge.
> "similar Q aaye -> pattern dekho, trick recall karo, laga do."
> broad/high-leverage tricks pehchano -> ek trick, kai problem.

---

## PATTERN 1 — TWO POINTER
```
   BROAD IDEA: do pointer (dono taraf se, ya slow-fast) -> ek saath move karke O(n).

   Move Zeroes           -> slow/fast; fast scan kare, non-zero mile to slow<->fast SWAP -> slow++. (zeros peeche)
   Valid Palindrome      -> left/right dono taraf se, compare karte andar aao.
   Container Most Water   -> left/right; area = min(h)*width; CHHOTI height wala pointer andar (bottleneck).
   3Sum                  -> sort -> ek number fix -> baaki 2 pointer target=-fixed; duplicates skip.
   Sort Colors (0,1,2)    -> 3 pointer (low/mid/high): 0->low-swap, 2->high-swap, 1->mid++. (Dutch flag)
```

---

## PATTERN 2 — SLIDING WINDOW
```
   BROAD IDEA: window [left..right]. right se EXPAND -> condition toote to left se SHRINK-till-valid -> ans record.
   ★ KEY: shrink ke baad window HAMESHA valid -> ans UNCONDITIONAL update (koi extra if nahi).

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
   Subarray Sum = K        -> ★ prefix-sum + map[sum]; har step (sum-k) map me? count add. map{0:1} se START.
   Longest Consecutive Seq -> ★ set me sab; count START tabhi jab (num-1) set me NAHI (sequence start) -> O(n).
```

---

## PATTERN 4 — PREFIX SUM
```
   BROAD IDEA: running sum pehle se rakho -> range/left-right sum O(1) (baar-baar loop nahi).

   Pivot Index          -> total nikaalo; leftSum chalao; right = total-leftSum-nums[i]; ★ compare BEFORE leftSum+=nums[i].
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
                         ★ bottom-row & left-col loop me guard: if(top<=bottom) / if(left<=right).
   Transpose          -> ans[i][j] = matrix[j][i]; naya matrix col×row size ka.
```

---

## PATTERN 6 — STACK
```
   BROAD IDEA: stack = LIFO. "kholo push, band karo pop-match" · ya MONOTONIC (next-greater/smaller).

   Valid Parentheses    -> opening push; closing aaye to top se match+pop; end me stack KHALI = valid.
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
   First & Last Position   -> 2 BS: ek left-most (mila to high=mid-1 continue), ek right-most.
   Search in Rotated       -> ★ mid; kaunsa half SORTED -> target us sorted half ki RANGE me hai? -> udhar jao.
   Find Min in Rotated     -> mid vs right; mid>right -> min right-half (low=mid+1), warna left (high=mid).
   Koko Bananas            -> ★ BS on ANSWER (speed k); hours(k)=sum(ceil(pile/k))<=h? -> chhoti k try. (ceil: (a+k-1)/k)
   Search 2D Matrix        -> ★ poore matrix ko 1D maano; index -> row=mid/col, col=mid%col.

   ★ 3 broad-trick: BS-on-ANSWER (Koko) · sorted-half-check (Rotated) · 2D-index-map (matrix).
```
