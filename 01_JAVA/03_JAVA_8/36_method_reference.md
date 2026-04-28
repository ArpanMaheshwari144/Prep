# Method Reference (`::`) — Lambda ka Shortcut

> **V90 — Java 8: Topic 36**

---

## 🟡 WHY — Lambda Ka Shortcut

→ Jab Lambda mein **sirf ek existing method call** ho raha ho — toh **`::` use karo. Aur short.**
→ `name -> System.out.println(name)` = `System.out::println`
→ Code aur **clean + readable**

---

## 💻 Code

### Lambda → Method Reference

```java
// Lambda
list.forEach(name -> System.out.println(name));

// Method Reference — same kaam, aur short
list.forEach(System.out::println);
```

---

## 📊 3 Common Types

| Type | Lambda | Method Reference |
|------|--------|------------------|
| **Static method** | `(x) -> Math.abs(x)` | `Math::abs` |
| **Instance method** | `(s) -> s.toUpperCase()` | `String::toUpperCase` |
| **Constructor** | `() -> new Patient()` | `Patient::new` |

---

## 💻 Examples

### Static method reference
```java
List<Integer> nums = List.of(-1, -2, 3);
nums.stream().map(Math::abs);              // [1, 2, 3]
```

### Instance method reference
```java
List<String> names = List.of("arpan", "rahul");
names.stream().map(String::toUpperCase);   // [ARPAN, RAHUL]
```

### Constructor reference
```java
List<String> names = List.of("Arpan");
names.stream().map(Patient::new);          // har name se naya Patient
```

---

## 🔴 TRAP — Method Reference Sirf Tab Use Karo Jab Existing Method Call Ho

> **Agar Lambda mein logic + method call DONO ho, Method Reference NAHI lagega**

```java
// ✅ Sirf method call — Method Reference safe
list.forEach(System.out::println);

// 🔴 Logic + method call — Method Reference nahi
list.forEach(name -> System.out.println("Hello " + name));   // ye Lambda hi rahega
```

---

## 💬 POWER PHRASE

> *"Method references are shorthand for lambdas that only call an existing method — `ClassName::methodName` for static, `instance::method` for instance, and `ClassName::new` for constructors."*

> **Yaad rakh:**
> Lambda mein sirf method call → `::` use karo
> Static = `Math::abs`
> Instance = `String::toUpperCase`
> Constructor = `Patient::new`
