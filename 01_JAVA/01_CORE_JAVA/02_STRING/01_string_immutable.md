# String immutable kyun hai

## Scene 1: email do baar register ho gaya

Tere paas ek `Set` hai jisme pehle se `"arpan@gmail.com"` pada hai. Naya user `"  Arpan@Gmail.com "` bhejta hai, aage-peeche
space aur bade akshar ke saath. Code use saaf karta hai, phir dekhta hai ki Set me pehle se hai ya nahi:

```java
String email = "  Arpan@Gmail.com ";
email.trim();
email.toLowerCase();
System.out.println("[" + email + "] " + seen.contains(email));
```

Chala ke dekha, output:

```
[  Arpan@Gmail.com ] false
```

Email jaisa tha waisa hi raha, aur Set ne bola "nahi hai", isliye wahi email doosri baar register ho gaya. Kyunki String
kabhi badalti nahi. `trim()` aur `toLowerCase()` purani String ko nahi chhoote, ek NAYI String bana ke lautate hain. Upar ke
code ne wo nayi String kahin rakhi hi nahi, isliye wo wahin kho gayi.

Ilaaj ye hai ki nayi String wapas variable me rakho:

```java
email = email.trim().toLowerCase();
```

```
[arpan@gmail.com] true
```

Matlab immutable ka seedha sa matlab yahi hai: String ek baar ban gayi to uske andar ka text kabhi nahi badalta. Badla hua
chahiye to naya object banta hai, aur variable ko us naye object ki taraf mod dena tera kaam hai.

## Scene 2: webhook ka "SUCCESS" match hi nahi hua

Payment company webhook bhejti hai aur usme `status` aata hai `"SUCCESS"`. Code me kisi ne likha hai
`if (status == "SUCCESS")`. Test me ye chal jaata hai, par production me kabhi true nahi hota.

```java
String a = "SUCCESS";
String b = "SUCCESS";
String c = new String("SUCCESS");   // JSON se padhi hui String aise hi nayi banti hai
System.out.println(a == b);         // true
System.out.println(a == c);         // false
System.out.println(a.equals(c));    // true
```

Ye isliye hota hai kyunki Java seedhi likhi String (literal) ko ek saanjhi jagah me rakhta hai, jise String Pool kehte hain.
`a` aur `b` dono ka text same hai, isliye dono pool ke ek hi object ki taraf ishaara karte hain, aur `==` true aata hai. Par
JSON se padhi hui String runtime pe nayi banti hai, pool ke bahar. `==` sirf ye dekhta hai ki dono ek hi object hain ya nahi,
text nahi dekhta. Isliye wahan false aata hai. Text milaana ho to hamesha `equals()` lagao.

```
STACK          String Pool              Pool ke bahar
a ──────────►  "SUCCESS"   ◄────── b
c ──────────────────────────────►  "SUCCESS" (naya object)
```

## Pool isi wajah se chal paata hai

Pool me ek hi `"SUCCESS"` ko hazaar variable share karte hain. Agar String badal sakti, to ek jagah kisi ne badli aur baaki
saari jagah bhi badal jaati. Isliye String immutable rakhi gayi, taaki share karna safe rahe. Iske teen fayde aur hain.
Pehla, kai thread ek hi String ko bina lock ke padh sakte hain, kyunki koi use badal hi nahi sakta. Doosra, HashMap ki key
safe rehti hai: key ka text badal jaata to uska hashCode bhi badal jaata, aur value kabhi milti hi nahi. Teesra, security:
file ka path ya DB ka URL check karne ke baad koi use chupke se badal nahi sakta.

## Ek aur trap: "Hel" + "lo"

```java
String s1 = "Hello";
String x  = "Hel";
System.out.println(s1 == ("Hel" + "lo"));   // true
System.out.println(s1 == (x + "lo"));       // false
```

Jab dono hisse seedhe likhe text hain, to compiler khud hi unhe jod ke `"Hello"` bana deta hai, isliye wo pool wala hi object
milta hai. Par jab ek hissa variable hai, to jodna chalte waqt hota hai aur naya object banta hai, isliye `==` false aata hai.
Yahan bhi jawab wahi hai: `equals()`.

## Prod wala trap

```java
if (status.equals("SUCCESS"))   // status null aaya to NullPointerException
if ("SUCCESS".equals(status))   // status null ho to bhi seedha false
```

Webhook me `status` field kabhi-kabhi aata hi nahi, aur tab ye null hoti hai. Pehli line pe request NPE se toot jaati hai.
Isliye constant ko aage rakho, kyunki `"SUCCESS"` kabhi null nahi hota.
