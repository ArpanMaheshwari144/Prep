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
| `private` | Yes | No | No | No |
| `default` | Yes | Yes | No | No |
| `protected` | Yes | Yes | Yes | No |
| `public` | Yes | Yes | Yes | Yes |

---

## TRAP 1

> **`default` matlab koi modifier nahi likha — public NAHI hota!**
> **Alag package ki subclass bhi access nahi kar sakti.**

## TRAP 2

> **`protected` = same package + ALAG package ki subclass bhi.**
> **Yehi `default` aur `protected` ka fark hai.**

## ★ TRAP 3 — protected ka chhupa niyam (alag package)

```java
// package a
public class Parent { protected int x; }

// package b
public class Child extends Parent {
    void test(Parent p, Child c) {
        this.x = 1;   // OK  — apne (inheritance se mile) x pe
        c.x = 1;      // OK  — Child type ka reference
        p.x = 1;      // COMPILE ERROR — Parent ka reference, alag package
    }
}
```
alag package me protected sirf INHERITANCE ke raaste milta, Parent object pakad ke nahi.

## TRAP 4 — top-level class pe sirf `public` ya default
`private class A {}` / `protected class A {}` top-level pe nahi chalta (nested class pe chalta).

---

## POWER PHRASE

> *"`private` is same class only. `default` is package-level. `protected` allows subclass access even across packages. `public` is accessible everywhere — always use the most restrictive modifier possible."*
