# LinkedList — Andar Se Samjho

> **V90 — Collections: Topic 26**

---

## STORY — Support Ticket Queue

→ Support ticket queue thi — **urgent tickets beech mein insert** karne the
→ ArrayList try kiya — beech mein insert karo toh **saare elements shift hote**. 10,000 tickets? **10,000 shifts**. Slow
→ **LinkedList beech mein insert ke liye bana hi hai** — sirf pointers badlo, koi shifting nahi → **O(1), par sirf jab us node pe pahle se khade ho** (warna wahan tak chalna = O(n), neeche TRAP 2)
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
| `add(index, x)` beech me | **O(n)** | **O(n)** | Array = shift · List = pehle us jagah tak CHALNA padta |
| node pehle se haath me (iterator) | — | **O(1)** | sirf 2-4 pointer badle — LinkedList ka asli faayda YAHI hai |
| `addFirst / removeFirst` | **O(n)** | **O(1)** | Array me sab khiskaana · List me head badlo |
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

**Use:** queue/stack, frequent beech-insert/delete (iterator ke saath).

★ **Queue/stack ke liye bhi `ArrayDeque` aksar LinkedList se TEZ hai** (Java docs khud yahi kehte): array pe chalta, har element ke liye Node object nahi banta, cache-friendly. LinkedList tab, jab beech se iterator pe hatana/jodna ho ya `null` rakhna ho (ArrayDeque null nahi leta).
`get(i)` me LinkedList aage YA peeche jo paas ho wahan se chalta hai (i < size/2 ? head : tail) — phir bhi O(n).

---

## POWER PHRASE

> *"LinkedList uses doubly linked nodes — each node holds data, prev and next pointers. Insert and delete are O(1) once you are at the node, since only pointers change, but reaching a position by index is O(n). In practice ArrayList usually wins because of cache locality, and ArrayDeque is the better queue or stack."*

> **Yaad rakh:**
> ArrayList → Zyada READ, index access, end pe add/remove
> LinkedList → Zyada INSERT/DELETE beech mein, queue/stack banani ho
