# Stream methods cheatsheet

Iski practice `01_JAVA/07_STREAM_API_WRITE` folder me hai (Q1 se Q17, aur REDO).

## Scene

Mart me 2 rack hain. Doodh wale rack pe ghee 550 aur paneer 300 hain, anaaj wale pe daal 140 aur aata 320. Mausi boli: "har rack ka hisaab do."

Loop se karo to har dabbe pe `map[rack] += price` likhna padta hai. Stream me `groupingBy` ka pehla argument batata hai ki dabbe kis cheez pe banenge. Doosra argument batata hai ki har dabbe ke andar kya rakhna hai. Slot ek hi hai, bas usme collector badalte raho:

```java
groupingBy(Item::rack, counting())                    // kitne
groupingBy(Item::rack, mapping(Item::name, toList())) // kaun kaun
groupingBy(Item::rack, summingInt(Item::price))       // kitne ka
```

Chala ke dekha:

```
{anaaj=2, doodh=2}
{anaaj=[daal, aata], doodh=[ghee, paneer]}
{anaaj=460, doodh=850}
sum: 1310          <- mapToInt(Item::price).sum()
```

## Dikkat -> hal

Saare methods 3 dabbon me aate hain. Ye list yaad rakhne ki cheez hai. Likhte waqt koi naam bhool jao to docs ya net pe dekh lo, ye normal hai:

```
BADLO:     filter · map · flatMap · distinct · sorted · limit / skip
AGGREGATE: count · sum / max · reduce · anyMatch · findFirst
COLLECT:   toList · toMap · groupingBy · partitioningBy · joining
```

Do cheezein dhyaan me rakhni hain. Pehli, naam sirf dabbe ke andar nikalo (`mapping`). Collect se pehle `.map(name)` lagaya to stream me sirf String bachegi aur rack kho jaayega, phir group kis cheez pe karoge. Doosri, `.sum()` sirf `IntStream` pe milta hai, isliye pehle `mapToInt` lagao.

## Prod wala trap

```java
l.stream().collect(toMap(Item::rack, Item::name));
```

Chala ke dekha:

```
Duplicate key anaaj (attempted merging values daal and aata)
```

Test data me har rack pe ek hi dabba tha, isliye sab chal gaya. Prod me usi rack pe doosra dabba aate hi crash ho gaya. Isliye `toMap` me teesra argument (merge) do:

```
toMap(Item::rack, Item::name, (x, y) -> x + ", " + y)   ->   {anaaj=daal, aata, doodh=ghee, paneer}
```

Ek key ki kai values chahiye to `toMap` ki jagah `groupingBy` lagao.

## Loop ya stream

Kaam seedha ho (filter, map, group, ginti) to stream chhoti aur saaf padti hai. Logic bhaari ho (beech me `break`, kai if-else, checked exception, ya breakpoint lagana ho) to loop behtar hai. Jo padhne me saaf lage wahi likho.

## Tera hands-on

`07_STREAM_API_WRITE` me Q6 (`counting`), Q13 (`mapping`) aur Q14 (`summingInt`) tune khud likhe hain. Upar ke teen collector wahi teen sawaal hain.
