# Adapter Pattern — Incompatible Interfaces ko Connect

**Kahani:** Kal banking me Ravi ko SMS bhejna tha. Tera code `SmsSender` interface jaanta hai, jisme `send(phone, text)` hai. Par bank ne jo purani SMS company li hai, uski class `OldSmsVendor` hai aur uska method `pushMessage(text, phone, "IN")` hai. Naam alag, kram alag, aur uski library tu badal nahi sakta. To beech me ek chhoti class bana di, `SmsAdapter implements SmsSender`. Wo andar se `OldSmsVendor` ko call karti hai. Bilkul travel plug jaisa: Indian pin, American socket, aur beech me adapter.

```
tera code ---send(phone, text)---> SmsAdapter (implements SmsSender)
                                        | translate
                                        v
                                OldSmsVendor.pushMessage(text, phone, "IN")   (badal nahi sakte)
```

**Khaas baat:**

1. Isme teen hisse hote hain. **Target** wo interface hai jo tera code maangta hai. **Adaptee** wo class hai jo pehle se bani hai aur tu use badal nahi sakta. **Adapter** wo class hai jo target ko implement karti hai, andar adaptee ko rakhti hai, aur har call ko adaptee ki bhasha me badal ke aage bhej deti hai.

2. Adaptee ko kabhi chhoona nahi hai. Adapter use sirf lapet ke rakhta hai aur translate karta hai, aur ye kaam composition se hota hai, matlab adaptee adapter ke andar ek field ki tarah rakha hota hai.

3. **Adapter vs Decorator:** adapter interface ko **badal** deta hai, matlab wo ek translator hai. Decorator interface wahi rakhta hai aur upar se naya kaam **jodta** hai, matlab wo ek enhancer hai.

**Prod wala trap:**

`Arrays.asList(arr)` jo list deta hai, wo asli `ArrayList` nahi hai. Wo array ke upar ek adapter hai, ek "view". Isliye uski size fixed hai, kyunki neeche array hai, aur `add()` karte hi exception aata hai. Par `set()` chal jaata hai, aur wo asli array ko bhi badal deta hai, kyunki list aur array ek hi data pe khade hain.

```java
List<String> list = Arrays.asList(arr);   // asli ArrayList NAHI, array ke upar adapter ("view")
list.add("d");      // UnsupportedOperationException (array ki size fixed)
list.set(0, "z");   // chalta hai, aur ASLI array bhi badal jaata
// badalne wali list chahiye -> new ArrayList<>(Arrays.asList(arr))
```

Agar tujhe aisi list chahiye jisme add/remove ho sake, to `new ArrayList<>(Arrays.asList(arr))` se nayi list bana lo.

**Tera project:** tere project me seedha adapter nahi bana. Asli kaam me ye wahan aata hai jahan Stripe ya PSP jaisi bahar ki library ko apne `PaymentProcessor` interface ke peeche lapet ke rakhte hain. Kal vendor badla, to sirf adapter badlega, baaki code nahi.

```
Interview: "Adapter = do alag interface ke beech translator. Jo class badal nahi sakte (legacy / third-party)
use apne interface ke peeche lapet do; adapter target implement karta, andar wali class ko call deta.
Jaise Stripe API ko apne PaymentProcessor ke peeche, ya Java me InputStreamReader (bytes -> chars),
Arrays.asList (array -> List). Decorator interface wahi rakh ke kaam jodta, adapter interface badalta."
```
