# Pass by value

Java hamesha copy bhejta hai.

- **`int` bheja:** number ki copy jaati hai. Method ne use badla to tera number wahi rehta hai.
- **Object bheja:** address ki copy jaati hai, aur dono copy ek hi ghar pe pahunchti hain. Method ne `a.balance -= 500` kiya to
  ghar ke andar badla, tujhe bhi dikhega. Par method ne `a = new Account()` kiya to sirf uski parchi pe naya address likha
  gaya, tera account wahi raha.

Misaal: tune dost ko apne ghar ke address ki photocopy di. Dost wahan gaya aur sofa hila diya, to tera ghar bhi badla, kyunki
ghar ek hi hai. Par dost ne apni parchi pe naya address likh diya, to teri parchi waisi hi rahi.

## Prod wala trap

```java
int bal = w.balance;  bal -= amt;      // sirf copy ghati, w.balance wahi raha
w.balance -= amt;                      // asli field badla
```

Wallet se paisa kaata, log me "deducted" bhi likh diya, par balance hila hi nahi. Map me `Integer` ho to
`map.put(id, map.get(id) - amt);` likhna padta hai.

## Interview line

```
Java is always pass by value. For objects the reference is copied, so changing fields
shows up for the caller, but reassigning the parameter does not. Copying a field into a
local int and changing that does nothing to the object.
```
