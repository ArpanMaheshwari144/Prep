# Type casting (upcasting aur downcasting)

Ye `instanceof` wali file ka hi doosra hissa hai.

## Tere bank se shuru

Tere `Main.java` me likha hai `Account a1 = new SavingsAccount(...)`. Object Savings ka hai, par reference `Account` ka. Isko
upcasting kehte hain: bachche ko parent ke dabbe me rakh diya. Ye apne aap hota hai, cast likhna nahi padta, aur hamesha safe hai,
kyunki har Savings account ek Account to hai hi.

Par upcast ke baad tu sirf wahi method bula sakta hai jo `Account` me hain. Savings ka koi khaas method bulana ho, to reference ko
wapas bachche me badalna padega. Isko downcasting kehte hain: `SavingsAccount s = (SavingsAccount) a1;`. Ye tujhe khud likhna padta
hai, aur ye risky hai.

```
UPCAST   Account a = new SavingsAccount(...)   apne aap hota hai, hamesha safe
DOWNCAST SavingsAccount s = (SavingsAccount) a  khud likhna padta hai; andar Savings na hua to ClassCastException
```

## Ye kis dikkat se bana

Upcasting isliye hai taaki saare account ek `List<Account>` me rakh sako aur ek hi loop me `calculateInterest()` bula sako.
Downcasting isliye hai taaki zaroorat pade to kisi ek type ka khaas kaam kar sako.

## Do trap

Pehla trap: compiler downcast ko rokta nahi, kyunki use nahi pata ki chalte waqt andar kya hoga. Agar andar FD tha aur tune
`(SavingsAccount)` likh diya, to crash compile pe nahi, chalte waqt hota hai. Isliye pehle `instanceof` se poochho, phir cast karo, ya
seedha `if (a instanceof SavingsAccount s)` likho.

Doosra trap: bilkul alag type pe cast, jaise `(Account) "hello"`, compile hi nahi hota. Kyunki String aur Account ka koi rishta nahi
hai, aur Java ko pehle se pata hai ki ye kabhi sahi nahi ho sakta.

## Prod wala trap

Jab JSON ko `Map<String, Object>` me padhte ho, jaise kisi webhook ka body, to Jackson chhote number ko `Integer` bana deta hai, `Long`
nahi. Phir koi `(Long) map.get("amount")` likhta hai, aur `ClassCastException` aata hai, sirf us din jab amount chhota ho. Bachao:
`((Number) map.get("amount")).longValue()`, jo Integer aur Long dono pe chalta hai. (Ye Jackson ke default niyam se likha hai, chala ke
nahi dekha.)
