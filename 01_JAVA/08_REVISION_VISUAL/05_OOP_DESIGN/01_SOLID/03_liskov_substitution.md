# 🐧 L — Liskov Substitution (LSP)

---

## Concept

```
Child class should be USABLE in place of parent
   WITHOUT breaking behavior
```

---

## Square vs Rectangle Classic Trap

```
GALAT BELIEF:
   Square IS-A Rectangle
   So: class Square extends Rectangle
```

```java
// ❌ VIOLATES LSP
class Rectangle {
    int width, height;
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
}

class Square extends Rectangle {
    void setWidth(int w) {
        width = w;
        height = w;   // forced equal
    }
    void setHeight(int h) {
        width = h;
        height = h;
    }
}
```

```java
// Test:
void test(Rectangle r) {
    r.setWidth(5);
    r.setHeight(10);
    assert r.area() == 50;   // ❌ FAILS with Square (area = 100)
}
```

```
✅ Fix:
   Don't force inheritance where behavior breaks
   Use composition or separate hierarchy
```

---

## Real-World Penguin Example

```
Parent: Bird (has fly method)
Child: Penguin extends Bird
   Penguin.fly() = ???  ❌ can't fly!
   
Fix:
   Bird interface
   FlyingBird, NonFlyingBird subtypes
   Penguin → NonFlyingBird
```

---

## Memory Hook

```
🐧 Penguin shouldn't extend FlyingBird
   "Child parent ko safely replace nahi kar saka =
    inheritance galat hai"
```

📚 [← Back to SOLID](00_overview.md)
