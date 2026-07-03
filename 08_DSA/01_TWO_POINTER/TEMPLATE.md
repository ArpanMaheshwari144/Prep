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
   - 3Sum (sort → fix-one → two-pointer → skip-dupes) — structure SOLO, 1 hint on inner dup-skip/bounds. ★ REDO priority: [ ] +3din [ ] +7din
     (yaad: triplet mile → low++/high-- PEHLE, phir skip-dupes; nums[high+1] bounds!)
```
