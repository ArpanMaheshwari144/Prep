# THINKING-FIRST — Solved Log

> Active-derive practice. Har problem ka APPROACH jo KHUD derive kiya (code baad mein).
> Yeh tracker + navigator: signal → pattern → approach.

---

## CORE PHILOSOPHY (Arpan ki apni, khud bola 20 Jun) — yeh sabse pehle padho

> **DSA = ~12 core TEMPLATE reuse karo + har problem ka chhota TWIST adapt karo. Bas.**
> Itni saari problem/template koi pagal yaad nahi rakhta — woh impossible + passive hai.
> Sirf ginti ke template OWN karo, baaki har baar twist nikaalo (jo main kar leta hoon).

```
   ~12 CORE TEMPLATES:
   two-pointer · sliding-window · binary-search · slow/fast · prefix-sum ·
   stack(monotonic) · DFS/BFS · heap/top-k · recursion+backtrack · 1D-DP · hashing
```
- Har naya problem = inme se EK template + ek chhota twist.
  (Cycle = slow/fast + meet-check. Middle = slow/fast + return-slow. Same template, alag twist.)
- **Yehi "blank ho jaunga" dar ka jawab:** infinite twists yaad NAHI rakhne — sirf 12 template + twist on-the-spot.
- Template automatic (reflex) ho jaaye → dimaag twist ke liye FREE → problem solve.

---

## REVISION TRACKER — spaced active-recall

> **Kaise:** neeche ka SIGNAL padho, approach DHAK ke **cold recall** karo (bol ke / kaagaz pe). Recall ho gaya → ✓ + gap aage badha. Blank → tabhi peek + dobara.
> **Schedule per problem:** R1 ≈ din 3 · R2 ≈ din 10 · R3 ≈ din 30. Solid → skip aage; blank → reset chhota.
> **Re-code:** sirf `[C]`-marked (coding tricky thi) ko laptop pe **bina dekhe** dobara likho. Baaki sirf **approach-recall** (2-3 min each).
> Box mein date/✓ bhar jab woh review kare.

| #  | Problem                  | Pattern        | R1 | R2 | R3 | re-code? |
|----|--------------------------|----------------|----|----|----|----------|
| 1  | Contains Duplicate       | Hashing        | ✓ 20Jun cold |    |    |          |
| 2  | Valid Anagram            | Hashing        | ✓ 20Jun cold |    |    |          |
| 3  | Two Sum                  | Hashing        | ✓ 20Jun cold |    |    |          |
| 4  | Group Anagrams           | Hashing        |    |    |    |          |
| 5  | Top K Frequent           | Hashing/bucket |    |    |    |          |
| 6  | Product Except Self      | Prefix×Suffix  |    |    |    |          |
| 7  | Valid Palindrome (alnum) | Two Pointer    |    |    |    |          |
| 8  | Valid Sudoku             | Grid           |    |    |    | `[C]` box-index |
| 9  | Rotate Image             | Grid           |    |    |    | `[C]` transpose+rev |
| 10 | Set Matrix Zeroes        | Grid           |    |    |    |          |
| 11 | Spiral Matrix            | Grid           |    |    |    | `[C]` 4-boundary |
| 12 | 3Sum                     | Two Pointer    |    |    |    | `[C]` dedup+2ptr |
| 13 | Longest Repeating Char Repl | Sliding Window |    |    |    | `[C]` maxFreq+window |
| 14 | Permutation in String    | Sliding Window |    |    |    | `[C]` map==+erase |
| 15 | Valid Parentheses        | Stack          |    |    |    | `[C]` pair-match |
| 16 | Min Stack                | Stack (design) |    |    |    | `[C]` pair {val,min} |
| 17 | Evaluate RPN             | Stack          |    |    |    | `[C]` pop b,a + stoi |
| 18 | Daily Temperatures       | Stack (monotonic) |    |    |    | `[C]` next-greater |
| 19 | Binary Search            | Binary Search  |    |    |    | `[C]` mid+1/mid-1 |
| 20 | Search in Rotated Sorted | Binary Search  |    |    |    | `[C]` which-half+range |
| 21 | Find Min in Rotated Sorted | Binary Search |    |    |    | `[C]` mid-vs-end, no target |
| 22 | Koko Eating Bananas      | BS-on-answer   |    |    |    | `[C]` feasible + int-ceil |
| 23 | Search Insert Position   | Binary Search  |    |    |    | `[C]` loop-bound + return low |
| 24 | Middle of Linked List    | LL slow/fast   |    |    |    | `[C]` fast 2x, return slow |
| 25 | Search a 2D Matrix       | Binary Search  |    |    |    | `[C]` 2D->1D, mid/col + mid%col |
| 26 | Linked List Cycle        | LL slow/fast   |    |    |    | (template reuse + meet-twist) |
| 27 | Merge Two Sorted Lists   | LL dummy-node  |    |    |    | `[C]` curr->next=NODE, equal-case |
| 28 | Remove Nth From End      | LL gap+dummy   |    |    |    | `[C]` n-gap, head-edge (fast==NULL) |
| 29 | Reverse Linked List      | LL 3-pointer   |    |    |    | `[C]` prev/curr/next, save-then-flip |
| 30 | Maximum Depth of Tree    | Trees (DFS rec)|    |    |    | base + recurse L/R + combine |
| 31 | Invert Binary Tree       | Trees (DFS rec)|    |    |    | base + swap(L,R) + recurse |
| 32 | Same Tree                | Trees (2-tree) |    |    |    | both-null/one-null base + val + recurse both |
| 33 | Diameter of Binary Tree  | Trees (depth+track) | ✓ 25Jun COLD SOLO |    |    | [C] CLEARED — cold re-code solo, 3 gotchas khud handle (told→derived) |
| 34 | Level Order Traversal    | Trees (BFS queue) |    |    |    | SOLO — queue.size()=level; v-not-cleared bug khud pakda |
| 35 | Binary Tree Right Side View | Trees (BFS twist) |    |    |    | same #34 template; off-by-one i==size-1 nudge se pakda (dry-run nahi) |
| 36 | Zigzag Level Order       | Trees (BFS twist) |    |    |    | SOLO — same #34 template + toggle flag; smart: direct index size-i-1 (no reverse) |
| 37 | Lowest Common Ancestor   | Trees (DFS bubble-up) |    |    |    | SOLO clean (no bug) — base(null/p-q) + combine(L-null->R, R-null->L, both->root) |
| 38 | Count Good Nodes         | Trees (DFS pass-down) |    |    |    | **COLD TEST — genuinely unseen, FULL SOLO** (pattern+key "carry path-max" khud derive, no signal) |
| 39 | Validate BST             | Trees (range window) |    |    |    | range(min,max) narrow; bug "early return true" -> AND of children (nudge se fix) |
| 40 | Kth Smallest in BST      | Trees (inorder=sorted) |    |    |    | SOLO clean — BST inorder=sorted -> v[k-1] (0-based) |
| 41 | Subsets (power set)      | Backtracking   |    |    |    | REP 1 of backtracking — choose/explore/un-choose; nudges (temp=param, 2-call not loop, pop ORDER) |
| 42 | Permutations             | Backtracking (used[]) |    |    |    | REP 2 — structure SOLO (used[]/loop/choose-explore-unchoose); nudge only C++ sizing + placement. Curve improving |
| 43 | Combination Sum          | Backtracking (start+reuse) |    |    |    | **REP 3 — FULL SOLO, zero help, all 3 twists first-try. JP actual OA Q. Curve proof (rep1 nudged→rep3 solo)** |
| 44 | Kth Largest Element       | Heap (size-K min-heap) |    |    |    | SOLO clean — size-K min-heap (size>k pop, top=ans). Last NEW pattern; DSA gate COMPLETE |

