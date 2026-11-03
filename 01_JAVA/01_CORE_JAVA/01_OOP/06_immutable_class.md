# Immutable class

## Tere mini_payment se shuru

`paymentservice` me `PayRequest` ek `record` hai: `record PayRequest(String orderId, double amount)`. Record Java ka bana-banaya
immutable class hai. Ek baar object ban gaya, to uska `orderId` ya `amount` koi badal nahi sakta, kyunki usme setter hota hi nahi.
Isiliye tere controller me `req.orderId()` likha hai, `getOrderId()` nahi.

Immutable class ka matlab hai ki object bante waqt jo value daali, wahi zindagi bhar rahegi. Kuch badalna ho, to naya object banao,
aur purana waisa hi rahega.

## Ye kis dikkat se bana

Pehli dikkat thread ki hai. Maan lo do thread ek hi `PayRequest` padh rahe hain. Agar use koi badal hi nahi sakta, to dono ke beech
ladai ka sawaal hi nahi uthta, aur lock lagane ki zaroorat bhi nahi padti.

Doosri dikkat HashMap key ki hai, jo Collections me padha tha. Agar key object ki value baad me badal jaaye, to uska `hashCode` badal
jaata hai. Phir HashMap use galat bucket me dhoondhta hai, aur tera data map me hote hue bhi nahi milta. Isliye `String` aur `Integer`
jaisi immutable cheezein sabse achhi key hoti hain.

Teesri dikkat ye hai ki tune koi object kisi function ko diya aur us function ne chupke se uski value badal di. Immutable object pe
ye ho hi nahi sakta.

## Banane ke 4 niyam

Class `final` ho, taaki koi bachcha class bana ke niyam na tod de. Fields `private final` hon. Value sirf constructor me set ho.
Aur setter ek bhi na ho.

## Prod wala trap: andar List ho to `final` bhi nahi bachata

Maine scratchpad me ek record banaya jisme ek List hai, aur chala ke dekha:

```java
record Order(String orderId, List<String> items) {}

List<String> list = new ArrayList<>(List.of("Pizza"));
Order o = new Order("O1", list);
list.add("Coke");             // bahar wali list me add kiya
o.items().add("Fries");       // getter se mili list me add kiya

// output: bahar se add ke baad order ke items = [Pizza, Coke]
//         getter se add ke baad = [Pizza, Coke, Fries]
```

Order "immutable" tha, phir bhi uske items badal gaye. Kyunki `final` sirf itna rokta hai ki `items` kisi doosri list ki taraf na
mude. List ke andar `add` karna wo nahi rokta. Upar se record ne bahar wali list ka hi reference rakh liya, apni alag copy nahi
banayi. Isliye bahar se jo add hua, wo order ke andar bhi dikh gaya.

Bachao: constructor me apni copy rakh lo, jaise `this.items = List.copyOf(items)`. Ye copy bhi banata hai, aur us copy me `add`
karne pe exception bhi deta hai. Agar `List.copyOf` nahi lagaya, to constructor aur getter dono me `new ArrayList<>(...)` se copy
deni padti hai.

## Interview line

```
An immutable class sets all state once in the constructor and never changes it: final class,
private final fields, no setters. In my payment service, PayRequest is a record, which is
immutable by default. It gives free thread safety and safe HashMap keys. The trap is a mutable
field like a List: final stops reassignment, not add(), so copy it in the constructor with
List.copyOf.
```
