# Strategy Pattern — Algorithm Switching

**Kahani:** Arpan checkout pe hai: Card, UPI ya Wallet. Har ek ka tareeka alag hai, par flow ek hi hai: "itna paisa do, haan ya na batao". Agar `PaymentService` me `if (CARD) ... else if (UPI) ...` ki 200 line ki chain likhi, to kal BNPL jodne ke liye wahi class kholni padegi. Strategy me har tareeka apni class me hai, sab ek `PaymentStrategy.pay()` interface ke peeche. PaymentService bas jo strategy mili, uska `pay()` chala deta hai.

```
PaymentService (context) --strategy.pay(amt)--> PaymentStrategy (interface)
                                                   |-- CardPayment
                                                   |-- UpiPayment
                                                   |-- WalletPayment   (naya = sirf nayi class)
```

**Khaas baat:**

1. Isme teen hisse hote hain. **Strategy interface** jo batata hai ki kaam kya hai. **Concrete strategies**, jisme har algorithm ki apni ek class hoti hai. Aur **Context**, jo interface ka reference rakhta hai aur kaam us strategy ko aage de deta hai. Matlab if-else ki chain ki jagah yahan polymorphism kaam karta hai.

2. **Strategy vs Factory:** factory ye tay karti hai ki "kaunsa object **banana** hai", aur strategy ye tay karti hai ki "kaam **kis tareeke** se karna hai". Aksar dono saath aate hain: `PaymentStrategyFactory.create("UPI")` bulao, to factory tujhe UpiPayment bana ke de deti hai, aur phir wo strategy ki tarah chalti hai.

3. **Strategy vs State:** strategy **bahar se** badli jaati hai, matlab client khud chunta hai ki kaunsa tareeka chahiye. State **andar se** khud badalta hai, jaise `count > 10` hua to object apne aap WARNING state me chala gaya.

**Prod wala trap** (ye singleton wale trap ka hi bhai hai):

Spring ka `@Service` bean singleton hota hai, matlab saari requests ek hi object use karti hain. Agar tune strategy ko us object ki field me rakh diya, to sab requests wahi ek field share karti hain.

```java
@Service class PaymentService {            // Spring bean = SINGLETON, sab request ek hi object
    private PaymentStrategy strategy;      // field me strategy
    void setStrategy(PaymentStrategy s) { this.strategy = s; }
}
// Arpan ne UPI chuna, usi pal Ravi ne Card -> Ravi ka setStrategy Arpan ki request beech me badal deta
// fix: field nahi, har call pe parameter: checkout(amount, strategy)
//      ya Map<String, PaymentStrategy> inject karo (Spring bean naam se khud bhar deta), map.get(type).pay()
```

Maan lo Arpan ne UPI chuna, aur usi pal Ravi ne Card chuna. Ravi ki request ka `setStrategy` chala, aur usne Arpan ki request ke beech me hi field badal di, to Arpan ka payment Card se chala jaayega.

Fix ke do raaste hain. Pehla, strategy ko field me mat rakho, har call pe parameter ki tarah do, jaise `checkout(amount, strategy)`. Doosra, `Map<String, PaymentStrategy>` inject karo. Spring is map ko bean ke naam se apne aap bhar deta hai, aur phir tu `map.get(type).pay()` se sahi strategy utha leta hai.

**Tera project:** [`todoapp/security/SecurityConfig.java`](../../07_PROJECTS/todoapp/src/main/java/com/arpan/todoapp/security/SecurityConfig.java) line 169 pe `passwordEncoder()` hai, jo `BCryptPasswordEncoder` return karta hai. Yahan `PasswordEncoder` interface hai aur BCrypt uski ek strategy hai. Kal Argon2 chahiye, to sirf ye ek line badlegi, login ka code nahi.

```
Interview: "Strategy = har algorithm apni class me, sab ek interface ke peeche, runtime pe chuno.
if-else chain ki jagah polymorphism; naya tareeka = nayi class (OCP). Mere todoapp me PasswordEncoder
bean BCrypt return karta, wahi strategy hai. Spring me Map<String, PaymentStrategy> inject karke
type se chunta hoon; singleton bean me strategy field me nahi rakhta, warna request mix."
```
