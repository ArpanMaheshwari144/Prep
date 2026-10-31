# Notification: hand notes (Arpan ka order)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** Arpan ek shopping app se order karta hai. App use SMS ("order placed"), email (bill) aur push ("shipped") bhejti hai, aur login pe OTP ka SMS.
**Shabd:** beech ka dabba = queue (Kafka) · alag line = topic + consumer group · parchi = idempotency · thodi der baad = retry + backoff, alag dabba = DLQ · teen-chaar company + VIP line = failover + circuit breaker + OTP priority · saanjha dabba = token bucket (Redis) · "le liya" ke baad asli haal = webhook + Tracking DB · tukde + parchi = chunks + checkpoint · ek hi saancha apne paas = template cache

Kafka se aaya: line, consumer group, DLQ. Rate limiter se: token bucket. Cache se: cache. Naya: circuit breaker, webhook, campaign tukde.
Pattern (Arpan): dikkat aaye to likh lo, jo abhi na ho sake use kahin alag rakh do.

&nbsp;

**1. Order Service khud SMS company ko call kare, company dheemi, "Place Order" 8 second ghoome**
Beech me ek dabba rakh lo. Wo kahe "aao, sab mere paas aa jao", sabko rakh le aur dheere-dheere aaram se bhejta rahe.
**queue (Kafka) + Notification Service, async**
Interview: "Order Service khabar Kafka me daal ke turant 'order placed' bolta hai. Notification Service line se uthake bhejta hai. SMS company dheemi ya band ho to order nahi atakta."

**2. Ek hi line, SMS dheema, bill aur push SMS ke peeche khade**
Line alag-alag kar do, jaise mandir ya airport security me ladies, gents, bachchon ki alag line. Sab apni line sambhalein.
**har raaste ka alag topic + consumer group**
Interview: "SMS, email, push ke alag topic, har ek ka apna worker group. SMS dheema ho to sirf SMS rukta hai."

**3. Worker ne SMS bhej diya, commit se pehle crash, Arpan ko do SMS**
Kaam ho gaya to parchi pe likh lo. Dobara aaye to pehle parchi dekho, likha hai to nahi bhejna. Parchi zindabaad.
**idempotency (khabar ki id + parchi)**
Interview: "Har khabar ki id. Bhejne ke baad 'ho gaya' likhte hain, dobara aaye to pehle check, mila to skip. OTP ya '₹5000 debited' do baar nahi jaata."

**4. Company bole "abhi bheed hai, baad me try karo"; kuch kabhi ja hi nahi sakti (galat number)**
Alag jagah rakh do, thodi der baad try karo. Papa busy hain to 100 rupaye thodi der baad maang lenge. Jo kabhi ja hi nahi sakti, use alag dabbe me, koi insaan dekh le.
**retry queue + backoff, DLQ**
Interview: "Temporary error pe retry line, har baar thoda zyada ruk ke. Galat number jaisi khabar DLQ me, line nahi rukti."

**5. SMS company poori band, har call 30 second latke, OTP nahi aaya**
Ek company nahi, teen-chaar rakho. Fail hua to parchi pe likho, do-teen baar try karke ek backup se bhejo. Backup se gaya to main company band hai: traffic backup se, beech-beech me main ko "tu theek hai?" pooch lo. Backup se bhi nahi gaya to number galat hai, DLQ. Aur OTP VIP hai, mandir ke VIP darshan jaisa, uski apni jaldi wali line.
**failover (backup company) + circuit breaker (CLOSED / OPEN / HALF-OPEN) + OTP ki alag tez line**
Interview: "Lagaataar fail pe breaker OPEN, seedha backup. Beech-beech me ek-do main pe try (HALF-OPEN), chala to CLOSED. OTP ka alag priority topic."

**6. 20 worker, company ki had 100 / second, sab milake 300, 429 aur block ki dhamki**
Company ko had badhane nahi bol sakte. Saanjha kar lo: ek jagah se sabko guzaaro, wahan ginti likhi ho. Tu bhej raha hai to doosra nahi. Bachi khabarein line me aaram se.
**saanjha token bucket (Redis)**
Interview: "Har worker bhejne se pehle Redis ke ek bucket se token leta hai. 20 ho ya 50 worker, company tak 100 / second hi jaate hain."

**7. Company ne "OK, le liya" bola, record me SENT, par SMS phone tak pahuncha hi nahi**
"Le liya" bhejna nahi hai. Company baad me batati hai bheja ya fail hua, sirf do raaste. Tab apni parchi me likho.
**webhook + Tracking DB (ACCEPTED -> DELIVERED / FAILED)**
Interview: "Company ka OK sirf ACCEPTED. Asli nateeja company webhook se batati hai, hum Tracking DB me haal badalte. Support ko FAILED dikhta hai, SENT nahi."

**8. Diwali: ek khabar "1 crore ko bhejo", ek worker, 40 lakh pe crash, dobara shuru se**
Ek worker 1 crore nahi sambhalega, tod do. Har worker ko apni range. Bhejte jao aur DB (moti diary) me mark karte jao, Redis me 1 crore nahi. Koi bimaar pada to parchi dekho: 10 me se 7 ho gaye, baaki 3 backup wala bhej de. Upar se idempotency.
**campaign tukde (chunks) + checkpoint (DB) + idempotency**
Interview: "Campaign ko chhote tukdon me baant ke kai worker. Progress DB me mark, crash pe wahin se. Id se dobara nahi jaata."

**9. Har SMS pe DB se pasand + saancha, campaign me DB pe bojh**
Diwali ka saancha sabke liye ek hi hai, baar-baar DB kyun jaana, apne paas rakh lo, bas naam badlo. Pasand har user ki alag, DB me hi rakho: token bucket ki wajah se DB pe ~100 / second hi query, OTP ki apni line hai. Cache tabhi jab naap ke dikhe ki bhaari pad raha.
**saancha cache, pasand DB se (naap ke hi cache)**
Interview: "Template cache me. User preference DB se, kyunki rate limit ki wajah se load chhota hai. Zaroorat dikhe tab preference cache."

&nbsp;

Poora design = [04_notification_system.md](04_notification_system.md)
