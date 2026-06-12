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

---

> **Status:** 11 done (6 Hashing + 1 Prefix-Suffix + 1 Two Pointer + 3 Grid, sab khud derive/process). **Arrays & Hashing COMPLETE.** Grid series ki 3 done.
> **Defer (kabhi phir):** Encode/Decode Strings. **Next:** agla pattern (Two Pointer / Sliding Window deepen) — ya aur grid.
> **RETENTION reminder:** yeh solved problems 20-din-cycle mein BINA dekhe cold re-try karna (recognition → retention). Bhoolna fail nahi, revise na karna fail.
