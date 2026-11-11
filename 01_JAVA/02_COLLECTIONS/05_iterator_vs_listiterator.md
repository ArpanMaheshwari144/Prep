# Iterator vs ListIterator

## Scene: expired surveys hatane hain

Tujhe list me se expired surveys hatane hain. Tune for-each loop chalaya aur uske andar `list.remove()` likh diya. Turant
`ConcurrentModificationException` (CME) aa gaya.

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
