# Cascade Types

## Kahani: Arpan ka order

Arpan order deta, andar 3 item: pen, copy, bag. Items sirf isi order ke.
- PA ko sirf order ki file, `save(order)`. Cascade laga ho to PA andar ki 3 item parchiyan bhi khud record room le jaata.
- Order delete kiya, items bhi saath gaye.
- Order se bag hataya (`items.remove(bag)`), `orphanRemoval` se bag ki row DB se bhi gayi. Warna bag "anaath" DB me pada.

Lekhak-kitaab alag: kitaab library ki bhi, kisi aur list me bhi. Lekhak delete pe kitaabein nahi udni chahiye -> cascade nahi.

## Tasveer

```
save(order)  -> INSERT orders        (pehle parent, id chahiye)
             -> INSERT order_items x3 (order_id ke saath)
delete(order)-> SELECT items -> DELETE har item alag -> DELETE order
items.remove(bag) + orphanRemoval -> DELETE bag
```

## 4 khaas baat

1. Ek sawaal: **"child ka jeena parent pe tika hai?"** Haan (Order-OrderItem, Post-Comment) -> `CascadeType.ALL +
   orphanRemoval`. Nahi (Author-Book, Category-Product) -> cascade nahi.
2. Types: PERSIST, MERGE, REMOVE, REFRESH, DETACH, ALL (paanchon). REMOVE sabse khatarnaak, ALL andha-dhundh nahi.
3. Bidirectional me list me add kaafi nahi, `item.setOrder(order)` bhi, warna `order_id` NULL. Helper `addItem()` jo dono kare.
4. Cascade REMOVE har child ki alag DELETE: 1000 item = 1000 query. Bulk ho to JPQL DELETE / DB ka `ON DELETE CASCADE`.

## Prod wala trap

```java
@ManyToOne(cascade = CascadeType.ALL)   // ulti disha me cascade!
Author author;
// bookRepo.delete(book) -> author bhi delete -> author ki baaki kitaabein bhi (FK error ya sab gaya)
```

Cascade parent -> child (`@OneToMany` pe), child -> parent (`@ManyToOne`) pe kabhi nahi.

## Tera project

usercrud: Author aur Book alag save / delete, isliye jaan-boojh ke cascade nahi. Interview me seedha: "mere project me
child independent tha, owned child (Order-OrderItem) pe ALL + orphanRemoval lagaunga."

## Interview line

```
Cascade parent ka operation child pe chalata hai. Main tab lagata hoon jab child ka lifecycle
parent se bandha ho, jaise Order-OrderItem: CascadeType.ALL + orphanRemoval. Shared child
(Author-Book) pe nahi, aur REMOVE / ALL soch ke, kabhi @ManyToOne pe nahi.
```
