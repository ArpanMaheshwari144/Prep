# final keyword

`final` matlab jo badlega nahi.

- **Variable pe:** ek baar value milti hai, dobara assign nahi hoti. Jaise bank ke `Account` me `private final String accountId`,
  jise constructor ke baad koi badle to compile hi nahi hota.
- **Method pe:** child class use override nahi kar sakti.
- **Class pe:** koi use extend nahi kar sakta, jaise `String`.

## Trap

`final List` me `add()` aur `clear()` chalte hain. `final` sirf ye rokta hai ki list kisi doosri list ki taraf ishaara na
kare, andar ka saaman badalne se nahi rokta.

## Interview line

```
final means it cannot change: a variable can't be reassigned, a method can't be
overridden, a class can't be extended. A final List can still be modified, only the
reference is fixed.
```
