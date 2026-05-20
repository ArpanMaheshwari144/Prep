# Class / Object / Constructor — Visual Revision

---

## 1️⃣ Class vs Object (Blueprint Analogy)

```
   BLUEPRINT (Class)              HOUSE (Object)
   ┌──────────────────┐              ┌──────────────────┐
   │  3 BHK design    │              │  My actual house  │
   │  Garden plan      │   →          │  Painted yellow   │
   │  Kitchen layout   │              │  Address: B-201   │
   │  Door positions   │              │  Owner: Arpan     │
   └──────────────────┘              └──────────────────┘
   
   Ek blueprint                       Many houses banayi
                                       ja sakti us blueprint se
```

```
ek blueprint = MANY houses possible:
   ┌──────┐  ┌──────┐  ┌──────┐  ┌──────┐
   │House1│  │House2│  │House3│  │House4│
   └──────┘  └──────┘  └──────┘  └──────┘
   each: yellow, owner Arpan, address X
   each: blue,   owner Suresh, address Y
```

---

## Java Translate

```java
// CLASS = blueprint
class House {
    String color;
    String owner;
    String address;
    
    void paintNewColor(String newColor) {
        color = newColor;
    }
}

// OBJECTS = actual houses
House house1 = new House();
house1.color = "yellow";
house1.owner = "Arpan";

House house2 = new House();
house2.color = "blue";
house2.owner = "Suresh";
```

```
   class House       = template
   new House()       = actually build kar
   house1, house2    = different real houses
```

---

## 2️⃣ Constructor (House Building Worker)

```
Sawaal: house1 BANI kaise jab "new House()" likha?
        Color, owner, address kab assign hue?

Answer: CONSTRUCTOR = woh "worker" jo object banata
         jab "new" likha
```

---

## Default Constructor

```java
class House {
    String color;
    String owner;
    
    // Tu likhna nahi padega — Java khud banata
    public House() {
        // empty default
    }
}

House h = new House();
```

```
Internally:
   1. Memory allocate
   2. Constructor call (default empty)
   3. Object reference return
```

---

## Custom Constructor

```java
class House {
    String color;
    String owner;
    
    // YOUR own constructor
    public House(String c, String o) {
        color = c;
        owner = o;
    }
}

House h = new House("yellow", "Arpan");
```

### Visual
```
   new House("yellow", "Arpan");
              │
              ▼
   ┌──────────────────────────┐
   │  CONSTRUCTOR (worker)     │
   │                            │
   │  Step 1: Memory allocate  │
   │  Step 2: Run constructor  │
   │     color = "yellow"       │
   │     owner = "Arpan"        │
   │  Step 3: Return reference │
   └────────────┬─────────────┘
                │
                ▼
            House h
            ┌─────────────────┐
            │ color: "yellow" │
            │ owner: "Arpan"  │
            └─────────────────┘
```

---

## 3️⃣ Constructor Rules

```
1. Method name = SAME as class name
2. NO return type (not even void)
3. Multiple constructors = OVERLOADING (different params)
4. If you don't write any → Java provides empty default
5. If you write ONE → default GONE
```

---

## 4️⃣ Constructor Overloading

```java
class House {
    String color;
    String owner;
    int rooms;
    
    public House() { }
    public House(String c) { color = c; }
    public House(String c, String o) {
        color = c;
        owner = o;
    }
    public House(String c, String o, int r) {
        color = c;
        owner = o;
        rooms = r;
    }
}

new House();                          // empty
new House("blue");                    // color only
new House("blue", "Arpan");           // color + owner
new House("blue", "Arpan", 3);        // all three
```

---

## 5️⃣ Constructor Chaining (this())

```
Repeat code avoid karne ke liye —
   ek constructor doosre ko call kare
```

```java
class House {
    String color;
    String owner;
    int rooms;
    
    public House() {
        this("default-white");        // calls 1-arg
    }
    
    public House(String c) {
        this(c, "Anonymous");         // calls 2-arg
    }
    
    public House(String c, String o) {
        this(c, o, 2);                // calls 3-arg
    }
    
    public House(String c, String o, int r) {
        color = c;
        owner = o;
        rooms = r;
    }
}

new House();
   ↓ chain:
   House() → House(c) → House(c,o) → House(c,o,r)
   = saare values fill ho jate
```

---

## 6️⃣ Constructor vs Method

```
┌───────────────────┬──────────────────┬─────────────────┐
│  Aspect           │  Constructor     │  Method         │
├───────────────────┼──────────────────┼─────────────────┤
│ Name              │ Same as class    │ Anything        │
│ Return type       │ NONE (not void)  │ Required        │
│ When called       │ When 'new' used  │ Manual call     │
│ Inherited?        │ NO               │ Yes             │
│ Override?         │ Cannot           │ Can             │
└───────────────────┴──────────────────┴─────────────────┘
```

---

## Memory Hooks

```
Class       = blueprint (template)
Object      = actual house (built)
Constructor = construction worker
              (memory allocate + initialize fields)

new keyword = trigger construction
this()      = call another constructor of same class
```

[← Back to JAVA](../)
