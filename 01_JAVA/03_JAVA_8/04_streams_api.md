# Streams API

**Koi poochhe "for loop hai to stream kyun" to aise bolo:**
```
Stream me main sirf bolta hoon KYA chahiye, KAISE nahi likhna padta (khaali map, containsKey, put), to bug kam ghuste.
Parallel karna ek shabd ka kaam (parallelStream). Par jahan logic ulta-pulta ho (beech me break, exception, kai jagah
state badalna), wahan for loop, kyunki lambi stream debug karna mushkil. Seedha kaam = stream, bada / ulta-pulta = loop.
```

## Streams kyun aayi (Java 8, 2014)

Mart me har dibbe (rasoi, nahana) ka kul daam. Chala ke dekha:

```java
// loop: KAISE bhi likhna padta
Map<String,Integer> m1 = new TreeMap<>();
for (Item it : l) {
  if (!m1.containsKey(it.dibba())) m1.put(it.dibba(), 0);
  m1.put(it.dibba(), m1.get(it.dibba()) + it.daam());
}

// stream: sirf KYA chahiye
l.stream().collect(groupingBy(Item::dibba, TreeMap::new, summingInt(Item::daam)));
```
```
loop:   {nahana=220, rasoi=870}
stream: {nahana=220, rasoi=870}
```

Loop me `put` chhoot jaaye to bug (Autodesk me `mp.put(target)` chhoota tha). Stream me ye "kaise" Java sambhalta. Usi waqt
machines me kai core aaye, `parallelStream()` se kaam baant ke chalta. Aur belt lazy: `findFirst` pehla milte hi ruk jaata (neeche).
Par 50-100 line ki stream me atke to debug mushkil, isliye seedha kaam stream, ulta-pulta kaam for loop.

## Parat 1: belt chalu kaise (shuruaat)

```java
List<String> saman = List.of("ghee", "aata", "daal", "chini");
int[] daam = {550, 320, 140, 60};

saman.stream().count()            // list se belt
Arrays.stream(daam).sum()         // array se belt
Stream.of("ghee", "aata").count() // seedha cheezein, belt
```
```
4
1070
2
```

**Trap:** `int[]` se number wali belt (`IntStream`) banti, usme `sum()` hai. `List<Integer>` se dabbon wali belt banti, usme nahi:

```
daam.stream().sum()                    ->  error: cannot find symbol  method sum()
daam.stream().mapToInt(x -> x).sum()   ->  1070
```

## Parat 2: filter (sirf kuch samaan rokna)

```java
l.stream().filter(it -> it.daam() > 200).toList()
```
```
[Item[naam=ghee, daam=550], Item[naam=aata, daam=320]]
```

Shart `true` to samaan aage, baaki belt se utar jaata.

**Trap:** `filter` asli list nahi chhoota, nayi list deta. Asli se hatana ho to `removeIf`:

```
sasta = filter(daam < 200) -> [daal 140, chini 60]    asli list: 4  (chaaron abhi bhi)
l.removeIf(daam < 200)     -> asli list: [ghee 550, aata 320]
```

PR me ye galti dikhti: `filter` lagaya aur samjha DB wali list saaf ho gayi.


## Parat 3: map (har samaan ko kuch aur bana dena)

```java
l.stream().map(Item::naam).toList()
l.stream().map(it -> it.daam() * 2).toList()
```
```
[ghee, aata, daal]
[1100, 640, 280]
```

`filter` samaan rokta, `map` samaan BADALTA: ek dabba andar, uski jagah kuch aur bahar (naam, dugna daam). Ginti utni hi.

**Trap:** Java 16 wala `toList()` jo list deta, usme jod nahi sakte:

```
naam.add("chini")                      ->  UnsupportedOperationException
collect(Collectors.toList()) pe add    ->  [ghee, aata, daal, chini]
```

Prod me aise phatta: kisi ne `collect(Collectors.toList())` ko "chhota likhte" bol ke `toList()` kiya, neeche usi list me `add`
ho raha tha. Compile ho jaata, chalne pe phat-ta.

## Parat 4: sorted (daam se lagana)

Chai aur aata dono 320:

