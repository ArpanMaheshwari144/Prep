# Strategy Pattern — Algorithm Switching

**Kahani:** Arpan checkout pe hai: Card, UPI ya Wallet. Har ek ka tareeka alag hai, par flow ek hi hai: "itna paisa do, haan ya na batao". Agar `PaymentService` me `if (CARD) ... else if (UPI) ...` ki 200 line ki chain likhi, to kal BNPL jodne ke liye wahi class kholni padegi. Strategy me har tareeka apni class me hai, sab ek `PaymentStrategy.pay()` interface ke peeche. PaymentService bas jo strategy mili, uska `pay()` chala deta hai.

```
PaymentService (context) --strategy.pay(amt)--> PaymentStrategy (interface)
                                                   |-- CardPayment
                                                   |-- UpiPayment
                                                   |-- WalletPayment   (naya = sirf nayi class)
```

**Khaas baat:**
1. Teen hisse hain: **Strategy interface**, **concrete strategies** (ek algorithm = ek class), aur **Context** (interface ka reference rakh ke kaam aage deta hai). Ye if-else ki chain ki jagah polymorphism hai.
2. **Strategy vs Factory:** factory tay karti hai "kaunsa object **banana** hai", strategy tay karti hai "kaam **kis tareeke** se karna hai". Aksar dono saath aate hain: `PaymentStrategyFactory.create("UPI")` se UpiPayment milta hai.
3. **Strategy vs State:** strategy **bahar se** badli jaati hai (client chunta hai). State **andar se** khud badalta hai (`count > 10` hua to WARNING).

**Prod wala trap** (singleton wale trap ka bhai):
```java
@Service class PaymentService {            // Spring bean = SINGLETON, sab request ek hi object
    private PaymentStrategy strategy;      // field me strategy
    void setStrategy(PaymentStrategy s) { this.strategy = s; }
}
// Arpan ne UPI chuna, usi pal Ravi ne Card -> Ravi ka setStrategy Arpan ki request beech me badal deta
// fix: field nahi, har call pe parameter: checkout(amount, strategy)
//      ya Map<String, PaymentStrategy> inject karo (Spring bean naam se khud bhar deta), map.get(type).pay()
```

**Tera project:** [`todoapp/security/SecurityConfig.java`](../../07_PROJECTS/todoapp/src/main/java/com/arpan/todoapp/security/SecurityConfig.java) line 169, `passwordEncoder()` jo `BCryptPasswordEncoder` return karta hai. `PasswordEncoder` interface hai aur BCrypt ek strategy hai. Kal Argon2 chahiye to sirf ye ek line badlegi, login ka code nahi.

```
Interview: "Strategy = har algorithm apni class me, sab ek interface ke peeche, runtime pe chuno.
if-else chain ki jagah polymorphism; naya tareeka = nayi class (OCP). Mere todoapp me PasswordEncoder
bean BCrypt return karta, wahi strategy hai. Spring me Map<String, PaymentStrategy> inject karke
type se chunta hoon; singleton bean me strategy field me nahi rakhta, warna request mix."
```
