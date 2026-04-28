# Checked vs Unchecked Exceptions

> **V90 Section 1 — Topic 9**

---

## 🟡 WHY — 2 Categories Kyun?

→ File padhne ka code likha. Compiler ne roka — **"Agar file nahi mili toh?"**
→ **Handle karo ya declare karo.** Ye **Checked Exception** hai
→ Compiler ko pehle se **pata hai ye risky operation hai** — force karta hai socho
→ NullPointerException pe compiler kabhi nahi rokta — kyunki **runtime pe hota** hai, pehle se predict nahi hota
→ Isliye 2 categories: **Checked = compiler ka darwaan. Unchecked = runtime ka dhoka.**

---

## 📊 Comparison

| | Checked | Unchecked |
|--|---------|-----------|
| **Compiler check?** | ✅ Compile time | ❌ Runtime fail |
| **Handle karna padta?** | YES (try-catch ya `throws`) | NO |
| **Examples** | `IOException`, `FileNotFoundException`, `SQLException` | `NullPointerException`, `ArrayIndexOutOfBoundsException`, `ArithmeticException` |
| **Source** | Outside dependencies (file/DB/network) | Programming bug |

```java
// Checked — handle karo ya declare karo
public void readFile() throws IOException {
    FileReader fr = new FileReader("test.txt");
}

// Unchecked — compiler nahi pucchta
String s = null;
s.length();    // NullPointerException — runtime
```

---

## 🔴 TRAP

> **`@Transactional` + Checked Exception = NO rollback by default!**
> **`rollbackFor` explicitly likhna MUST hai!**

```java
@Transactional(rollbackFor = IOException.class)   // ✅ explicit
public void process() throws IOException { ... }
```

→ For `IOException` you must add `rollbackFor = IOException.class`
→ Default `@Transactional` sirf **RuntimeException** pe rollback karta
→ Critical point — interview mein ye galti pakad ke gehre questions aate

---

## 💬 POWER PHRASES

> *"Checked exceptions are verified at compile time — you must handle or declare them. Unchecked exceptions happen at runtime."*

> *"`@Transactional` only rolls back on RuntimeException by default — for IOException you must add `rollbackFor = IOException.class`."*

> **Yaad rakh:** Checked = Compiler ka darwaan. Unchecked = Runtime ka dhoka. `@Transactional` sirf RuntimeException pe rollback.
