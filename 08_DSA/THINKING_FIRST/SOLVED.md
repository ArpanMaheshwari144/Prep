# THINKING-FIRST — Solved Log

> Active-derive practice. Har problem ka APPROACH jo KHUD derive kiya (code baad mein).
> Yeh tracker + navigator: signal → pattern → approach.

---

## #1 — Contains Duplicate   (Easy | Hashing)
- **Signal:** "koi value repeat hui?" → Hashing (set/map)
- **Approach (derived):** map mein count store; jab count > 1 → `true`; warna `false`.
- Note: set se bhi ho jaata ("pehle dekha?") — same O(n), code thoda chhota (count nahi).

## #2 — Valid Anagram   (Easy | Hashing)
- **Signal:** "same letters + same counts, bas alag order?" → Hashing (count map)
- **Approach (derived):** s ke char-counts map mein bharo; phir t ke har char pe `count--`;
  end mein **sab 0 → true**, warna `false`.
- Note: different-length / extra-char khud handle ho jaata (counts balance nahi → false).

## #3 — Two Sum (unsorted)   (Easy | Hashing)
- **Signal:** "do numbers ka jod = target, unsorted" → Hashing (complement map)
- **Approach (derived):** map mein value→index store karte jao. Har num pe check: (target − num) map mein hai?
  haan → dono index return; nahi → current ko map mein daal, aage badho.
- **Key:** complement = target − current. Map se O(1) check.

## #4 — Group Anagrams   (Medium | Hashing)
- **Signal:** "anagrams ko saath group karo" → Hashing (sorted-string as key)
- **Approach (derived):** har word ko SORT karo → sorted-string = KEY → `map[key]` mein word push.
  Same anagram → same sorted-key → ek group. Map ki VALUES = answer.
- **Trick:** sorted version = anagrams ki common PEHCHAAN.

## #5 — Top K Frequent Elements   (Medium | Hashing)
- **Signal:** "K sabse zyada baar aane wale" → Hashing (frequency map) + top-k
- **Approach (derived):** map mein frequency (element → count) → phir top-k frequency waale nikaalo.
- **Trick (top-k nikaalna):** frequency-se-sort (O(n log n)) YA **bucket-sort** (index = frequency,
  peeche se k uthao → O(n), interview-favourite).

## #6 — Product of Array Except Self   (Medium | Prefix × Suffix)
- **Signal:** "har point pe baaki SAB ka guna, NO division" → prefix × suffix
- **Approach (derived):** left[] = prefix products → `left[i]=prod; prod*=nums[i]` (pehle store, phir self).
  right[] = suffix products (ulti taraf, same trick). `result[i] = left[i] * right[i]`.
- **Key:** "pehle store (before ka product), phir apna daalo" → isse self exclude hota.
- **Space-opt (FYI):** result[] mein left bharo, phir suffix-VARIABLE se peeche-se multiply → O(1) extra.

---

> **Status:** 6 done (5 Hashing + 1 Prefix-Suffix, sab khud derive). Aage yahin add hota rahega.
