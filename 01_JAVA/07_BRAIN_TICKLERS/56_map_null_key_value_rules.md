# Map Implementations — Null Key/Value Rules

> **V90 — Brain Ticklers: Topic 56 (clarification)**

---

## 🤔 Common Confusion

> "LinkedHashMap null key allow karta hai ya nahi?"
> Jawab: **HAAN, allow karta hai** — kyunki LinkedHashMap **HashMap ko extend** karta. Same null rules.

---

## 📊 Sab Maps — Null Rules

| Map | null key? | null value? | Reason |
|-----|-----------|-------------|--------|
| **HashMap** | ✅ 1 allowed | ✅ multiple | hashCode special handle (`null` → bucket 0) |
| **LinkedHashMap** | ✅ 1 allowed | ✅ multiple | HashMap **extend** karta — same rules |
| **TreeMap** | ❌ NO | ✅ allowed | sort = `null.compareTo()` = crash |
| **ConcurrentHashMap** | ❌ NO | ❌ NO | thread safety + ambiguity |
| **Hashtable** (legacy) | ❌ NO | ❌ NO | thread-safe, null banned |

---

## 💻 Code Proof — LinkedHashMap null key WORKS

```java
LinkedHashMap<String, Integer> map = new LinkedHashMap<>();
map.put(null, 100);                    // ✅ kaam karta
map.put("Arpan", 200);
map.put(null, 999);                    // null key 1 hi hota — value REPLACE

System.out.println(map);
// {null=999, Arpan=200} — INSERTION ORDER + null key allowed
```

---

## 🔴 TreeMap mein null Banned KYU?

→ TreeMap **sort karta** keys ko (Red-Black Tree)
→ Sort karne ke liye `compareTo()` chahiye
→ `null.compareTo(...)` = **NullPointerException**
→ Isliye TreeMap khud `null` key block karta — `put(null, x)` = NPE

---

## 🔴 ConcurrentHashMap mein null Banned KYU?

→ Multi-thread mein `get(key)` ne null return kiya — **kya matlab?**
→ Option 1: Key nahi hai
→ Option 2: Value null hai
→ Single-thread mein `containsKey()` se check kar sakte
→ **Multi-thread mein** `get()` aur `containsKey()` ke **beech doosre thread ne data badal diya** — **race condition**
→ **Ambiguity hi mat aane do** — null banned (key + value dono)

---

## 🎯 1-Line Yaad

> **HashMap, LinkedHashMap → null key 1 allowed (LinkedHashMap = HashMap ka child).**
> **TreeMap → sort crash, no null key. Null value OK.**
> **ConcurrentHashMap → race condition risk, NO null AT ALL (key + value).**

---

## 💬 POWER PHRASE

> *"HashMap and LinkedHashMap allow one null key — LinkedHashMap extends HashMap so same rules apply. TreeMap rejects null keys because it cannot compare them. ConcurrentHashMap rejects all nulls to avoid the race condition between `get()` and `containsKey()`."*
