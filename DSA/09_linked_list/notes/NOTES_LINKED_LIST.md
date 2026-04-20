# Pattern 10: Linked List

## Ye kya hai?

Bhai dekh — train ke dabbe soch. Har dabba mein 2 cheezein:
1. **Data** — dabbe mein kya rakha hai
2. **Next pointer** — agla dabba kaunsa hai

```
[10] → [20] → [30] → [40] → NULL
```

**NULL** = end. Koi agla nahi.

## Array vs Linked List:

```
Array:       Sab ek saath memory mein. Index se access O(1). Insert/Delete O(n).
Linked List: Har node alag. Pointers se connected. Insert/Delete O(1). Access O(n).
```

## Node kaise banta hai:

```cpp
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};
```

## Kaise ghoomte hain:

```
ListNode* temp = head;
while(temp != NULL){
    cout << temp->val;
    temp = temp->next;
}
```

---

## Questions Jo Kiye

| # | Question | Key trick |
|---|----------|-----------|
| 41 | Reverse Linked List | 3 pointers: prev, curr, next. Har node ka next ulta kar. Return prev. |
| 42 | Middle of LL | Slow-fast. fast != NULL && fast->next != NULL. |
| 43 | Detect Cycle | Slow-fast. Mile toh cycle. Fast NULL toh no cycle. |
| 44 | Merge Two Sorted LL | newhead = l1, l1 aage badha, dummy se build. Chhota element pehle. |
| 45 | Remove Nth from End | count nikaal, count==n → head->next. Warna slow-fast. |

## Merge Sorted LL — Copy pe dry run se seekha:

**Problem:** newhead = l1 kiya toh dummy bhi [1] pe, l1 bhi [1] pe. dummy->next = l1 → cycle!

**Fix (dry run se mili):** `l1 = l1->next` — pehle l1 aage badha, phir dummy se build shuru.

```
newhead = [1]          ← head save kiya
l1 = l1->next = [3]   ← l1 aage badha (ye line copy pe dry run se aayi!)
dummy = newhead = [1]  ← dummy [1] se build karega

Ab dummy->next = l2/l1 — koi cycle nahi kyunki l1 aage ja chuka.
```

**Base cases:** l1 NULL → return l2. l2 NULL → return l1.

## Q45: Remove Nth Node from End — Slow-Fast + Count

**Approach:** Pehle count nikaal (list ki length). Agar count==n → head hatao. Warna fast ko n step aage bhej, phir dono saath chalo.

**Dry Run:**
```
[1] → [2] → [3] → [4] → [5]  n=2

count=5. 5!=2.
fast ko 2 aage: fast=[3].
Dono chalo: slow=[3], fast=[5]. fast->next=NULL → STOP.
slow->next = slow->next->next. [3]→[5].
Result: [1]→[2]→[3]→[5] ✅
```

**Galtiyan:**
1. **Count loop mein head badha diya** — head NULL ho gaya, lost. Alag variable use kar.
2. **dummy NULL pe dummy->next** — count ke baad dummy NULL hai, next → CRASH.
3. **head hatana hai (count==n)** — return head->next, bas.
4. **fast->next check bhoolna** — while mein fast && fast->next dono check.

## Linked List ✅ DONE

---

## Dry Run — Reverse LL

```
[1] → [2] → [3] → NULL

prev=NULL, curr=[1]

Step 1: next=[2]. curr->next=NULL. prev=[1]. curr=[2].
  NULL ← [1]   [2] → [3]

Step 2: next=[3]. curr->next=[1]. prev=[2]. curr=[3].
  NULL ← [1] ← [2]   [3]

Step 3: next=NULL. curr->next=[2]. prev=[3]. curr=NULL. STOP.
  NULL ← [1] ← [2] ← [3]

return prev = [3] ✅
```

**IMPORTANT:** `return prev` karo `curr` nahi — curr NULL ho chuka hai loop ke baad.

---

## Silly Mistakes

1. **return curr likhna** — curr NULL hai loop ke baad. prev return kar.

---

## Ek Line Mein Yaad Rakh

> Reverse LL = **"3 pointers. next yaad rakh, pointer ulta kar, aage badh."**
