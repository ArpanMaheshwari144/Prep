# Day 1 — RECURSION

**Date:** 2026-04-19
**Time:** ~2 hours
**Problems:** 4 (all Accepted, all 0ms beats 90-100%)
**Status:** ✅ Concept LOCKED

---

## 🎭 ANALOGIES THAT CLICKED

### **Boxes analogy (went deep)**
> Dark room mein bade boxes — tu khol ke andar jata, chhota milta, aur khola, chhota... chabhi milti sabse neeche. Chabhi le ke wapas aata.

**Kya represent karta:**
- Bada → chhota box = smaller problem (recursive call)
- Chabhi milna = base case (stop condition)
- Wapas aana = return value bubble up / stack unwind

### **CEO + team counting analogy (delegation)**
> CEO khud count nahi karta. Manager se poochta. Manager TL se. TL employee se. Sabse neeche employee "main 1" bolta. Wapas upar — har level apna +1 add karte upar bheje.

**Kya represent karta:**
- Delegation = recursion on smaller input
- Base case = bottom person answers
- Upar jate each level builds = return value combining

---

## 🧠 2 RECURSION MODELS I NOW OWN

### **MODEL 1 — Side-effect Recursion (counter going DOWN)**

**Pattern:**
- Class-level variable
- Traverse, update variable at each step
- Return value not meaningful for most calls

**Essence:** "Mere paas note-book hai, har visited node pe likhta jaunga."

**My natural thinking matched this** (box scene: "neeche se count karunga" was actually MODEL 2 — the delegation one).

**Real problems I did (side-effect):**
- #230 Kth Smallest BST (count via inorder)
- #102 Level Order (class-level result vector)

---

### **MODEL 2 — Return-value Recursion (delegation + bubble UP)**

**Pattern:**
- Base case returns small answer
- Parent combines left + right returns
- Answer flows UPWARD

**Essence:** "Mujhe pata nahi. Andar poochunga. Answer mila → upar bhejunga."

**Real problems I did (return-value):**
- #509 Fibonacci — fib(n) = fib(n-1) + fib(n-2)
- #231 Power of Two — isPowerOfTwo(n/2)
- #344 Reverse String — helper(left+1, right-1)
- #50 Pow(x, n) — half * half [optimized]
- #104 Max Depth Tree — 1 + max(left, right)
- #543 Diameter — height returned, diameter side-tracked (HYBRID)
- #98 Validate BST — bounds passed down, bool up
- #236 LCA — node returned up

---

## 🔑 3 CHEEZEIN HAR RECURSION MEIN

1. **BASE CASE** — kab rukna?
2. **RECURSIVE CALL** — smaller input pe khud ko call
3. **COMBINE/TRACK** — return value ya side-effect

---

## 📝 PROBLEMS SOLVED (with insights)

### **1. #509 Fibonacci Number** (Easy)
- Classic 2-branch delegation: fib(n-1) + fib(n-2)
- Base cases: fib(0)=0, fib(1)=1 (separate, not merged!)
- Runtime: 11ms
- **Lesson:** Merge base cases galat hota if values different. Separate if-elif karo.

### **2. #231 Power of Two** (Easy)
- Delegation with multiple base cases:
  - n == 1 → true
  - n <= 0 OR n % 2 != 0 → false
  - Recurse: isPowerOfTwo(n/2)
- Runtime: 2ms, beats 100%
- **Lesson:** Multiple base conditions clear logic banata.

### **3. #344 Reverse String** (Easy)
- In-place recursion with 2 pointers
- Base: left >= right → stop
- Swap + recurse on smaller range
- Runtime: 0ms, beats 100%
- **Lesson:** Two-pointer + recursion = clean shrinking pattern. Void function (no return), side-effect of swap.

### **4. #50 Pow(x, n)** (Medium)
- Optimized: x^n = x^(n/2) * x^(n/2) if even, * x if odd
- **O(log n)** instead of naive O(n)
- Edge case: negative n → convert x = 1/x, n = -n
- **Overflow trap:** n = INT_MIN, -n overflows. Use long long for n.
- Runtime: 0ms, beats 100%
- **Lesson:** Halving trick massive speedup. Type safety matters at INT boundaries.

---

## 🐛 BUGS I HIT (and learned from)

1. **#509 Fibonacci** — Merged base case `n==0 || n==1 → return 1` wrong. fib(0) should return 0.
2. **#50 Pow** — `n = (long long) -n` cast after `-n` — operation already overflowed in int. Fix: type expand BEFORE operation (helper function with long long param).

---

## 🎯 WHAT'S IN MY HEAD NOW

**Any recursion problem:**
- Base case → kab rukna?
- Can I divide into smaller version of same problem?
- Model 1 or Model 2? (counter or delegation?)
- Any edge cases (null, overflow, empty)?

**Interview-ready for recursion basics.** Advanced (backtracking, memoization) = future days.

---

## 📅 NEXT

Day 2 — Arrays / Linear Scan. Simple pattern but foundational.
