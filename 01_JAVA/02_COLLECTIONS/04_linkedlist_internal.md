# LinkedList: andar se

## Scene: support ticket queue

Maan lo ek urgent ticket ko queue ke beech me ghusana hai. ArrayList me uske baad ke saare ticket khiskane padte. LinkedList
me sirf do pointer badalne padte hain, baaki sab jahan hai wahin rehta hai.

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

## Chala ke dekha: 50,000 element

```
LinkedList get(i) loop : 881 ms
LinkedList for-each    : 0 ms
ArrayList  get(i) loop : 0 ms
LinkedList beech me add (node haath me): 5 us
ArrayList  beech me add             : 12 us
```

- LinkedList pe `get(i)` wala loop 881 ms, wahi kaam for-each se 0 ms.
- Beech me add: LinkedList thoda tez nikla, par farak microseconds ka hai. ArrayList ka khiskana ek hi baar ki memory copy
  hai, jo bahut tez hoti hai.

## Trade-off

"Beech me daalna O(1)" tabhi sach hai jab node pehle se haath me ho. Us jagah tak pahunchna ho to node-node chalna padta
hai, jo O(n) hai.

Node memory me bikhre hote hain, isliye CPU cache kaam nahi aata aur padhna dheema hota hai. Isi wajah se aam taur pe
ArrayList jeet jaata hai. Queue ya stack chahiye to `ArrayDeque` lo. Wo LinkedList se tez hai, kyunki usme har element ke
liye alag Node nahi banta.

## Prod trap: index wala loop

```java
List<Ticket> tickets = new LinkedList<>(...);   // 50,000
for (int i = 0; i < tickets.size(); i++) {
    process(tickets.get(i));   // har get node-node chalta -> O(n²)
}
```

Ye loop dekhne me bilkul seedha lagta hai. Par LinkedList me har `get(i)` head ya tail (jo paas ho) se shuru karke i tak
chalta hai. 50,000 pe upar wala 881 ms aaya, aur list badhti jaaye to ye ginti n² ki raftaar se badhti hai.

Ilaaj: for-each ya iterator chalao, jo ek-ek node aage badhta hai. Ya phir ArrayList lo.
