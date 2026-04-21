# Day 4 — SLIDING WINDOW

**Date:** 2026-04-21 (morning, fresh dimag se)
**Time:** ~2 hours
**Problems:** 4 (all Accepted)
**Status:** ✅ Concept LOCKED — dono flavors cover

---

## 🎭 ANALOGY

**Train ki khidki** — size fixed (ya variable), aage-peechhe slide karti hai.
- Jo **nikal gaya** left se → minus
- Jo **aaya** right se → plus
- Pura recalculate mat karo — bas edges track karo

**Minimal scene — ek frame, elements flowing through, edge updates only.**

---

## 🧠 CORE PATTERN

**Sliding Window = 2 pointers (i, j), running value (sum/count/map), update on slide.**

Brute force O(N·K) — har window ka scratch se compute.
Sliding window O(N) — pichhle window ka result use karo.

---

## 🎯 2 FLAVORS

### **Flavor 1 — Fixed Size Window (K given)**
- Window size = K (constant)
- Slide by 1: add new, minus old
- Track max/min of some metric
- Example: Max sum of K elements, Max avg, Max vowels in K-substring

### **Flavor 2 — Variable Size Window (condition-based)**
- Window grow/shrink based on condition
- `j++` to grow; `i++` in while loop to shrink
- Shrink condition is problem-specific
- Example: Longest substring no repeat, Min subarray sum ≥ target

---

## 📋 UNIVERSAL TEMPLATE

```cpp
int i = 0, j = 0;
while (j < n) {
    // 1. ADD: include nums[j] in window
    
    // 2. SHRINK: 
    //    - Fixed: if (j-i+1 > k) → remove, i++
    //    - Variable: while (<condition>) → remove, i++
    
    // 3. UPDATE: track ans (on valid window)
    
    j++;
}
```

**Order: ADD → SHRINK → UPDATE → j++**  
**Order change = bugs.**

---

## 📝 PROBLEMS SOLVED

### **1. #643 Maximum Average Subarray I (Easy, Fixed)**
- Fixed K window, running sum, return max avg as double
- **Bugs I made:**
  - Initially reused `sum` variable for both running sum and avg → corrupted
  - `int / int` lost decimals → used `(double)sum / k` cast
  - `maxAvg = 0.0` wrong for all-negative windows → used `INT_MIN`
- Runtime: 4ms, Beats 25%
- **Lesson:** Core logic vs edge cases are **separate debugging concerns**. Get logic right first, then types + initial values.

### **2. #1456 Maximum Number of Vowels in Substring of Given Length (Easy, Fixed)**
- Fixed K window, count vowels, track max count
- **Bug I made:** Nested window size check **inside** `isVowel(s[j])` block
  - Result: when s[j] was NOT vowel, `i++` never happened → window kept growing past K → condition `j-i+1 == k` never re-hit after first mismatch
  - `ans` stayed `INT_MIN` → returned `-2147483648`
- Runtime: Accepted after reorder
- **Lesson:** Shrink logic must fire **every iteration**, not only on some element property. Window lifecycle is independent of element value.

### **3. #3 Longest Substring Without Repeating Characters (Medium, Variable)** ⭐
- Variable window using `unordered_set`
- Shrink condition: `s[j]` already in set
- **Bug I made:** Insertion order wrong
  - Did `insert → update → shrink` (shrink always true because I just inserted!)
  - Shrink became infinite-delete loop
- Fix: Reorder to `shrink → insert → update`
- Runtime: Accepted
- **Lesson:** **Order of operations matters.** Same ops, different sequence = different behavior. When stuck, swap order and trace again.

### **4. #209 Minimum Size Subarray Sum (Medium, Variable)**
- Variable window, shrink while `sum >= target`, track min length
- Edge case: if never valid, return 0 (used `INT_MAX` sentinel)
- Runtime: 0ms, **Beats 100%** (first shot, zero debugging)
- **Lesson:** After 3 problems, pattern internalized. Clean execution possible when pattern + order clear upfront.

---

## 🚨 META-SKILL UNLOCKED TODAY — DEBUGGING RULES

### **Rule 1 — Intent ≠ Execution (Confirmation Bias)**

My biggest discovery today:

> **Visualization is superpower for WRITING code. But for DEBUGGING my own code — it becomes a trap.**

When I dry-run my own buggy code mentally, my brain replays **what I intended**, not **what's actually written**. I "see" sliding and i++ happening, even when the code has them nested wrong and they never fire.

**Concrete evidence:** In #1456, I mentally traced my buggy code and got answer 3. Actual execution: `INT_MIN`. My mental sim filled in gaps with intent.

**Cure:** Switch to **literal mechanical trace mode** when debugging.
- Finger on line number
- Read aloud: "Line 9, sum=1, j=0, condition... skip"
- Slow, boring, bulletproof
- Don't assume state, don't skip variables

### **Rule 2 — Order Matters**

Same operations, different sequence = different behavior. When something fails silently, **swap order and trace again**.

Example from today: `insert → shrink` vs `shrink → insert` — one works, one infinite-loops.

### **Rule 3 — Adversarial Input**

Don't dry-run with happy-path input. Test with input that **exposes bugs**:
- If code checks vowels, try string starting with non-vowel
- If code handles empty, try empty input
- If integer division suspected, try values where decimal matters

---

## 🔑 WHAT'S IN MY HEAD NOW

**Any array/string "find something in contiguous range" problem:**
- Can sliding window fit?
- Fixed (K given) or Variable (condition given)?
- What do I track? (sum, count, map?)
- Shrink condition — kab chhoti karni hai window?
- Update — kab ans track karna?

**And when debugging any code:**
- Stop visualizing
- Read literal
- Test adversarial input
- Swap order if stuck

---

## 🏆 CONFIDENCE CHECK

Today's peak moment: **#209 submitted 0ms Beats 100% in first shot, zero debug**. Proof that pattern locked.
Today's growth moment: **Caught myself doing confirmation bias on #1456**. Named the bug honestly. This meta-awareness is the real win.

Day 4 DONE. Cumulative: 14 problems, 4 concepts locked in 3 days.
