# Iterator vs ListIterator

## Scene: expired surveys hatane hain

Tujhe list me se expired surveys hatane hain. Tune for-each loop chalaya aur uske andar `list.remove()` likh diya. Turant
`ConcurrentModificationException` (CME) aa gaya.

## Java ne aisa kyun banaya: attendance register

Pehle saaf: list se seedha hatane me koi dikkat nahi. `list.remove(x)` loop ke bahar likho to kuch nahi hota. Dikkat
sirf tab hai jab tu list pe CHAL bhi raha hai aur usi waqt HATA bhi raha hai.

Teacher register pe ungli rakh ke naam bol rahi hai, ungli row 3 pe hai. Beech me kisi ne row 2 mita di.
- Saare naam ek row upar khisak gaye.
- Ungli row 3 pe hi hai, par wahan ab doosra baccha aa gaya.
- Jo baccha row 3 pe tha, wo ab row 2 pe hai, ungli ke peeche. Wo bina bulaye chhoot gaya.

Iterator bhi ek ungli hai jo yaad rakhti hai "main index 3 pe hoon". Beech me hataya, sab khiske, ungli ek element kood
gayi. Neeche wale S102 output me yahi hua: S103 chup-chaap chhoot gaya, error nahi, bas galat nateeja. Prod me 100 me se
1 survey kabhi expire hi nahi hoga aur kisi ko pata nahi chalega.

Java ne socha: chup-chaap galat nateeja dene se behtar hai zor se chillao.
- `modCount` = list ka register: "mujhme kitni baar badlaav hua", har add / remove pe +1.
- Iterator ki copy = "jab maine chalna shuru kiya tab ginti kitni thi".
- Har `next()` pe dono milao. Alag nikli = kisi ne meri ungli ke neeche list badal di, ab ungli ka bharosa nahi -> CME.

Copy faltu nahi hai, yahi to wo tareeka hai jisse pata chalta hai ki list badli ya nahi. HLD jaisa hi: copy pe likh ke
rakho, aur jo copy se mel na khaaye uspe alert kar do ki kuch chhoota.

"Concurrent" ka matlab "do thread" nahi, matlab "tere chalne ke beech me badlaav hua". Ek hi thread me bhi aata hai (tera
loop), aur doosra thread list badle tab bhi yahi alarm bajta hai.

`it.remove()` safe kyun: yahan ungli khud hatati hai, to use pata hai ki element khiske. Wo apni jagah theek kar leti hai
aur ginti bhi badha leti hai, kuch nahi chhootta.

S102 pe alarm kyun nahi baja: check `next()` pe hota hai. S102 hatne ke baad `hasNext()` ne dekha ungli list ke ant pe
hai, to `next()` bulaya hi nahi gaya, ginti mili hi nahi. Isliye Java bhi kehta hai ki ye alarm "best effort" hai, iske
bharose mat raho: `removeIf` / `it.remove()` hi likho.

## Exception kyun aata hai

List apne har badlaav ki ginti rakhti hai, jise `modCount` kehte hain. Iterator bante waqt is ginti ki ek copy apne paas
rakh leta hai. Tune seedha list se hataya to list ki ginti badh gayi, par iterator ki copy purani reh gayi. Agle `next()`
pe dono milaata hai, mel nahi khaati, exception.

```
list.remove()  -> list ka modCount 5, iterator ki copy abhi 4 -> agle next() pe CME
it.remove()    -> iterator khud hatata hai, dono 5             -> SAFE
```

## Asli prod trap: chala ke dekha (har baar ek survey hataya)

```
S100 hataya -> CME, dekhe: [S100]
S101 hataya -> CME, dekhe: [S100, S101]
S102 hataya -> koi exception nahi, dekhe: [S100, S101, S102]
S103 hataya -> CME, dekhe: [S100, S101, S102, S103]
```

Aakhri se pehla (S102) hataya to loop chup-chaap ruk gaya. Hatane ke baad list chhoti hui, cursor size ke barabar ho
gaya, aur loop ko laga kaam khatam. S103 dekha hi nahi gaya.

Yaad rakh: **"exception nahi aaya" ka matlab "code sahi hai" nahi hota.**

## Hal

```
it.remove()  -> [S100, S101, S103]
removeIf     -> [S100, S101, S103]
```

- `it.remove()`: dono ginti saath me update karta hai.
- Aaj ke code me seedha `surveys.removeIf(s -> s.isExpired())`, jo andar se iterator hi chalata hai.

## ListIterator: Iterator ka bada bhai

Aage bhi chalta hai aur peeche bhi, aur chalte-chalte `set` aur `add` bhi kar sakta hai. Sirf List pe milta hai.

```
next() se pehle set -> IllegalStateException
ListIterator set + add -> [S100, S101-OK, S101b, S103]
peeche chala -> S103 S101b S101-OK S100
```

- Cursor kisi element pe nahi, do element ke beech me hota hai.
- `remove` aur `set` us element pe chalte hain jo aakhri baar lautaya gaya tha. Isliye `next()` ke bina `set` /
  `remove` kiya to IllegalStateException.

## Trade-off

for-each saaf dikhta hai, par usme element hata nahi sakte. Iterator thoda lamba likhna padta hai, par hatana safe hai.
Aaj ke code me seedha `removeIf`.
