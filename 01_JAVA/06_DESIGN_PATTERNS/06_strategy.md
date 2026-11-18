# Strategy Pattern — Algorithm Switching

**Koi poochhe "Strategy kya hai" to aise bolo:**
```
Ek kaam ke kai tareeke, har tareeka apni class me, sab ek interface ke peeche; chalte waqt chuno kaunsa.
Checkout pe Card / UPI / Wallet: PaymentService bas strategy.pay() bolta, kaun sa tareeka hai use farak nahi.
Naya tareeka (BNPL) = nayi class, purana code nahi khulta. Factory "kaunsa object banana", Strategy "kis tareeke se karna".
```

## Kahani

Arpan checkout pe hai: Card, UPI ya Wallet. Har ek ka tareeka alag hai, par flow ek hi hai: "itna paisa do, haan ya na batao". Agar
`PaymentService` me `if (CARD) ... else if (UPI) ...` ki chain likhi, to kal BNPL jodne ke liye wahi class kholni padegi. Strategy me har
tareeka apni class me hai, sab ek `PaymentStrategy.pay()` interface ke peeche. PaymentService (context) bas jo strategy mili, uska `pay()`
chala deta hai. Ye wahi O (Open/Closed) wali soch hai.

```
PaymentService (context) --strategy.pay(amt)--> PaymentStrategy (interface)
                                                   |-- CardPayment
                                                   |-- UpiPayment
                                                   |-- WalletPayment   (naya = sirf nayi class)
```

Factory se farak: factory tay karti hai "kaunsa object banana hai", strategy tay karti hai "kaam kis tareeke se karna hai"; aksar dono
saath aate hain. State se farak: strategy bahar se client chunta hai, state object ke andar se khud badalta hai.

## Trap

Arpan ne UPI chuna, usi pal Ravi ne Card chuna. `PaymentService` Spring bean hai, yaani saari requests ke liye ek hi object, aur strategy
usi object ki field me rakhi hai. Chala ke dekha:

```
--- strategy field me ---
  Arpan (UPI chuna): Card se 500
  Ravi (Card chuna): Card se 300
--- strategy parameter me ---
  Arpan (UPI chuna): UPI se 500
  Ravi (Card chuna): Card se 300
```

**Dikkat:** Arpan ne UPI chuna tha, par uske 500 Card se kat gaye, kyunki Arpan ki request chal hi rahi thi ki Ravi ke `setStrategy` ne
beech me field badal di. Ye wahi Singleton wali dikkat hai: wahan receipt pe Ravi ka naam aaya tha, yahan Ravi ka payment ka tareeka.

**Hal:** strategy field me mat rakho. Har call pe parameter do (`checkout(amount, strategy)`), ya `Map<String, PaymentStrategy>` inject
karke `map.get(type).pay()` karo. Spring is map ko bean ke naam se khud bhar deta hai; ye wahi Map hai jo Factory me 100 type ke liye laga tha.

## Tera project

[`todoapp/security/SecurityConfig.java`](../../07_PROJECTS/todoapp/src/main/java/com/arpan/todoapp/security/SecurityConfig.java) line 169 pe
`passwordEncoder()` `BCryptPasswordEncoder` return karta hai. `PasswordEncoder` interface hai aur BCrypt uski ek strategy. Kal Argon2 chahiye,
to sirf ye ek line badlegi, login ka code nahi.
