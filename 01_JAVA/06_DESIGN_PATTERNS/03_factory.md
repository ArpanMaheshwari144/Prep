# Factory pattern

Factory matlab object kaunsa banega, ye faisla ek jagah. Bulane wala bas bolta hai "mujhe savings chahiye", `new
SavingsAccount(...)` khud nahi likhta.

## Tera bank: AccountFactory

```java
public static Account create(String type, String id, String name, double balance) {
    switch (type.toLowerCase()) {
        case "savings": return new SavingsAccount(id, name, balance);
        case "current": return new CurrentAccount(id, name, balance);
        case "fd":      return new FixedDeposit(id, name, balance);
        default: throw new IllegalArgumentException("Unknown account type: " + type);
    }
}
```

## Dikkat jo isse hal hui

Agar 10 jagah `new SavingsAccount(...)` likha hota aur kal Savings ka constructor badalta, to 10 jagah badalna padta. Ab sirf
factory me badalna hai. Aur factory `Account` lautati hai, kisi khaas class ko nahi, isliye bulane wale ko pata hi nahi ki andar
kaunsa bachcha hai. Ye wahi polymorphism hai jo OOP me dekha.

## Trade-off

Naya account type (Senior Citizen) aaye to `switch` me ek `case` jodna padta hai, yaani purani file kholni padti hai. Chhote
project ke liye ye theek hai. Bahut saare type hon to `Map<String, Supplier<Account>>` me register karte hain, taaki factory ki
file chhedni hi na pade.

## Spring me

Spring khud ek badi factory hai: tu `new UserService()` kabhi nahi likhta, Spring object banata hai aur de deta hai.
