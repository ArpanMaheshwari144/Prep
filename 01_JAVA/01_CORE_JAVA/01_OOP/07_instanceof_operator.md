# instanceof

## Tere bank se shuru

Tere bank me saare account ek `List<Account>` me rakhe hain. Maan lo tujhe sirf FD wale account ki maturity date dekhni hai, aur
`getMaturityDate()` sirf `FixedDeposit` class me hai, `Account` me nahi. Tu `account.getMaturityDate()` likhega to compile hi nahi
hoga, kyunki reference `Account` ka hai aur Java ko nahi pata ki andar FD hai ya Savings.

Isliye pehle poochna padta hai ki "andar asal me kaun hai?". Yahi `instanceof` karta hai:

```java
for (Account a : accounts) {
    if (a instanceof FixedDeposit fd) {    // andar FD hai? haan to wahi object 'fd' naam se de do
        fd.getMaturityDate();
    }
}
```

Java 16 se pehle check aur cast alag-alag likhna padta tha: pehle `if (a instanceof FixedDeposit)`, phir
`FixedDeposit fd = (FixedDeposit) a;`. Ab ek hi line me dono ho jaate hain.

## Ye kis dikkat se bana

Bina check kiye cast karoge, jaise Savings account ko `(FixedDeposit) a` bana diya, to code compile to ho jaayega, par chalte waqt
`ClassCastException` aayega aur request wahin toot jaayegi. `instanceof` pehle hi bata deta hai ki cast safe hai ya nahi. Aur `null`
pe ye exception nahi deta, seedha `false` deta hai.

## Prod wala trap: design ka

Maan lo kisi ne byaaj aise likha:

```java
if (a instanceof SavingsAccount) { ... 4% ... }
else if (a instanceof FixedDeposit) { ... 7% ... }
else if (a instanceof CurrentAccount) { ... 0 ... }
```

Kal "Senior Citizen" account aaya, to ye `if-else` har jagah dhoondh ke badalna padega, aur ek bhi jagah bhool gaye to us account ka
byaaj galat aayega. Tune apne code me ye galti nahi ki: har account khud `calculateInterest()` likhta hai, aur tu sirf
`a.calculateInterest()` bulata hai.

Matlab jahan `instanceof` ki lambi chain dikhe, samajh lo wahan polymorphism hona chahiye tha. `instanceof` ek-aadh jagah ke liye
theek hai, jaise sirf FD ki maturity dekhni ho.

## Interview line

```
instanceof checks the real type of an object at runtime before a cast, so we avoid
ClassCastException, and it returns false for null. Since Java 16 the check and cast are one
step: if (a instanceof FixedDeposit fd). But a long instanceof chain usually means missing
polymorphism; in my bank each account overrides calculateInterest, so I never branch on type.
```
