# Day 2 — ARRAYS / LINEAR SCAN

**Date:** 2026-04-20
**Time:** ~1 hour
**Problems:** 3 (all Accepted, all 0ms beats 100%)
**Status:** ✅ Concept LOCKED

---

## 🎭 ANALOGY

**Shelf ke saamne khada hu — 10 boxes line mein. Ek ek kholta, dekhta, notebook mein kuch note karta, aage.** End tak notebook mein answer.

**Minimal action:** box → decision → aage.

---

## 🧠 CORE PATTERN

**Linear Scan = 3 cheez:**
1. **Variable** (max, min, sum, count) — kuch track karo
2. **Loop** — start to end ek baar
3. **Decision + Update** — har element pe check + update

**Time O(n), Space O(1).**

**Essence:** *"Ek variable rakh, ek baar ghoom, har step pe update."*

---

## 🎯 3 VARIANTS I learned today

### **Variant 1 — Single tracker (classic max/min)**
- Example: Find max/min in array
- 1 variable, update at each step

### **Variant 2 — Streak with reset (#485 Max Consecutive Ones)**
- 2 variables: `current` streak, `max` streak
- If condition met → `current++`
- Else → `current = 0` (reset)
- Always: `max = max(max, current)`

### **Variant 3 — Track past best + current diff (#121 Buy/Sell)**
- 2 variables: `minPrice` (past best), `maxProfit` (best diff)
- Each step:
  - Update `minPrice` if lower
  - Calculate `price - minPrice` = potential profit
  - Update `maxProfit`

### **Variant 4 — Running sum with reset (#53 Kadane's)** ⭐
- 2 variables: `currentSum`, `maxSum`
- Each step:
  1. `currentSum += arr[i]`
  2. `maxSum = max(maxSum, currentSum)` — PEHLE update
  3. `if currentSum < 0: currentSum = 0` — phir reset
- **ORDER CRITICAL:** maxSum update **before** reset, warna all-negative array fail

---

## 📝 PROBLEMS SOLVED

### **1. #485 Max Consecutive Ones (Easy)**
- 2 vars: `currentOnes`, `maxOnes`
- If 1 → current++, update max
- If 0 → reset current
- Runtime: 0ms beats 100%

### **2. #121 Best Time to Buy/Sell Stock (Easy)**
- 2 vars: `minPrice = INT_MAX`, `maxProfit = 0`
- Each day: update minPrice, calculate profit, update maxProfit
- Single pass — classic "past best + current diff" pattern
- Runtime: 0ms beats 100%

### **3. #53 Maximum Subarray (Medium) ⭐**
- Kadane's Algorithm
- 2 vars: `currSum = 0`, `maxSum = INT_MIN`
- Each element: add, update max, reset if negative
- Runtime: 0ms beats 100%

---

## 🎯 WHAT'S IN MY HEAD NOW

**Any array problem:**
- Linear scan fit? (single pass possible?)
- Kya track karna — 1 variable, 2 variables, streak, running sum?
- Reset kab karna?
- Order matter karta? (like Kadane's max-before-reset)

**Future problems using this pattern:**
- Find Max/Min
- Running Sum
- Count Frequency
- Adjacent comparison (Check Sorted)
- Kadane's variants (Maximum Product Subarray)

---

## 📅 NEXT

Day 3 — **Two Pointer** (Valid Palindrome, Container with Most Water, etc.)

Similar to linear scan but with 2 pointers moving from ends OR same direction.