> **Daily ~1hr:** 40 min NAYA (active derive) + 20 min REVISION (upar ka recall + 1 cold re-code).
> Sab re-solve NAHI — approach recall sasta, code sirf `[C]` wale.

---

## #1 — Contains Duplicate   (Easy | Hashing)
- **Signal:** "koi value repeat hui?" → Hashing (set/map)
- **Approach (derived):** map mein count store; jab count > 1 → `true`; warna `false`.
- Note: set se bhi ho jaata ("pehle dekha?") — same O(n), code thoda chhota (count nahi).

## #2 — Valid Anagram   (Easy | Hashing)
- **Signal:** "same letters + same counts, bas alag order?" → Hashing (count map)
- **Approach (derived):** s ke char-counts map mein bharo; phir t ke har char pe `count--`;
  end mein **sab 0 → true**, warna `false`.
- Note: different-length / extra-char khud handle ho jaata (counts balance nahi → false).

## #3 — Two Sum (unsorted)   (Easy | Hashing)
- **Signal:** "do numbers ka jod = target, unsorted" → Hashing (complement map)
- **Approach (derived):** map mein value→index store karte jao. Har num pe check: (target − num) map mein hai?
  haan → dono index return; nahi → current ko map mein daal, aage badho.
- **Key:** complement = target − current. Map se O(1) check.

## #4 — Group Anagrams   (Medium | Hashing)
- **Signal:** "anagrams ko saath group karo" → Hashing (sorted-string as key)
- **Approach (derived):** har word ko SORT karo → sorted-string = KEY → `map[key]` mein word push.
  Same anagram → same sorted-key → ek group. Map ki VALUES = answer.
- **Trick:** sorted version = anagrams ki common PEHCHAAN.

## #5 — Top K Frequent Elements   (Medium | Hashing)
- **Signal:** "K sabse zyada baar aane wale" → Hashing (frequency map) + top-k
- **Approach (derived):** map mein frequency (element → count) → phir top-k frequency waale nikaalo.
- **Trick (top-k nikaalna):** frequency-se-sort (O(n log n)) YA **bucket-sort** (index = frequency,
  peeche se k uthao → O(n), interview-favourite).

## #6 — Product of Array Except Self   (Medium | Prefix × Suffix)
- **Signal:** "har point pe baaki SAB ka guna, NO division" → prefix × suffix
- **Approach (derived):** left[] = prefix products → `left[i]=prod; prod*=nums[i]` (pehle store, phir self).
  right[] = suffix products (ulti taraf, same trick). `result[i] = left[i] * right[i]`.
- **Key:** "pehle store (before ka product), phir apna daalo" → isse self exclude hota.
- **Space-opt (FYI):** result[] mein left bharo, phir suffix-VARIABLE se peeche-se multiply → O(1) extra.

## #7 — Valid Palindrome (alnum + case)   (Easy | Two Pointer)
- **Signal:** "aage-se-peeche same? par junk ignore karo" → Two Pointer (opposite ends)
- **Approach (derived):** left=0, right=end. Do haath andar ki taraf.
  **2 faraq simple palindrome se:** (1) `isalnum` nahi → us pointer ko skip (left++/right--, compare mat karo),
  (2) compare `tolower(s[left])` vs `tolower(s[right])` (case ignore). Mismatch → false; sab pass → true.
- **Trick:** built-in `isalnum` / `tolower` — koi helper banane ki zaroorat nahi.

---

## #8 — Valid Sudoku   (Medium | Hashing on grid)
- **Signal:** "har row/col/3x3-box mein digit repeat to nahi?" → set per row/col/box
- **Approach (derived):** har row → set, har col → set, har box → set. Cell repeat (`.` skip) → false; sab pass → true.
- **Box-indexing twist (CRACKED):** box ka corner step `0,3,6` → `board[boxRow+r][boxCol+c]` (corner + andar ka 0..2 offset). 4 nested loop.
- **Meta:** raat ko "impossible" laga (thakaan) → fresh dimaag pe "kuch khaas nahi". Problem same, STATE badla. Dry-run + mind-visualize = solved khud.

---

## #9 — Rotate Image (90° clockwise, in-place)   (Medium | Grid)
- **Signal:** "2D grid ko rotate/transform" → grid-index manipulation
- **Approach (derived):** observation — har COLUMN → ek ROW ban jaati, **ULTI**. Direct in-place = overwrite (data loss). 2-step: **TRANSPOSE** (`swap(board[i][j], board[j][i])`, sirf `j>i` half) + har **ROW REVERSE**.
- **Key:** swap (assign nahi, warna data udd jaata); sirf upper-triangle (`j>i`, warna double-swap undo). Bug tha: `j>1` typo (logic sahi thi).

