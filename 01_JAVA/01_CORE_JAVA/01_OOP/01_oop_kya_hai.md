# OOP kya hai

## Tere SimpleBankSystem se (`model/Account.java`)

Tere `Account` me `balance` ka koi `setBalance()` hai hi nahi. Paisa sirf `deposit()` aur `withdraw()` se badalta hai, aur
ye dono badalne se pehle check karte hain ki amount theek hai ya nahi.

Ab soch bina OOP ke kya hota. Saare account ka balance ek bade array me pada hota, jaise `balances[0] = 1000`. Code me kahin
se bhi koi `balances[0] = 999999` likh deta, aur use rokne wala koi nahi hota. Tune 700 ticket me khud dekha hai ki "ye data
kisne aur kahan badla" dhoondhna kitna mushkil hota hai.

```
BINA OOP                          OOP (tera Account)
balances[] (sabke liye khula)     ┌─────────────────────────┐
   ^   ^   ^                      │ balance  (bahar se band) │
   |   |   |                      │ deposit()  -> check      │
 koi bhi, kahin se bhi            │ withdraw() -> check      │
                                  └─────────────────────────┘
                                    sirf darwaze se andar
```

## Ye kis dikkat se bana

OOP me data aur us data ke niyam ek hi jagah band rehte hain, aur us band dabbe ko object kehte hain. Agar data badalna hai,
to sirf darwaze se badlega, aur darwaza matlab method. Us darwaze pe check laga hota hai, isliye koi bhi galat value bina
check ke andar nahi ja sakti.

## Trade-off

Har cheez ke liye class, constructor aur getter likhne padte hain, isliye code lamba ho jaata hai. Par badle me fayda ye milta
hai ki galat value andar aane ka raasta sirf ek hota hai, to galti bhi sirf ek jagah se ho sakti hai.

## Prod trap (tere hi code me tha)

Tera `withdraw()` sirf ye dekhta tha ki `amount > balance` to nahi, par ye nahi dekhta tha ki amount minus me to nahi:

```java
Account acc = new SavingsAccount("A1", "Arpan", 1000);
acc.withdraw(-500);   // 1000 - (-500) = 1500.0 -> nikaalne ke naam pe paise ban gaye
```

`deposit()` me `amount <= 0` ka check tha, par `withdraw()` me ye check nahi tha. Matlab OOP tujhe darwaza to deta hai, par
darwaze pe guard kitna pakka hai, ye likhne wale pe hai. Ab `withdraw()` me bhi `amount <= 0` ka check laga diya hai.

## Interview line

```
OOP keeps data and the rules for changing it in one place. In my bank project, balance had
no setter; it changed only through deposit and withdraw, which validate the amount. That way
a bad value can enter only through one door, and that door is where I put the check.
```
