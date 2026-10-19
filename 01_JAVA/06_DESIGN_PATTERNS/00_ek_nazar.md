# 6 Design Pattern — Ek Nazar (kab kaunsa)

```
BUILDER    -> 5+ field, kai optional: object ko kadam-kadam banao, aakhir me build() (Subway sandwich)
SINGLETON  -> poore app me sirf EK chahiye (config, pool): private constructor + getInstance() (President)
FACTORY    -> caller sirf "type" bole, kaunsi class bane ye ek jagah tay ho (pizza counter)
OBSERVER   -> ek ghatna, kai sunne wale, bhejne wala unhe jaane bhi nahi (YouTube subscribe)
ADAPTER    -> jo hai uski shape match nahi karti, beech me translator (travel plug)
STRATEGY   -> ek kaam ke kai tareeke, if-else ki jagah har tareeka alag class, runtime pe badlo (checkout)
```

## Pehchaan ka shortcut

```
"kaise BANAU?"           -> Builder / Factory / Singleton
"kaise JODU / FIT karu?" -> Adapter
"kaise BATAU sabko?"     -> Observer
"kaise BADLU tareeka?"   -> Strategy
```

## Tere project me kahan

```
BUILDER    -> usercrud: Jwts.builder() (JWT) · Spring Security User.builder()
SINGLETON  -> SimpleBankSystem: BankConfig enum · saare Spring bean (default singleton)
FACTORY    -> SimpleBankSystem: AccountFactory.create("savings", ...)
OBSERVER   -> SimpleBankSystem: EventPublisher + EmailListener · Spring @TransactionalEventListener
ADAPTER    -> 3rd party (Stripe / Twilio) apne interface ke peeche · Arrays.asList (view)
STRATEGY   -> usercrud: PasswordEncoder (BCrypt) · Comparator
```

Poori file: [builder](01_builder.md) · [singleton](02_singleton.md) · [factory](03_factory.md) ·
[observer](04_observer.md) · [adapter](05_adapter.md) · [strategy](06_strategy.md) · code: `practice/DP1-DP5`
