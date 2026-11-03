# OOP kya hai

## Tere SimpleBankSystem se (`model/Account.java`)

`balance` ka koi `setBalance()` nahi. Paisa sirf `deposit()` / `withdraw()` se badalta, aur dono pehle check karte.

Bina OOP: saare account ka balance ek bade array me, `balances[0] = 1000`. Code me kahin se bhi koi
`balances[0] = 999999` likh de, rokne wala koi nahi. 700 ticket me dekha hai: "data kisne, kahan badla" dhoondhna kitna mushkil.

```
BINA OOP                          OOP (tera Account)
balances[] (sabke liye khula)     ┌─────────────────────────┐
   ^   ^   ^                      │ balance  (bahar se band) │
   |   |   |                      │ deposit()  -> check      │
 koi bhi, kahin se bhi            │ withdraw() -> check      │
                                  └─────────────────────────┘
                                    sirf darwaze se andar
```

## Kis dikkat se bana

Data aur uske niyam ek jagah band (object). Badalna ho to sirf darwaze (method) se, darwaze pe check.

## Trade-off

Har cheez ki class, constructor, getter = code lamba. Badle me: galti sirf ek jagah se ho sakti.

## Prod trap (tere hi code me tha)

`withdraw()` sirf `amount > balance` dekhta tha, minus amount nahi:

```java
Account acc = new SavingsAccount("A1", "Arpan", 1000);
acc.withdraw(-500);   // 1000 - (-500) = 1500.0 -> nikaalne ke naam pe paise ban gaye
```

`deposit()` me `amount <= 0` ka check tha, `withdraw()` me nahi. OOP darwaza deta hai, guard kitna pakka ye likhne wale pe.
Ab `withdraw()` me bhi `amount <= 0` check laga diya.

## Interview line

```
OOP keeps data and the rules for changing it in one place. In my bank project, balance had
no setter; it changed only through deposit and withdraw, which validate the amount. That way
a bad value can enter only through one door, and that door is where I put the check.
```