## #10 — Set Matrix Zeroes   (Medium | Grid)
- **Signal:** "cell 0 → uski poori row + col 0" → row/col marking
- **Approach (derived):** TWIST — chalte-chalte 0 mat karo (cascade → poori matrix 0). 2-pass: **Pass 1** `zeroRow[]`/`zeroCol[]` (bool) mark; **Pass 2** agar `zeroRow[i]` YA `zeroCol[j]` → cell 0.
- **Key:** record-first-then-apply (Rotate ka overwrite-lesson dobara use).

---

## #11 — Spiral Matrix   (Medium | Grid)
- **Signal:** "grid ko boundary-by-boundary clockwise ghoomna" → 4 shrinking boundaries
- **Approach (derived):** 4 boundaries `top, bottom, left, right`. Order: top row (L→R) → right col (T→B) → bottom row (R→L) → left col (B→T). Har edge ke baad woh boundary ek kadam ANDAR sikudti; `top<=bottom && left<=right` tak.
- **Key:** har edge apni CURRENT boundary pe chale (`matrix[top][j]`, `matrix[i][right]`...); guard chahiye (non-square mein double-print se bachne ko).
- **Honest note:** apna purana LeetCode solution dekh ke kiya — PAR copy nahi, mind mein dry-run + image bana ke samjha (active processing). Recognition se solve hua; retention ke liye 20-din baad COLD re-try karna hoga ([[active-recall-spacing]]).

## #12 — 3Sum   (Medium | Two Pointer)
- **Signal:** "teen number ka sum = target, unique triplets" → SORT + fix-one + Two-Pointer
- **Approach (derived):** array SORT. `for i` (har element FIX) → target = −nums[i] → `low=i+1, high=n-1` two-pointer (Two Sum II). sum<target → low++; > → high--; == → triplet record.
- **Dedup twist:** outer `i` repeat (`nums[i]==nums[i-1]`) → skip (warna same triplet dobara). Inner low/high duplicates bhi skip.
- **Honest note:** basic (sort+fix+two-pointer) KHUD derive kiya. Dedup pe dry-run kiya, stuck hua → apna purana LeetCode solution dekha, samjha, lagaya (struggle-FIRST phir peek = legit). 20-din cold re-try → `[C]`.

---

## #13 — Longest Repeating Character Replacement   (Medium | Sliding Window)
- **Signal:** "longest substring with a condition (k changes allowed)" → variable Sliding Window
- **Approach (derived):** left/right window + count map. Window VALID jab `(windowLen − maxFreq) ≤ k`. right badhao + maxFreq update; invalid → left badhao (count--); ans = max valid windowLen.
- **Key trick (elegant):** `(windowLen − maxFreq)` = kitne chars badalne padenge → `≤ k` to valid. Kaunse char badle, track nahi karna — bas "kitne non-majority". maxFreq sirf naye char ke count se update (poore map pe loop nahi).
- **Note:** yehi woh problem thi jo pehli baar samajh nahi aayi thi — fresh dimaag pe khud derive + samjha. Index-bug (left pointer) khud pakda.

---

## #14 — Permutation in String   (Medium | Sliding Window + Hashing)
- **Signal:** "s2 mein s1 ka anagram substring?" → FIXED Sliding Window (size = s1.len) + char-count
- **Approach (derived):** s1Count (fixed) + windowCount (s2 pe slide, size s1.len). Window size == s1.len → `mp1 == mp2 ?` → true.
- **2 naye tools (khud dry-run se pakde):** (1) `mp1 == mp2` → poore maps ek line mein compare (loop nahi); (2) count `0` → **erase** key (warna zero-value keys `==` bigaadti). Common SW+map-compare gotcha.

---

## #15 — Valid Parentheses   (Easy | Stack)
- **Signal:** "matching pairs / nesting / LAST-opened PEHLE-closed (LIFO)" → **Stack**
- **Approach (derived):** opening `( { [` → push; closing → stack empty? false : top-match? pop : false; end → stack empty? true.
- **Naya:** Stack DS seekha (push/pop/top/empty, LIFO = plates ki dher). Pair-match: `'(' ↔ ')'` etc — `==` se direct nahi (alag chars), explicit pair-check ya expected-closing-push trick.
- **Note:** pehla Stack problem + pehla "production-muscle" rep — idea tha, syntax pehle copy kiya, phir KHUD apni style mein (match→continue/else-false + empty-guard) likha. Idea + syntax dono apne.

---

## #20 — Search in Rotated Sorted Array   (Medium | Binary Search)
- **Signal:** "rotated sorted + search O(log n)" → modified Binary Search
- **Approach (derived):** mid==target → return. Warna **2 alag sawaal:** (1) kaunsa half SORTED? `nums[low] <= nums[mid]` → left sorted, warna right. (2) target us sorted-half ki RANGE mein? → wahan jao : doosre jao. (NESTED, flat nahi.)
- **Key:** Sawaal-1 mein target NAHI (sirf low vs mid); Sawaal-2 mein target. Range-check `&&` se — `nums[low] <= target && target < nums[mid]`. **C++ chained comparison `a < x < b` GALAT** (bool ban jaata — `(a<x)<b`), `&&` chahiye.
- **Note:** sabse tough yet. Two-question structure derive kiya; atka → debug-prints + dry-run se buggy line KHUD pakdi; seekha C++ chained-comparison nahi chalta (LOGIC sahi thi, LANGUAGE gotcha — alag cheez). Real debugging + generation, transcription nahi.

## #29 — Reverse Linked List   (Easy | Linked List — 3-pointer walk)  [REDEMPTION]
- **Signal:** "reverse / direction badlo" → 3-pointer walk (prev, curr, next)
- **Approach (derived):** prev=NULL, curr=head. loop: `next = curr->next` (aage bachao) → `curr->next = prev` (arrow ulta) → `prev = curr` → `curr = next`. return prev.
- **Key:** next ko PEHLE save karo, phir flip (warna aage ki list kho jaati). return PREV (curr to NULL).
- **Note (REDEMPTION):** ye WAHI problem tha jisne aaj subah spiral kiya (Claude ne template de diya → frustrate → delete → "main nahi karunga"). Raat ko spoiler BHOOL gaya → BLANK se khud derive → first try, saare edge pass, CLEAN SOLO. Proof: spoiler-free + thoda time = he derives it himself. LL ka foundational base owned.

