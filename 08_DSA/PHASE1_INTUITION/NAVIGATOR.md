# DSA NAVIGATOR — Signal → Pattern (Arpan's recognition map)

> Konovo `.claude-context/NAVIGATOR.md` jaisa, par DSA pe. Problem mein SIGNAL dekho → PATTERN nikaalo → file kholo.
> Built BY Arpan (his recognition), drill-style. "Pattern recognition = asli DSA-skill."

---

## SIGNAL QUICK-INDEX — "problem mein YE dikhe → YE pattern"

| SIGNAL (problem mein kya dikha)                         | PATTERN                        | flavor / note                  | file |
|---------------------------------------------------------|--------------------------------|--------------------------------|------|
| SORTED array + pair/triplet, sum = target               | Two Pointer                    | opposite-ends (start↔end)      | 02   |
| longest/shortest SUBSTRING/subarray with a condition    | Sliding Window                 | + map/set to track window      | 03   |
| "koi DUPLICATE hai?" / membership / "pehle dekha?"       | Hashing                        | HashSet (membership only)      | 04   |
| COUNT contiguous subarrays, sum = k (NEGATIVES present) | Hashing + Prefix Sum (combo)   | running sum + map (curr-k seen); window FAILS (negatives) | 04 |
| SORTED array + target ka index dhoondho                 | Binary Search                  | low/high/mid, half cut         | 06   |
| MAX SUM of a contiguous subarray                        | Kadane                         | current_sum reset + max snapshot | 07 |
| in-place REARRANGE/filter (remove-dup sorted, move zeros)| Two Pointer                   | slow/fast (slow places, fast scans) | 02 |
| MANY range-sum queries on a fixed array                 | Prefix Sum                     | precompute once → O(1) per query | 05 |
| "smallest/largest VALUE that works" (Koko/Ship, min speed/cap in D days) | Binary Search on Answers | search answer-space + feasibility check | 06 |
| PALINDROME / symmetry check (start vs end)              | Two Pointer                    | opposite-ends (start↔end)      | 02/08 |
| GROUP anagrams together                                 | Hashing                        | HashMap grouping (sorted-word = key) | 04 |
| FREQUENCY / count / first non-repeating char            | Hashing                        | frequency map (key → count)    | 04   |
| FIRST and LAST position of target in sorted array       | Binary Search                  | TWO binary searches (first occ + last occ) | 06 |
| search in ROTATED sorted array                          | Binary Search                  | modified (ek half hamesha sorted → narrow) | 06 |
| REVERSE string/array in-place                           | Two Pointer                    | opposite-ends, SWAP            | 02/08 |

---

## HOW TO USE (har naye problem pe)

```
   1. SIGNAL pakdo — problem mein kya clue hai? (sorted? subarray? count? duplicate? sum=k?)
   2. NAVIGATOR mein dhoondho → PATTERN nikaalo
   3. WHY check (1 line) → file kholo agar yaad refresh chahiye
   4. CODE (rep) → dry-run → run
   5. GENERALIZE — naya signal mile to yahan ADD karo (navigator grow karta jaaye)
```

> Built BY Arpan, drill-style — 15/15 recognition (3 rounds). Recognition INSTANT (his strength);
> bottleneck sirf code-fluency (reps). "Pattern recognition = asli DSA-skill, gate isi se khulta."
> Aage naye signals/variants aate jaayenge — add karte raho (Konovo navigator ki tarah, jeevit document).

