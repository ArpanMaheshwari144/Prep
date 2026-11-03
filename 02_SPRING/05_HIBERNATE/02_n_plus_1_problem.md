# N+1 Problem

## Kahani: wahi PA, 5 lekhak

Arpan PA se kehta hai: "saare lekhak ki list lao, phir har lekhak ki kitaabein gino." PA ek baar record room jaata hai aur 5 lekhak ki
list le aata hai. Phir Arpan pehle lekhak ki kitaabein maangta hai, to PA dobara record room jaata hai. Doosre lekhak ke liye phir jaata hai. Is tarah 5 lekhak ke liye 5 chakkar aur lagte hain.

Kul chakkar hue 1 + 5, isi ko **N+1** kehte hain. PA bewakoof nahi hai, use bola hi gaya tha ki "kitaabein tabhi lana jab maangi jaayein" (yahi LAZY hai).

Hal ye hai ki pehli baar me hi bolo "lekhak aur unki kitaabein, dono ek saath le aana". Yahi `JOIN FETCH` hai, aur isme sirf 1 chakkar lagta hai.

## Tasveer

```
BAD (LAZY + loop)                     GOOD (JOIN FETCH)
select * from author        1         select a.*, b.* from author a
select book where author=1  +1          LEFT JOIN book b ...       1
select book where author=2  +1
...                                   = 1 query
= 1 + N
```

Bure tareeke me pehle ek query saare author laati hai, phir har author ke liye ek alag book query chalti hai, to kul 1 + N query. Achhe tareeke me ek hi query JOIN karke author aur book dono le aati hai.

## 4 khaas baat

1. Iski jad ye hai ki `@OneToMany` default LAZY hota hai, aur hum loop me `getBooks()` bulate hain. EAGER kar dene se ye theek nahi hota, kyunki tab har jagah kitaabein load hone lagti hain, chahe zaroorat ho ya nahi.
2. Hal teen hain. Pehla `LEFT JOIN FETCH` hai (sirf JOIN likha to jis lekhak ki koi kitaab nahi, wo list se gaayab ho jaata hai). Doosra `@EntityGraph(attributePaths = "books")` hai. Teesra
   `@BatchSize(10)` hai, jisme 100 lekhak ke liye 1 + 10 query lagti hain.
3. Pakadne ka tareeka: `show-sql=true` lagao, aur logs me ek hi select baar-baar dikhe to samajh lo N+1 hai.
4. Iska ek likhne wala bhai bhi hai: loop me `setPrice` kiya to N alag UPDATE jaate hain. Iska hal ek `@Modifying` bulk UPDATE hai, jo neeche dikhaya hai.

## Likhne wala bhai + almaari ka trap

```java
List<Product> products = repo.findAll();     // PA 100 file le aaya
for (Product p : products)
    p.setPrice(p.getPrice() * 0.9);          // commit pe 100 alag UPDATE
```

Hal ye hai ki PA ko ek hi hukum do: "record room me jaake saari files pe khud daam kam kar do".

```java
@Modifying(clearAutomatically = true)
@Query("UPDATE Product p SET p.price = p.price * 0.9")
void discountAll();                          // 1 UPDATE, DB khud 100 row badalta
```

Par yahan ek trap hai. PA jo bhi file laata hai, use apni **almaari** (1st-level cache) me rakh leta hai, aur usi transaction me dobara maangi jaaye to almaari se hi de deta hai.
Bulk UPDATE seedha record room me badlaav karta hai, almaari ki file ko nahi chhoota, isliye almaari wali file purani reh jaati hai:

```java
Product p = repo.findById(1L).get();   // almaari me: daam 100
repo.discountAll();                    // record room me ab 90
repo.findById(1L).get().getPrice();    // almaari se -> 100 (purana!)
```

`clearAutomatically = true` lagane se bulk UPDATE ke baad almaari khaali ho jaati hai, isliye agli baar PA record room se naya daam (90) laata hai.
HLD se jodo to ye wahi dikkat hai jahan DB badal gaya par Redis me purana data pada hai. Wahan hum key DELETE karte hain, yahan almaari khaali karte hain.

## Prod wala trap

```java
@ManyToOne                 // default EAGER!
Author author;
// bookRepo.findAll() -> har book ka author alag query = ulta N+1
```

`@ManyToOne` default me EAGER hota hai, isliye `bookRepo.findAll()` karte hi har book ke author ke liye alag query chalti hai. Ye ulta N+1 hai. Hal ye hai ki `@ManyToOne(fetch = FetchType.LAZY)` lagao, aur jahan author sach me chahiye wahan JOIN FETCH karo.

## Tera project

usercrud me `/n1/bad` aur `/n1/good` endpoint hain (21-Aug). 5 author pe logs me dekha ki bad wale me 6 query chali, aur good wale me sirf 1 query.

## Interview line

```
N+1 tab hota hai jab LAZY relation ko loop me access karte: 1 query parent ki, phir har
parent pe ek child query. Main show-sql logs me same query repeat dekh ke pakadta hoon, aur
JOIN FETCH ya @EntityGraph se ek query me laata hoon; bahut data ho to @BatchSize.
```
