# PREFIX_SUM — template (Arpan-seekha)

> Abhi khaali. Jab tu is pattern ko PRODUCE karega, tab apne words me template yahan bharenge (pre-fill nahi — peek se bachne ke liye).

> ★ REDO SCHEDULE: har SOLVED problem +3/+7 din BLANK redo. (dry-run habit: "done" se pehle ek example trace — silly-slip pakadta.)

## Solved (produce + spaced-redo track)
```
   - Highest Altitude (running-sum + max track; maxi=0 se init -> start(0) bhi candidate) — SOLO ✓, no-hint | redo: [ ] +3din [ ] +7din
     ★ KEY: max ko 0 se init karo (agar sab gains negative -> answer 0/start). ★ + Arpan ne CLAUDE ka galat test-data (148, sahi 175) pakda -> "verify, blindly mat maan" (yahi dry-run instinct).
   - Product of Array Except Self (prefix-PRODUCT: left[i]=i se pehle sabka product, right[i]=i ke baad sabka; ans[i]=left[i]*right[i]; NO division; O(n)) — SOLO ✓ 4/4 | redo: [ ] +3din [ ] +7din
     ★ KEY: prod *= nums[i] (guna, prod++ NAHI). do pass: left forward, right backward. store BEFORE multiply.
   - Pivot Index (total; leftSum running; rightSum=total-leftSum-nums[i]; ★ CHECK pehle -> PHIR leftSum+=nums[i]) — DONE ✓ 5/5 (7-Jul, copy-pen se). | redo: [ ] +3din [ ] +7din
     ★ jo thake me atka tha, calmer+copy-pen se ho gaya. ★★ Arpan ne apna bug KHUD pakda (dry-run+cout) -> "add pehle, compare baad" ka slip -> freeze-antidote LIVE. (pehli buggy: 01_pivot_index.cpp; ye REDO clean.)
```
