# HASHING — template (Arpan-seekha)

> ★ REDO SCHEDULE: har SOLVED problem ko +3 din AUR +7 din -> BLANK pe COLD-redo (dekh ke nahi). neeche checkbox mark karo. yehi recognition->generation = asli retention. (dry-run habit bhi: "done" se pehle ek example trace.)

## ★ CORE
```
   "seen it? / kitni baar?" -> O(1) lookup se nested-loop O(n²) -> O(n).
   SET = existence (dekha ya nahi) · MAP = value/count/index yaad rakhna.
   ★ map ke 2 use dekhe: {value->INDEX} (Two Sum) · {char->COUNT} (Anagram). soch: "map me KYA store karu?"
```

## Solved (produce + spaced-redo track)
```
   - Two Sum (map {value->index}; "(target-num) seen?" find!=end -> pair; else add. ONE pass O(n)) — SOLO ✓ | redo: [ ] +3din [ ] +7din
   - Valid Anagram (COUNT map: s ++ , t -- , sab EXACTLY 0 -> anagram; length-check first) — SOLO, 1 silly-slip (>1 vs !=0, khud fix) | redo: [ ] +3din [ ] +7din
     (yaad: index-map nahi, COUNT-map. check "!= 0" (koi bhi non-zero = not anagram), ">1" NAHI.)
```
