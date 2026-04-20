# Day 3 — TWO POINTER

**Date:** 2026-04-20 (afternoon — Day 2 ke baad same day continued)
**Time:** ~1 hour
**Problems:** 3 (all Accepted)
**Status:** ✅ Concept LOCKED

---

## 🎭 ANALOGY

**Book padh raha hu — 2 fingers use karta.**
- Flavor 1: Ek start pe, ek end pe — converging (palindrome)
- Flavor 2: Dono same taraf, slow/fast — scan (duplicates)

**Minimal scene — 2 fingers, 2 positions, controlled move.**

---

## 🧠 CORE PATTERN

**Two Pointer = 2 pointers, 1 loop, decision har step pe kaunsa move kare.**

### **Flavor 1 — Opposite Ends (converging)**
- `left = 0, right = n-1`
- Loop `while left < right`
- Decision: kaun move kare based on condition
- Use: palindrome check, container water, sorted 2-sum

### **Flavor 2 — Same Direction (slow/fast)**
- `slow = 0, fast = 1` (or 0 and 0)
- Loop while `fast < n`
- Slow moves only on condition; fast always moves
- Use: remove duplicates, move zeroes, middle of LL

---

## 🎯 "Brute Force → Two Pointer" Mental Model

Jab O(n²) pairs check karna ho → sochao:
- **Array sorted hai?** → opposite ends with condition-based move (most problems)
- **In-place rearrange?** → slow/fast

Two pointer cuts O(n²) → O(n).

---

## 📝 PROBLEMS SOLVED

### **1. #125 Valid Palindrome (Easy)**
- 2 pointers: left=0, right=n-1
- Skip non-alphanumeric on both sides
- Compare lowercase, mismatch → false
- Runtime: 0ms beats 100%
- **Lesson:** Skip logic in both if-else, then compare.

### **2. #26 Remove Duplicates from Sorted Array (Easy)**
- Same-direction two pointer (slow=0, fast=1)
- On different value: slow++, nums[slow] = nums[fast]
- Fast always ++
- Return slow + 1
- Runtime: 0ms beats 100%
- **Lesson:** "Slow = where to write, Fast = where to read." Sorted array + consecutive dups exploit.

### **3. #11 Container With Most Water (Medium) ⭐**
- Opposite ends with SMART MOVE
- area = width × min(heights)
- Move the SHORTER bar (optimistic, only shorter can improve)
- Runtime: 5ms
- **Lesson:** Greedy choice — move shorter because longer move = guaranteed loss. Interview classic.

---

## 🔑 WHAT'S IN MY HEAD NOW

**Any array problem:**
- Can I use 2 pointers instead of nested loops?
- Opposite ends (sorted data?) or same direction (in-place rearrange?)
- What's the move decision rule?

**Future patterns using two pointer:**
- 3Sum, 4Sum (fix one, two-pointer rest)
- Sort Colors (Dutch flag)
- Linked List cycle detection (Floyd's — already done)
- Middle of LL (already done)
- Reverse LL (already done)
- Longest substring without repeating (sliding window variant)

---

## 📅 NEXT

Day 4 — **Sliding Window** (similar to same-direction two pointer but with different "window" concept for max/min within range)
