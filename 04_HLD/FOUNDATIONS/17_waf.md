# WAF (Web Application Firewall)

> **NAV** — KYA: WAF (bouncer, kya rokta, kya nahi, count mode). lagta hai: [rate-limiter](../SYSTEM_DESIGNS/02_rate_limiter/02_rate_limiter.md) · [URL shortener](../SYSTEM_DESIGNS/01_url_shortener/01_url_shortener.md)

## Scene: Arpan ke hi jawab

Rate limiter me: "ek banda roz hazaar baar password reset maar raha, insaan ke liye ye possible nahi, us IP ko WAF me daal do". URL me:
"machine se hazaar chit, pattern pata lag jaayega, gate pe bhaga do". DIKKAT_CHAIN me is parivaar ka naam bhi wahi: **BOUNCER**. WAF wahi
bouncer hai, jo app se pehle gate pe khada hai aur har request ko dekhta hai.

```
Internet -> CDN -> WAF -> LB -> app servers -> DB
```

## Bouncer kya-kya rokta hai

- **Kaali list:** ye IP pehle se badnaam hai, seedha bahar (IP reputation).
- **Had:** ek IP se ek minute me 1000 request, to roko. Wahi rate limiter jo padha, bas gate pe laga hai.
- **Pehchana hua hamla:** URL me `?id=1' OR '1'='1` (SQL injection), comment me `<script>` (XSS), ya `../../etc/passwd` (path traversal).
  Inka pattern pata hai, isliye seedha block (signature).
- **Ajeeb bartaav:** roz 10 request aati thi, aaj ek IP se 1000, ya baaki duniya se alag dikha, to CAPTCHA ya block (anomaly).

Ye niyam khud nahi likhte: AWS WAF ya Cloudflare ke bane-banaye niyam (managed rules) lagate hain, jo naye hamle aane pe apne aap update
hote hain. Apne niyam bhi jod sakte ho, jaise "/admin sirf office ke IP se".

## WAF kya NAHI pakadta (Compares 07 se jod)

Usercrud ka `DELETE /users/5` wala chhed: Ravi ka token sahi hai, request bilkul saaf dikhti hai, koi SQL ya `<script>` nahi. Bouncer
sirf dekhta hai banda kaisa dikh raha hai; andar jaake kis kamre me ghusa, wo app ka kaam hai. Isliye OWASP ki list ka number 1 hamla,
broken access control, WAF se nahi, owner check se rukta hai. Kamzor password rakhna ya purani library chalana bhi WAF theek nahi karta.

## Prod wala trap: sahi aadmi bhi bahar

User ne comment likha: "I love your SELECT * FROM products page!". WAF ko `SELECT * FROM` dikha, SQL injection samjha, comment block.
Grahak naraaz (false positive).

**Hal:** naya niyam seedha block pe mat lagao. Pehle **count mode**: WAF sirf likhta hai ki kya rokta, rokta kuch nahi. Kuch din log
dekho, jo sahi log ruk rahe the unke liye chhoot do (jaise comment wale endpoint pe), phir **block mode** on.