```java
sorted(Comparator.comparing(Item::daam))                           // sasta pehle
sorted(Comparator.comparing(Item::daam).reversed())                // mehnga pehle
sorted(Comparator.comparing(Item::daam).thenComparing(Item::naam)) // daam barabar to naam se
```
```
[daal, chai, aata, ghee]
[ghee, chai, aata, daal]
[daal, aata, chai, ghee]
```

Daam barabar ho to list me jo pehle tha wahi pehle (chai). `thenComparing` se barabar waalon me naam se kram (aata, chai).

**Trap:** bina bataye `sorted()`:

```
l.stream().sorted()   ->  ClassCastException
```

Compile ho jaata, chalne pe phat-ta. Number / String apne aap sort hote, apni banayi class (`Item`) nahi; `Comparator` do.
Wahi jo DSA me `sort(v.begin(), v.end(), cmp)` me likhta, bas Java ki bhasha me.

## Parat 5: ant wale (count, sum, max), belt ka hisaab

```java
l.stream().count()                                // kitne dabbe
l.stream().mapToInt(Item::daam).sum()             // kul daam (parat 1 wala mapToInt)
l.stream().max(Comparator.comparing(Item::daam))  // sabse mehnga
```
```
3
1010
Optional[Item[naam=ghee, daam=550]]
```

`max` seedha dabba nahi, dibbe ke andar dabba (`Optional`) deta, kyunki list khaali bhi ho sakti:

```
khaali list pe max                       ->  Optional.empty
.map(Item::naam).orElse("kuch nahi")     ->  kuch nahi
.get() khaali pe                         ->  NoSuchElementException
```

**Trap:** test me list hamesha bhari, `.get()` chal jaata. Prod me kisi din khaali aayi (aaj koi samaan nahi) aur `.get()` phat
gaya. `orElse` se batao khaali ho to kya.

## Scene

Mart me 4 dabbe hain: ghee 550, daal 140, aata 320, chai 400. Mausi boli: "300 se mehnge dabbe chuno aur unke naam do."

Loop se karo to naya list banao, `for` lagao, uske andar `if`, phir `add`. Saare kaam ek hi loop me ghuse rehte hain. Stream me har kaam ek alag station hai, aur dabbe ek belt pe chalte hain:

```java
Stream<String> s = l.stream()
        .filter(i -> { print("filter " + i.name()); return i.price() > 300; })  // station: chuno
        .map(Item::name);                                                         // station: naam
// abhi tak terminal nahi lagaya
s.collect(Collectors.toList());                                                   // maal uthao
```

Chala ke dekha:

```
== terminal nahi ==
                         <- kuch nahi chhapa, belt chali hi nahi
== ab collect ==
filter ghee
filter daal
filter aata
filter chai
[ghee, aata, chai]
```

## Dikkat -> hal

`filter` aur `map` sirf naksha banate hain, ye intermediate hain. Belt tabhi chalti hai jab end pe koi maal uthane wala ho, jaise `collect`, `count` ya `forEach` (ye terminal hain). Aur har dabba ek-ek karke poori line se guzarta hai, isliye jawab milte hi belt ruk jaati hai:

```
== findFirst ==
dekha ghee
mila ghee            <- daal, aata, chai ko chhua bhi nahi
```

`sorted()` is niyam ka apvaad hai, kyunki sort karne ke liye use saare dabbe pehle jama karne padte hain. Aur ek stream ek hi baar chalti hai. Wahi `s` dobara chalaya to ye aaya:

```
stream has already been operated upon or closed
```

## Prod wala trap

```java
orders.parallelStream()
      .map(o -> paymentApi.check(o))   // har order pe API call, rukne wala kaam
      .collect(...);
```

Saari `parallelStream` poori JVM me ek hi chhota thread pool share karti hain. Uska naam common ForkJoinPool hai aur size cores - 1 hota hai. Usme API ya DB jaisa rukne wala kaam daala to pool ke saare thread intezaar me baith jaate hain, aur app ki baaki har `parallelStream` bhi atak jaati hai. Ye wahi thread pool wali kahani hai jo multithreading me dekhi thi. `parallelStream` sirf tab lagao jab data bada ho, kaam CPU wala ho aur items ek-doosre se alag hon.

## Tera project

SimpleBankSystem ke `getAccountHolderNames()` me yahi belt lagi hai. `findAll()` se dabbe aaye, `map(Account::getHolderName)` beech ka station hai, aur `collect` end pe maal uthata hai.
