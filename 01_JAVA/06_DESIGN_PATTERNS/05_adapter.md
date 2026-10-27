# Adapter Pattern — Incompatible Interfaces ko Connect

**Kahani:** Kal banking me Ravi ko SMS bhejna tha. Tera code `SmsSender` interface jaanta hai, jisme `send(phone, text)` hai. Par bank ne jo purani SMS company li hai, uski class `OldSmsVendor` hai aur uska method `pushMessage(text, phone, "IN")` hai. Naam alag, kram alag, aur uski library tu badal nahi sakta. To beech me ek chhoti class bana di, `SmsAdapter implements SmsSender`. Wo andar se `OldSmsVendor` ko call karti hai. Bilkul travel plug jaisa: Indian pin, American socket, aur beech me adapter.

```
tera code ---send(phone, text)---> SmsAdapter (implements SmsSender)
                                        | translate
                                        v
                                OldSmsVendor.pushMessage(text, phone, "IN")   (badal nahi sakte)
```

**Khaas baat:**
1. Teen hisse hain: **Target** (jo interface tera code maangta hai), **Adaptee** (jo class pehle se hai, badal nahi sakte), aur **Adapter** (target implement karta hai, andar adaptee rakhta hai aur call aage bhejta hai).
2. Adaptee ko kabhi chhoona nahi. Adapter sirf lapet ke translate karta hai, composition se.
3. **Adapter vs Decorator:** adapter interface **badalta** hai (translator). Decorator wahi interface rakh ke kaam **jodta** hai (enhancer).

**Prod wala trap:**
```java
List<String> list = Arrays.asList(arr);   // asli ArrayList NAHI, array ke upar adapter ("view")
list.add("d");      // UnsupportedOperationException (array ki size fixed)
list.set(0, "z");   // chalta hai, aur ASLI array bhi badal jaata
// badalne wali list chahiye -> new ArrayList<>(Arrays.asList(arr))
```

**Tera project:** project me seedha adapter nahi bana. Asli kaam me ye wahan aata hai jahan Stripe ya PSP jaisi bahar ki library ko apne `PaymentProcessor` interface ke peeche lapet ke rakhte hain. Kal vendor badla, to sirf adapter badlega.

```
Interview: "Adapter = do alag interface ke beech translator. Jo class badal nahi sakte (legacy / third-party)
use apne interface ke peeche lapet do; adapter target implement karta, andar wali class ko call deta.
Jaise Stripe API ko apne PaymentProcessor ke peeche, ya Java me InputStreamReader (bytes -> chars),
Arrays.asList (array -> List). Decorator interface wahi rakh ke kaam jodta, adapter interface badalta."
```
