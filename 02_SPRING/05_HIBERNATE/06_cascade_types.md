# Cascade Types

## Scene: Flipkart pe tera order

Ek order me 3 item hain: pen, copy aur bag. Ye item sirf isi order ke hain, order ke bina inka koi matlab nahi.

```java
@OneToMany(mappedBy = "order", cascade = CascadeType.ALL, orphanRemoval = true)
List<OrderItem> items;

orderRepo.save(order);          // INSERT orders, phir INSERT order_items x3
orderRepo.delete(order);        // DELETE har item, phir DELETE order
order.getItems().remove(bag);   // commit pe DELETE bag
```

- Sirf `save(order)` bola, par Hibernate andar ke 3 item bhi khud le gaya. Pehle order ka INSERT hua, kyunki item ko `order_id` chahiye.
- Order delete kiya to item bhi saath me gaye.
- List se bag hataya to `orphanRemoval` ne DB se bhi bag ki row uda di. Ye na hota to bag bina order ke DB me pada rehta.

## Ek hi sawaal: "kya child ka jeena parent pe tika hai?"

- **Haan** (Order-OrderItem, Post-Comment): `CascadeType.ALL` + `orphanRemoval`.
- **Nahi** (Author-Book, Category-Product): cascade mat lagao. Isi liye usercrud me Author aur Book alag-alag save / delete hote hain. Author
  hataya to kitaabein nahi udni chahiye.

## Do chhoti dikkat

- **Do taraf ka rishta:** sirf `items.add(item)` kaafi nahi, `item.setOrder(order)` bhi karna padta hai, warna DB me `order_id` NULL jaata
  hai. Isliye ek `addItem()` helper banao jo dono kaam saath kare.
- **1000 item pe 1000 DELETE:** cascade har child ka alag DELETE chalata hai. Bada delete ho to ek JPQL `DELETE` ya DB ka
  `ON DELETE CASCADE` use karo.

## Prod wala trap

```java
@ManyToOne(cascade = CascadeType.ALL)   // child se parent ki taraf
Author author;
// bookRepo.delete(book) -> author bhi delete hone chala
```

Ek kitaab delete ki to Hibernate uska author bhi delete karne chala. Ya to FK error aata hai (author ki baaki kitaabein abhi hain), ya author
ki saari kitaabein saath me chali jaati hain. Cascade hamesha parent se child ki taraf lagta hai (`@OneToMany` pe), child se parent ki taraf
(`@ManyToOne` pe) kabhi nahi.
