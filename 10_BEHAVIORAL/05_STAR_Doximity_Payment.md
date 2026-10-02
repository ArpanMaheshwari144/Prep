# STAR — Doximity Vendor Payment Leak (~$20K)  [Konovo]

> Q ye cover karta: "biggest IMPACT" · "initiative jo KHUD drive kiya" · "business sense / beyond-code" ·
> "leadership ko convince".

## STAR (Hinglish)

```
   S: Konovo tool vendors ko survey-completion pe pay karta (tool ke THROUGH).
      DOXIMITY ko tool se payment nahi ja raha tha -> company alag se UPFRONT haath se pay kar rahi thi
      -> tool ke bahar -> UNTRACKED -> ~$20K, kaafi time se.

   T: pata karo tool Doximity ko kyun nahi pay kar raha + fix -> leak roko, payment track ho.

   A: code + repo + API traces + logs -> payment flow -> method tak pahuncha.
      ROOT CAUSE: ek IF-condition me Doximity TEST USER ke saath tha -> "survey complete ho to paisa mat bhejo".
      kisi ne assign nahi kiya -> KHUD TPM / tech lead ko meeting me uthaya.
      sirf code nahi, BUSINESS IMPACT dikhaya: tool se nahi jaa raha -> upfront haath se -> untracked -> loss.
      CTO: "yes, address this" -> condition KHUD fix ki.
      test server pe test -> dobara test -> prod -> wahan bhi test -> MONITOR.

   R: payment tool se wapas jaane laga -> sab TRACKED -> "untracked, around $20K" wala leak ruka.
      dikkat dobara nahi aayi. CTO se recognition.
```

## SPOKEN (English)

```
   "At Konovo, our platform pays third-party survey vendors through the tool once they complete surveys.
    One vendor, Doximity, wasn't receiving payments through the tool, so the company had been paying them
    separately and upfront, outside the system. That was untracked, around $20K over time.

    I traced the payment flow through the code, API traces and logs down to the method, and found the root
    cause: an if-condition had Doximity grouped with test users, so when they completed a survey, no payment
    was sent.

    Nobody had assigned this to me, but I raised it with our TPM and tech lead in a meeting, and I framed it
    beyond the code: because the tool wasn't paying them, we were paying manually and untracked, and that
    was a loss.

    The CTO agreed, and I fixed the condition right away. I tested it on our test server, retested, released
    to production, tested there and monitored it. After that, Doximity's payments flowed through the tool,
    everything was tracked, the leak stopped, and the issue never came back."
```
