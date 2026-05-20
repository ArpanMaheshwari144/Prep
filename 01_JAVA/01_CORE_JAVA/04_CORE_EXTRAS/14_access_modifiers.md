# Access Modifiers

> **V90 — Core Java Extras: Topic 14**

---

## Scope (Narrow → Wide)

```
┌─────────┬─────────┬───────────┬──────────┐
│ private │ default │ protected │  public  │
│  Same   │  Same   │   Same    │ EVERY-   │
│ class   │ package │  package  │  WHERE   │
│  ONLY   │ (no kw) │ + subclass│  access  │
└─────────┴─────────┴───────────┴──────────┘
  NARROW                              WIDE

Rule: MOST RESTRICTIVE possible. private pehle, zarurat pe badhao.
```

---

## STORY — Payment System

→ Tune payment system banaya
→ **Card number** = koi bhi seedha access na kare → **`private`**
→ **Transaction ID** = same package mein share karo → **`default`**
→ **Verification logic** = subclasses mein bhi kaam aaye → **`protected`**
→ **API response format** = sab ke liye → **`public`**
→ **Galat modifier** lagaya toh ya **security hole** hai ya **unnecessary restriction**
→ `private` → same class only. `default` → same package
→ `protected` → same package + subclass. `public` → sab

---

## Comparison

| Modifier | Same Class | Same Package | Subclass (alag pkg) | Anywhere |
|----------|------------|--------------|---------------------|----------|
| `private` | | | | |
| `default` | | | | |
| `protected` | | | | |
| `public` | | | | |

---

## TRAP 1

> **`default` matlab koi modifier nahi likha — public NAHI hota!**
> **Alag package ki subclass bhi access nahi kar sakti.**

## TRAP 2

> **`protected` = same package + ALAG package ki subclass bhi.**
> **Yehi `default` aur `protected` ka fark hai.**

---

## POWER PHRASE

> *"`private` is same class only. `default` is package-level. `protected` allows subclass access even across packages. `public` is accessible everywhere — always use the most restrictive modifier possible."*
