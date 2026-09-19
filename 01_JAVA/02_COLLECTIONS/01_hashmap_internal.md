# HashMap — Internal Working

> **V90 Section 1 — Topic 6**

---

## WHY — HashMap O(1) KYUN?

→ Array mein **index se access = O(1)**
→ HashMap = **key ko number mein convert (hashCode)** → number se index nikalo `((n-1) & hash)` → **seedha us index pe jao**
→ 10 lakh entries mein bhi **TURANT**
→ Capacity **power of 2** KYUN? Bitwise AND = **1 CPU instruction** = modulo se FAST
→ **Load factor 0.75** = Poisson distribution pe sweet spot

---

## put(key, value) FLOW

```
put("name", "Arpan")
       │
       ▼
key.hashCode()  →  raw hash number
       │
       ▼
hash ^ (hash >>> 16)  →  spread bits
       │
       ▼
(n-1) & hash  →  BUCKET INDEX
       │
       ▼
Bucket empty?
   ╱       ╲
 YES         NO (Collision!)
  │            │
  ▼            ▼
Store      equals() check chain mein
here          ╱       ╲
           MATCH    NO MATCH
             │         │
             ▼         ▼
          REPLACE    ADD to chain
          value     (LinkedList / Tree)

Java 8: chain 8+ nodes + table 64+  →  Red-Black Tree
Average: O(1)  |  Worst: O(log n) tree, O(n) chain
```

---

## STORY — Library Manage

→ Soch tu ek library manage kar raha — **10,000 books**
→ Koi aaya: "Harry Potter chahiye." Ek-ek check karega? **O(n)? Nahi.**
→ Library ne system banaya — book ka naam se **shelf number calculate**
→ "Harry Potter" → **Shelf 42**. Seedha wahan jao
→ **Exactly yehi HashMap karta** — hashCode → index → seedha bucket → **O(1)**
→ `put("name","Arpan")` → hashCode → index → store
→ `get("name")` → same index → **O(1)**
→ **Collision:** do keys ka same index → LinkedList us bucket mein
→ Java 8: 8+ entries → LinkedList → **Red-Black Tree** → O(n) se O(log n)

---

## ★ RESIZE + LOAD-FACTOR MATH (8-Sep, deep-grill)

> HashMap andar array (buckets), default size **16**, load factor **0.75**.

**Kab double hoti (parking-lot):**
```
THRESHOLD = capacity * loadFactor = 16 * 0.75 = 12
entries 12 paar -> array DOUBLE (16->32), threshold->24. Phir 24 paar->64... har baar DOUBLE.
```
16-spot lot: 12 (75%) bharte hi bhari lagne lagta (collision badhta) -> bharne se PEHLE double kar dete.

**0.75 KYUN (trade-off):**
```
Chhota (0.5) -> jaldi resize + bahut khaali spot -> MEMORY waste
Bada  (1.0)  -> kam memory PAR chain lambi -> SLOW
0.75         -> sweet spot (Poisson: 0.75 pe bucket me 8+ collision ~0.00000006) = speed vs memory
```

### ★ Java-8 SPLIT-trick — resize pe koi dobara-hashCode NAHI
Capacity power-of-2 hai; double karne pe index-mask me sirf **ek naya bit** judta = value **oldCap (16)** khud. Har entry ko us ek bit se do me baant do:
```
hash & oldCap(16):
    == 0  -> LO list -> WAHI index (i)
    != 0  -> HI list -> index (i + oldCap) = i+16
```

**Numeric example — 2 keys (hash 5 aur 21) jo old table me SAME bucket the:**
```
hash 5   = 0 0101      hash 21 = 1 0101
OLD index (& 1111): 5&15=5 , 21&15=5   -> DONO bucket 5 (collision, chain me saath)

resize 16->32, split-bit = 16 (1 0000):
  A(5) : 0 0101 & 1 0000 = 0      -> LO -> index 5   (wahi)
  B(21): 1 0101 & 1 0000 = 16     -> HI -> index 5+16=21

verify seedha naya index (& 11111): 5&31=5 ✓ , 21&31=21 ✓

OLD(16): bucket[5]-> A(5)-> B(21)        NEW(32): bucket[5]->A , bucket[21]->B
```
Koi dobara-hashCode nahi — bas ek bit dekha. Chain apne-aap 2 bucket me bat gayi (collision kam).