## #28 — Remove Nth Node From End   (Medium | Linked List — two-pointer GAP)
- **Signal:** "end se Nth / ek pass" → two-pointer with GAP of n (+ head-edge handling)
- **Approach (derived):** fast ko PEHLE n kadam aage. phir slow+fast SAATH. fast end pe → slow us node ke PICHLE pe (jise hatana, uska previous) → link skip. head-hatane wala edge alag.
- **Key (he spotted the edge himself, needed nudge only for the handle-idiom):** n-advance ke baad agar `fast == NULL` → n = length → HEAD hi hatana → `return head->next`. (Idiom seekha; soch — edge spot karna — uski thi.)
- **Honest note:** gap + previous-node logic KHUD nikaala. Phasa sirf head-removal edge ke HANDLE pe (fast==NULL→head->next) — Claude ne THINKING-direction di (code nahi), Arpan ne return khud nikaala. Perfect new-help-model example. His own framing: "edge SPOT karna meri soch thi; handle ka IDIOM naya seekha" (soch ≠ idiom; idioms exposure se jamte hain).

## #27 — Merge Two Sorted Lists   (Easy | Linked List — DUMMY node)
- **Signal:** "do sorted lists ko ek sorted mein jodo" → two-pointer on lists + DUMMY node
- **Approach (derived):** dummy node se shuru (head-sambhalna jhanjhat khatam), curr=dummy. dono lists ke aage pointer; jo chhota → `curr->next = woh node`, curr aage, us list ka pointer aage. ek khatam → doosri ki bachi poonch seedha jod do. return `dummy->next`.
- **2 REAL insights (bug se KHUD nikaale, ~1hr struggle = real generation, NOT copy-paste):**
  - **`curr->next = l1` (NODE khud), NOT `l1->next`** — poora node link karo, uska next nahi. Pehle `l1->next` laga raha tha → aakhri node include/compare nahi ho raha tha. = LL ka CORE concept (node-linking). Ek baar click → saare LL problem mein kaam.
  - **EQUAL case (1==1) handle hona ZAROORI** — `>`/`>=` mein uljha, phir pakda ki equal-value ek branch mein jaana chahiye warna woh node atak jaata.
