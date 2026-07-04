# SLIDING_WINDOW — template (Arpan-seekha)

> Abhi khaali. Jab tu is pattern ko PRODUCE karega, tab apne words me template yahan bharenge (pre-fill nahi — peek se bachne ke liye).

## Solved (produce + spaced-redo track)
```
   - Max Sum Subarray of size K (FIXED window: right add / shrink pe left minus -> O(n), recompute nahi)
     — SOLO blank-produce ✓ | redo: [ ] +3din [ ] +7din
   - Minimum Size Subarray Sum (VARIABLE window: grow right, WHILE sum>=target shrink + record min-len; INT_MAX->0 if none)
     — SOLO blank-produce ✓ | redo: [ ] +3din [ ] +7din

   - Longest Substring Without Repeating (VARIABLE window + unordered_map<char,int> count; dup pe while-shrink; count 0 -> erase)
     — SOLO produce, 1 hint on erase-syntax (KEY se erase). | redo: [ ] +3din [ ] +7din
     (yaad: s[i]=KEY (char), mp[s[i]]=VALUE (count). mp.erase(s[i]) -> KEY se, value se nahi.)

   ★ PATTERN (Arpan-generalized): SKELETON same -> right add -> [check] -> record + left shrink.
     FIXED = "if (size==k)" + track SUM  |  VARIABLE = "while (condition)" + track LEN. bas if->while, sum->len.
     VARIABLE+dup = window me set/map se track -> "while (dup)" shrink. skeleton wahi, condition rich.
```
