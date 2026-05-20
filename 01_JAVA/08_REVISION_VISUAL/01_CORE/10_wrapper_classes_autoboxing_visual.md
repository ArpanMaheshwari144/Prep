# Wrapper Classes + Autoboxing — Visual Revision

---

## 1️⃣ Problem (Why Wrappers Exist)

```
PRIMITIVES (raw):
   int, long, double, char, boolean...
   
   Problem hai:
      Object nahi hain
      Collections mein nahi daal sakte
         List<int>     ← compile error
         List<Integer> ← works
      null nahi ho sakte
         int x = null;  ← 
      Methods nahi (toBinaryString, parseInt, etc.)
```

---

## 2️⃣ Solution = WRAPPER CLASSES

```
Imagine primitives = raw items
Wrapper = item ko BOX mein daal do
   Ab box ko anywhere bhej sakte
   = Object ban gaya
```

---

## 8 Primitive → 8 Wrapper Classes

```
┌──────────┬──────────────┐
│ Primitive│  Wrapper      │
├──────────┼──────────────┤
│ byte     │  Byte         │
│ short    │  Short        │
│ int      │  Integer      │
│ long     │  Long         │
│ float    │  Float        │
│ double   │  Double       │
│ char     │  Character    │
│ boolean  │  Boolean      │
└──────────┴──────────────┘
```

---

## 3️⃣ Manual Wrapping/Unwrapping (Old Way)

```java
// Wrapping (primitive → object)
int num = 5;
Integer obj = Integer.valueOf(num);   // box karo

// Unwrapping (object → primitive)
int back = obj.intValue();            // box se nikalo
```

```
Boring boilerplate
Hard to read
```

---

## 4️⃣ Autoboxing/Unboxing (Java 5+)

```java
// AUTO-BOXING (primitive → object, automatic)
int num = 5;
Integer obj = num;                    // Java does Integer.valueOf(num)

// AUTO-UNBOXING (object → primitive, automatic)
Integer obj = 10;
int back = obj;                       // Java does obj.intValue()
```

```
Magic:
   No manual boxing/unboxing
   Java compiler converts behind scenes
```

---

## 5️⃣ Real Use — Collections

```java
// Cannot do this:
List<int> list = ...;            // compile error

// Do this:
List<Integer> list = new ArrayList<>();
list.add(5);                     // autobox: 5 → Integer.valueOf(5)
int x = list.get(0);             // auto-unbox: Integer → int
```

---

## 6️⃣ Useful Methods (Why Wrappers Matter)

```java
Integer.parseInt("123")          // String → int (5)
Integer.toString(123)            // int → String
Integer.toBinaryString(10)       // 10 → "1010"
Integer.MAX_VALUE                // 2^31 - 1
Integer.MIN_VALUE                // -2^31

Boolean.parseBoolean("true")     // String → boolean

Character.isDigit('5')           // true
Character.isLetter('a')          // true
Character.toUpperCase('a')       // 'A'
```

---

## 7️⃣ THE Interview Trap — Integer Cache

```
Java mein Integer cache:
   -128 to 127 → SAME object reused (cached)
   Beyond range → NEW object every time
```

```java
Integer a = 100;
Integer b = 100;
a == b;            // TRUE  (cached, same object)

Integer c = 200;
Integer d = 200;
c == d;            // FALSE (new objects, different memory)

// Always use .equals():
c.equals(d);       // TRUE
```

### Visual
```
   CACHE (-128 to 127):
   ┌─────────────────────────────────┐
   │   Integer(100) ←──┐              │
   │                    │              │
   └────────────────────┼──────────────┘
                        │
            ┌───────────┴──────────┐
            ▼                      ▼
           a = 100                b = 100
           (same object!)
   
   Both a, b POINT to SAME cached Integer
   a == b → true (reference equal)
```

```
   BEYOND CACHE (> 127):
   c = 200 → new Integer(200) at 0x1234
   d = 200 → new Integer(200) at 0x5678
   
   Different objects
   c == d → false
   c.equals(d) → true (content same)
```

---

## 8️⃣ Why Cache Exists?

```
Reason: PERFORMANCE
   Small numbers used a LOT (-128 to 127)
   Same Integer(5) banana baar baar = waste
   
   Cache mein rakhe = reuse
   = Memory + speed benefit
```

---

## 9️⃣ Common Trap — NullPointerException

```java
Integer x = null;
int y = x;            // NullPointerException!

// Java tries: int y = x.intValue();
// x is null → NPE
```

```
RULE:
   Auto-unboxing null wrapper = CRASH
   Always check null first
```

---

## Performance Trap — Loop

```java
// BAD — autoboxes every iteration
Integer sum = 0;
for (int i = 0; i < 1000000; i++) {
    sum = sum + i;       // unbox sum, add, autobox back
}

// GOOD — use primitive
int sum = 0;
for (int i = 0; i < 1000000; i++) {
    sum = sum + i;       // no boxing overhead
}
```

```
Lesson:
   Use primitive when possible
   Wrapper only when needed (collections, null possible)
```

---

## Memory Hooks

```
Primitive = raw item (int, long, etc.)
Wrapper   = boxed item (Integer, Long, etc.)

Autobox    = primitive → wrapper (auto)
Unbox      = wrapper → primitive (auto)

Cache range = -128 to 127
   Inside: same object (==)
   Outside: new object (use .equals())

Null unbox = NPE
Loop unbox = slow (use primitive)
```

[← Back to JAVA](../)
