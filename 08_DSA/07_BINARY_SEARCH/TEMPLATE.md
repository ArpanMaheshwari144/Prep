# BINARY_SEARCH — template (Arpan-seekha)

> Abhi khaali. Jab tu is pattern ko PRODUCE karega, tab apne words me template yahan bharenge (pre-fill nahi — peek se bachne ke liye).

> ★ REDO SCHEDULE: har SOLVED problem ko +3 din AUR +7 din -> BLANK pe REDO. (dry-run habit: "done" se pehle ek example trace — silly-slip pakadta.)

## Solved (produce + spaced-redo track)
```
   - Binary Search basic (sorted; low/high, mid=low+(high-low)/2; nums[mid]==target->mid; <target->low=mid+1; else high=mid-1; O(log n)) — SOLO produce | redo: [ ] +3din [ ] +7din
     GALTI (7-Jul): low++ / high-- likh diya (thake me fatigue-slip) — chahiye tha low=mid+1 / high=mid-1.
     ★ CORE: HALF discard karo (mid ke us paar JUMP), ek-ek nahi -> warna O(n) linear ban jaata.
     ★ tests-PASS ≠ algo-SAHI: low++/high-- ne bhi correct output diya tha, par O(n) tha (dhoka). algo dekho, sirf output nahi.
   - Search Insert Position (same BS; found->mid; NOT-found-> return low) — SOLO ✓ 5/5, no-hint, mid±1 sahi | redo: [ ] +3din [ ] +7din
     ★ KEY: target na mile -> loop khatam pe `low` = insertion-point (jahan daalna chahiye). yahi twist.
   - First & Last Position (duplicates; 2 BS: leftmost=match pe high=mid-1, rightmost=match pe low=mid+1) — SOLO ✓ 5/5 | redo: [ ] +3din [ ] +7din
     ★ KEY: match pe RUKO mat -> us taraf aur dhundo. ★ GALTI-fix: dono BS ke beech low/high RESET karna (0..n-1) — warna doosra loop chalta hi nahi.
   - Search in Rotated Sorted Array (mid pe: ek half HAMESHA sorted -> pehchaan -> target us range me? narrow) — 5/5, PAR prior-exposure + THODA PEEK (Arpan-honest) -> recognition-heavy, NOT full-generation.
     ★ isliye BLANK-REDO pending (no-peek) +1-2 din -> tab lock. | redo: [ ] BLANK-no-peek [ ] +7din
     core: nums[low]<=nums[mid] (left sorted) -> target [low..mid] me? high=mid-1 : low=mid+1. warna (right sorted) -> target [mid..high] me? low=mid+1 : high=mid-1.
   - Find Minimum in Rotated (nums[mid]>nums[high] -> min DAAYE -> low=mid+1; warna high=mid-1; return nums[low]) — SOLO ✓ 5/5, NO peek (rotated ka reinforce) | redo: [ ] +7din
     ★ KEY: mid ko HIGH se compare. ★ VALUE chahiye -> return nums[low] (INDEX low nahi). rotated-soch ab baith rahi.
```
