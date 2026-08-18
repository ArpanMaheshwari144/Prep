# Optional — NullPointerException Se Bachao

> **V90 — Java 8: Topic 38**

---

## STORY — Production Crash

→ Ek din **user profile fetch** karna tha — `getUserById(101)`
→ **User nahi mila — `null` aaya**
→ Tune seedha `user.getName()` call kiya — **`NullPointerException`. Production crash.**
→ Har jagah `if (user != null)` likhne laga — **code ugly, phir bhi kahin miss ho jaata**
→ **Java 8 ne socha — `null` ki jagah ek dabba do**
→ **Dabba khali bhi ho sakta, bhara bhi**. Lekin **kholne se pehle CHECK karna padega**

---

## WHY — Optional?

→ **`Optional` = dabba (container).** Andar value ho bhi sakti, na bhi
→ Kholne se pehle check karo. **`null` ka safe replacement**
→ `orElse()` = khali toh default. `orElseThrow()` = khali toh exception

---

## Old vs New

```java
// Purana style — null check bhool gaye = crash
User user = getUserById(101);     // null aa sakta hai
user.getName();                    // CRASH — NullPointerException

// Optional style — dabba diya, safe access
Optional<User> user = findById(101);
String name = user
    .map(u -> u.getName())          // andar hai toh naam nikalo
    .orElse("Unknown");             // khali toh default

// Khali toh exception
User u = user.orElseThrow(() -> new RuntimeException("User nahi mila!"));
```

---

## Visualization — Optional = Dabba

```
                Optional — Dabba Hai (Value Hold Karta)

╔════════════════════════════════════════════════════════════╗
║ 3 Tarike Optional Banane Ke                                ║
╚════════════════════════════════════════════════════════════╝

  Optional.of("Arpan")              Optional.ofNullable(name)         Optional.empty()
  ┌────────────────┐                ┌────────────────┐                ┌────────────────┐
  │  Optional      │                │  Optional      │                │  Optional      │
  │  ┌──────────┐  │                │  ┌──────────┐  │                │  ┌──────────┐  │
  │  │ "Arpan"  │  │                │  │ ?  ya    │  │                │  │  EMPTY   │  │
  │  └──────────┘  │                │  │ EMPTY    │  │                │  │  (khali) │  │
  └────────────────┘                │  └──────────┘  │                │  └──────────┘  │
   GUARANTEED                       └────────────────┘                └────────────────┘
   value hai                         null bhi ho sakta                  jaante hain khali
                                     (90% yahi use)


╔════════════════════════════════════════════════════════════╗
║ Safe Access                                                ║
╚════════════════════════════════════════════════════════════╝

  Optional<User> u = findById(101);
                     ↓
                ┌─────────┐
                │ User    │  ya  EMPTY
                └─────────┘

  u.orElse("Unknown")          ← khali toh default
  u.orElseThrow(() -> ...)     ← khali toh exception
  u.map(u -> u.getName())      ← bhara hai toh transform, khali toh skip


╔════════════════════════════════════════════════════════════╗
║ Trap — get() Direct Use                                    ║
╚════════════════════════════════════════════════════════════╝

  Optional<User> u = Optional.empty();
       │
       ▼
  ┌─────────┐
  │  EMPTY  │
  └─────────┘
       │
       ▼
  u.get();        NoSuchElementException — khali dabba kholne ki koshish

  Safe:
  u.orElse(default)        ← khali toh default mil jayega
  u.isPresent() ? u.get() : default
```

---

## Optional Banane Ke 3 Tarike

| Method | Use case |
|--------|----------|
| `Optional.of("Arpan")` | **GUARANTEE:** andar value hai. Null diya toh **NullPointerException** |
| `Optional.ofNullable(name)` | **SHAYAD:** null bhi ho sakta, safe. **90% time YAHI use** |
| `Optional.empty()` | **KHALI:** jaante hain kuch nahi hai |

```java
Optional.of("Arpan");          // guaranteed value
Optional.of(null);             // NPE — crash

Optional.ofNullable(name);     // null safe (90% yahi)
Optional.empty();              // explicitly empty
```

---

## Important Methods

```java
optional.isPresent();                       // andar kuch hai? true/false
optional.orElse("default");                 // khali toh default
optional.orElseThrow(() -> new RuntimeException("..."));  // khali toh exception
optional.map(u -> u.getName());             // transform — khali toh skip
optional.filter(u -> u.isActive());         // condition — false toh empty
optional.get();                             // DANGEROUS — khali toh crash
```

---

## TRAP 1 — `optional.get()` Seedha Mat Karo

> **Khali ho toh `NoSuchElementException`. Hamesha `orElse()` ya `orElseThrow()` use karo.**

```java
optional.get();                          // crash if empty
optional.orElse("default");              // safe
optional.orElseThrow(() -> new ...);     // safe with exception
```

## TRAP 2 — Method Parameter Mein Optional Mat Do

> **Optional sirf RETURN type mein use karo — parameter mein nahi.**
> ```java
> void process(Optional<User> u) { ... }    // bad practice
> Optional<User> findById(Long id) { ... }  // sahi use
> ```

---

## POWER PHRASE

> *"Optional is a container that may or may not hold a value — it replaces null checks with a fluent API. Use `orElse()` for defaults, `orElseThrow()` for exceptions, and `map()` to transform safely. Never call `get()` directly. Use `ofNullable()` when the value might be null, `of()` when guaranteed non-null."*

> **Yaad rakh:**
> Optional = null ka safe replacement
> `of()` = guaranteed
> `ofNullable()` = maybe null (90% yahi)
> `orElse / orElseThrow` = safe access
> `get()` seedha = dangerous
> Sirf return type mein use karo, parameter mein nahi
