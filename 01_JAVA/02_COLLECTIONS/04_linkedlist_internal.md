# LinkedList: andar se

## Kahani: support ticket queue

Urgent ticket beech me ghusana hai. ArrayList me 10,000 ticket khiskane padte; LinkedList me sirf do pointer badalte.

```
null <- [T101] <-> [T102] <-> [T103] -> null
         head                  tail
har node = prev | data | next   (memory me bikhre, pointer se jude)
```

## Tune DSA me khud banaya

Tera LRU cache (blank redo 5/5) = HashMap + doubly linked list. Node map se pehle hi haath me, to hatana / aage laana
O(1). LinkedList ka asli fayda bas yahi: node haath me ho.

## Trade-off

"Beech me daalna O(1)" tabhi jab node haath me ho; wahan tak head se chalna O(n). Node bikhre = CPU cache kaam nahi
aata. Isliye aam taur pe ArrayList jeetta. Queue / stack = `ArrayDeque` (LinkedList se tez, har element ka Node nahi).

## Prod trap

```java
List<Ticket> tickets = new LinkedList<>(...);   // 50,000
for (int i = 0; i < tickets.size(); i++) {
    process(tickets.get(i));   // har get head se chalta -> O(n²), ghanton ka kaam
}
```

Dekhne me seedha. Ilaaj: for-each / iterator (ek-ek node aage), ya ArrayList.

## Interview line

```
LinkedList only wins when I already hold the node, like in an LRU cache built from a HashMap and a
doubly linked list; otherwise reaching a position is O(n) and the nodes are scattered in memory, so
I default to ArrayList and use ArrayDeque for queues.
```