**"16 kyun" (do alag kaam, confuse mat ho):**
```
Old INDEX me:  hash & (16-1)=01111   <- neeche ke 4 bit (mask)
Split me:      hash & 16   =10000    <- sirf naya 5th bit
```
16->32 pe mask 01111->11111 hua = ek naya bit juda, uski value = 16 = oldCap KHUD.
`naya-index = purana-index (bit 0) YA purana-index + oldCap (bit 1)`.
Isiliye **hash & oldCap** = wahi akela deciding bit — random 16 nahi.

**Aur "i+16 kyun, 32 nahi" (16+16=32 halves):**
```
NEW array (32) = do 16-16 aadhe:
   index:  0 ...... 15  |  16 ...... 31
           LOWER half        UPPER half

Purane buckets 0..15 the. Har purana bucket i split:
   LO -> i        -> LOWER half (0..15) me rehta
   HI -> i + 16   -> UPPER half (16..31) me jaata   (i=0..15 -> i+16=16..31)
```
i+16 hamesha upar wale aadhe me girta -> isiliye +oldCap. Aur 16+16=32 = pura array bhara (neeche+upar).

### treeify / untreeify (2 number)
```
treeify:   bucket 8+ nodes -> TREE, PAR sirf jab table size >= 64
           (table <64 + bucket bhara -> tree nahi, RESIZE karo -> usse hi collision kam)
untreeify: tree nodes 6 pe wapas LinkedList. (8 banao/6 todo -> gap taaki border pe flip-flop na ho)
```

---

## TRAP

> **HashMap null key ALLOWED. ConcurrentHashMap mein null key/value → NullPointerException!**

**Null banned in CHM kyu?**
→ `get(key)` → null aaya. Key nahi hai ya value null?
→ Single thread mein `containsKey()` check karo
→ Multi thread mein `get()` aur `containsKey()` ke **BEECH doosre thread ne data badal diya** — answer galat!
→ **Race condition.** Null banned = ambiguity hi mat aane do.

---

## POWER PHRASE

> *"HashMap uses hashCode() to find the bucket index and equals() to resolve collisions in the LinkedList. Java 8 converts the bucket to a Red-Black Tree when it has more than 8 entries — O(n) becomes O(log n)."*

**Yaad rakh:** hashCode → index → store. Collision = LinkedList. Java 8: 8+ entries → Tree.

---

## RED-BLACK TREE — bucket 8+ pe kya banta (visual, 2-Jul sekha)

```
   ek bucket me 8+ nodes -> LinkedList ko RED-BLACK TREE bana deta (treeify) -> us bucket search O(n) -> O(log n).
   sort kis pe: HASH value pe.

   Red-Black tree = self-balancing BINARY SEARCH TREE -> SORTED (left chhota, right bada).
   Rules (4):
     1. root HAMESHA black
     2. RED node ke bachche BLACK (do red lagataar nahi)
     3. har path me BLACK nodes ki count SAME
     -> in rules ka maqsad: tree BALANCED rahe -> height ~log n -> O(log n) guarantee.

   Insert 10,20,30 (seedha chain -> unbalanced -> rotation+recolor -> balance):
                    20 (B)          <- black root
                   /      \
               10 (R)     30 (R)    <- red children, dono taraf barabar black-height
```

```
   B-TREE vs RED-BLACK (dono sorted+balanced -> O(log n), bas shape/jagah alag):
   B-TREE     -> MULTI-WAY (kai keys/node), mota-chauda -> DISK (DB index).
   RED-BLACK  -> BINARY (2 child, 1 key), patla + colored -> RAM (HashMap bucket).
```

---

## ═══ HANDS-ON — HashMap KHUD BANA KE DEKHI (19-Sep) ═══

