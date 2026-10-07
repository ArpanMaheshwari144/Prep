# STAR — ek hi doctor ke kai account, haath ka merge band karne wala dedup tool  [Konovo]

> Q ye cover karta: "kuch banaya jo team ka kaam bachaye" · "edge case / false positive kaise pakde" ·
> "lead ke saath milke design badla" · "data quality wala issue".

## STAR (Hinglish)

```
   S: ek hi doctor ke kai account ban jaate the, email ke peeche number laga ke
      (arpanmah, arpanmah1, arpanmah2). Ops team inhe haath se ek-ek karke merge karti thi.

   T: merge apne aap ho jaaye, aur sirf sahi log merge hon.

   A: user table me email1 / email2 / email3 (ek user ke max 3) -> merge hone wala email
         us user ka secondary email ban jaata.
      team ki Excel (kya merge karna hai) upload -> har email pe:
         1  peeche ka number hata ke base email (arpanmah1 -> arpanmah)
         2  first name + last name DONO mile tabhi aage
         3  pakka karo: phone / address / NPI (US doctor ID) / RPPS (France doctor ID)
         sab mile -> merge
      testing me galat merge aaye (sirf number hata ke milana kaafi nahi tha)
         -> lead se baat karke rule kade kiye: naam + phone / address / NPI ke bina merge nahi.

   R: haath ka merge khatam, ops team saare merge isi tool se karne lagi.
      LEARNING: milte-julte record me galat merge zyada mehnga hai -> pakke ID (NPI) se confirm karo.
```

## SPOKEN (English)

```
   "At Konovo, the same physician often had several accounts with numbered emails, like
    arpanmah, arpanmah1, arpanmah2. The ops team was merging these by hand, one by one.

    I built a deduplication tool to do it. The team uploaded an Excel sheet of accounts to merge.
    For each email, the tool stripped the trailing number to get the base email, and then matched
    the person in our database — first and last name had to match, and then phone, address, or the
    physician ID, NPI in the US or RPPS in France. Only then it merged, and the extra email became
    a secondary email on that user.

    In testing we saw wrong merges — matching on the email alone wasn't enough. I discussed it with
    my lead and we tightened the rules so nothing merged without the name plus a hard identifier.

    After that, manual merging stopped and the ops team moved all merges to the tool. My learning
    was that a wrong merge costs more than a missed one, so confirm on a real ID."
```

## POOCHE TO

```
   "Fuzzy matching kaunsa algorithm?" -> rule-based: suffix hatana + naam, phone, address, NPI / RPPS match.
                                         similarity score wala algorithm nahi lagaya.
   "False positive kaise roke?"       -> sirf email base pe merge nahi; naam dono + phone / address / NPI.
   "Kitne duplicate hate?"            -> gina nahi tha. Fark ye tha ki haath ka kaam band hua
                                         aur team ne saare merge isi pe kar diye.
   "Tumhara role kya tha?"            -> tool banaya, matching rules likhe, testing me galat merge pakde,
                                         lead ke saath rules kade kiye.
```
