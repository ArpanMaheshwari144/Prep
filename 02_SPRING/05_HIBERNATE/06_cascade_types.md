# Cascade Types

## Kahani: Arpan ka order

Arpan ek order deta hai, jiske andar 3 item hain: pen, copy aur bag. Ye items sirf isi order ke hain.
- PA ko sirf order ki file di jaati hai, `save(order)`. Agar cascade laga hai, to PA andar ki 3 item parchiyan bhi khud record room le jaata hai.
- Order delete kiya, to items bhi saath me delete ho gaye.
- Order se bag hataya (`items.remove(bag)`), to `orphanRemoval` ki wajah se bag ki row DB se bhi chali gayi. Ye na hota to bag "anaath" ban ke DB me pada rehta.

Lekhak aur kitaab ka rishta alag hai. Kitaab library ki bhi hai, kisi aur list me bhi ho sakti hai. Isliye lekhak delete karne pe kitaabein nahi udni chahiye, aur yahan cascade nahi lagta.

## Tasveer

```
save(order)  -> INSERT orders        (pehle parent, id chahiye)
             -> INSERT order_items x3 (order_id ke saath)
delete(order)-> SELECT items -> DELETE har item alag -> DELETE order
items.remove(bag) + orphanRemoval -> DELETE bag
```

`save(order)` pe pehle orders me INSERT hota hai, kyunki items ko order ki id chahiye, phir 3 order_items INSERT hote hain. `delete(order)` pe pehle items SELECT hote hain, phir har item ka alag DELETE chalta hai, aur aakhir me order DELETE hota hai. Aur `items.remove(bag)` ke saath orphanRemoval ho to bag ka DELETE chalta hai.

## 4 khaas baat

1. Bas ek sawaal poochho: **"kya child ka jeena parent pe tika hai?"** Agar haan (Order-OrderItem, Post-Comment), to `CascadeType.ALL` aur
   `orphanRemoval` lagao. Agar nahi (Author-Book, Category-Product), to cascade mat lagao.
2. Types ye hain: PERSIST, MERGE, REMOVE, REFRESH, DETACH, aur ALL jo in paanchon ko ek saath laga deta hai. REMOVE sabse khatarnaak hai, isliye ALL andha-dhundh mat lagao.
3. Bidirectional rishte me sirf list me add karna kaafi nahi hai, `item.setOrder(order)` bhi karna padta hai, warna DB me `order_id` NULL chala jaata hai. Isliye ek helper `addItem()` banao jo dono kaam ek saath kare.
4. Cascade REMOVE har child ke liye alag DELETE chalata hai, matlab 1000 item to 1000 query. Bulk delete ho to JPQL DELETE ya DB ka `ON DELETE CASCADE` use karo.

## Prod wala trap

```java
@ManyToOne(cascade = CascadeType.ALL)   // ulti disha me cascade!
Author author;
// bookRepo.delete(book) -> author bhi delete -> author ki baaki kitaabein bhi (FK error ya sab gaya)
```

Yahan cascade ulti disha me laga hai. Ek book delete ki to uska author bhi delete hone jaata hai, aur phir ya to FK error aata hai ya author ki baaki kitaabein bhi chali jaati hain.
Cascade hamesha parent se child ki taraf jaata hai (`@OneToMany` pe), child se parent ki taraf (`@ManyToOne` pe) kabhi nahi.

## Tera project

usercrud me Author aur Book alag-alag save aur delete hote hain, isliye wahan jaan-boojh ke cascade nahi lagaya. Interview me seedha bolo: "mere project me
child independent tha, owned child (Order-OrderItem) pe ALL + orphanRemoval lagaunga."

## Interview line

```
Cascade parent ka operation child pe chalata hai. Main tab lagata hoon jab child ka lifecycle
parent se bandha ho, jaise Order-OrderItem: CascadeType.ALL + orphanRemoval. Shared child
(Author-Book) pe nahi, aur REMOVE / ALL soch ke, kabhi @ManyToOne pe nahi.
```
