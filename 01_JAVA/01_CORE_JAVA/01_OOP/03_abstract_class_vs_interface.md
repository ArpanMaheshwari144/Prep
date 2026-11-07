# Abstract class vs Interface: dono tere bank me pehle se

Tere bank me dono pehle se maujood hain. `Account` ek abstract class hai, jo batati hai ki ye cheez "kya HAI". `AccountRepository`
ek interface hai, jo batata hai ki ye cheez "kya KAR SAKTI hai".

```
Account (ABSTRACT CLASS)                 AccountRepository (INTERFACE)
"ye kya HAI"                             "ye kya KAR SAKTA hai"
 - balance, accountId  (state)            save() · findById() · findAll() · delete()
 - deposit/withdraw    (likha hua code)   (sirf wada, code nahi)
 - calculateInterest() (bachcha bharega)        │ implements
      │ extends                          InMemoryAccountRepository  (aaj)
 Savings / Current / FD                  JpaAccountRepository       (kal, MySQL)
```

## Ye kis dikkat se bane

**Abstract class:** teeno account me `balance` bhi same hai aur deposit/withdraw ka code bhi same hai. To use ek baar abstract
class me likh do, aur sab bachche wahi le lein. State (matlab field) aur constructor bhi yahin rehte hain, kyunki ye sab
accounts ka saanjha hissa hai.

**Interface:** `AccountService` ko sirf itna jaanna hai ki "save ho jaata hai, find ho jaata hai". Andar HashMap hai ya MySQL,
isse service ko koi fark nahi padta. Isliye aaj `InMemory` wala chal raha hai, kal MySQL wala laga do, to bhi service ki ek
line nahi badlegi. Tere `EventListener` me bhi yahi hai: aaj `EmailListener` hai, kal `SmsListener` laga sakte ho.

todoapp me bhi yahi dikhta hai: `TodoRepository extends JpaRepository<Todo, Long>`. Spring tujhe sirf interface deta hai, aur
uske andar ka code Spring khud bana deta hai.

## Trade-off

Ek class sirf EK abstract class ko extend kar sakti hai, par interface kai saare implement kar sakti hai. Ulta dekho to,
interface me state nahi hoti: na field, na constructor, sirf constant rakh sakte ho.

## Prod trap

Agar tune kisi chalte hue interface me naya method jod diya, to jitni bhi classes us interface ko implement karti hain, wo sab
ek saath compile hona band ho jaati hain, kyunki kisi ne bhi wo naya method likha nahi hai.

Java 8 me yahi dikkat aayi thi. `Collection` me `stream()` aur `Iterable` me `forEach()` jodne the, aur seedha jodte to duniya bhar ki list
classes toot jaatin. Isliye `default` method laaye gaye: method ki body interface me hi likh do, to purani classes bina chhede
chalti rehti hain.

```java
// AccountRepository me count() jodna ho:
default long count() { return findAll().size(); }   // warna InMemory + aage ki har class tootegi
```
