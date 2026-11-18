# Singleton pattern

**Koi poochhe "Singleton kya hai" to aise bolo:**
```
Poori app me us class ka sirf ek object, jaise config ya connection pool. Seedha "null hai to
bana do" do thread me do object bana deta, isliye enum (ya double-check lock). Spring ka har
@Service by default singleton hai, isliye uske field me request ka data kabhi mat rakho.
```

Singleton matlab poori app me us class ka sirf ek object. Config, logger, connection pool jaisi cheezon ke liye, kyunki do alag
config object ho gaye to ek jagah limit 1 lakh hogi aur doosri jagah kuch aur.

## Dikkat

Seedha tareeka hai "object nahi hai to bana do". Do thread ek saath chhode:

```java
static Config get() { if (inst == null) inst = new Config(); return inst; }
```

```
Config kitni baar bana: 2
```

Dono ne ek hi pal me "null hai" dekha aur dono ne apna-apna object bana diya, to singleton hi toot gaya.

## Sabse aasaan ilaaj: enum

Bank ka `BankConfig` enum hai jisme `INSTANCE` hai. JVM khud pakka karta hai ki ek hi object bane, thread aur reflection dono se
safe. Ek line ka kaam hai.

## Spring me

Har `@Service` / `@Repository` by default singleton hota hai: Spring ek hi object banata hai aur saari request use share karti
hain. Isliye singleton bean ke field me koi badalne wali cheez rakhi to saari request use ek saath chhuengi. Ye wahi "singleton
me field" wala PR bug hai.

Chala ke dekha (ek hi bean, field `currentUser`, do request 10 ms ke farak se):

```
Arpan ne pay kiya, receipt pe naam: Ravi
Ravi ne pay kiya, receipt pe naam: Ravi
```

Arpan ki receipt pe Ravi ka naam: Ravi ki request ne beech me wahi field badal di. Ilaaj: request ka data method ke andar local
variable / parameter me rakho, field me sirf wo jo kabhi na badle (repo, config).
