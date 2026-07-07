# STAR — Doximity Vendor Payment Leak (~$20K)  [Konovo]

> Q ye cover karta: "biggest IMPACT" · "initiative jo tune KHUD driva" · "business sense / beyond-code" · "leadership ko convince".
> (Ye story HikariCP/WAF se ALAG competency dikhati — financial IMPACT + stakeholder-communication.)

## STAR (Hinglish notes — yaad ke liye)
```
   S (Situation): Konovo tool third-party VENDORS ko survey-completion pe pay karta hai (contract-based, THROUGH the tool).
        ek vendor — DOXIMITY — ko tool se payment nahi ja raha tha (code bug). isliye company use SEPARATELY/UPFRONT
        manually pay kar rahi thi -> tool ke bahar -> UNTRACKED -> ~$20K ka leak/loss, aur ye kaafi TIME se chal raha tha.
   T (Task)     : pata karo tool Doximity ko kyun nahi pay kar raha, + fix karo -> leak roko + payments tracked ho.
   A (Action)   :
        1. API TRACES investigate ki -> payment-flow follow kiya.
        2. ROOT-CAUSE mila: ek IF-condition (uske andar ek specific condition) jo Doximity ki payment ko BLOCK kar rahi thi.
        3. INITIATIVE liya -> TPM / tech-lead ko raise kiya, MEETING me uthaya (kisi ne assign nahi kiya tha).
        4. sirf code nahi -> BUSINESS IMPACT dikhaya: tool se pay na hone se company alag se upfront de rahi thi
           -> untracked -> loss. "tool se jaye to har payment TRACK hota; ye nahi jaa raha isliye leak."
        5. CTO ne green-light diya -> "yes, address this."
        6. wo condition turant FIX ki.
   R (Result)   : payment tool ke through wapas flow karne lagi -> Doximity ko sahi pay hone laga -> sab TRACKED
        -> ~$20K ka leak/loss RUKA. CTO ki recognition mili.
```

## SPOKEN (English — interview me bolna, ~60-90 sec)
```
   "At Konovo, our platform pays third-party survey vendors through the tool once they complete sampled surveys.
    One vendor, Doximity, wasn't receiving their payments through the tool — so the company had been paying them
    separately and upfront, outside the system. That was untracked and had been leaking around $20K over time.

    I looked into the payment flow through our API traces and found the root cause: an if-condition in the code
    was blocking Doximity's payments from going through.

    Nobody had assigned this to me, but I raised it with our TPM and tech lead and brought it up in a meeting.
    I framed it beyond just the code — I showed the business impact: because the tool wasn't paying them, we were
    paying manually and upfront, which wasn't tracked and was causing a loss. Once payments flow through the tool,
    everything is tracked.

    The CTO agreed it needed to be addressed, and I fixed the condition right away. After that, Doximity's payments
    flowed correctly through the tool again, everything was tracked, and the leakage stopped."
```

## KYA DEMONSTRATE karta (interviewer ye dekhta)
```
   - IMPACT / business-critical ($20K, financial leak) — bade impact wala kaam.
   - INITIATIVE — kisi ne assign nahi kiya; tune KHUD pakda + drive kiya.
   - BEYOND-CODE / business sense — sirf bug nahi, "untracked upfront payment = loss" wala angle samjha aur bola.
   - STAKEHOLDER communication — TPM/lead/meeting/CTO tak, impact ke saath raise kiya.
```

## DELIVERY tips
```
   - "MAINE / I" bolo. NUMBER highlight kar ($20K, "kaafi time se").
   - asli value = BUSINESS-IMPACT wala framing (untracked upfront -> loss). code-fix chhota tha, IMPACT bada.
   - calm + structured -> S->T->A->R. bolne ki PRACTICE (loud, 2-3 baar).
   - confirm/add: exact $ figure · kitne time se chal raha tha · fix ke baad koi metric.
```
