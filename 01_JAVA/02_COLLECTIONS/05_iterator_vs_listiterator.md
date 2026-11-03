# Iterator vs ListIterator

## Kahani: expired surveys hatane hain

for-each ke andar `list.remove()` -> `ConcurrentModificationException`.

## Exception kyun

List har badlaav ki ginti rakhti (`modCount`). Iterator bante waqt uski copy rakhta. Seedha list se hataya -> list ki
ginti badhi, iterator ki copy purani -> agle `next()` pe mel nahi -> exception.

```
list.remove()  -> modCount 5, iterator ki copy 4 -> agle next() pe CME
it.remove()    -> dono 5 -> SAFE
```

Ilaaj: `it.remove()`, ya seedha `surveys.removeIf(s -> s.isExpired())` (andar se iterator hi).

## Asli prod trap (1-Oct chala ke dekha)

```
[S100, S101, S102, S103]   ->  loop me hataya
S100 -> CME    S101 -> CME    S102 -> KOI exception nahi, S103 dekha hi nahi    S103 -> CME
```

Aakhri se pehla hataya to loop chup-chaap ruk gaya (cursor == size). Exception nahi = code sahi lagta, par ek survey
bina check ke reh gaya. "Exception nahi aaya" != "code sahi".

## ListIterator

Iterator ka bada bhai: aage + peeche, chalte-chalte `add` aur `set`, sirf List pe. Cursor do element ke beech hota;
`remove` / `set` aakhri lautaye element pe chalte, isliye `next()` ke bina = IllegalStateException.

## Trade-off

for-each = saaf, par hata nahi sakte. Iterator = lamba, hatana safe. Aaj ke code me `removeIf`.

## Interview line

```
Removing from a list inside a for-each loop changes modCount behind the iterator's back, so the
next call throws ConcurrentModificationException; worse, removing the second-last element ends the
loop silently and skips the last one, so I use iterator.remove() or removeIf.
```
