# SOLID: ek misaal (tera payment app) pe paanchon

SOLID 5 aadatein hain jinse code badalna aasaan rehta hai. Matlab jab naya kaam jodo to purana code na toote, aur test likhte
waqt asli cheez ki jagah mock aasaani se lag jaaye.

Paanchon ko ek hi kahani pe samjhenge: tera payment app, jo paisa kaatta hai, DB me likhta hai aur SMS bhejta hai.

```
S  ek class, ek kaam            -> PaymentService sirf paisa; DB = Repository, SMS = NotificationService
O  naya jodo, purana mat chhedo -> PaymentMethod interface; kal Crypto aaya to bas nayi class
L  bachcha baap ki jagah        -> FD ne withdraw() me exception phenki = galat; FD ko withdraw diya hi mat
I  mota interface mat banao     -> report wali class sirf Readable; use delete() pe majboor mat karo
D  interface pe tiko            -> service me new MySqlRepo() nahi; repo constructor se bahar se aaye
```

## Ek nazar (misaal ke saath)

**S (Single Responsibility):** ek class sirf ek kaam kare. PaymentService me paisa, DB aur SMS teeno the, to marketing ne SMS
badla aur paisa wali class kholni padi. Isiliye Controller, Service aur Repository alag hote hain, jaise restaurant me cook,
waiter aur cashier alag: cook badla to cashier pe asar nahi.

**O (Open / Closed):** naya payment mode aaya to `else if` ki chain mat badhao, nayi class banao. Jaise board pe naya socket
laga dete ho, poori building ki wiring nahi kholte. Ye wahi Strategy pattern hai (`PaymentMethod` interface).

**L (Liskov Substitution):** jahan baap chalta hai, wahan bachcha daal do to kuch na toote. Penguin Bird hai par ud nahi sakta;
FD Account hai par withdraw nahi kar sakta, to use withdraw wali class se mat banao. Pehchaan: override karke exception phenki
ya method khaali chhoda, to L toota.

**I (Interface Segregation):** chhote interface rakho. TV ke remote me AC ke button nahi hote. Read-only class ke paas
`delete()` hoga hi nahi, to koi galti se bula bhi nahi sakega.

**D (Dependency Inversion):** service andar `new` nahi likhti, interface leti hai, aur asli object bahar se aata hai. Socket
interface hai, usme koi bhi charger laga do. Spring ka constructor injection yahi karta hai, isliye test me
`new PaymentService(mockRepo)` chal jaata hai.

## PR drill me kahan dikhta

- "controller me hi DB query likh di" = S toota.
- "andar `new` likha hai" ya "field pe `@Autowired`" = D toota.

Har letter ki poori kahani uski apni file me hai: [S](02_S_single_responsibility.md), [O](03_O_open_closed.md),
[L](04_L_liskov_substitution.md), [I](05_I_interface_segregation.md), [D](06_D_dependency_inversion.md).
