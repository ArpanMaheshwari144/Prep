# STAR — opted-out users ko SMS ja rahe the, query galat row se match kar rahi thi  [Konovo]

> Q ye cover karta: "production bug kaise pakda" · "root cause" · "fix verify kaise kiya" ·
> "compliance / user trust wala issue" · "data se kaise saabit kiya ki fix kaam kar raha".

## STAR (Hinglish)

```
   S: SMS reminders un users ko bhi ja rahe the jinhone opt-out (STOP) kiya tha -> compliance risk.

   T: pata karna kyun ja rahe hain, aur band karna.

   A: query dekhi -> opt-out check SMS ki apni row ki jagah EMAIL wali opt-out row se match ho raha tha,
         aur ye 2 query paths me tha -> dono query maine theek ki (ab SMS ki row se match).
      test server pe chalaya -> opted-out user ko SMS nahi gaya -> fix kaam kar raha tha -> phir prod.
      release ke baad bache hue cases ka time milaya:
         pehle list me aaya, phir opt-out    = by design (neeche dekho), bug nahi
         pehle opt-out, phir bhi list me aaya = asli bug
      18 me se 18 cases by design nikle, asli bug ek bhi nahi.

   R: release ke baad opted-out users ko zero SMS.
      LEARNING: "dikh raha bug" aur "asli bug" alag ho sakte hain -> time milake saabit karo.
```

## BY DESIGN (bug nahi) — pooche to

```
   cron job din me EK baar chalti -> usi waqt opt-out check -> user us din ki list me aa gaya
   list me aane ke baad user opt-out kare -> us din ka SMS / email phir bhi jaata (ek baar)
   agli run me check hota -> opted-out -> phir kabhi nahi jaata
```

## SPOKEN (English)

```
   "At Konovo, we found that SMS reminders were going to users who had already opted out,
    which is a compliance risk.

    I traced it to the opt-out check in the query. It was matching the user's EMAIL opt-out row
    instead of the SMS opt-out row, and this happened in two query paths. I fixed both queries
    to match the SMS row, verified on the test server that an opted-out user no longer got the
    SMS, and then it went to production.

    After the release, I checked the remaining cases by comparing when the user entered the
    send list with when they opted out. All 18 were cases where the user opted out after the
    daily job had already picked them up — that's by design, not the bug.

    After the fix, opted-out users received zero SMS. My learning was that something that looks
    like the same bug may not be — you prove it with the timestamps."
```

## POOCHE TO

```
   "Fix kya tha?"                 -> opt-out check EMAIL row ki jagah SMS row se match karwaya, 2 query paths me.
   "Verify kaise kiya?"           -> test server pe opted-out user ko SMS nahi gaya; release ke baad 18 cases ka time milaya.
   "Fix ke baad bhi kabhi jaa
    sakta hai?"                   -> haan, ek baar: cron din me ek baar list banati; list ke baad opt-out kiya to
                                     us din ka chala jaata. agli run se kabhi nahi. ye design hai, bug nahi.
   "Tumhara role kya tha?"        -> query pakdi, dono jagah theek ki, test pe verify, release ke baad cases check kiye.
```