> **Kya kiya:** HashMap ko padha nahi — uska andar ka kaam CHHE chhoti file me
> **khud likh ke** chalaya. Har file ek hi sawaal uthati hai, jiska jawab agli file deti hai.
>
> **Kaise:** Claude ne sirf **STUB** diya (comment me kadam, code NAHI). Code Arpan ne likha,
> chalaya, aur output se samjha. Jahan atka, wahan sirf us line ka bug point kiya gaya.
>
> **File kahan:** `01_JAVA/02_COLLECTIONS/demo/`  — `java H1_IndexKaise.java` se chalti hain.
>
> **Neeche jo bhi number hai wo ASLI OUTPUT hai** — machine pe chala hua, likha hua nahi.

```
H1  index kaise banta        ->  sawaal khada hua: sirf 4 bit? to takkar hogi?
H2  takkar sach me hoti      ->  sawaal: ilaaj kya
H3  ilaaj = spread (XOR)     ->  sawaal: takkar phir bhi hui, ab kya
H4  bucket + chain banayi    ->  sawaal: chain me SAHI key kaise pehchane
H5  ==  vs  equals           ->  sawaal: apni class me ye likhe hi na to
H6  hashCode/equals contract ->  poora
```

---

### H1 — `index = (n-1) & hash`   · *shabd se array ki jagah*

**Sawaal:** HashMap andar se ARRAY hai. Array maangta hai NUMBER (`table[5]`). Key hai SHABD (`"Arpan"`).
Shabd ko number kaise banayein, aur wo number 0-15 me kaise rahe?

```java
int n = 16;
int hash  = "Arpan".hashCode();
int index = (n - 1) & hash;
```
```
hashCode : 63535788        <- shabd se number (pakka: wahi string = wahi number)
n - 1    : 15
index    : 12
```

**12 kahan se:**
```
15         =  ... 0000 0000 0000 1111      sirf 4 bit ON
hashCode   =  ... 0011 1100 1010 1100
   &          ------------------------
index      =  ... 0000 0000 0000 1100  =  12
                                 ^^^^  sirf 4 bit bache, upar ke 28 MAR gaye
```
4 bit ka sabse bada = `1111` = 15 → jawab **kabhi** array ke bahar nahi ja sakta.

**`&` kyun, `%` kyun nahi:**
```
%  ->  division hai = CPU ka sabse dheema kaam. yahan lakhon baar chalta hai.
   ->  aur -7 % 16 = -7 (NEGATIVE) -> table[-7] = crash
&  ->  ek cycle. aur sirf bits chhaanta -> negative ho hi nahi sakta
```
★ **Shart:** ye trick tabhi chalti jab `n` = 2 ki power (tabhi `n-1` = `1111...`).
**Isi liye HashMap ki capacity HAMESHA 2 ki power hoti.** `new HashMap<>(20)` likho to andar **32** banta.

---

### H2 — takkar sach me hoti hai   · *28 bit phenkne ka nateeja*

**Sawaal:** agar index sirf aakhri 4 bit dekhta hai, to do key jinke UPAR ke bits alag hain
par NEECHE ke same — kya wo hamesha takrayengi?

Wahi H1 wala code, paanch key pe:
```
key         hashCode      index
Arpan       63535788        12
bank         3016252        12      <- WAHI khaana
JP              2374         6
audit       93166555        11
ledger   -1106662039         9
```

**Kyun takrayi:**
```
Arpan  :  00000011 11001001 01111010 1010 1100
bank   :  00000000 00101110 00000110 0011 1100
          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  ^^^^
          yahan POORA farak                yahan DONO me 1100
```
6 crore vs 30 lakh — aur index ne unhe dekha hi nahi.

★★ **Do kism ki takkar (ye farak asli hai):**
```
ITTEFAQ ki  ->  poora hashCode hi same ho. 16 khaane, 5 key -> hoga hi. buraai nahi.
DESIGN ki   ->  hashCode ALAG hain, phir bhi takra rahe (kyunki 28 bit phenke).
                ye HAR BAAR hoga. sambhala nahi ja sakta.
```
Aur ek bucket me dher = `[12] -> Arpan -> bank -> +500 aur` → bucket to turant milta,
par phir **list me chalna** padta → **O(1) khatam**.

---

### H3 — `spread = h ^ (h >>> 16)`   · *upar ke bits ko NEECHE laana*

**Sawaal:** 28 bit bekaar ja rahe. Index me zyada bit use nahi kar sakte (array chhota hai).
To upar ke bits ko **neeche laao**.

