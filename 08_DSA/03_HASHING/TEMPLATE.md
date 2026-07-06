# HASHING — template (Arpan-seekha)

> ★ REDO SCHEDULE: har SOLVED problem ko +3 din AUR +7 din -> BLANK pe REDO (dekh ke nahi). neeche checkbox mark karo. yehi recognition->generation = asli retention. (dry-run habit bhi: "done" se pehle ek example trace.)

## ★ CORE
```
   "seen it? / kitni baar?" -> O(1) lookup se nested-loop O(n²) -> O(n).
   SET = existence (dekha ya nahi) · MAP = value/count/index yaad rakhna.
   ★ map ke 2 use dekhe: {value->INDEX} (Two Sum) · {char->COUNT} (Anagram). soch: "map me KYA store karu?"
```

## Solved (produce + spaced-redo track)
```
   - Two Sum (map {value->index}; "(target-num) seen?" find!=end -> pair; else add. ONE pass O(n)) — SOLO ✓ | redo: [ ] +3din [ ] +7din
   - Valid Anagram (COUNT map: s ++ , t -- , sab EXACTLY 0 -> anagram; length-check first) | redo: [ ] +3din [ ] +7din
     GALTI: final check me ">1" likh diya -> chahiye tha "!= 0" (koi bhi non-zero count = not anagram).
   - Group Anagrams (map {sorted-word -> list}; word sort = canonical KEY -> same key = same group) | redo: [ ] +3din [ ] +7din
   - Subarray Sum = K (PREFIX-SUM + map {sum->count}; (sum-k) seen? -> add count; map[0]=1 init) | redo: [ ] +3din [ ] +7din
     NOTE: naya insight (prefix-sum) — seed se seekha, solo nahi hua. REDO pe khud nikalna (yehi tikega). tip: mp[sum-k] seedha (na ho to 0).
```
