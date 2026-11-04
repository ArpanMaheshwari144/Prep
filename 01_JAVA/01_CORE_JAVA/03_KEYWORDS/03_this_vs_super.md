# this vs super

- **`this`** matlab ye wala object, apni class. `this.accountId` object ka field hai (jab parameter ka naam bhi wahi ho), aur
  `this(...)` isi class ka doosra constructor bulata hai.
- **`super`** matlab parent class. `super.method()` parent wala method bulata hai (child ne override kiya ho tab bhi), aur
  `super(...)` parent ka constructor bulata hai, jaise bank ke `SavingsAccount` me `super(id, name, balance)`.

## Trap

```java
Account(String accountId) { accountId = accountId; }
```

Chala ke dekha to field `null` aaya. Yahan dono taraf wala `accountId` parameter hi hai, matlab parameter ne apne aap ko value
de di aur field khaali reh gaya. Isliye `this.accountId = accountId` likhte hain.

`this()` aur `super()` dono constructor ki pehli line hi ho sakte hain (poora = `01_OOP/08_constructor_chaining.md`).

## Interview line

```
this refers to the current object and super to the parent. this.field fixes the
name clash with a parameter, and this() or super() call another constructor,
always as the first line.
```
