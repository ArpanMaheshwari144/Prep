# HashMap vs ConcurrentHashMap

> **V90 Section 1 — Topic 7**

---

## 📖 STORY — Race Condition

→ Ek baar **2 background threads** ek saath HashMap mein data daal rahe the
→ Kuch requests pe data **corrupt** ho raha tha — entries gayab, kuch overwrite
→ **Root cause:** HashMap **thread-safe nahi** hai
→ Dono threads ne ek saath same bucket mein likh diya — ek ka data gaya
→ **ConcurrentHashMap ne fix kiya** — **bucket-level locking**
→ Alag buckets pe alag threads kaam kar sakte, **bina poora map lock kiye**
→ HashMap: 2 threads ek saath = race condition, **data corrupt**
→ ConcurrentHashMap: bucket-level locking → alag buckets pe alag threads → **safe**

---

## 📊 Comparison

| | HashMap | ConcurrentHashMap |
|--|--------|-------------------|
| **Thread-safe?** | ❌ NOT | ✅ YES |
| **Locking** | None | Segment/bucket-level |
| **null key** | Allowed | **NOT allowed** (NPE) |
| **null value** | Allowed | **NOT allowed** (NPE) |
| **Use case** | Single-thread, fast | Multi-thread, safe |
| **Performance** | Fast (single thread) | Concurrent reads/writes safe |

---

## 🔴 TRAP

> **null key allowed in HashMap — but in ConcurrentHashMap NOT allowed.**

> **HashMap is NOT thread-safe — use it in single-threaded code.**

---

## 💬 POWER PHRASE

> *"HashMap is not thread-safe — use it in single-threaded code. ConcurrentHashMap uses segment-level locking so multiple threads can read and write concurrently without corrupting data."*
