# volatile: Visibility (aur Ordering)

## Scene: mausi ne bola "dukaan band", peeche wala cashier nahi ruka

Raat 10 baje mausi chillaati hai "dukaan band!". Sab cashier ruk jaate hain, bas peeche wala ek nahi rukta. Usne subah ek baar
board dekha tha, "KHULA", aur ab baar-baar board dekhne ki jagah apni yaad pe chal raha hai, billing karta ja raha hai.
Chala ke dekha:

```
Bina volatile: mausi ne 'band' bola, 2 sec baad bhi cashier chal raha? true    <- latak gaya
volatile:      mausi ne 'band' bola, 2 sec baad bhi cashier chal raha? false   <- turant ruka
```

Bina volatile ke worker thread ne flag ko apni yaad (CPU cache, aur JIT ne read ko loop se bahar nikaal diya) me rakh liya,
aur asli memory me jaake dekha hi nahi. `volatile` ka matlab hai "har baar asli board dekho".

## Khaas baat

`volatile` sirf dikhne ki guarantee deta hai, atomic nahi banata. `volatile int ghee; ghee--` me file 04 wala jhagda ab bhi
rahega, kyunki padhna, ghatana aur likhna abhi bhi teen alag kaam hain.

```
volatile      -> sirf dikhna            -> FLAG (ek likhe, baaki padhein)
AtomicInteger -> dikhna + atomic (CAS)  -> COUNTER (file 06)
synchronized  -> dikhna + atomic (lock) -> 2+ line ka block ek saath
```

`volatile` ka doosra kaam kram (ordering) hai. Tiffin ki misaal: pehle tiffin locker me rakho, phir board pe "READY" likho.
`volatile` board ye pakka karta hai ki agar READY dikha, to tiffin andar milega hi, kyunki CPU dono kaam ka order ulta nahi kar
sakta (isse happens-before kehte hain). Isiliye DCL singleton me `instance` pe `volatile` zaroori hai, warna doosre thread ko
aadha bana object mil sakta hai.

```java
private static volatile Singleton instance;
```

Har jagah `volatile` mat lagao, kyunki har baar asli memory se padhna mehnga hai. Sirf wahan lagao jahan doosra thread wo
variable padhta hai.

## Prod wala trap

Usercrud [`VolatileDemoController`](../../07_PROJECTS/usercrud/src/main/java/com/arpan/usercrud/controller/VolatileDemoController.java)
me (27-Aug) bina volatile ke bhi worker ruk gaya tha. Par plain Java me (4-Oct) wahi code latak gaya, jaise upar latka.
Matlab "mere laptop pe chal gaya" ka koi bharosa nahi hai. Server ki JVM pe shutdown ke waqt app band hi nahi hoga, aur ye
bug koi test nahi pakadta. Isliye stop wale flag pe `volatile` hamesha likho.
