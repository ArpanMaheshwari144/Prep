# DSA Complete Notes — Arpan Maheshwari
### 68 Questions | 13 Patterns | Built from Scratch with Dry Runs
### "Dry Run = DSA ka 95%. Visualize kar, code khud aayega."

---

# TABLE OF CONTENTS

1. [Linear Scan](#1-linear-scan)
2. [Two Pointer](#2-two-pointer)
3. [Sliding Window](#3-sliding-window)
4. [Hashing](#4-hashing)
5. [Prefix Sum](#5-prefix-sum)
6. [Binary Search](#6-binary-search)
7. [Stack](#7-stack)
8. [Recursion & Backtracking](#8-recursion--backtracking)
9. [Linked List](#9-linked-list)
10. [Trees](#10-trees)
11. [Queue](#11-queue)
12. [Graphs](#12-graphs)
13. [All Templates — Ek Jagah](#13-all-templates)
14. [Common Galtiyan — Master List](#14-common-galtiyan)
15. [Pattern Recognition Cheat Sheet](#15-pattern-recognition-cheat-sheet)
16. [Progress Tracker](#16-progress-tracker)

---

# 1. LINEAR SCAN

## Kya hai?
Sabse basic pattern. Array mein **ek baar start se end tak ghoom** aur kuch kaam kar. Bas 3 cheezein:
- **Loop** — har element dekh
- **If** — decide kar kaam ka hai ya nahi
- **Variable** — kuch yaad rakh (max, sum, count)

## Real Life Example
Tu ek shelf pe rakhey boxes dekh raha hai. Ek ek box khol ke dekh. Jo kaam ka hai wo yaad rakh. End mein answer de.

## Kab use karna hai?
- "Sabse bada/chhota dhundh" → max/min track
- "Sum nikaal" → variable mein add
- "Kitni baar aaya?" → count
- "Sorted hai ya nahi?" → adjacent pairs compare
- "Maximum subarray sum?" → Kadane's

## Time & Space
- **Time: O(n)** — ek baar ghoom
- **Space: O(1)** — sirf 1-2 variables

## Template
```
int variable = starting_value;
for(int i = 0; i < n; i++){
    if(condition){
        variable update
    }
}
return variable;
```

## Sub-patterns

### 1. Ek Variable Track (Max, Min, Sum, Count)
```
Find Max:    maxNum = INT_MIN. nums[i] > maxNum → update.
Find Sum:    sum = 0. sum += nums[i].
Count:       count = 0. nums[i] == target → count++.
```

### 2. Do Variables Track (Second Max)
```
maxNum = INT_MIN, secondMaxNum = INT_MIN
nums[i] > maxNum → secondMaxNum = maxNum, maxNum = nums[i]  (shift down)
else if nums[i] > secondMaxNum && nums[i] != maxNum → secondMaxNum = nums[i]
```
**Kyun `!= maxNum`?** Duplicates mein max aur secondMax same ho jayenge warna.

### 3. Adjacent Pairs Compare (Check Sorted)
```
i = 1 se start (0 nahi — pehle wala chahiye compare ke liye)
nums[i-1] > nums[i] → return false
Loop khatam → return true
```

### 4. Kadane's Algorithm (Max Subarray Sum)
**Real Life:** Business mein profit/loss. Kaunse consecutive dinon mein sabse zyada kamaya?

**Har element pe decision:** purane sum ke saath continue ya fresh start?
```
sum = 0, maxSum = INT_MIN
sum += nums[i]
maxSum = max(maxSum, sum)    ← PEHLE update
if(sum < 0) sum = 0          ← PHIR reset
```
**ORDER MATTERS:** Pehle maxSum update, phir reset. Ulta kiya → all-negative fail.

## Questions (9)
| # | Question | Key Trick |
|---|----------|-----------|
| 01 | Find Max | INT_MIN se start, compare & update |
| 02 | Find Max Index | maxNum + index saath track |
| 03 | Array Sum | sum += nums[i] |
| 04 | Even Sum | nums[i] % 2 == 0 check |
| 05 | Count Target | nums[i] == target → count++ |
| 06 | Has Duplicate | HashSet — set mein hai → duplicate |
| 07 | Second Max | 2 variables, shift down, != maxNum |
| 08 | Check Sorted | nums[i-1] > nums[i] → false |
| 09 | Kadane's | sum + maxSum, sum < 0 → reset. Pehle update PHIR reset |

## Galtiyan
1. **maxNum = 0 se start** — negative array fail. INT_MIN use kar.
2. **return true loop ke andar** — pehle hi pair mein return, poora check nahi.
3. **Second max mein != maxNum bhoolna** — duplicates galat.
4. **Kadane's mein pehle reset phir update** — all-negative fail.
5. **i=0 se start jab i-1 chahiye** — crash.

---

# 2. TWO POINTER

## Kya hai?
Do pointers — dono apni jagah se kaam karte hain.

## Real Life
Tu aur tera dost. Ek raste ke dono end pe khade ho. Dono ek dusre ki taraf chalo (opposite). Ya saath chalo ek tez ek dheema (same direction).

## 3 Types

### Type 1: Opposite Direction
```
 →          ←
low        high
```
Dono sides se aao. Swap ya compare.

**Template:**
```
int low = 0, high = n-1;
while(low < high){
    // swap ya compare
    low++; high--;
}
```

**Use:** Reverse, Palindrome, Two Sum Sorted, Container Water
**`<` kyun `<=` nahi?** low==high pe ek hi element — swap/compare bekar.

### Type 2: Same Direction (Slow-Fast)
```
 →  →
slow fast
```
**Real Life:** Factory conveyor belt. Fast har item check karta. Slow khaali dabba leke khada — kaam ka mile toh rakh.

**Template:**
```
int slow = 0, fast = 0;
while(fast < n){
    if(kaam ka hai){
        assign ya swap
        slow++;
    }
    fast++;
}
```
**IMPORTANT:** Pehle assign/swap, PHIR slow++. Ulta → pehli position skip.

**Use:** Remove duplicates, Move zeroes, Remove element

### Type 3: Two Arrays
```
End se start. Bada element peeche daalo.
```
**Template:**
```
int i = m-1, j = n-1, k = m+n-1;
while(i >= 0 && j >= 0){
    bada wala nums1[k] pe. k--;
}
while(j >= 0) nums1[k] = nums2[j]; j--; k--;
```
**Kyun end se?** Start se karo toh purane elements overwrite.
**Equal case:** `<=` ya `>=` use kar — sirf `<` `>` se infinite loop.

## Questions (8)
| # | Question | Type | Key Trick |
|---|----------|------|-----------|
| 10 | Reverse Array | Opposite | swap(low, high) |
| 11 | Palindrome | Opposite | s[low] != s[high] → false |
| 12 | Two Sum Sorted | Opposite | sum bada → high--, chhota → low++ |
| 38 | Container Water | Opposite | min(h[l],h[r]) * distance, chhoti wall move |
| 13 | Remove Duplicates | Same | nums[fast] != nums[slow] → slow++, assign |
| 14 | Move Zeroes | Same | nums[fast] != 0 → swap, slow++ |
| 15 | Remove Element | Same | nums[fast] != val → assign, slow++ |
| 16 | Merge Sorted | Two Arrays | End se, bada peeche, k=m+n-1 |

## Galtiyan
1. **fast=1 se start** — pehla element skip.
2. **Pehle slow++ phir assign** — pehli position skip.
3. **k = m-n+1** — GALAT. k = m+n-1.
4. **nums1[j] likhna nums2[j] ki jagah** — j nums2 ka pointer.
5. **maxArea = area likhna** — overwrite! max(maxArea, area) kar.

---

# 3. SLIDING WINDOW

## Kya hai?
Ek window jo slide karti hai. Left se ek nikalta, right se ek aata. Pura dobaara calculate nahi — bas update karo.

## Real Life
Train ki window. Scene slide hota hai — purana jaata, naya aata.

## 2 Types

### Fixed Size (k diya)
**Template — ORDER MAT BADLO:**
```
while(j < n){
    sum += nums[j];              // 1. ADD
    if(j-i+1 > k){              // 2. SHRINK
        sum -= nums[i]; i++;
    }
    if(j-i+1 == k){             // 3. UPDATE
        maxSum = max(maxSum, sum);
    }
    j++;                         // 4. j++
}
```
**Maine khud dry run se discover kiya:** pehle update phir shrink kiya → galat. **ADD → SHRINK → UPDATE → j++.**

### Variable Size (condition based)
**Template:**
```
while(j < n){
    mp[s[j]]++;                  // 1. ADD
    while(condition toote){      // 2. SHRINK (WHILE, if nahi!)
        mp[s[i]]--; if(0) erase; i++;
    }
    if(valid) ans = max(ans, j-i+1);  // 3. UPDATE
    j++;                         // 4. j++
}
```
**if vs while:** Fixed mein if (ek element). Variable mein while (multiple shrink).

**Shrink mein confusion tha:** i pe same character ho ya alag — farak nahi padta. While loop khud handle karega.

## Questions (4)
| # | Question | Type | Shrink | Update |
|---|----------|------|--------|--------|
| 17 | Max Sum K | Fixed | j-i+1 > k | j-i+1 == k |
| 18 | Max Avg K | Fixed | same | double use karo |
| 19 | Longest No Repeat | Variable | mp[s[j]] > 1 | har valid window |
| 20 | Longest K Unique | Variable | mp.size() > k | mp.size() == k |

## Galtiyan
1. **Order galat** — ADD → SHRINK → UPDATE → j++. KABHI MAT BADLO.
2. **if lagana while ki jagah (variable)** — ek baar shrink kaafi nahi.
3. **mp.erase() bhoolna** — mp.size() galat rahega.
4. **Update mein condition na lagana** — invalid window count hogi.
5. **int use karna jab double chahiye** — average mein decimal katega.

---

# 4. HASHING

## Kya hai?
Dictionary — seedha "M" section khol. O(1) lookup.

**HashSet** — "hai ya nahi?" (haan/nahi)
**HashMap** — "hai toh kahan?" ya "kitni baar?" (key:value)

## Complement Search Pattern
```
for(int i = 0; i < n; i++){
    int complement = target - nums[i];
    if(mp.count(complement)) return {mp[complement], i};
    mp[nums[i]] = i;
}
```
**"Mera complement map mein hai? Haan → mil gaya. Nahi → khud ko daal de."**

## Two Pointer vs Hashing
```
Two Sum SORTED   → Two Pointer (O(1) space)
Two Sum UNSORTED → HashMap (O(n) space)
```

## Galtiyan
1. **mp[nums[i]] likhna complement ki jagah** — complement ka index chahiye.
2. **push_back mein pair daalna** — ans[0], ans[1] use kar.
3. **Running sum banana** — Two Sum mein individual elements ka pair, running sum nahi.

---

# 5. PREFIX SUM

## Kya hai?
**Real Life:** Shopkeeper. Pehle se total bana ke rakh. Phir koi bhi range sum O(1).

## Formula
```
prefix[0] = nums[0]
prefix[i] = prefix[i-1] + nums[i]

sum(L, R) = L == 0 ? prefix[R] : prefix[R] - prefix[L-1]
```

## Galtiyan
1. **prefix[0] = 0 ya 1** — prefix[0] = nums[0].
2. **L=0 handle na karna** — prefix[-1] crash.

---

# 6. BINARY SEARCH

## Kya hai?
**Real Life:** Dictionary. Beech mein khol, aadha kaat. 1000 pages, 10 steps.
```
Linear: O(n). 10 lakh = 10 lakh steps.
BS:     O(log n). 10 lakh = 20 steps.
```
**ZAROORI: Array SORTED hona chahiye.**

## 3 Types

### Simple BS
```
while(low <= high){
    mid = low + (high-low)/2;
    nums[mid] == target → return mid
    nums[mid] < target  → low = mid+1
    nums[mid] > target  → high = mid-1
}
return -1;
```

### Boundary Finding (First/Last Position)
Target mila toh **return mat karo — aur dhundho.** 2 BS lagao.
```
Pehli position: target mila → ans[0]=mid, high=mid-1 (LEFT jao)
Aakhri:         target mila → ans[1]=mid, low=mid+1 (RIGHT jao)
```

### BS on Answer
Answer ki range pe BS. Helper function se check.
```
low = min_possible, high = max_possible
canDo(mid) true  → ans=mid, high=mid-1 (minimum dhundh)
canDo(mid) false → low=mid+1
```

**CONTINUOUS (Ship Packages):** Add karte jao, capacity cross → day++, sum=weights[i]. **day=1** se start.
**INDEPENDENT (Koko Bananas):** ceil(pile/speed) add karo. **hours=0** se start. **(double) cast** zaroori.

## `<=` kyun?
```
[5] target=5. while(low < high) → 0 < 0? NAHI → miss!
while(low <= high) → 0 <= 0? HAAN → found!
```
**BS mein HAMESHA `<=`.**

## Questions (5)
| # | Question | Type | Key Trick |
|---|----------|------|-----------|
| 23 | Binary Search | Simple | target → return mid |
| 24 | First/Last Position | Boundary | 2 BS, return mat karo |
| 25 | Square Root | BS on Answer | mid*mid <= x? Return high |
| 26 | Ship Packages | BS on Answer | CONTINUOUS, day=1, sum=weights[i] |
| 27 | Koko Bananas | BS on Answer | INDEPENDENT, hours=0, (double) cast |

## < vs <= — Poore DSA Mein
| Pattern | Condition | Kyun |
|---|---|---|
| Two Pointer Opposite | `<` | milne pe kaam khatam |
| Scanner (sort colors) | `<=` | har element dekhna hai |
| Binary Search | `<=` | last element bhi check |
| Sliding Window | `j < n` | j end tak |

## Galtiyan
1. **`<` lagana `<=` ki jagah** — last element miss.
2. **`(low+high)/2`** — overflow. `low+(high-low)/2`.
3. **Boundary mein turant return** — aur dhundhna hai.
4. **Ship mein day=0** — pehla din miss. day=1.
5. **Koko mein hours=1** — extra hour. hours=0.
6. **Integer division + ceil** — `(double)` cast zaroori.

---

# 7. STACK

## Kya hai?
Plates ka stack. Upar se rakh, upar se utha. **LIFO.**

## Valid Parentheses
Open → push. Close → top se match. End mein khaali → true.
```
if(open) st.push(ch)
else if(!st.empty() && match) st.pop()
else return false
return st.empty()
```

## Next Greater Element
Stack mein **INDEX** push. Bada aaye → **while** se pop, answer daalo.
```
while(!st.empty() && nums[st.top()] < nums[i]){
    ans[st.top()] = nums[i]; st.pop();
}
st.push(i);
```
**3 important:** INDEX push (value nahi). WHILE (if nahi). Har element push (chhota bhi).

## Min Stack
2 stacks — st1 normal, st2 min track.
```
Push: st2 khaali → val push.
      val > st2.top() → st2.top() push (min same).
      val <= st2.top() → val push (naya min).
Pop:  dono se pop.
getMin: st2.top().
top: st1.top() (st2 NAHI!).
```

## Galtiyan
1. **st.top() bina empty check** — crash.
2. **Value push, index nahi (NGE)** — answer kahan daalna pata nahi.
3. **if lagana while ki jagah (NGE)** — multiple pop miss.
4. **top() mein st2.top()** — wo min hai, top nahi.

---

# 8. RECURSION & BACKTRACKING

## Kya hai?
**Real Life:** Mirror mein mirror. Khud ko chhoti problem de, jab tak base case.

## 2 cheezein zaroori:
1. **Base case** — kab rukna hai
2. **Recursive call** — chhoti problem de

## Basic Recursion
```
solve(n):
    if(base case) return answer
    return kuch_kaam + solve(n-1)
```

### Patterns:
- **Sum:** n + sumN(n-1). Base: n==1.
- **Factorial:** n * fact(n-1). Base: n==0 or 1.
- **Fibonacci:** fib(n-1) + fib(n-2). Base: n==0→0, n==1→1.
- **Power:** base * power(base, exp-1). Base: exp==0→1.
- **Array Sum:** nums[index] + arraySum(index+1). Base: index==n→0.
- **Reverse String:** reverseStr(s.substr(1)) + s[0]. Base: size<=1.

## Pick / Not-Pick (SABSE IMPORTANT — DP ka base)
**Real Life:** Dukaan. Har item — LO ya MAT LO.

```
solve(index):
    if(index == n): result mein current daal. return.
    
    // PICK
    current.push_back(nums[index])
    solve(index + 1)
    
    // BACKTRACK
    current.pop_back()
    
    // NOT-PICK
    solve(index + 1)
```
**Backtrack = jo kiya wo undo karo. Push kiya toh pop. Add kiya toh subtract.**

## Questions (9)
| # | Question | Type |
|---|----------|------|
| 31 | Sum of N | Basic |
| 32 | Factorial | Basic |
| 33 | Fibonacci | Basic |
| 34 | Power | Basic |
| 35 | Array Sum Recursive | Array |
| 36 | Reverse String | String |
| 37 | Palindrome Recursive | String (pichla pattern reuse!) |
| 39 | Subsets | Pick/Not-Pick |
| 40 | Subset Sum Target | Pick/Not-Pick + condition |

## Galtiyan
1. **Base case bhoolna** — infinite recursion.
2. **index-1 likhna index+1 ki jagah** — peeche jayega.
3. **Backtrack bhoolna** — push kiya toh pop karo.
4. **Empty string ka base case** — substr(1) crash.

---

# 9. LINKED LIST

## Kya hai?
**Real Life:** Train ke dabbe. Har dabba mein data + next pointer.
```
[10] → [20] → [30] → NULL
```

## Node:
```cpp
struct ListNode {
    int val;
    ListNode* next;
};
```

## Reverse LL — 3 Pointers
```
prev=NULL, curr=head
while(curr != NULL):
    next = curr->next    ← agla yaad rakh
    curr->next = prev    ← pointer ulta
    prev = curr          ← prev aage
    curr = next          ← curr aage
return prev              ← PREV return, curr nahi (curr NULL hai)
```

## Middle LL — Slow-Fast
```
while(fast != NULL && fast->next != NULL):
    slow = slow->next
    fast = fast->next->next
return slow
```
**DONO check:** `fast != NULL && fast->next != NULL`.

## Detect Cycle — Slow-Fast
Same slow-fast. Mile toh cycle. Fast NULL toh no cycle.

## Merge Two Sorted LL
```
newhead = l1. l1 = l1->next. dummy = newhead.
Chhota element dummy->next pe. dummy aage.
```
**Copy pe dry run se seekha:** `l1 = l1->next` — ye line cycle se bachati hai.

## Remove Nth from End
Count nikaal. count==n → return head->next. Warna slow-fast.

## Palindrome LL — Middle + Reverse (COMBO)
**Pattern combine karta:** `#876 Middle` + `#206 Reverse`. Interview favorite.

```
Step 1: Middle nikaal (fast/slow)
Step 2: Middle se aage WALA half reverse kar
Step 3: p1 = head, p2 = reversed head
        Simultaneously chalao. p2 khatam hote hi stop.
        Kahi bhi val mismatch → false. All match → true.
```

**DRY RUN [1,2,3,2,1]:**
```
slow = 3 (middle)
reverse(3): 3→2→1 ⟶ 1→2→3
p1 = head(1), p2 = reversed(1)
  1==1 ✓ → advance
  2==2 ✓ → advance
  3==3 ✓ → p2 NULL → STOP
return TRUE ✓
```

**Kyun `while(p2)` NOT `while(p1)`?**
Odd length mein first half ek extra — middle. Par middle matter nahi karta palindrome mein. `p2` khatam hote hi stop kaafi.

---

## PATTERNS MASTERED — Linked List (Apr 17, 2026)

| Pattern | LeetCode | Key Insight |
|---------|----------|-------------|
| **Floyd's Cycle** (slow/fast) | #141 | Fast 2x → meet = cycle. No meet = NULL. |
| **3-pointer Reverse** | #206 | prev, curr, next. Return prev (curr NULL). |
| **Dummy Node Merge** | #21 | Dummy removes head edge case. |
| **Middle (fast/slow)** | #876 | Same Floyd's, different use. Second middle for even. |
| **Middle + Reverse Combo** | #234 Palindrome | Step 1: middle. Step 2: reverse 2nd half. Step 3: compare. |
| **Path Equalize (head swap)** | #160 Intersection | Pointer null hit → switch to other list's head. Total distance same → meet at intersection (ya NULL). |
| **Gap Trick + Dummy Node** | #19 Remove Nth from End | Fast ko n+1 steps aage, phir dono saath chalao. Fast NULL → slow deletion se 1 pehle. Dummy = head delete edge case. |

**INTERVIEW FREQUENCY:** #141, #206, #21, #160, #19 — TOP FAANG favorites. #160 head-swap aur #19 gap trick Google/Amazon famous.

**ONE-LINE PATTERNS:**
- Cycle? → slow/fast, meet check
- Middle? → slow/fast, fast end = slow middle
- Reverse? → 3 pointer, prev return
- Merge 2 sorted? → dummy + tail
- Palindrome? → middle + reverse + compare
- Intersection (O(1) space)? → pA/pB, null hit → switch to other head
- Remove Nth from End? → dummy + gap(n+1) + simultaneous walk

---

## #19 Remove Nth from End — Gap Trick DRY RUN

**Problem:** Nth from end node hatao. `[1,2,3,4,5], n=2 → [1,2,3,5]`

**Key rule:** Slow ko **deletion target se EK NODE PEHLE** rukna chahiye (kyunki `slow->next = slow->next->next` skip karta).

**Trick:** 
1. **Dummy node** banao → head delete edge case solve
2. Fast ko **n+1 steps** aage bhejo (gap = n+1)
3. Dono saath chalao jab tak fast = NULL
4. Skip karo: `slow->next = slow->next->next`

**Gap n+1 kyun, n nahi?** Agar gap n rakha → slow DELETION target pe khada hoga, pehle nahi. One step extra deletion hoga. `<=` vs `<` ka fark.

```
[1,2,3,4,5], n=2

dummy → 1 → 2 → 3 → 4 → 5 → NULL
  ↑
slow, fast

Step 1: fast ko n+1 = 3 steps aage
  i=0: fast=1
  i=1: fast=2
  i=2: fast=3

dummy → 1 → 2 → 3 → 4 → 5 → NULL
  ↑             ↑
 slow          fast   (gap = 3)

Step 2: while(fast != NULL), dono saath
  slow=1, fast=4
  slow=2, fast=5
  slow=3, fast=NULL → STOP

dummy → 1 → 2 → 3 → 4 → 5 → NULL
                ↑
               slow   ← '3' pe (deletion '4' se 1 pehle) ✓

Step 3: slow->next = slow->next->next
  3->next = 4 (target)
  3->next = 5
  
Result: dummy → 1 → 2 → 3 → 5 → NULL
return dummy->next = 1 ✓
```

**Edge case — Delete head, [1,2,3], n=3:**
```
fast moves 4 steps → NULL immediately
slow at dummy
slow->next = 1, slow->next = 2
Return dummy->next = 2 ✓

Dummy ke bina: special if check chahiye head delete ke liye.
```

**Pro tip (C++):** `delete nodeToDelete;` add karo — memory leak prevent. Interview mein extra points.

---

## #160 Intersection — Path Equalize DRY RUN

**Problem:** 2 LL intersect karti (same NODE, not value) — pehla shared node return.

**Trick:** Length differ → parallel walk fail. **Head swap** se dono ka total distance same → meet at intersection.

```
A: 4 → 1 → 8 → 4 → 5 → NULL     (len 5)
B: 5 → 6 → 1 → 8 → 4 → 5 → NULL  (len 6, shared from node '8')

pA = headA, pB = headB
while (pA != pB):
    pA null? → pA = headB. else → pA = pA->next
    pB null? → pB = headA. else → pB = pB->next
return pA
```

**Why same distance?**
- pA path: A_full (5) + B_unique (3) = 8
- pB path: B_full (6) + A_unique (2) = 8  
- SAME → must meet at same position

**No intersection case:** Dono NULL pe meet → while exit → return NULL. (null == null true)

---

## Questions (7)
| # | LeetCode | Question | Key Trick |
|---|----------|----------|-----------|
| 41 | #206 | Reverse | 3 pointers, return prev |
| 42 | #876 | Middle | slow-fast, dono NULL check |
| 43 | #141 | Detect Cycle | slow-fast, mile → cycle |
| 44 | #21 | Merge Sorted | dummy + tail, `tail->next = list1 ? list1 : list2` end mein |
| 45 | #19 | Remove Nth | dummy + fast n+1 steps aage + dono saath → slow deletion se 1 pehle. Gap trick. |
| 46 | #234 | Palindrome | middle + reverse 2nd half + compare (p2 khatam → stop) |
| 47 | #160 | Intersection | 2 pointer, null hit → switch to other head. Path equalize. |

---

# 10. TREES

## Kya hai?
**Real Life:** Family tree. Root upar, children neeche.
```
    1
   / \
  2   3
 / \
4   5
```

## DFS Traversals (teeno mein sirf order badal)
```
Inorder:   Left → Push → Right     (Root IN beech)
Preorder:  Push → Left → Right     (Root PRE pehle)
Postorder: Left → Right → Push     (Root POST baad)
```
**BASE CASE: if(root == NULL) return. BHOOLNA = CRASH.**

## Max Depth
```
1 + max(maxDepth(left), maxDepth(right))
Base: NULL → 0
```

## Balanced Tree
```
isBalanced(root):
    NULL → true
    !isBalanced(left) || !isBalanced(right) → false
    abs(leftDepth - rightDepth) > 1 → false
    return true
```
**3 galtiyan thi:** abs() bhoolna, sirf root check (children nahi), child false toh parent true return.

## Level Order (BFS) — Queue
```
queue mein root. while khaali nahi:
    size = q.size()    ← PEHLE save!
    for(size baar): nikaal, val daal, children push
```

## Invert Binary Tree — Pointer Swap
**Pattern:** Every node pe left ↔ right swap. Recursively sab nodes ke liye.

```
BEFORE:      AFTER:
    4          4
   / \        / \
  2   7  →   7   2
 /\  /\     /\  /\
1 3 6 9    9 6 3 1

invert(root):
    NULL → return NULL
    left = invert(root->left)
    right = invert(root->right)
    swap(root->left, root->right)    ← ACTUAL pointers swap, NOT local vars!
    return root
```
**GALTI:** `swap(l, r)` with LOCAL variables → swap kuch nahi karta tree pointers pe. `swap(root->left, root->right)` karo.

## Symmetric Tree — 2-Arg Mirror Recursion
**Pattern:** Single tree check nahi — tree ke LEFT aur RIGHT halves ek dusre ka MIRROR hain ya nahi. Helper `isMirror(l, r)` chahiye jo DO nodes saath compare kare.

```
SYMMETRIC:        NOT SYMMETRIC:
     1                1
    / \              / \
   2   2            2   2
  /\  /\             \   \
 3 4  4 3             3   3

isMirror(l, r):
    both NULL → true
    one NULL  → false
    vals diff → false
    recurse CROSS:
        isMirror(l.left, r.right)  ← outer pair
        isMirror(l.right, r.left)  ← inner pair
    both true → true
```
**Trick:** CROSS-check — `l.left` vs `r.right` aur `l.right` vs `r.left`. Ye "mirror" ka saar hai.

---

## PATTERNS MASTERED — Trees (Apr 18, 2026)

| Pattern | LeetCode | Key Insight |
|---------|----------|-------------|
| **Simple Recursion** | #104 Max Depth | `1 + max(left, right)`, base NULL→0 |
| **Pointer Swap** | #226 Invert | `swap(root->left, root->right)` — actual pointers, not locals |
| **2-Arg Mirror** | #101 Symmetric | Helper `isMirror(l, r)` — CROSS check l.L vs r.R |
| **BFS with Queue** | #102 Level Order | `size = q.size()` SAVE pehle, phir loop |
| **Return-Node LCA** | #236 LCA | `root==p/q → return root`. Both sides non-null → current is LCA |

**INTERVIEW FREQUENCY:** #104, #226, #236 — TOP 3. #226 Google famous. #236 LCA FAANG favorite.

**ONE-LINE PATTERNS:**
- Max depth? → `1 + max(left, right)`
- Invert? → Pointer swap on root, recurse both sides
- Symmetric? → Mirror helper with 2 args, cross-check
- Level order? → Queue BFS, size SAVE pehle
- LCA? → Base `p/q→root`, both non-null→LCA, else pass up

---

## #236 LCA — Return-Node Pattern DRY RUN

**Problem:** Find lowest common ancestor of p and q.

```
        3
       / \
      5   1
     /|   |\
    6 2   0 8
      |\
      7 4

LCA(6, 4):
  LCA(3, 6, 4):
    l = LCA(5, 6, 4):
      l = LCA(6, ...) → 6==p → RETURN 6
      r = LCA(2, 6, 4):
        r_inner = LCA(4, ...) → 4==q → RETURN 4
        → pass up: return 4
      l=6, r=4 (both non-null) → 5 is LCA!
      → return 5
    r = LCA(1, 6, 4) → both subtrees null → return null
    l=5, r=null → return 5 (pass up)
  
Return 5 ✓
```

**Magic:** Jaise hi p/q mile → upar propagate. Jab both sides se non-null aaye → current node = SPLIT = LCA.

**BASE CASES (critical):**
1. `root == NULL` → return NULL
2. `root == p` → return root (found p, pass up)
3. `root == q` → return root (found q, pass up)

**GALTI:** `root == p && root == q` wrong! Ek node ek saath 2 alag nodes nahi ho sakta. Use `||`.

---

## Questions (12)
| # | LeetCode | Question | Type / Key Trick |
|---|----------|----------|------------------|
| 43 | — | Inorder | DFS: Left → Push → Right |
| 44 | — | Preorder | DFS: Push → Left → Right |
| 45 | — | Postorder | DFS: Left → Right → Push |
| 46 | #104 | Max Depth | DFS: `1 + max(left, right)`, NULL→0 |
| 47 | — | Balanced Tree | DFS: abs(leftD - rightD) > 1 → false |
| 48 | #102 | Level Order | BFS: Queue, `size=q.size()` SAVE pehle, null root check |
| 49 | #236 | LCA | DFS: p/q→root, both non-null→LCA. `&&` nahi, `||` use |
| 50 | #226 | Invert Tree | DFS: `swap(root->left, root->right)` — pointers, not locals |
| 51 | #101 | Symmetric | DFS: 2-arg mirror helper, CROSS-check (l.L vs r.R, l.R vs r.L) |

---

# 11. QUEUE

## Kya hai?
Line mein khade log. **FIFO** — pehle aaya pehle gaya.
```
push → peeche se daal
front → aage wala dekh
pop → aage se nikaal
```
**BFS ka base hai Queue.** Level Order Traversal mein use kiya.

---

# 12. GRAPHS

## Kya hai?
**Real Life:** Cities aur roads. Nodes + Edges.

Tree vs Graph: Graph mein **cycles** ho sakte, **multiple paths**.

## Adjacency List:
```
0 --- 1
|     |
3 --- 2

adj[0] = [1, 3]
adj[1] = [0, 2]
```

## DFS — Recursion + Visited
```
dfs(node):
    visited[node] = true
    result.push_back(node)
    for neighbor: !visited → dfs(neighbor)
```

## BFS — Queue + Visited
```
queue.push(start). visited = true.
while queue: nikaal, result daal, neighbors push (if !visited)
```

## Number of Islands — DFS on Grid
Grid mein '1' mile → count++, DFS se sab connected '1' visited mark.
```
4 directions: {{1,0}, {0,1}, {-1,0}, {0,-1}}
canVis check: bounds + grid=='1' + !visited — TEENO zaroori
```

## Cycle Detection — DFS + Parent
```
Visited hai aur parent NAHI → CYCLE!
dfs(node, parent):
    visited = true
    for neighbor:
        !visited → dfs(neighbor, node). true → cycle!
        visited && neighbor != parent → cycle!
```
**Ek line ki wajah se bahut dikkat:** `== false` likhna `== true` ki jagah.

## Topological Sort — Kahn's (BFS + In-degree)
```
In-degree nikaal (kitne nodes pehle aane chahiye)
In-degree 0 wale queue mein
Queue se nikaal, result daal, neighbors ka indegree--
Indegree 0 ho → queue mein daal
```
**Confusion tha:** indegree kaise calculate karo — har node ke neighbors loop kar, neighbor ka indegree++.
**q.push(i)** karo, q.push(indegree[i]) NAHI — index push, value nahi!

## Questions (5)
| # | Question | Key Trick |
|---|----------|-----------|
| 46 | DFS Graph | visited + recursion |
| 47 | BFS Graph | queue + visited |
| 48 | Number of Islands | DFS grid, 4 dirs, canVis |
| 49 | Cycle Detection | visited + parent check |
| 50 | Topological Sort | indegree + BFS |
| 51 | Dijkstra | Priority Queue, dist update, auto copy (& nahi) |

---

# 13. DP (Dynamic Programming)

## Kya hai?
**DP = Recursion + Yaad rakh.** Recursion mein same cheez baar baar calculate hoti. DP mein ek baar calculate, yaad rakh, dobaara puche → seedha answer.

```
Recursion: O(2^n) — same kaam baar baar. SLOW.
DP:        O(n)   — yaad rakh ke skip. FAST.
```

## Template — Top-Down (Memoization):
```
solve(n, dp):
    base case → return value
    dp[n] != -1 → return dp[n]        // yaad hai → return
    ans = recursive calls              // calculate
    return dp[n] = ans                 // yaad rakh
```

## DP Pattern Summary:
| Question | Choices | Operation | Base |
|---|---|---|---|
| Climbing Stairs | solve(n-1), solve(n-2) | ADD (total ways) | n==0→1, n==1→1 |
| Coin Change | solve(amt-coin) for each | MIN (min coins) | amt==0→0, amt<0→INT_MAX |
| Min Cost Stairs | solve(i+1), solve(i+2) | MIN (min cost) | i>=n→0 |
| LIS | pick(i+1,i), not-pick(i+1,prev) | MAX (longest) | i>=n→0 |

## Questions (6):
| # | Question | Key Trick |
|---|----------|-----------|
| 52 | Climbing Stairs | ways(n) = ways(n-1)+ways(n-2). Base n==0→1 |
| 53 | Coin Change | 1+solve(amt-coin). INT_MAX overflow check! |
| 54 | Min Cost Stairs | cost[i]+min(solve(i+1),solve(i+2)). Base i>=n→0 |
| 55 | LIS | pick/not-pick. prevIdx==-1 ya nums[i]>prev tab pick |
| 56 | 0/1 Knapsack | pick(weight fit toh) / not-pick. MAX value. FIRST ATTEMPT! |
| 57 | LCS | match: 1+solve(i-1,j-1). no match: max(solve(i-1,j), solve(i,j-1)) |

## Galtiyan:
1. **n==0 return 0 (Climbing)** — return 1 chahiye
2. **1+INT_MAX overflow (Coin Change)** — pehle check, INT_MAX toh skip
3. **solve(i+1) dono jagah (Min Cost)** — ek i+1 ek i+2
4. **pick = INT_MAX (LIS)** — max mein jeetega. INT_MIN rakho
5. **dp check aur dp store alag jagah** — same index pe karo
6. **LCS mein match pe pick+not-pick dono** — match pe sirf pick, no match pe sirf not-pick

---

# 14. ALL TEMPLATES — EK JAGAH

## Linear Scan
```
variable = start. for loop. if condition → update. return variable.
```

## Two Pointer — Opposite
```
low=0, high=n-1. while(low < high). swap/compare. low++, high--.
```

## Two Pointer — Same Direction
```
slow=0, fast=0. while(fast < n). kaam ka → assign/swap, slow++. fast++.
```

## Two Pointer — Two Arrays
```
i=m-1, j=n-1, k=m+n-1. End se. Bada peeche. while(j>=0) remaining.
```

## Sliding Window — Fixed
```
ADD → SHRINK (if > k) → UPDATE (if == k) → j++. ORDER MAT BADLO.
```

## Sliding Window — Variable
```
ADD → SHRINK (while condition) → UPDATE (if valid) → j++.
```

## Hashing
```
complement = target - nums[i]. mp.count? Found. Nahi? mp[nums[i]] = i.
```

## Prefix Sum
```
prefix[0]=nums[0]. prefix[i]=prefix[i-1]+nums[i].
L==0? prefix[R]. L>0? prefix[R]-prefix[L-1].
```

## Binary Search
```
while(low <= high). mid = low+(high-low)/2. Compare. low=mid+1 ya high=mid-1.
```

## Stack — NGE
```
while(!empty && nums[top] < nums[i]) → ans[top]=nums[i], pop. push(i).
```

## Recursion — Pick/Not-Pick
```
Base: index==n → store. Pick: push, recurse. Backtrack: pop. Not-pick: recurse.
```

## Graph — DFS
```
visited=true. push result. for neighbors: !visited → dfs.
```

## Graph — BFS
```
queue push start. while !empty: front, pop, push result, neighbors push.
```

## Topo Sort
```
indegree nikaal. 0 wale queue. nikaal, indegree--, 0 ho → push.
```

## Dijkstra
```
dist(n, INT_MAX). pq.push({0, src}). auto [d,node] = pq.top() (COPY, & nahi!).
Neighbors: newDist < dist[neighbor] → update + push.
```

## DP — Memoization
```
base case → return. dp[n] != -1 → return dp[n]. Calculate. dp[n] = ans. Return.
Total ways → ADD. Min cost/coins → MIN. Longest → MAX.
```

---

# 15. COMMON GALTIYAN — MASTER LIST

### Har jagah:
1. **Base case bhoolna** — NULL, empty, n==0 → handle karo
2. **Off by one** — i=0 vs i=1, < vs <=, slow+1 vs slow

### Linear Scan:
3. **maxNum = 0** — INT_MIN use karo
4. **Kadane's order** — pehle update, phir reset

### Two Pointer:
5. **Pehle slow++ phir assign** — pehli position skip
6. **fast=1 se start** — pehla element miss
7. **maxArea = area** — overwrite! max() use kar

### Sliding Window:
8. **ADD-SHRINK-UPDATE order galat** — galat answer
9. **if vs while** — variable mein while chahiye

### Binary Search:
10. **< lagana <= ki jagah** — last element miss
11. **(low+high)/2** — overflow
12. **Ship: day=0, Koko: hours=1** — galat count
13. **Integer division + ceil** — (double) cast

### Stack:
14. **st.top() bina empty check** — crash
15. **Value push, index nahi** — NGE mein

### Linked List:
16. **return curr** — curr NULL hai, prev return kar
17. **Count loop mein head badha diya** — original lost

### Trees:
18. **if(root==NULL) return bhoolna** — crash
19. **Preorder mein inorder call** — copy paste galti

### Graphs:
20. **Visited track na karna** — infinite loop
21. **canVis mein vis check missing** — infinite loop
22. **q.push(indegree[i])** — value push! q.push(i) karo
23. **== false likhna == true ki jagah** — cycle detect fail

---

# 15. PATTERN RECOGNITION CHEAT SHEET

| Question mein ye dikhein | Pattern use kar |
|---|---|
| Sabse bada/chhota, sum, count | Linear Scan |
| Reverse, palindrome, pair in sorted | Two Pointer Opposite |
| Filter, rearrange, duplicates hatao | Two Pointer Same Direction |
| Merge two sorted | Two Pointer Two Arrays |
| K consecutive elements | Sliding Window Fixed |
| Longest/shortest subarray with condition | Sliding Window Variable |
| Pair in unsorted, lookup, duplicate | Hashing |
| Range sum baar baar | Prefix Sum |
| Sorted mein search | Binary Search |
| Min capacity/speed dhundho | BS on Answer |
| Brackets, recent/closest | Stack |
| Saare subsets/combinations | Recursion Pick/Not-Pick |
| Reverse LL, middle, cycle | Linked List Slow-Fast |
| Tree traverse | DFS (recursion) |
| Level by level | BFS (queue) |
| Connected components, islands | Graph DFS/BFS |
| Prerequisites, ordering | Topological Sort |

---

# 16. PROGRESS TRACKER

**Total: 68 Questions | 13 Patterns | Quiz: 85% (17/20)**

| Pattern | Questions | Status |
|---|---|---|
| Linear Scan | 9 | ✅ DONE |
| Two Pointer (3 types) | 8 | ✅ DONE |
| Sliding Window | 4 | ✅ DONE |
| Hashing | 1 | ✅ DONE |
| Prefix Sum | 1 | ✅ DONE |
| Binary Search (3 types) | 5 | ✅ DONE |
| Stack | 3 | ✅ DONE |
| Recursion | 9 | ✅ DONE |
| Linked List | 5 | ✅ DONE |
| Trees | 7 | ✅ DONE |
| Queue | (BFS mein cover) | ✅ DONE |
| Graphs | 6 | ✅ DONE |
| DP | 6 | ✅ MEDIUM DONE |

### Baaki:
- Bit Manipulation
- Hard questions har topic ke

---

## Ek Line Mein Yaad Rakh — Poora DSA:

> **"Pattern pehchaan. Dry run kar. Template lagaa. Code likh. Galti hui → dry run se pakad. Yehi DSA hai."**

---

*Generated by Arpan Maheshwari during DSA practice sessions.*
*"Dry Run = DSA ka 95%. Visualize kar, code khud aayega."*
*"Fear nikla. Maza aaya. Maa chod di DSA ki."*