- **Note:** ye Cycle (#26 copy-paste) ke ULT — ASLI derivation thi (naya tool dummy-node + real logic + null-edges). 1hr laga = dimaag actually kaam kar raha tha = woh permanent gad gaya. New tool owned: dummy-node.

## #26 — Linked List Cycle   (Easy | Linked List — SLOW/FAST, Floyd's)
- **Signal:** "cycle hai ya nahi, bina extra space" → slow/fast (Floyd's)
- **Approach (derived):** slow 1, fast 2. cycle HAI → fast slow ko PAKAD lega (`slow==fast` → true). cycle NAHI → fast NULL pe (`while(fast && fast->next)` khatam → false).
- **Honest note:** ye BADA derivation NAHI tha — slow/fast TEMPLATE Middle-of-LL se REUSE (copy-paste jaisa), naya sirf meet-check twist. AUR YEHI GOOD HAI: template ab reflex (bina soche reuse aaya) = exactly the goal. Arpan ne khud flag kiya "ye copy-paste tha" (accurate self-awareness) + bola "DSA = template reuse, warna itni saari kaun yaad rakhega" (= core philosophy upar).

## BINARY SEARCH — `<=` vs `<` (Arpan ki apni insight, khud socha 20 Jun)
- **Search-and-return template (most common) → hamesha `low <= high`.** Kyunki tu element DHOONDH raha hai — jab `lo` aur `hi` mil jaayein (sirf ek element bacha), woh akela element TARGET ho sakta. `<` use kiya to loop us last element ko check kiye bina nikal jaata → MISS.
- **Example:** `[1,3,5,7]` target=7 → lo=hi=3 pe pahunch jaata. `<=` → mid=3, arr[3]=7 = found ✓. `<` → 3<3 false → loop khatam → miss ✗.
- **Rule:** `high = n-1` (last valid index) → `low <= high` (== zaroori). [`low < high` wala alag template — `hi = n`, boundary-find, mid check kiye bina converge — woh isse mat confuse karna.]
- Tune ye test-case fail dekh ke KHUD reason kiya (kuch case jahan lo==hi run hi nahi hue). Senior-thinking.

## #44 — Kth Largest Element in an Array   (Medium | Heap)   SOLO · DSA gate COMPLETE
- **Signal:** "Kth largest / top-K" → HEAP (priority_queue).
- **Approach (SOLO):** size-K MIN-heap. har element push; `if(size > k) pop()` (sabse chhota nikal jaata). ant me K sabse bade bachte → `top()` = Kth largest.
- **Key:** "Kth LARGEST" → MIN-heap of size K (counterintuitive par sahi — top of size-K min-heap = Kth largest). C++: `priority_queue<int, vector<int>, greater<int>>` = min-heap. Alt: max-heap + (k-1) pops.
- **Heap basics:** min/max hamesha top; push/pop O(log n), top O(1); use = top-K/Kth/running-min-max.
- **Note:** SOLO clean first-try. Heap = LAST new pattern. **DSA gate-level COMPLETE** — Hashing/Two-Ptr/Sliding-Window/Prefix/Binary-Search/Stack/LinkedList/Grid/Trees(+BST)/Backtracking/Heap all done (44). Now → pure REVISION rotation (1/pattern/day), no new for JP.

## #43 — Combination Sum   (Medium | Backtracking — start-index + reuse)   REP 3 · FULL SOLO · JP OA Q
- **Signal:** "sum==target ke saare combos, reuse allowed" → backtracking + 3 twist.
- **Approach (FULL SOLO):** helper(cand, target, idx, temp&, ans&). base: target==0 → push(temp); target<0 → return (prune). loop i from idx: push(cand[i]) → recurse(target-cand[i], **i** — NOT i+1, reuse) → pop.
- **3 twists (all SOLO first-try):** (1) REUSE → recurse with same `i` not i+1; (2) TARGET base → ==0 found, <0 prune; (3) START index → loop from idx (no going back) → kills duplicate combos.
- **CURVE PROOF (the point):** rep1 Subsets#41 = 3 conceptual nudges; rep2 Permutations#42 = structure-solo + 1-2 small nudges; rep3 Combination-Sum#43 = **ZERO help, all-solo, first-try.** Exactly the trees curve (diameter#33 nudged → Good-Nodes#38 cold-solo). Directly answers his earlier doubt ("bina help nahi banta") — 3 reps → solo. AND it's JP's literal OA question, solved alone. Backtracking now OWNED at template level.

## #42 — Permutations   (Medium | Backtracking — used[])   REP 2
- **Signal:** "saare arrangements / order" → backtracking + `used[]` track (kaunse use ho chuke).
- **Approach:** helper(nums, used&, temp&, ans&). base temp.size()==n → ans.push(temp). loop i: `if(!used[i])` → CHOOSE (used[i]=true, push) → EXPLORE (recurse) → UN-CHOOSE (pop, used[i]=false). All 3 INSIDE the if.
- **Twist vs subsets:** subsets = index lo/chhodo (kuch elements); permutations = SAARE elements alag ORDER → loop over all + used[] to avoid reuse.
- **Note:** REP 2 of backtracking. Structure (used[]/loop/choose-explore-unchoose) Arpan set up SOLO; nudges only = (1) C++ `vector<bool> used(n,false)` sizing (empty → runtime crash, mechanic he'd seen), (2) choose/explore/unchoose all-inside-the-if placement. **Curve improving vs rep-1** (rep-1 needed 3 conceptual nudges; rep-2 conceptual structure was his). His take: "thoda dimaag khula, help li par pehle se kam — progress."

## #41 — Subsets / Power Set   (Medium | Backtracking)   REP 1 (new pattern)
- **Signal:** "saare subsets/combinations" → BACKTRACKING (choices ka ped). Har element: LO ya CHHODO.
- **Approach:** helper(idx, nums, temp&, ans&). base idx==size → ans.push(temp). idx pe 2 call: (1) LIYA — temp.push(nums[idx]) → recurse(idx+1) → **temp.pop() (undo)** → (2) NAHI LIYA — recurse(idx+1). 2^n subsets.
- **Key:** template = CHOOSE → EXPLORE → UN-CHOOSE. (1) `temp` PARAMETER hona chahiye (local nahi — carry across recursion). (2) idx-model = 2 recursive call, NO for-loop (for-loop = doosra model, mix mat karo). (3) **pop ORDER**: undo "liya"-call ke BAAD aur "nahi liya"-call se PEHLE (warna exclude branch mein bhi element ghus jaata).
- **Note:** FIRST backtracking problem EVER — needed 3 nudges (temp=param, 2-call-not-loop, pop-order). NORMAL for rep-1 of a new pattern. Trees-curve repeat expected: diameter#33 nudged → Good-Nodes#38 cold-solo; backtracking will go same (rep 1 help → rep 3-4 solo). His settled take: "sabar + practice — reps se baithta."

## #40 — Kth Smallest Element in a BST   (Medium | Trees — inorder=sorted)   SOLO
- **Signal:** "BST + k-th smallest" → inorder (left→node→right) BST pe SORTED deta → k-th nikaalo
- **Approach (SOLO):** inorder helper (reference vector bharता) → poora sorted list → return `v[k-1]` (0-based, isi liye k-1). base null → return.
- **Key:** BST ka inorder HAMESHA sorted (ascending) — kyunki left(chhote)→node→right(bade). Yeh BST ka doosra core concept (pehla = range-window #39).
- **Note:** SOLO clean, koi bug nahi. (Optimization possible: poora vector na banake counter se k-th pe early-stop — par yeh approach bhi sahi/clear.) #40 milestone — 40 active-derive.

## #39 — Validate Binary Search Tree   (Medium | Trees — range window)
- **Signal:** "valid BST?" → har node ek allowed WINDOW (min,max) ke andar; window neeche jaate NARROW hota
- **Approach:** root window (-inf,+inf). har node: `min < val < max` ? nahi → false. left recurse → (min, val) [max=node]; right recurse → (val, max) [min=node]. base null → true. C++: `long long` inf (node INT_MIN/MAX bhi ho sakti).
- **Key:** TRAP = node ko sirf parent se compare karna GALAT (2 under 8 valid-lagta par root-5 ke right me 2<5 → invalid). Window se pakda jaata: right gaye → min=5 → 2<5 → false.
- **Bug (nudge se fix):** pehle "current node range me → TURANT return true" likha → root hamesha range me → SAB valid aaye. Fix: true return karne ki jagah **AND of children** — "valid = current theek AND left-valid AND right-valid". (Claude ne structure-flaw point kiya: early-return + dead-recursion; Arpan ne AND laga ke theek kiya.) 7/7 pass.
- **Note:** BST ka pehla concept. Naya pattern = range/bounds pass-down (Count-Good-Nodes ke pass-down se related, par yahan 2 bounds + AND-combine).

## #38 — Count Good Nodes in Binary Tree   (Medium | Trees — DFS pass-down)   COLD TEST · FULL SOLO
- **Signal:** (NONE given — cold test) → node ko judge karne ko "raaste ka max" chahiye → DFS me max NEECHE pass + count
- **Approach (derived COLD, no help):** har node tak path ka **max-so-far** carry karo. `node->val >= maxSoFar` → good (count++). recurse left/right with `max(maxSoFar, node->val)`. (BFS bhi valid — queue me `{node, pathMax}`, par level-loop NAHI chahiye.)
- **Key:** "judge karne ko kya yaad chahiye?" = path ka max. Pehchaan ki **level ka role nahi** (traversal, level-grouping nahi) — khud bola.
- **WHY THIS MATTERS (proof entry):** Arpan ne shaq kiya tha "DSA nahi ban raha, pichhle saare solution dekh ke kiye the (recall, derive nahi)." Test: ek GENUINELY-UNSEEN problem, koi signal/approach nahi, leetcode band. Usne pattern khud pehchaana (DFS/BFS), level-irrelevance khud notice ki, key insight (carry path-max) khud derive ki, 6/6 pass. **Cold generation WORKS** — recognition nahi, asli derive. His own conclusion: "dimaag hai, bas DSA me thoda slow" (accurate — slow ≠ nahi banta; generation hafton me banta).

## #37 — Lowest Common Ancestor (Binary Tree)   (Medium | Trees — DFS bubble-up)   SOLO
- **Signal:** "do node ka milne ka point / common ancestor" → DFS + jawab UPAR bubble karo
- **Approach (derived SOLO):** base — `node null → null`; `node == p ya q → wahi node` (mil gaya). recurse left+right. combine — `left null → return right`; `right null → return left`; **dono non-null → return root** (p ek taraf, q doosri → yahi LCA).
- **Key:** har node upar ek jawab bhejta. Dono taraf se kuch mila = p,q split = current node hi meeting-point. Ek taraf se = LCA us taraf neeche.
- **Note:** SOLO, **pehli baar mein clean (koi bug nahi)**. Combine-logic (left/right null cases + dono→root) Arpan ne KHUD derive ki; Claude ne sirf base-case ka sawaal poocha. 6/6 pass incl ancestor-of-itself + split-sides. Naya pattern: DFS bubble-up (jawab neeche se upar lautana).

## #36 — Zigzag Level Order Traversal   (Medium | Trees — BFS twist)   SOLO
- **Signal:** "level by level + alternate ULTA / zigzag" → BFS (#34 template) + alternate-reverse twist
- **Approach (derived):** #34 ka SAME BFS template (queue + `size=queue.size()` per level) + `bool flag` har level pe toggle. Twist: queue ka order MAT chhedo (hamesha L→R nikaalo) — bas placement badlo.
- **Key (his own smart move):** reverse-at-end ki jagah **direct index placement** — `v.resize(size)` phir `index = flag ? i : size-i-1`. Reversed level mein i=0→last, i=1→second-last. Ek operation bach gaya, cleaner. (Claude ne reverse-at-end hint ki thi; usne better variant khud derive kiya.)
- **2 cheezein khud handle ki (nudge se):** (1) reversed index `size-i-1` (constant `size-1` nahi); (2) `v` ko `resize(size)` — khaali vector pe `v[index]=` UB hota.
- **Note:** SOLO. BFS template ab 3 problem (#34 level-order + #35 right-view + #36 zigzag) — ek template, teen twist. "12 template + twist" live proof.

## #35 — Binary Tree Right Side View   (Medium | Trees — BFS twist)   SOLO
- **Signal:** "har level se EK node (right se dikhne wala)" → BFS (level-order ka twist)
- **Approach (derived):** #34 ka SAME BFS template (queue + `size = queue.size()` per level). Twist: har level se sirf **last node** (`i == size-1`) ans mein push; bachche (left,right) hamesha queue mein.
- **Key:** level ka last node = right-most (right se wahi dikhta). Off-by-one: loop `i<size` → last node pe `i == size-1`, NOT `size` (0-based).
- **Note:** Pehle `i==size` likha (kabhi true nahi hota → empty). Khud dry-run se NAHI — Claude ke nudge ("loop i<size, last node pe i kya? i==size hota hai?") se `size-1` pakda. Off-by-one chhota tha. Asli win = **BFS template bina socke reuse** ("12 template + twist" live). Ek template se ab 2 problem (#34 level-order + #35 right-view). (#34 wala bug khud dry-run se pakda tha; ye wala nudge se.)

## #34 — Level Order Traversal   (Medium | Trees — BFS / queue)   SOLO
- **Signal:** "level by level / har level alag list" → BFS → QUEUE (DFS nahi)
- **Approach (derived):** root queue mein → jab tak queue khaali na ho: `size = queue.size()` (= POORA ek level) → us size jitne node ek-ek front se nikaalo (value level-list mein), har node ke left/right (non-null) queue ke peeche push → level-list ans mein push.
- **Key:** `queue.size() = ek pura level` = BFS ka dil. Level-list har baar **fresh (clear)** honi chahiye.
- **Note:** SOLO solved. Ek bug khud pakda — level-list (`v`) ko loop ke bahar declare kiya tha, har level mein clear nahi hota tha → purani values jud ke aati ([[1],[1,2,3]...]). Paper dry-run se khud dhoonda + theek kiya, bina bataye. Derived = stick. JP yeh DIRECTLY pucchta (real writeup).

## #33 — Diameter of Binary Tree   (Easy-but-tricky | Trees — depth + track-max)  [C] RE-REVISIT
- **Signal:** "longest path / diameter" → depth-recursion + har node pe max track
- **Approach:** helper depth lautata (`1+max(l,r)`) AUR side mein global `maxi = max(maxi, l+r)` update karta. diameter func: `maxi=0` reset → depth chalao → return maxi.
- **Key (3 gotchas — yahi confuse karte):** (1) function DEPTH lautata, diameter SIDE-effect (maxi); (2) diameter-func maxi return kare, depth nahi; (3) global maxi har call pe RESET (warna pichla value reh jaata).
- **HONEST note:** ye TODAY's toughest tree tha — depth + track-another-thing pattern, pehli baar = confusing. Passed via Claude's thinking-nudges (3 distinct bugs), NOT full solo derivation. Arpan honestly flagged "samajh nahi aaya, recursion-with-extra-layer hard". **[C] = cold re-revisit when fresh (paper dry-run slowly).** This is the recognition-vs-generation gap on novel twists — builds slow (weeks), normal at ~2 days active.

## #32 — Same Tree   (Easy | Trees — recursion on TWO trees)
- **Signal:** "do tree compare / same hai?" → recursion dono pe SAATH
- **Approach (derived):** base — dono NULL → true; ek NULL ek nahi → false (DONO directions). node-kaam — `p->val != q->val` → false. recurse — `sameTree(p->left,q->left) && sameTree(p->right,q->right)` (dono true tabhi same).
- **Key:** ab DO tree parallel (naya flavour). base mein DONO "ek-null" cases (p-null-q-not AUR p-not-q-null) — warna NULL->val pe crash. combine with `&&` (not `||`).
- **Note:** solo. base-case ka mirror-null slip khud pakda (trivial oversight, koi bhi miss kare — not a skill gap). Tree-template extends to 2-tree comparison.

## #31 — Invert Binary Tree   (Easy | Trees — DFS recursion)
- **Signal:** "tree transform / har node pe kuch karna" → recursion (DFS)
- **Approach (derived):** `if(root==NULL) return NULL;` (base) → `swap(root->left, root->right)` (node-kaam) → invert left, invert right → return root.
- **Key:** SAME tree-template as max-depth — only the "node pe kaam" changed (max-depth = `1+max(l,r)`; invert = `swap(L,R)`). Structure same: base + node-work + recurse L/R.
- **Note:** 2nd tree problem, clean solo first-try. Tree-template owned (proof: 2 problems, same shape, only node-work differs). Calm pace (Arpan set "aaram se chal, jaldi nahi" — understanding > speed).

## #30 — Maximum Depth of Binary Tree   (Easy | Trees — DFS recursion)  [TREES START]
- **Signal:** "tree ki height/depth ya har node pe compute" → RECURSION (DFS)
- **Approach (derived):** `if(root==NULL) return 0;` (base) → `l=maxDepth(left); r=maxDepth(right);` → `return 1 + max(l,r)`. left/right khud solve karte, tu bas combine.
- **Key — THE TREE TEMPLATE (90% tree problems isi shape):** base case (NULL) + recurse left & right + combine. (height, sum, count, diameter, balanced, mirror — sab "base + recurse L/R + combine".)
- **Note:** Trees pattern START, first problem first-try clean. Trees = his SPATIAL home (recursion + branches) — felt natural. Template owned on problem #1.

## #25 — Search a 2D Matrix   (Medium | Binary Search — 2D as 1D)
- **Signal:** "fully sorted grid (rows sorted + rows ordered) + search O(log)" → BS on virtual 1D index
- **Approach (derived):** poori matrix = ek lambi sorted line. Physically flatten NAHI — dimaag mein 1D maano, index `0..m*n-1` pe plain BS. mid (flat index) → cell: `row = mid/cols`, `col = mid%cols`. value `matrix[mid/col][mid%col]` → compare → left/right/found.
- **Key:** mid ko (row,col) mein todna = divide/remainder (#columns se). `hi = row*col - 1`. Baaki sab normal BS.
- **Note:** ye problem PEHLE spoiled hua tha (Claude ne formula de diya). AAJ Arpan ne `mid/cols`, `mid%cols` KHUD re-derived ("6/4=1, 6%4=2, saaf dikh raha") + poora BS khud likha + khud run. Cold re-derivation = passed. Spoiled cheez ko apni samajh se phoda = real learning proof. Binary Search COMPLETE (6 problems, all variants).

## #24 — Middle of the Linked List   (Easy | Linked List — SLOW/FAST pointer)
- **Signal:** "middle / half / cycle without counting length" → SLOW + FAST pointer (naya tool)
- **Approach (derived):** slow 1 kadam, fast 2 kadam. `while(fast && fast->next)` → jab fast aakhir pe, slow theek beech. even length → doosra middle (problem ne yahi maanga). return slow.
- **Key:** loop condition `fast && fast->next` (dono check — warna fast->next->next NULL pe crash). Naya pattern: slow/fast = middle, cycle-detect, palindrome-LL sab isi pe.
- **Note:** FIRST clean INDEPENDENT solo-win under the new self-run flow (he runs own code via Code Runner, Claude out of loop). Slow/fast khud derive kiya, first try, saare edge (even/odd/single) pass. "main apna malik." DSA fear ka root = dependence; independence ne kaata. Spatial mode (arrows) se nikaala.

## #23 — Search Insert Position   (Easy | Binary Search)
- **Signal:** "sorted + position dhoondo (mil jaaye to index, na mile to insert-jagah)" → plain Binary Search
- **Approach (derived):** normal BS. target == mid → return mid. Loop khatam (na mila) → `low` wahi insert-position pe khada hota → return low.
- **Key:** `high = n-1` + `low <= high` (consistent) — warna `high=n` + `<=` mix → mid index `n` chhoo leta → out-of-bounds CRASH. Loop-bound + return value ka dhyaan.
- **Note:** calm solo-win (de-drama). Crash (target sab se bada, mid → index n) khud cout/debug-print se trace kiya, loop-bound + return khud fix kiya. Chhoti index-cheez, koi bada bug nahi — handled calmly.

## #22 — Koko Eating Bananas   (Medium | Binary Search ON THE ANSWER)
- **Signal:** "min/max VALUE jo ek condition satisfy kare + monotonic N..N Y..Y" → Binary Search on the ANSWER (array pe nahi, answer-space pe)
- **Approach (derived):** search-space `lo=1 .. hi=max(pile)` = possible speeds. Kisi speed `k` pe FEASIBLE? → `sum(ceil(pile/k)) <= h`. PASS → `ans=mid, high=mid-1` (aur dheere dhoondo); FAIL → `low=mid+1` (tez karo). boundary (pehli PASS) = min speed.
- **Key:** naya sub-pattern — answer khud BS karte hain (feasibility line sorted N..N Y..Y). check-fn `sum(ceil(pile/k))`. **int-ceil = `(pile+k-1)/k`** (NOT `ceil(int/int)` — woh pehle floor kar deta!). `low` 1 se (0 → div-by-zero crash).
- **Note:** multi-bug safar, SAB khud debug kiya — (1) missing return khud pakda, (2) low=0 div-0 crash khud fix, (3) `ceil(int/int)` bug DRY-RUN se nikaala (int division pehle floor → ceil bekaar), (4) exact-division edge → int-ceil trick lagaaya. Copy pe trace karke phoda. "Copy uthao, trace karo, code aata" — jiya.

## #21 — Find Minimum in Rotated Sorted Array   (Medium | Binary Search)
- **Signal:** "rotated sorted + min dhoondo O(log n)" → modified Binary Search
- **Approach (derived):** min = pivot (jahan order tuta). mid ko **END se compare** karo: `nums[mid] > nums[high]` → drop mid ke RIGHT mein → `low = mid+1`; warna min LEFT half mein (mid included) → `high = mid`. Range simat jaaye → `nums[low]` = min.
- **Key:** **koi TARGET nahi** (Search-in-Rotated se yahi farq — Arpan ne khud pakda). Sawaal "target kahan?" nahi, balki "min kis taraf?" — sirf mid-vs-end. SIMPLER hai (ek comparison, nested if-else nahi). `high=mid` (mid-1 nahi — mid khud min ho sakta).
- **Note:** Search-in-Rotated ka milta-julta — usi din kiya (pattern reinforce). Tune khud reframe kiya "target hai hi nahi" → mid-vs-end nikaala → dry-run [4,5,6,7,0,1,2] copy pe → first proper try pass. **Copy-pe-trace = generation** (aaj ki badi seekh).

## #19 — Binary Search   (Easy | Binary Search — pattern start)
- **Signal:** "sorted + search" → Binary Search (har step AADHA kaato)
- **Approach (derived):** lo=0, hi=n-1. `while(lo<=hi)`: `mid = lo+(hi-lo)/2`; ==target→return mid; <target→`lo=mid+1`; >target→`hi=mid-1`. End → -1.
- **Key:** `lo=mid+1` / `hi=mid-1` (HALF skip = O(log n)) — NOT `lo++`/`hi--` (woh LINEAR O(n)!). `mid = lo+(hi-lo)/2` (overflow-safe). `lo<=hi`.
- **Note:** chhote tests `lo++`/`hi--` (linear) se bhi PASS ho gaye — "test-pass ≠ correct" flag se pakda. Live lesson: chhote tests O(n) vs O(log n) miss karte. (Foundational BS template OWN ho gaya.)

## #18 — Daily Temperatures   (Medium | Stack — MONOTONIC)
- **Signal:** "har element ke liye agla BADA (kitni door)" → monotonic stack (next-greater pattern)
- **Approach (derived):** stack of INDICES (decreasing temp). ans init 0. `for i`: while top CHOTA (`temp[i] > temp[st.top()]`) → idx=pop, `ans[idx] = i - idx`; push i. End → jo bache = 0.
- **Key:** answer POP ke waqt set hota (POP-hue ka, current ka nahi); INDEX store karo (distance ke liye); "warmer" = strictly `>` (na ki `>=` — equal warmer nahi).
- **Note:** pehla MONOTONIC STACK. Pehle pseudo-code transcribe kiya tha → phir Arpan ne khud demand kiya "yeh real nahi" → BLANK se apni thinking se DOBARA likha (confused hua purana-naya mix se, khud untangle kiya, pass). ASLI generation. Meta-lesson: cold-generate karte waqt purana code mat dekho.

## #17 — Evaluate Reverse Polish Notation   (Medium | Stack)
- **Signal:** "postfix evaluate / last-2-numbers pe operate" → Stack
- **Approach (derived):** stack<int>. number → `push(stoi(token))`; operator → `b=pop, a=pop, push(a op b)`. End → top = answer.
- **Key:** order `a op b` (b = top/2nd-operand, a = next/1st) — `-` aur `/` mein matter. `stoi` = string→int. Helper `operate(a,b,op)` khud banaya.

## #16 — Min Stack   (Medium | Stack design)
- **Signal:** "stack + getMin() sab O(1)" → har element ke saath min-so-far track karo
- **Approach (derived):** 1 stack of PAIRS `{value, minSoFar}`. `push(x)`: m = empty? x : min(x, top.second); push {x,m}. `getMin()` = top.second. Pop pe pichla pair apne aap pichla-min de deta (scan nahi).
- **Naya:** pehla DESIGN problem (class). 2-stack se 1-stack (pair) tak khud derive kiya. getMin O(1) bina scan.

---

> **Status:** 33 done (6 Hashing + 1 Prefix-Suffix + 2 Two Pointer + 3 Grid + 2 Sliding Window + 4 Stack + 6 Binary Search + 5 Linked List + 4 Trees). **Arrays&Hashing + Two Pointer COMPLETE. BINARY SEARCH COMPLETE (6). LINKED LIST COMPLETE (5). TREES going (max-depth + invert + same-tree solo; diameter [C] re-revisit). Palindrome-LL = Middle+Reverse combine (do later).**
> **NOW: CONSOLIDATE PHASE (21 Jun) — ek-do din ONLY REVISION (HLD+Java+DSA), no new problems, then resume. DSA revision = active-recall (Claude gives name+signal, he recalls approach cold). 14/14 cold recall on 21 Jun.**
> **Next pattern: TREES (DFS/BFS) — spatial, his mode.**
> **COLD REDO started (20 Jun):** new `08_DSA/COLD_REDO/` — redo ALL patterns blank/self-run (purane mein kahin Claude-help thi; ye clean ownership). Original PHASE2_CODING = reference (peek nahi). Nothing deleted.
> **NEW FLOW (20 Jun):** he runs his own code (Code Runner, Ctrl+Alt+N); Claude gives problem+signal+test-cases, runs only when he says "done" (to witness). Independent = no fear. Help only on his ask, thinking-direction never code.
> **Binary Search COMPLETE:** basic + rotated-search + rotated-min + Koko(BS-on-answer) + search-insert + search-2D. (Search-2D was spoiled earlier; he re-derived it himself today.) Aage: more Linked List (cycle, palindrome, reverse cold).
> **Meta-milestone (19 Jun):** Arpan ne khud pakda "scaffold-transcription ≠ real learning" + "cold-generate karte waqt purana mat dekho" + **"DSA = COPY + PEN: baith, trace kar, jahan jaaye uske hisaab se code likh do — bas, aur kuch nahi"** (bina copy = impossible; spatial dimaag → trace se code aata). Aage minimal stubs (signal only, no pseudo-code) — woh structure khud generate karega.
> **Milestone:** "medium easy lagne laga"; self-written syntax; pehla design problem.
> **Defer:** Encode/Decode Strings. **Next:** Search a 2D Matrix / Koko (BS finish), phir naya pattern (Linked List / Trees).
> **RETENTION reminder:** solved problems 20-din-cycle mein BINA dekhe cold re-try (recognition → retention). Bhoolna fail nahi, revise na karna fail.