```java
int spread = h ^ (h >>> 16);
int index  = (n - 1) & spread;      // <- spread se, h se NAHI
```

**Bits ka safar (har bit ko naam de ke dekha):**
```
position :  31                    16   15                     0
   h     =  A B C D E F G H I J K L M N O P | a b c d e f g h i j k l m n o p
            └────── upar ka aadha ─────┘   └──── neeche ka aadha ────┘

   h >>> 16 = 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 | A B C D E F G H I J K L M N O P
              └─ khaali, 0 bhara ─────────┘   └─ BADE akshar ab NEECHE ────┘

   XOR  ->  spread = A B C ... O P | a^A  b^B  ...  o^O  p^P

   index (& 15) dekhta hai:   m^M  n^N  o^O  p^P
                                ^    ^    ^    ^
                          M,N,O,P = bit 19,18,17,16  -> UPAR se AA GAYE
```

**Asli output — dono taraf:**
```
key        RAW index    SPREAD index
Arpan         12            5       <- ALAG ho gaye
bank          12            2       <-
JP             6            6
audit         11            6       <- ab YE do takra gaye
ledger         9            0
```

★ **XOR takkar KHATAM nahi karta** — wo takkar ko *design ki* se *ittefaq ki* bana deta hai:
```
XOR se pehle  ->  "upar alag, neeche same" wali key HAMESHA takraati  (pakka nuksan)
XOR ke baad   ->  poore 32 bit faisle me. koi pattern nahi bacha.
```
Ittefaq ki takkar sambhal jaati (chain, aur bahut ho to tree). Design ki nahi sambhalti.

**`^` kyun, `&`/`|` kyun nahi:**
```
&  ->  bits MAARTA hai, sab 0 ki taraf jhukta
|  ->  bits sirf JODTA hai, sab 1 ki taraf jhukta
^  ->  na 0 ki taraf na 1 ki taraf. jaankari na banti na marti, bas MIL jaati.
```

**`16` hi kyun (Arpan ka sawaal — 10 ya 15 kyun nahi):**
```
shift 16  ->  bit 31 -> bit 15 ,  bit 16 -> bit 0
              POORA upar ka aadha, POORE neeche wale aadhe pe. ek bhi bit nahi chhoota.
shift 10  ->  sirf bit 10-13 index tak pahunche. bit 14 se upar ka KUCH NAHI.
shift 4   ->  sirf padosi bits. aadhe number ko haath bhi nahi laga.
shift 24+ ->  bits 32 ke bahar nikal jaate = jaankari MAR jaati.
```
`int` = 32 bit, `16` uska theek aadha → **ek shift + ek XOR me itna kaam koi aur number nahi karta.**
(JDK source ka comment khud ise "speed / utility / bit-spreading quality" ka **trade-off** kehta hai.)

★ **Java 7 vs 8:** Java 7 me ye **4 shift + 4 XOR** tha — behtar mixing, mehnga.
Java 8 me ghata ke **ek** kar diya, kyunki treeify aa gaya (bucket bhar jaye to tree) —
ab thodi kharab mixing jaan-leva nahi rahi.

★★ **`>>>` vs `>>` (yahan Arpan phasa tha):**
```
>>   ->  SIGN wala shift. negative negative hi rehta (khaali jagah me 1 bharta)
>>>  ->  bina sign ka. khaali jagah me HAMESHA 0
```
`ledger` ka hashCode negative hai — `>>` pe jawab hi badal jaata.
JDK me pakka `>>>`, kyunki yahan number "aadha" nahi karna, **bits ki jagah** badalni hai.

★ **Aur:** shift me `n` mat likhna, `16` likhna. `n=16` pe sanyog se sahi chalega, par
`n=32` pe `>>> 32` ho jaayega — aur Java me int pe 32 ka shift = **koi shift nahi**. Chup-chaap galat.

★ `ledger` ka spread bhi negative tha, phir bhi index `0` aaya — `& 15` ke baad 4 bit bachte,
aur 4 bit se negative banta hi nahi. **`%` hota to negative aata aur crash hota.**

---

### H4 — table + chain KHUD banayi   · *takkar ki SHAKAL*

**Sawaal:** ab tak sirf number the. Takkar dikhti kaisi hai?

