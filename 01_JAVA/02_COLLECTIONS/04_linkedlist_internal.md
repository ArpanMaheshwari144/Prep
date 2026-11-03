# LinkedList: andar se

## Kahani: support ticket queue

Maan lo ek urgent ticket ko queue ke beech me ghusana hai. ArrayList me uske baad ke 10,000 ticket khiskane padte. Par
LinkedList me sirf do pointer badalne padte hain, baaki sab jahan hai wahin rehta hai.

```
null <- [T101] <-> [T102] <-> [T103] -> null
         head                  tail
har node me teen cheez: prev | data | next
node memory me bikhre hote hain, bas pointer se jude hote hain
```

## Tune DSA me khud banaya

Tera LRU cache (blank redo 5/5) HashMap aur doubly linked list se bana tha. Wahan node map se pehle hi haath me aa jaata
tha, isliye use hatana ya aage laana O(1) me ho jaata tha. LinkedList ka asli fayda bas yahi hai: jab node pehle se haath
me ho.

## Trade-off

"Beech me daalna O(1)" tabhi sach hai jab node pehle se haath me ho. Agar us jagah tak pahunchna hai, to head se ek-ek
node chalna padta hai, jo O(n) hai.

Doosri baat, node memory me bikhre hote hain, isliye CPU cache kaam nahi aata aur padhna dheema hota hai. Isi wajah se
aam taur pe ArrayList jeet jaata hai. Queue ya stack chahiye to `ArrayDeque` lo. Wo LinkedList se tez hai, kyunki usme
har element ke liye alag Node nahi banta.

## Prod trap

```java
List<Ticket> tickets = new LinkedList<>(...);   // 50,000
for (int i = 0; i < tickets.size(); i++) {
    process(tickets.get(i));   // har get head se chalta -> O(n²), ghanton ka kaam
}
```

Ye loop dekhne me bilkul seedha lagta hai. Par LinkedList me har `get(i)` head se shuru karke i tak chalta hai, isliye
50,000 ticket pe ye ghanton ka kaam ban jaata hai. Ilaaj: for-each ya iterator chalao, jo ek-ek node aage badhta hai.
Ya phir ArrayList lo.

## Interview line

```
LinkedList only wins when I already hold the node, like in an LRU cache built from a HashMap and a
doubly linked list; otherwise reaching a position is O(n) and the nodes are scattered in memory, so
I default to ArrayList and use ArrayDeque for queues.
```
