# STAR — list-matching jobs fail, Fargate ko IP nahi mili (6 me se 2 subnet bhare)  [Konovo]

> Q ye cover karta: "production issue jo tumne debug kiya" · "infra / AWS wala root cause" ·
> "jab write access nahi tha tab kya kiya" · "dobara na ho uske liye kya kiya".

## STAR (Hinglish)

```
   S: ek din achanak list-matching (bid list) jobs SAB fail hone lage. jobs ghanton atke rahe.

   T: pata karna kyun fail ho rahe, jobs chalu karna, aur ye dobara na ho.

   A: pata tha: har Fargate (ECS) task ko chalne ke liye subnet se ek IP chahiye.
         -> pehla andaza: IP khatam.
      logs + metrics dekhe -> andaza sahi: 6 me se 2 VPC subnets ki saari IP use me thi.
      kyun: kuch files bahut BADI thi -> unke job bahut der se chal rahe the, IP pakde baithe the.
         baaki jobs line me lage, mauka nahi mila. upar se team ne aur upload kiya -> sab fail.
      fix (AWS ka write access hamare paas nahi tha -> infra / DevOps se):
         infra ne subnet badhaye, IP badhai -> load sab subnets me barabar bat jaaye.
      code me (hamara hissa):
         job pe TIMEOUT lagaya -> itni der chale to khud fail ho jaaye, server / IP block na kare.
         phir us file ko dekhte ki usme kya dikkat thi.
      badi file:
         client ko bola badi file ko chhote tukdon me tod ke upload karo.
         code me khud tukde NAHI kiye -> false positive ka risk nahi le sakte the,
            aur file me sensitive info hoti hai.
      do tarah ke jobs:
         IP na milne se FAIL hue jobs -> humne khud dobara chalaye.
         badi file pe ATKE jobs       -> unhe fail kiya, client ko bola file (tod ke) dobara upload kare.

   R: jobs wapas chalu, load subnets me bata, lambi job ab khud timeout hoke baaki ko nahi rokti.
      LEARNING: ek bhaari job poore shared resource (IP) ko gher sakti hai -> har job pe had (timeout)
                lagao, aur bade input ko pehle hi chhota karwao.
```

## SPOKEN (English)

```
   "At Konovo, one day all our list-matching jobs suddenly started failing and stayed stuck for hours.

    I knew every Fargate task needs an IP from the subnet to start, so my first guess was that we had run
    out of IPs. The logs and metrics confirmed it: two of our six VPC subnets had every IP in use. A few very
    large files had jobs running for a long time and holding their IPs, the other jobs were queued behind
    them, and when the team uploaded more, everything failed.

    We didn't have AWS write access, so I took it to the infra team, and they expanded the subnets and added
    IPs so the load spread evenly. On our side, I added a timeout to the job, so if it runs too long it
    fails on its own instead of blocking the server, and then we investigate that file.

    For the large files, we asked the client to split them into smaller files before uploading. We didn't
    split them in code, because we couldn't risk false positives and the files contain sensitive data.
    I re-ran every job that had failed for lack of IPs. The ones stuck on huge files, we marked as failed
    and asked the client to upload those files again, split into smaller ones.

    My learning was that one heavy job can hold a shared resource like IPs, so every job needs a limit,
    and big inputs should be made smaller before they come in."
```

## POOCHE TO

```
   "IP kyun chahiye thi?"            -> har Fargate task ko subnet se ek IP milti hai, bina IP task start nahi hota.
   "AWS me tumne kya badla?"         -> kuch nahi, write access infra ke paas tha. infra ne subnet / IP badhaye.
   "Tumhara role kya tha?"           -> logs + metrics se wajah pakdi, infra ko diya, code me timeout lagaya,
                                        client ko file todne ko bola, IP wale failed jobs khud dobara chalaye,
                                        badi file pe atke jobs fail karke client se dobara upload karwaya.
   "Code me file kyun nahi todi?"    -> false positive ka risk + file me sensitive info.
   "Badi file aati kyun thi?"        -> client khud file nahi banate, kahin aur se milti hai, bina dekhe upload kar dete.
                                        hamare paas aisi file upload karne ke niyam (algorithm) hain, par sab follow nahi karte.
   "Timeout ke baad kya?"            -> job khud fail, server / IP free; phir us file ki dikkat dekhte.
   "Kitni IP badhi?"                 -> kitni IP / subnet badhane ka faisla VP + senior engineer ka tha, kaam DevOps ka.
                                        mera kaam nahi tha, isliye number mere paas nahi.
```
