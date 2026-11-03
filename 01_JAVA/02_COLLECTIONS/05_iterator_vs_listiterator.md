# Iterator vs ListIterator

## Kahani: expired surveys hatane hain

Tujhe list me se expired surveys hatane hain. Tune for-each loop chalaya aur uske andar `list.remove()` likh diya. Turant
`ConcurrentModificationException` aa gaya.

## Exception kyun aata hai

List apne har badlaav ki ginti rakhti hai, jise `modCount` kehte hain. Iterator jab banta hai, to wo is ginti ki ek copy
apne paas rakh leta hai. Ab agar tune seedha list se kuch hataya, to list ki ginti badh gayi, par iterator ki copy purani
reh gayi. Agle `next()` pe iterator dono ginti milata hai, mel nahi khaati, aur exception aa jaata hai.

```
list.remove()  -> list ka modCount 5 ho gaya, iterator ki copy abhi 4 -> agle next() pe CME
it.remove()    -> iterator khud hatata hai, to dono 5 ho jaate hain -> SAFE
```

Ilaaj ye hai ki `it.remove()` use karo, kyunki wo dono ginti saath me update karta hai. Ya seedha
`surveys.removeIf(s -> s.isExpired())` likho, jo andar se iterator hi use karta hai.

## Asli prod trap (1-Oct chala ke dekha)

```
list: [S100, S101, S102, S103]   ->  loop ke andar ek-ek ko hata ke dekha
S100 hataya -> CME
S101 hataya -> CME
S102 hataya -> KOI exception nahi, par S103 dekha hi nahi gaya
S103 hataya -> CME
```

Jab aakhri se pehla element (S102) hataya, to loop chup-chaap ruk gaya, kyunki us waqt cursor size ke barabar ho gaya aur
loop ko laga ki kaam khatam. Exception nahi aaya, isliye code sahi lagta hai, par ek survey bina check ke reh gaya.
Yaad rakh: "exception nahi aaya" ka matlab "code sahi hai" nahi hota.

## ListIterator

ListIterator ko Iterator ka bada bhai samjho. Ye aage bhi chal sakta hai aur peeche bhi, aur chalte-chalte `add` aur `set`
bhi kar sakta hai. Par ye sirf List pe milta hai.

Iska cursor kisi element pe nahi, do element ke beech me hota hai. `remove` aur `set` us element pe chalte hain jo aakhri
baar lautaya gaya tha. Isliye agar tune `next()` bulaye bina hi `remove` ya `set` kiya, to IllegalStateException aata hai.

## Trade-off

for-each saaf dikhta hai, par usme element hata nahi sakte. Iterator thoda lamba likhna padta hai, par usse hatana safe
hai. Aaj ke code me seedha `removeIf` likhte hain.

## Interview line

```
Removing from a list inside a for-each loop changes modCount behind the iterator's back, so the
next call throws ConcurrentModificationException; worse, removing the second-last element ends the
loop silently and skips the last one, so I use iterator.remove() or removeIf.
```