```java
List<String>[] table = new List[n];          // 16 khaane, sab null

for (String k : keys) {
    int index = ...;                          // H3 wala hisaab
    if (table[index] == null) {
        table[index] = new ArrayList<>();     // pehli key -> nayi list
    }
    table[index].add(k);                      // ★ add, set NAHI
}
```
★ `set` purani ko **mita** deta, `add` peeche **jodta**. Chain isi ek shabd se banti hai.

**Asli output:**
```
 0 : [ledger]
 1 : .
 2 : [bank]
 3 : .
 4 : .
 5 : [Arpan]
 6 : [JP, audit]      <- CHAIN
 7 : .   ...   15 : .
```

**Teen cheez:**
```
1. takkar = ek dabbe me DO parchi. ek dusri ko MITAATI nahi, saath rehti hai.

2. 11 khaane khaali = ye GALTI nahi, jaan-boojh ke hai
      bhari table  -> kam memory, ZYADA takkar, lambi chain -> dheema
      khaali table -> zyada memory, KAM takkar -> tez
   loadFactor 0.75 = "75% bharte hi dugni kar do" -> poori bharne hi nahi deta

3. ARRAY + LIST dono chahiye
      array akela -> takkar sambhal nahi sakta
      list akeli  -> dhoondhne me chalna padta
```

★ **Asli HashMap me `ArrayList` NAHI hoti** — ek chhota `Node` hota hai:
```java
static class Node<K,V> { final int hash; final K key; V value; Node<K,V> next; }
```
Kyun: **zyadatar bucket me sirf EK cheez hoti hai.** `ArrayList` = ek object + andar ek array (do cheez).
`Node` = ek object + ek pointer. Lakhon entries pe ye farak bada ho jaata.
Aur wahi `next` chain hai: `table[6] -> [JP|next] -> [audit|null]`

---

### H5 — `==` vs `.equals()`   · *chain me SAHI key pehchanna*

**Sawaal:** index ne bataya KAHAN dekhna hai. Par bucket 6 me **do** key hain.
Index kehta "yahan dekho", ye nahi kehta "mil gaya". To milayen kaise?

```java
String val1 = "audit";
String val2 = new String("audit");
```
```
audit       <- val1
audit       <- val2        DONO me same maal
false       <- val1 == val2
true        <- val1.equals(val2)
```
```
==        ->  "kya ye WAHI cheez hai?"   (memory me ek hi jagah?)
.equals() ->  "kya ye BARABAR hai?"      (andar ka maal same?)
```
`new String(...)` ne **naya dabba** banaya, maal wahi bhara. `==` ke liye wo alag cheez hai.
(misaal: tera aur padosi ka fridge — ek hi model. `equals` haan, `==` nahi.)

**Wahi cheez chain pe (`table[6]` me ghum ke):**
```
==      equals
false   false     JP
false   TRUE      audit      <- yahi
```
```
==       ->  EK BAAR BHI true nahi. audit pe bhi nahi.
equals   ->  sirf audit pe true.
```

★ **Agar HashMap `==` pe tikta:**
```java
map.put("audit", 1);
map.get(new String("audit"));    // -> null
```
**Apni hi daali hui key wapas na milti.** Har baar exactly wahi reference dena padta.

**Par JDK me TEENO hain, aur KRAM maayne rakhta:**
```java
if (e.hash == hash && ((k = e.key) == key || (key != null && key.equals(k))))
```
```
1. e.hash == hash  ->  do int ka compare. SABSE SASTA. alag hua to turant agli entry.
2. k == key        ->  wahi object? turant haan. (aksar yahi hota)
3. key.equals(k)   ->  tabhi, jab upar wale do se faisla na ho
```
★ Jo sabse **mehnga** hai (tera likha hua `equals` — lambi String, poora object) wo
**sabse aakhir** me hai, aur zyadatar baar bulaya hi nahi jaata.

---

### H6 — hashCode / equals ka CONTRACT   · *apni class ko key banane pe*

**Sawaal:** ab tak key String thi (uska hashCode/equals JDK ne theek likha hua hai).
Asli code me key `Employee`/`OrderId` hoti hai. Wo do method na likhe to kya tootega?

Ek hi class, teen haalat — **saara output asli:**

