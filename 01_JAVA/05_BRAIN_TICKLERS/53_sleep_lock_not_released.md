# `sleep()` Mein Lock Release Nahi Hota

> **V90 — Brain Ticklers: Topic 53**

---

## 📖 STORY

→ `synchronized` method mein `Thread.sleep()` call kiya
→ **Doosra thread** us method mein ja sakta? **NAHI**
→ `sleep()` = **lock apne paas rakhti hai**, so raha hai but **lock nahi chhoda**
→ Doosra thread **wait** karega
→ Agar `wait()` hota toh **lock release** hota — doosra andar ja sakta tha

---

## 🟡 WHY sleep Lock Rakhta? "Main thak gaya, rest chahiye"

→ Kaam **abhi baaki** hai, lock chhodu toh **doosra beech mein data badal dega**
→ `wait()` = "main doosre ka intezaar kar raha hoon" — mera kaam ruka, doosre ko karne do, **isliye lock chhoda**
→ **`sleep()` = selfish rest. `wait()` = generous wait.**

---

## 🆚 Quick Compare

| | `sleep()` | `wait()` |
|--|----------|----------|
| Lock release? | ❌ NO | ✅ YES |
| Class | `Thread` (static) | `Object` |
| Use | Just delay | Inter-thread comm |

---

## 💬 POWER PHRASE

> *"`sleep()` pauses the thread but holds the lock — it's a 'selfish rest'. `wait()` releases the lock so other threads can enter — it's a 'generous wait'. Use `sleep()` for delays, `wait()` for inter-thread coordination."*
