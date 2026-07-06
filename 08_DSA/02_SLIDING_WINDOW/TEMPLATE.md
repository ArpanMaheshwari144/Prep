# SLIDING_WINDOW — template (Arpan-seekha)

> ★ REDO SCHEDULE: har SOLVED problem ko +3 din AUR +7 din -> BLANK pe REDO (dekh ke nahi). neeche checkbox mark karo. yehi recognition->generation = asli retention. (dry-run habit bhi: "done" se pehle ek example trace.)

## ★ CORE INSIGHT (Arpan, 5-Jul) — sab SW code EK HI TEMPLATE hai
```
   har SW problem ka code LAGBHAG COPY-PASTE same hai (pichle wale ki tarah). skeleton:
     1. window AAGE badhao (right++, element andar lo)
     2. koi CONDITION check karo
     3. condition pe SHRINK karo (left++, element bahar)
     4. ANSWER store karo (max/min/len)
   -> steps SAME. bas CONDITION badalti + ORDER thoda change hota (kab record, kab shrink).
   -> ek baar skeleton aa gaya -> har naya problem = wahi template + naya condition. isliye "easy" lagta.
```

## Solved (produce + spaced-redo track)
```
   - Max Sum Subarray of size K (FIXED window: right add / shrink pe left minus -> O(n), recompute nahi)
     — SOLO blank-produce ✓ | redo: [ ] +3din [ ] +7din
   - Minimum Size Subarray Sum (VARIABLE window: grow right, WHILE sum>=target shrink + record min-len; INT_MAX->0 if none)
     — SOLO blank-produce ✓ | redo: [ ] +3din [ ] +7din

   - Longest Substring Without Repeating (VARIABLE window + unordered_map<char,int> count; dup pe while-shrink; count 0 -> erase)
     — SOLO produce, 1 hint on erase-syntax (KEY se erase). | redo: [x] BLANK-REDO 6-Jul (SOLO, 6/6, template exact, no-hint) [ ] +7din
     (yaad: s[i]=KEY (char), mp[s[i]]=VALUE (count). mp.erase(s[i]) -> KEY se, value se nahi.)
     ★ C++ nugget (count vs value — confuse hota):
        mp[key]        =>  us key ki VALUE (frequency — jo tu badha/ghata raha).
        mp.count(key)  =>  wo key HAI ya nahi (EXISTENCE -> 0 ya 1). key add hai to hamesha 1.
        -> "value 0 pe erase" karna ho to:  if (mp[s[i]] == 0) mp.erase(s[i]);    (VALUE check, count nahi)
        -> "if (mp.count(s[i]) == 0) erase" = DEAD (key exist -> count hamesha 1 -> kabhi chalta hi nahi).
        -> aur erase yahan OPTIONAL hai (cleanup/memory ke liye, correctness ke liye nahi).

   - Longest Repeating Character Replacement (window + char-freq; VALID if (size - maxFreq) <= k, warna shrink)
     — ★ SOLO produce ✓ (ye pehle "nahi hua tha" / defer hua tha -> aaj khud phad diya!). maxFreq-not-decrement trick sahi.
     | redo: [ ] +3din [ ] +7din
     (yaad: majority char rakho, baaki (size-maxFreq) ko k badal me same banao. maxFreq shrink pe ghatana zaroori nahi.)

   ★ PATTERN (Arpan-generalized): SKELETON same -> right add -> [check] -> record + left shrink.
     FIXED = "if (size==k)" + track SUM  |  VARIABLE = "while (condition)" + track LEN. bas if->while, sum->len.
     VARIABLE+dup = window me set/map se track -> "while (dup)" shrink. skeleton wahi, condition rich.
```
