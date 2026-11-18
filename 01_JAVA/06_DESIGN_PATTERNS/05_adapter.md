# Adapter Pattern — Incompatible Interfaces ko Connect

**Koi poochhe "Adapter kya hai" to aise bolo:**
```
Do alag bhasha wali cheezon ke beech translator. Jo class badal nahi sakte (purani / bahar ki library),
use apne interface ke peeche lapet do. Travel plug jaisa: Indian pin, American socket, beech me adapter.
Kal vendor badla to sirf adapter badlega, baaki code nahi.
```

## Kahani

Banking me Ravi ko SMS bhejna hai. Tera code `SmsSender` interface jaanta hai, jisme `send(phone, text)` hai. Par bank ne jo purani SMS
company li hai, uski class `OldSmsVendor` hai aur uska method `pushMessage(text, phone, "IN")` hai. Naam alag, kram alag, aur uski library
tu badal nahi sakta. To beech me ek chhoti class bana di, `SmsAdapter implements SmsSender`, jo andar se `OldSmsVendor` ko call karti hai.

```
tera code ---send(phone, text)---> SmsAdapter (implements SmsSender)
                                        | translate
                                        v
                                OldSmsVendor.pushMessage(text, phone, "IN")   (badal nahi sakte)
```

Teen hisse hain. **Target** wo interface hai jo tera code maangta hai. **Adaptee** wo class hai jo pehle se bani hai aur tu use badal
nahi sakta. **Adapter** target ko implement karti hai, adaptee ko apne andar ek field ki tarah rakhti hai (composition), aur har call ko
adaptee ki bhasha me badal ke aage bhej deti hai. Decorator se farak: adapter interface ko badal deta hai (translator), decorator
interface wahi rakh ke upar se kaam jodta hai.

## Trap

`Arrays.asList(arr)` bhi ek adapter hai: array ko List ki shakal deta hai, par asli ArrayList nahi hai. Chala ke dekha:

```
list ki class: java.util.Arrays$ArrayList
add("Amit"): UnsupportedOperationException
set(0,"Rahul") ke baad arr[0] = Rahul
new ArrayList pe add: [Rahul, Ravi, Priya, Amit]
```

**Dikkat:** class ka naam bhi `ArrayList` dikhta hai, par ye `Arrays` ke andar wali class hai. Neeche array hai, isliye size fix hai
aur `add` karte hi exception aata hai. `set` chal jaata hai aur asli array bhi badal deta hai, kyunki list aur array ek hi data pe khade hain.

**Hal:** jis list me jodna-hatana ho, use `new ArrayList<>(Arrays.asList(arr))` se banao.

## Tera project

Tere project me seedha adapter nahi bana. Asli kaam me ye wahan aata hai jahan Stripe ya PSP jaisi bahar ki library ko apne
`PaymentProcessor` interface ke peeche lapet ke rakhte hain. Kal vendor badla, to sirf adapter badlega, baaki code nahi.
