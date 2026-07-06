# STAR — Wrong Client-IP (X-Forwarded-For / AWS) bug  [Konovo]

> Q ye cover karta: "initiative batao" · "koi bug jo tune KHUD dhundha" · "jab team ne agree nahi kiya" · "convince kaise kiya" · "subtle prod bug".

## STAR (Hinglish notes — yaad ke liye)

```text
   S (Situation): ek din normally CODE padh raha tha -> ek method dikha jo thoda ABNORMAL laga.
        wo method client ki IP nikaal raha tha. iske regarding ek chhota ticket bhi tha, par usme
        info kuch nahi thi -> client ne bas ek Excel di thi, aur kuch nahi.
   T (Task)     : shaq ko confirm karna -> kya galat IP uthai ja rahi hai? aur agar haan to prove + fix.
   A (Action)   :
        1. samajha flow: request AWS se aati -> AWS apne data-center ki IP header me PEHLE append kar deta,
           client ki ASLI IP uske BAAD aati. hamara code header ki PEHLI IP utha raha tha
           -> AWS ki IP mil rahi thi, client ki NAHI. (subtle bug — dikhta normal hai.)
        2. client se call pe baat ki -> unhone confirm kiya "aisa aisa ho raha hai" -> shaq pakka hua.
        3. us method ka DRY-RUN kiya -> shaq aur badha.
        4. APP LOGS dekhe -> aur clear hua. phir AWS CloudWatch logs dekhe -> POORA yakeen ho gaya
           (actual header value me AWS-IP-first dikha).
        5. lead ko bataya -> pehle koi MAANA nahi (dismiss).
        6. ek meeting me jab sab the -> maine sab ke SAAMNE proof/subut rakh diya -> sabne khud dekh liya
           -> tab accept hua.
   R (Result)   : fix bada nahi tha -> method me chhota change (sahi IP index uthao — client wali).
        ab correct client-IP capture hone lagi. root-cause khud dhundha + prove karke + convince karke close kiya.
```

## SPOKEN (English — interview me bolna, ~60-90 sec)

```text
   "At Konovo, while reading through some code one day, I noticed a method that looked a bit off — it was the one
    extracting the client's IP address. There was a small ticket around it, but almost no information — the client
    had just shared an Excel sheet and nothing more.

    I looked into how the request flowed. When a request came through AWS, AWS was appending its own data-center IP
    to the forwarded header first, and the actual client IP came after that. Our code was reading the first IP in
    the header — so it was picking up AWS's IP, not the client's. It was subtle, because on the surface everything
    looked fine.

    To confirm, I got on a call with the client, did a dry-run of the method, checked our application logs, and then
    the AWS CloudWatch logs — where I could actually see the header with AWS's IP first. That made me certain.

    When I first raised it, it wasn't accepted. So I put the evidence together and presented it in a team meeting
    where everyone could see it for themselves. Once they did, we agreed on it. The fix itself was small — just
    reading the correct IP from the header — and after that we were capturing the right client IP."
```

## KYA DEMONSTRATE karta (interviewer ye dekhta)

```text
   - INITIATIVE / proactive — kisi ne assign nahi kiya, code padhte-padhte KHUD pakda.
   - SYSTEMATIC proof — shaq -> client-call -> dry-run -> app-logs -> CloudWatch (layer-by-layer, guess nahi).
   - CONVICTION + INFLUENCE — pehle koi nahi maana -> evidence banaya -> meeting me demonstrate -> convince kiya.
     (ye HikariCP story se ALAG skill dikhata — wahan ownership+debug, yahan initiative+persuasion.)
   - technical depth — X-Forwarded-For / proxy IP-chain samajh (AWS prepend, header order).
```

## DELIVERY tips

```text
   - "MAINE / I" bolo — ye story specially "khud pakda + khud convince kiya" pe TIKI hai, wahi highlight kar.
   - conviction wala part mat chhodo -> "pehle nahi maana, maine proof rakha" = ye asli value hai (persistence).
   - calm + structured -> S->T->A->R. bolne ki PRACTICE (loud, 2-3 baar).
   - confirm/add (agar yaad aaye): kitne time me pakda, koi metric/impact (kitne clients affected the?).
```
