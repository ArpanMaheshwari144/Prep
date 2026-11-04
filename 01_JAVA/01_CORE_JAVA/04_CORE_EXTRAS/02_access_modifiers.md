# Access modifiers

```
private    -> sirf apni class
default    -> (kuch nahi likha) apna package
protected  -> apna package + alag package ki child class
public     -> sab jagah
```

Niyam: sabse chhota rakho. `private` se shuru karo, zaroorat pade tabhi badhao.

## Tera bank

Bank ke `Account` me `balance` pehle `protected` tha, to Savings ya FD seedha `balance = 0` likh sakte the aur `withdraw()` ka
check beech me aata hi nahi tha. `private` karne ke baad bachche sirf `getBalance()` se padhte hain, aur badalna sirf
`deposit()` / `withdraw()` se hota hai. Access modifier ka asli kaam yahi hai.

## Trap

`default` ka matlab `public` nahi hai. Kuch nahi likha to wo cheez sirf apne package me dikhti hai, alag package ki child class
ko bhi nahi. Wo `protected` ka kaam hai.

## Interview line

```
private is class only, default is package, protected adds subclasses in other packages,
public is everywhere. I keep fields private, like balance in my bank project, so it only
changes through deposit and withdraw.
```
