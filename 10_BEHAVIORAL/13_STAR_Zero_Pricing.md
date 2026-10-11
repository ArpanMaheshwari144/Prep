# STAR — bidding page pe random $0 pricing (do Lambda call, doosri ne duplicate row banayi)  [Konovo]

> Q ye cover karta: "production bug jo tumne debug kiya" · "reproduce kaise kiya" ·
> "data kharab ho gaya tha to kya kiya" · "kisi aur ke code ki galti pakdi".

## STAR (Hinglish)

```
   S: feasibility (bidding) tool pe kabhi-kabhi bid ki pricing $0 dikhti thi. random, har baar nahi.

   T: pata karna $0 kahan se aata, fix karna, aur jo kharab data ban chuka use saaf karna.

   A: bug reproduce kiya -> logs dekhe -> ek save pe EK ki jagah DO Lambda call dikhi.
      pehli call: save -> sahi value likhi.
      user GET page pe gaya -> code me wahan ek aur Lambda call lagi thi.
         kyun: GET pe value thodi der blank (null) dikhti thi, isliye kisi developer ne
         wahan dobara call daal di thi.
      doosri call: us pal value null thi -> code null save nahi karta, uski jagah 0 daal deta
         -> aur update nahi, NAYI row bana deta.
         -> ek bid ki DO row: ek sahi, ek $0 wali (data-warehouse table me).
      fix: GET pe dobara Lambda call hata di -> ab ek save = ek call = ek row.
      cleanup: script likh ke saari duplicate $0 wali rows hata di (wo bekaar rows thi).

   R: $0 pricing band, duplicate rows saaf.
      LEARNING: GET (padhna) kabhi write na kare. UI ki blank value chhupane ke liye
                doosri call lagana = data kharab karna.
```

## SPOKEN (English)

```
   "At Konovo, our bidding tool sometimes showed a bid's price as zero dollars. It was random, not every time.

    I reproduced it and checked the logs, and saw two Lambda calls for one save instead of one. The first
    call saved the correct value. But when the user opened the page, the code made a second Lambda call,
    because a developer had added it to cover a moment where the value showed blank on load. At that
    moment the value was null, the code couldn't save null so it wrote zero, and instead of updating it
    inserted a new row. So one bid ended up with two rows, one correct and one with zero pricing.

    I removed the second call from the page load, so one save means one call and one row. Then I wrote a
    script to delete all the duplicate zero-price rows.

    My learning was that a read should never write, and you don't fix a display gap by adding another
    write call."
```

## POOCHE TO

```
   "Kaise pata chala do call ho rahi?"   -> reproduce kiya, logs me ek save pe do Lambda call dikhi.
   "$0 kyun aata tha?"                    -> doosri call ke waqt value null thi; code null save nahi karta, 0 daal deta.
   "Overwrite ya nayi row?"               -> nayi row. isliye ek bid ki do row: ek sahi, ek $0.
   "Doosri call thi hi kyun?"             -> GET pe value thodi der blank dikhti thi, kisi developer ne usko
                                             chhupane ke liye wahan call daal di.
   "Kitni rows saaf ki?"                  -> exact count yaad nahi, high priority thi, ginti pe dhyaan nahi diya.
                                             script se saari duplicate $0 rows hata di.
```
