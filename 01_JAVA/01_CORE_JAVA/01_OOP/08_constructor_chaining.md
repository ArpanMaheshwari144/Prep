# Constructor chaining (`this()` aur `super()`)

## Tere bank se shuru

Tere `SavingsAccount` ke constructor ki pehli line hai `super(id, name, balance);`. Iska matlab hai ki pehle parent (`Account`) ka
constructor chalao, wo `accountId`, `holderName` aur `balance` set karega, uske baad bachcha apna kaam karega. Account ke saare field
Account hi set karta hai, aur teeno bachche bas `super(...)` bula dete hain. Isi ko constructor chaining kehte hain: ek constructor
apna kaam doosre constructor ko de deta hai.

Iske do roop hain. `super(...)` parent ka constructor bulata hai, aur `this(...)` usi class ka doosra constructor bulata hai.

Maan lo tujhe Savings account bina balance ke bhi banana hai, jisme shuru me 0 rahe. To naya constructor likh ke wahi teen line
dobara likhne ki jagah:

```java
SavingsAccount(String id, String name) {
    this(id, name, 0);     // apne hi 3-argument wale constructor ko kaam de diya
}
```

Ye bilkul delegation jaisa hai: jaise koi lead asli kaam ek bande ko de deta hai aur har jagah khud dobara nahi karta.

## Ye kis dikkat se bana

Agar teen constructor hon aur teeno me wahi set karne aur check karne ka code ho, to kal check badalne pe teen jagah badalna padega,
aur ek bhi jagah bhoole to bug. Chaining se asli kaam ek hi constructor me rehta hai, aur baaki sab usi ko bulate hain.

## Do niyam

`this()` ya `super()` hamesha constructor ki pehli line honi chahiye. Kyunki object banane me pehle neev (parent ka hissa) padti hai,
uske baad upar ki manzil banti hai. Neev se pehle kuch karne nahi diya jaata.

Ek constructor me dono ek saath nahi aa sakte, kyunki dono ko pehli line chahiye aur pehli line ek hi hoti hai.

## Prod wala trap (tere Spring project se juda)

Agar tu `super(...)` nahi likhta, to Java chupke se `super();` khud daal deta hai, matlab parent ka khaali (no-arg) constructor. Tere
`Account` me khaali constructor hai hi nahi, sirf 3 argument wala hai. Isliye agar koi bachcha `super(...)` likhna bhool jaaye, to
compile error aata hai ki `Account()` nahi mila.

Yahi cheez Spring me doosre roop me kaatti hai. Tere usercrud ke `User` entity pe `@NoArgsConstructor` laga hai, aur tune comment me
likha hai "JPA REQUIRED". Kyunki Hibernate DB se row padh ke object banata hai, aur wo pehle khaali constructor se object banata hai,
phir field bharta hai. Jaise hi tu koi argument wala constructor likhta hai, Java ka apne aap wala khaali constructor gayab ho jaata hai.
Phir agar `@NoArgsConstructor` bhool gaye, to app chalte waqt Hibernate error deta hai ki "No default constructor for entity".

## Interview line

```
Constructor chaining means one constructor delegating to another: this(...) for the same class,
super(...) for the parent, and it must be the first line. In my bank project every account
calls super(id, name, balance), so the common fields are set in one place. If you skip super,
Java inserts super() with no arguments, which fails when the parent has no empty constructor.
The same idea bites JPA: an entity needs a no-arg constructor, so in my usercrud User I keep
@NoArgsConstructor once I add an all-args one.
```
