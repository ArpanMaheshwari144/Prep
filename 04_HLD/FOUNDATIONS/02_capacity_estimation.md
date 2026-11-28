# Capacity estimation — mota hisaab, utna hi jitna faisla badle

> **NAV** — KYA: kitne log, kitni request, kitna data (mota hisaab). lagta hai: [banking](../SYSTEM_DESIGNS/13_banking_system/13_banking_system.md) · [url-shortener](../SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md) · [twitter-feed](../SYSTEM_DESIGNS/03_twitter_feed/03_twitter_feed.md) · peeche: [01 HLD kya hai](01_hld_kya_hai.md)

## Koi poochhe to aise bolo

Capacity estimation ek mota hisaab hai ki kitne log aayenge, ek second mein kitni request aayengi aur kitna data jama hoga. Exact number nahi chahiye, utna hi chahiye jitne se faisla badle. Jaise "data kai TB hai, ek machine mein nahi aayega, isliye baanto", ya "padhne wale likhne walon se 100 guna hain, isliye cache lagao".

## Kahani (restaurant)

Restaurant kholne se pehle socho ki din mein kitne log aayenge. 100 log aate hain to 1 chef aur 5 table kaafi hain. 10,000 aate hain to 10 chef, kai kitchen aur parking chahiye. Chef ki ginti mein 3-4 upar-neeche hone se farak nahi padta. Farak sirf is baat ka hai ki ek kitchen chalega ya kai chahiye.

Hisaab ki ek chaabi:

```
1 din ~ 1 lakh second (10^5)

1 crore request roz  ->  1 crore / 1 lakh = ~100 request / second
peak (shaam, sale)   ->  avg ka 2-3 guna
```

**Banking round me aise bola (number jahan faisla badla):**

```
80-90 lakh user, 30-40 lakh roz aate     ->  ek bank ke liye bahut, par koi dikkat nahi
15 lakh roz sirf balance dekhte          ->  padhna bahut zyada, isliye balance Redis me
1 tareekh: 1 lakh me 70 hazaar salary dekhne  ->  wahi baat, DB pe 70 hazaar query kyun
```

Yahan numbers ne Redis ka faisla karwaya. Isse aage ka hisaab (bandwidth, kitne TB) is design me kuch nahi badalta, isliye bola hi nahi.

## Ek trap

Numbers pe atak jaana, calculator nikaalna ya 1200 aur 1300 pe bahas karna. Interviewer design dekhta hai. Mota number bolo, maanyata saaf bolo ("maan lo 1 crore user roz") aur aage badho. Jahan number se design nahi badalta, wahan number bolne ki zaroorat hi nahi.
