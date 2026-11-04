# Singleton pattern

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

## Interview line

```
Singleton means one instance for the whole app. A plain lazy getInstance is not thread
safe; two threads can both create it. The simplest safe way is an enum, like my BankConfig.
Spring beans are singletons by default, so they must not keep mutable fields.
```