```
haalat              obj1.hashCode   obj2.hashCode   equals   map.get(obj2)
(a) kuch nahi likha   149928006       713338599      false      null
(b) sirf equals       149928006       713338599      TRUE       null      ★ khatarnak
(c) dono likhe             132             132       true       "A"
```
*(dono Employee ki `id` = 101, dono `new` se bane)*

**(a) kyun fail:** `Object` ka default `hashCode()` object ki **PEHCHAAN** se banta hai,
tere `id` se nahi. Do `new` = do pehchaan = do hash. Aur default `equals()` = wahi jo `==`.
```
put(obj1) -> hash 149928006 -> kisi bucket me
get(obj2) -> hash 713338599 -> DUSRE bucket me dekha -> khaali -> null
```

★★ **(b) — sabse zaroori hissa.** `equals` BILKUL SAHI likha, aur phir bhi `null`.
```
get() do kadam ka hai:
   kadam 1 : hash se BUCKET   <- yahan HASH chahiye, equals ka kaam hi nahi
   kadam 2 : chain me equals  <- yahan equals chahiye

tune kadam 2 theek kiya. par baat kadam 2 tak PAHUNCHTI HI NAHI.
hash alag -> galat bucket -> chain hai hi nahi -> equals KABHI BULAYA HI NAHI GAYA.
```
Tera `equals` sahi hai. **Bas wo kabhi chalta nahi.**

**CONTRACT (ek line):**
```
do object equals se barabar   ->   unka hashCode BHI barabar hona CHAHIYE
```
**Ulta zaroori NAHI:**
```
hash SAME, object alag  ->  CHALTA hai   (wahi JP/audit — equals faisla kar deta)
hash ALAG               ->  baat equals tak pahunchti hi nahi -> faisla HOTA HI NAHI
```

★ **(b) khatarnak kyun — code SAHI DIKHTA hai:**
```
code review    ->  "equals likha hai, theek hai"
unit test      ->  assertEquals(e1, e2)  ->  PASS
aur phir bhi map kaam nahi karta

prod me:   cache.put(new Employee(101), salary);
           cache.get(new Employee(101));   // null
           -> code sochta "cache me nahi hai", DB se dobara utha leta
           -> kaam chalta rehta, bas cache KABHI hit nahi hota
           -> koi error nahi, koi exception nahi. SIRF DHEEMA.

Set me aur bura:  set.add(new Employee(101));  x2  ->  size() = 2  (duplicate baith gaye)
```

★ **Contract theek hone ke baad bhi hash KHARAB ho sakta:**
```java
public int hashCode() { return 1; }     // contract TODTA NAHI (equal object ka hash equal hai)
```
Par saari key ek bucket me → ek lambi chain → HashMap ek LIST ban gaya → O(1) khatam.
```
contract TOOTA               ->  cheez MILTI hi nahi        (correctness bug)
contract theek, hash kharab  ->  milti hai par DHEEMA        (performance bug)
```
**Do alag dard hain. Interview me dono poochhe jaate hain.**

---

### ★ IS HANDS-ON KA NICHOD — ek nazar

```
put(k,v):  hash = h ^ (h>>>16)            <- upar ke bits neeche laao (H3)
           index = (n-1) & hash           <- array ki jagah (H1) . n = 2 ki power
           khaali? nayi Node. bhara? chain me AAGE jodo (H4)

get(k)  :  wahi hash, wahi index          <- BUCKET tak pahuncho
           chain me: hash == ? phir == ?  phir equals?   (H5)
           ★ hash galat -> equals tak baat pahunchti hi nahi  (H6)
```

**Ek line (interview me bolne layak):**
> *"Index sirf hash ke neeche ke bits dekhta hai, isliye HashMap pehle `h ^ (h>>>16)` karta hai
> taaki upar ke bits bhi faisle me aayein. Index bata deta hai KAHAN dekhna hai, par bucket me kai
> key ho sakti hain — isliye chain me hash, phir `==`, phir `equals` se pehchaan hoti hai.
> Aur isi wajah se hashCode aur equals dono chahiye: equals akela likha to baat equals tak
> pahunchti hi nahi, kyunki hash alag hone pe HashMap dusre bucket me dekhta hai."*
