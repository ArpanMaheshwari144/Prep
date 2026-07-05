# TWO-POINTER — template (Arpan-seekha)

> jaise-jaise produce karega, apne words me yahan add karte jaana.

## Core idea
```
   do pointer, array pe smartly move → nested-loop (O(n²)) ki jagah EK pass (O(n)).
```

## 2 common forms

### 1. SLOW–FAST (in-place, order maintain) — e.g. Move Zeroes
```
   slow = next-good-position, fast = SCAN karta jaaye.
   fast pe condition-match (jaise non-zero) → swap(slow, fast), slow++.
   fast++ har baar.
   -> good elements aage aa jaate, baaki (zero) apne aap peeche chala jaata.
```

### 2. OPPOSITE-ENDS (sorted / compare) — e.g. Two Sum II, Palindrome
```
   L = start, R = end.
   condition ke hisaab se pointer move:
     sum > target → R-- | sum < target → L++ | (palindrome: L/R chars compare, L++ R--)
   -> SORTED hone se move predictable → O(n).
```

## Kab lagta
```
   sorted array + pair/triplet · dono ends se compare (palindrome) · in-place do-taraf (move-zeroes, sort-colors).
```

## Solved (produce + spaced-redo track)
```
   - Move Zeroes (slow-fast swap) — SOLO blank-produce ✓ | redo: [ ] +3din [ ] +7din
   - Valid Palindrome (opposite-ends L/R compare) — SOLO blank-produce ✓ | redo: [ ] +3din [ ] +7din
   - Container Most Water (opposite-ends, move-shorter-wall) — SOLO blank-produce ✓ | redo: [ ] +3din [ ] +7din
   - 3Sum (sort → fix-one → two-pointer → skip-dupes) — 3-Jul: structure SOLO + 1 hint (dup-skip/bounds).
     ★ 5-Jul COLD-REDO: SOLO 4/4, bina hint — GAP CLEARED ✓ (jaha atka tha, ab automatic). redo: [x] +1 [ ] +7din
     (yaad: triplet mile → skip-dupes (while low<high && nums[low]==nums[low+1]) → phir low++/high-- advance.)
   - Sort Colors (Dutch flag, 3-pointer low/mid/high) — SOLO produce, atka ==2 pe. ★ REDO priority: [ ] +3din [ ] +7din
     (yaad: ==2 → swap(mid,high) + high-- SIRF, mid++ NAHI (high se aaya element unchecked). 0 → swap(low,mid)+low+++mid++. 1 → mid++.)
```
