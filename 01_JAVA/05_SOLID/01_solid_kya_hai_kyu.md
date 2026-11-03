# SOLID: ek misaal (tera payment app) pe paanchon

SOLID 5 aadatein hain jinse code badalna aasaan rehta hai. Matlab jab naya kaam jodo to purana code na toote, aur test likhte
waqt asli cheez ki jagah mock aasaani se lag jaaye.

Paanchon ko ek hi kahani pe samjhenge: tera payment app, jo paisa kaatta hai, DB me likhta hai aur SMS bhejta hai.

## Ek nazar

**S (Single Responsibility):** ek class sirf ek kaam kare. Isiliye Controller, Service aur Repository alag-alag hote hain.

**O (Open / Closed):** naya kaam aaye to nayi class banao, purani mat chhedo. Jaise `PaymentMethod` interface ke peeche har
payment ka apna class, jo Strategy pattern hai.

**L (Liskov Substitution):** bachcha baap ki jagah lag sake aur kuch na toote. Jaise FD ke `withdraw()` me exception phenkna
galat hai.

**I (Interface Segregation):** interface chhote rakho. Jo class sirf padhti hai (read-only), us pe `delete()` mat thopo.

**D (Dependency Inversion):** service asli class pe nahi, interface pe tike. Spring ka constructor injection (DI) yahi karta hai.

```
S  ek class ek kaam         -> Controller / Service / Repository alag
O  naya = nayi class         -> PaymentMethod interface (Strategy)
L  bachcha baap ki jagah     -> FD withdraw me exception = galat
I  chhote interface          -> read-only ko delete() mat thopo
D  interface pe tiko         -> constructor injection (Spring DI)
```

Har letter ki poori kahani uski apni file me hai: [S](02_S_single_responsibility.md), [O](03_O_open_closed.md),
[L](04_L_liskov_substitution.md), [I](05_I_interface_segregation.md), [D](06_D_dependency_inversion.md).

## Interview line

```
In my payment code I keep one responsibility per class: controller, service, repository. New payment
methods come in as new classes behind a PaymentMethod interface, so I extend without touching tested
code. Subclasses must honour the parent contract, so a fixed deposit does not inherit withdraw.
Interfaces stay small, and services depend on interfaces with the implementation injected by Spring,
which also makes them easy to mock in tests.
```
