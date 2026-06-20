# THINKING-FIRST — Solved Log

> Active-derive practice. Har problem ka APPROACH jo KHUD derive kiya (code baad mein).
> Yeh tracker + navigator: signal → pattern → approach.

---

## REVISION TRACKER — spaced active-recall

> **Kaise:** neeche ka SIGNAL padho, approach DHAK ke **cold recall** karo (bol ke / kaagaz pe). Recall ho gaya → ✓ + gap aage badha. Blank → tabhi peek + dobara.
> **Schedule per problem:** R1 ≈ din 3 · R2 ≈ din 10 · R3 ≈ din 30. Solid → skip aage; blank → reset chhota.
> **Re-code:** sirf `[C]`-marked (coding tricky thi) ko laptop pe **bina dekhe** dobara likho. Baaki sirf **approach-recall** (2-3 min each).
> Box mein date/✓ bhar jab woh review kare.

| #  | Problem                  | Pattern        | R1 | R2 | R3 | re-code? |
|----|--------------------------|----------------|----|----|----|----------|
| 1  | Contains Duplicate       | Hashing        |    |    |    |          |
| 2  | Valid Anagram            | Hashing        |    |    |    |          |
| 3  | Two Sum                  | Hashing        |    |    |    |          |
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

> **Status:** 24 done (6 Hashing + 1 Prefix-Suffix + 2 Two Pointer + 3 Grid + 2 Sliding Window + 4 Stack + 5 Binary Search + 1 Linked List). **Arrays&Hashing + Two Pointer COMPLETE. Sliding Window + Stack deepened. Binary Search strong (5). Linked List STARTED (slow/fast).**
> **NEW FLOW (20 Jun):** he runs his own code (Code Runner, Ctrl+Alt+N); Claude only gives problem+signal+test-cases. Independent = no fear. Help only on his ask, thinking-direction never code.
> **Binary Search:** 5 problems (incl rotated + BS-on-answer). Search-a-2D-Matrix + Reverse-LL deferred (spoiled — do COLD fresh). Aage: more Linked List (slow/fast → cycle, palindrome; + reverse cold).
> **Meta-milestone (19 Jun):** Arpan ne khud pakda "scaffold-transcription ≠ real learning" + "cold-generate karte waqt purana mat dekho" + **"DSA = COPY + PEN: baith, trace kar, jahan jaaye uske hisaab se code likh do — bas, aur kuch nahi"** (bina copy = impossible; spatial dimaag → trace se code aata). Aage minimal stubs (signal only, no pseudo-code) — woh structure khud generate karega.
> **Milestone:** "medium easy lagne laga"; self-written syntax; pehla design problem.
> **Defer:** Encode/Decode Strings. **Next:** Search a 2D Matrix / Koko (BS finish), phir naya pattern (Linked List / Trees).
> **RETENTION reminder:** solved problems 20-din-cycle mein BINA dekhe cold re-try (recognition → retention). Bhoolna fail nahi, revise na karna fail.
