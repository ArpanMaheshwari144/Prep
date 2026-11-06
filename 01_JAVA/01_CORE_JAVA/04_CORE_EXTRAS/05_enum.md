# enum

`enum` matlab gine-chune pakke naam, inke alawa kuch nahi.

```java
enum Status { PAID, REFUNDED }
Status s = Status.EMI;   // compile error, EMI hai hi nahi
```

Agar status `int` me rakhte (1 = PAID, 2 = REFUNDED), to koi `99` bhi bhej deta aur compiler kuch nahi kehta. Enum me jo naam
likhe hain wahi chalte hain.

## Tera bank

`BankConfig` ek enum hai jisme `INSTANCE` hai, matlab singleton enum se bana hai. JVM khud pakka karta hai ki iska ek hi object
bane, thread aur reflection dono se safe.

## Prod wala trap

JPA by default enum ko DB me uske number (`ordinal`) se save karta hai. Ek refund wali row DB me `1` ban ke gayi. Baad me kisi
ne enum ke sabse upar `PENDING` jod diya, aur wahi row aaj padhi:

```
DB me likha: 1  ->  aaj padha to: PAID
```

Refund wala order ab PAID dikh raha hai, kyunki naya naam aage judne se saare number khisak gaye. Ilaaj:
`@Enumerated(EnumType.STRING)`, taaki DB me number ki jagah naam `"REFUNDED"` jaaye.

Enum ko `==` se milana sahi hai, kyunki har naam ka ek hi object hota hai. Ye String wala trap nahi hai.
