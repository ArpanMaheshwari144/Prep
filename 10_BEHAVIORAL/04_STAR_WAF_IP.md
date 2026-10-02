# STAR — Wrong Client-IP (X-Forwarded-For / AWS) bug  [Konovo]

> Q ye cover karta: "initiative" · "bug jo KHUD dhundha" · "team/senior agree nahi kiya" ·
> "disagreement kaise handle kiya" · "convince kaise kiya" · "subtle prod bug".

## STAR (Hinglish)

```
   S: code padh raha tha -> getClientIP method ajeeb laga. header me 2 IP, code HARDCODED pehli utha raha.
      chhota Jira ticket, info nahi -> client ki ek Excel thi, usme IPs REPEAT ho rahi thi.

   T: confirm karna -> hum ASLI client-IP le rahe hain ya nahi? nahi to prove + fix.
      (IP audit, geo, rate-limit / WAF rules sab me lagti thi)

   A: flow trace -> HAMARE SETUP me header ki pehli entry AWS edge IP thi, asli client IP uske baad
         ("AWS hamesha pehle lagata" mat bolna — jo dekha wahi: "in our setup")
      proof: CloudWatch + Papertrail + method khud test -> repeat IPs US data-center ki thi, client ki nahi.
      lead + TPM ne mana kiya ("saalon se aise chal raha") -> concern fair tha.
      ARGUE nahi kiya -> evidence leke regular monthly review (VP + CTO) me rakha, blame nahi, sirf DATA.
      VP ne maana -> lead/TPM bhi same page.
      fix: hardcoded index hataya -> AWS infra-IP pattern pehchan ke asli client IP nikali, format badle to bhi na toote.
      test -> prod -> ~7-10 din logs MONITOR -> VP ko update.

   R: sahi client IP aane lagi. 300+ vendor integrations me FALSE DUPLICATES band, AWS WAF IP rules theek.
      VP aur CTO se recognition.
      LEARNING: "leadership ko jargon nahi, DATA chahiye tha. opinion ki jagah evidence -> faisla 5 minute me."
```

## SPOKEN (English)

```
   "At Konovo, while reading through some code, I noticed our getClientIP method looked off. The header had
    multiple IPs and the code was hardcoded to always take the first one. There was a small Jira ticket with
    almost no information, just an Excel from the client, and in it the IPs were repeating.

    I wanted to confirm whether we were capturing the real client IP, because that IP fed our audit logs,
    geo logic and IP-based rules.

    I traced the flow. In our setup, the first entry in the forwarded header was the AWS edge IP, with the
    real client IP after it, so we were capturing AWS's IP. That caused false duplicates across 300+ vendor
    integrations and broke every IP rule in our AWS WAF. I confirmed it across CloudWatch, Papertrail, and by
    testing the method myself; the repeating IPs were US data-center addresses, which couldn't be clients.

    My lead and TPM weren't convinced, and their concern was fair, it had run this way for years. So instead
    of arguing, I took the evidence to our regular monthly review with the VP and CTO, no blame, just data.
    The VP agreed it was a real bug, and the lead and TPM came on board too.

    I removed the hardcoded index and made the logic robust, detecting the AWS infrastructure IP and
    extracting the actual client IP, so it wouldn't break if the header format changed. We shipped it, I
    monitored the logs for about ten days and kept the VP updated, and I got recognition from the VP and CTO.

    My takeaway: leadership didn't need logs or jargon, they needed data. When I replaced opinion with
    evidence, the decision took five minutes. I take every disagreement in with data now."
```
