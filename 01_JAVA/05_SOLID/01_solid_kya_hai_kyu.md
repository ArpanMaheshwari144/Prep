# SOLID: ek misaal (tera payment app) pe paanchon

SOLID = 5 aadatein jisse code badalna aasaan rahe: naya kaam jodo to purana na toote, test me mock lag jaaye.
Paanchon ek hi kahani pe: tera payment app (paisa kaato, DB me likho, SMS bhejo).

## Ek nazar

```
S  ek class ek kaam         -> Controller / Service / Repository alag
O  naya = nayi class         -> PaymentMethod interface (Strategy)
L  bachcha baap ki jagah     -> FD withdraw me exception = galat
I  chhote interface          -> read-only ko delete() mat thopo
D  interface pe tiko         -> constructor injection (Spring DI)
```

Har letter ki poori kahani: [S](02_S_single_responsibility.md) · [O](03_O_open_closed.md) ·
[L](04_L_liskov_substitution.md) · [I](05_I_interface_segregation.md) · [D](06_D_dependency_inversion.md)

## Interview line

```
In my payment code I keep one responsibility per class: controller, service, repository. New payment
methods come in as new classes behind a PaymentMethod interface, so I extend without touching tested
code. Subclasses must honour the parent contract, so a fixed deposit does not inherit withdraw.
Interfaces stay small, and services depend on interfaces with the implementation injected by Spring,
which also makes them easy to mock in tests.
```
