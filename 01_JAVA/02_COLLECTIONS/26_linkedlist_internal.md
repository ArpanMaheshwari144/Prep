# LinkedList — Andar Se Samjho

> **V90 — Collections: Topic 26**

---

## STORY — Support Ticket Queue

→ Support ticket queue thi — **urgent tickets beech mein insert** karne the
→ ArrayList try kiya — beech mein insert karo toh **saare elements shift hote**. 10,000 tickets? **10,000 shifts**. Slow
→ **LinkedList beech mein insert ke liye bana hi hai** — sirf pointers badlo, koi shifting nahi → **O(1)**
→ Lekin tradeoff — **index se seedha access nahi hota**, HEAD se traverse karna padta
→ Rule: **zyada read = ArrayList. Zyada insert/delete beech mein = LinkedList**

---

## Internal — Doubly Linked Nodes

Har node mein **3 cheezein**:

```
prev | data | next

null ← [101] ↔ [102] ↔ [103] → null
       HEAD              TAIL
```

→ **Koi continuous memory nahi** — har node alag jagah RAM mein, pointers se connected
→ **Memory zyada** — har node mein `prev` + `next` extra hai
→ `add/remove` = sirf pointers badle → **O(1)**
→ `get` = HEAD se traverse karo → **O(n)**

---

## ArrayList vs LinkedList — Ek Saath

| Operation | ArrayList | LinkedList | Kyu? |
|-----------|-----------|------------|------|
| `get(index)` | **O(1)** | **O(n)** | Array = direct, Node = traverse |
| `add(end)` | **O(1)** | **O(1)** | Dono fast |
| `add(middle)` | **O(n)** | **O(1)** | Array = shift, Node = pointers |
| `remove(middle)` | **O(n)** | **O(1)** | Array = shift, Node = pointers |
| **Memory** | Less | More | Node mein prev + next extra |

---

## TRAP 1

> **"LinkedList is always faster" — GALAT!**
> **`get()` mein ArrayList wins. Real world mein READ zyada hota — isliye 90% cases mein ArrayList better.**

## TRAP 2

> **LinkedList insert = traverse O(n) + pointer change O(1) = NET O(n)** (target position dhoondhne mein traverse).
> ArrayList ka shift bhi O(n).
> **Practice mein ArrayList cache-friendly hai** — continuous memory.
> **LinkedList = scattered nodes = cache miss.**

---

## Real Use Case

```java
LinkedList<Ticket> ticketQueue = new LinkedList<>();
ticketQueue.addFirst(urgentTicket);    // O(1)
ticketQueue.addLast(normalTicket);     // O(1)
```

**Use:** queue/stack, frequent beech-insert/delete.

---

## POWER PHRASE

> *"LinkedList uses doubly linked nodes — each node holds data, prev and next pointers. Insert and delete are O(1) since only pointers change, but `get` is O(n) since traversal is needed. Use when frequent insertion and deletion in the middle is required."*

> **Yaad rakh:**
> ArrayList → Zyada READ, index access, end pe add/remove
> LinkedList → Zyada INSERT/DELETE beech mein, queue/stack banani ho
