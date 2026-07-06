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
   - Subarray Sum = K (PREFIX-SUM + map {sum->count}; (sum-k) seen? -> add count; map[0]=1 init) | redo: [x] BLANK-REDO 6-Jul (SOLO, 4/4, sum-k SAHI, WHY self-annotated) [ ] +7din
     NOTE: prefix-sum idea seed se. GALTI (pichli baar): "k-sum" likha -> dry-run ne "wrong" dikhaya par KYUN samajh nahi aaya -> seed dekh ke "sum-k" fix kiya.
     ★ 6-Jul BLANK-REDO: nemesis CLEARED — khud se prefix[j]-prefix[i-1]=k -> rearrange -> sum-k nikaala, blank code 4/4 solo. recognition->generation proof.
     ★ WHY sum-k (redo pe yaad): prefix[i]-prefix[j]=k -> prefix[j]=sum-k -> map me "sum-k" (purana prefix jo k-kam ho) dhundo. k-sum ULTA. tip: mp[sum-k] seedha (na ho to 0).
   - Longest Consecutive Sequence (SET; (num-1) na ho -> START -> aage count -> max; O(n)) — SOLO ✓ | redo: [ ] +3din [ ] +7din
```
